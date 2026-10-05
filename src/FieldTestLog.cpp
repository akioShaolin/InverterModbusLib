#include "FieldTestLog.h"

#include <stdio.h>
#include <string.h>

void FieldTestLog::setError(const char* message) {
    snprintf(_error, sizeof(_error), "%s", message);
}

const char* FieldTestLog::stateName() const {
    switch (_state) {
        case STOPPED: return "STOPPED";
        case RECORDING: return "RECORDING";
        case STOPPING: return "STOPPING";
        case FAULT: return "FAULT";
        default: return "UNAVAILABLE";
    }
}

bool FieldTestLog::refreshUsage() {
    FSInfo info{};
    if (!_mounted || !LittleFS.info(info)) {
        setError("LittleFS.info failed; storage capacity is unknown");
        return false;
    }
    _capacityBytes = info.totalBytes;
    _usedBytes = info.usedBytes;
    return true;
}

bool FieldTestLog::begin(bool geometrySafe) {
    if (active()) {
        setError("Stop the session before mounting storage");
        return false;
    }
    _geometrySafe = geometrySafe;
    if (!geometrySafe) {
        _mounted = false;
        _state = UNAVAILABLE;
        setError("Flash geometry is unsafe; filesystem access disabled");
        return false;
    }
    LittleFSConfig config;
    config.setAutoFormat(false); // A failed mount must NEVER erase prior tests.
    LittleFS.setConfig(config);
    _mounted = LittleFS.begin();
    if (!_mounted) {
        _state = UNAVAILABLE;
        setError("LittleFS mount failed; explicit format is required");
        return false;
    }
    if (!refreshUsage()) {
        _state = FAULT;
        return false;
    }
    _state = STOPPED;
    _error[0] = '\0';
    return true;
}

bool FieldTestLog::validFilename(const char* path) {
    if (!path || strlen(path) != 16 || strncmp(path, "/test-", 6) != 0 ||
        strcmp(path + 12, ".csv") != 0) return false;
    for (uint8_t i = 6; i < 12; ++i) {
        if (path[i] < '0' || path[i] > '9') return false;
    }
    return true;
}

bool FieldTestLog::budgetAllows(uint32_t bytes, bool critical) const {
    const uint32_t headroom = critical ? 0 : COMMAND_HEADROOM_BYTES;
    if (_writtenBytes + _queuedBytes + bytes + headroom > MAX_FILE_BYTES)
        return false;
    return availableBytes() >= _queuedBytes + bytes + STORAGE_RESERVE_BYTES + headroom;
}

bool FieldTestLog::canRecord() const {
    return _mounted && _state == RECORDING && _count < QUEUE_CAPACITY &&
        budgetAllows(MAX_LINE_BYTES, false);
}

bool FieldTestLog::canAcceptCommand() const {
    // Keep room in RAM for acceptance, completion and a context/result row.
    return canRecord() && _count <= QUEUE_CAPACITY - 3 &&
        budgetAllows(3 * MAX_LINE_BYTES, false);
}

bool FieldTestLog::start(const char* csvHeader) {
    if (!_mounted || !_geometrySafe) {
        setError("Storage unavailable; cannot start a test session");
        return false;
    }
    if (active()) {
        setError("A test session is already active or is stopping");
        return false;
    }
    if (!csvHeader || strlen(csvHeader) > MAX_LINE_BYTES - 2 ||
        strchr(csvHeader, '\n') || strchr(csvHeader, '\r')) {
        setError("CSV header is invalid or too long");
        return false;
    }
    if (!refreshUsage()) return false;
    if (availableBytes() < STORAGE_RESERVE_BYTES + COMMAND_HEADROOM_BYTES + MAX_LINE_BYTES) {
        setError("Insufficient storage; download and explicitly remove old tests");
        return false;
    }

    uint32_t largest = 0;
    Dir dir = LittleFS.openDir("/");
    while (dir.next()) {
        const String entry = dir.fileName();
        char path[24];
        snprintf(path, sizeof(path), "%s%s", entry.startsWith("/") ? "" : "/", entry.c_str());
        if (!validFilename(path)) continue;
        uint32_t number = 0;
        for (uint8_t i = 6; i < 12; ++i) number = number * 10 + (path[i] - '0');
        if (number > largest) largest = number;
    }
    if (largest >= 999999) {
        setError("Test file numbering exhausted; existing files are preserved");
        return false;
    }
    char nextName[sizeof(_name)];
    snprintf(nextName, sizeof(nextName), "/test-%06lu.csv", static_cast<unsigned long>(largest + 1));
    if (LittleFS.exists(nextName)) {
        setError("Next test file already exists; overwrite refused");
        return false;
    }
    _file = LittleFS.open(nextName, "w");
    if (!_file) {
        _state = FAULT;
        setError("Could not create the next test file");
        return false;
    }
    snprintf(_name, sizeof(_name), "%s", nextName);
    _head = _tail = _count = 0;
    _queuedBytes = _writtenBytes = _writtenLines = _dropped = _maxWriteMicros = 0;
    _ioErrors = _capacityStops = 0;
    _lastFlushMs = millis();
    _dirty = false;
    _state = RECORDING;
    _error[0] = '\0';
    return appendCsvLine(csvHeader, true);
}

bool FieldTestLog::appendCsvLine(const char* line, bool critical) {
    if (!_mounted || (_state != RECORDING && !(critical && _state == STOPPING))) {
        ++_dropped;
        setError("No recording session is accepting this log row");
        return false;
    }
    const size_t length = line ? strnlen(line, MAX_LINE_BYTES) : MAX_LINE_BYTES;
    if (length > MAX_LINE_BYTES - 2 || memchr(line, '\n', length) || memchr(line, '\r', length)) {
        ++_dropped;
        setError("CSV row is invalid or exceeds the bounded line buffer");
        return false;
    }
    if (_count == QUEUE_CAPACITY) {
        ++_dropped;
        setError("Log RAM queue full; at least one row was dropped");
        return false;
    }
    if (!budgetAllows(length + 1, critical)) {
        ++_dropped;
        ++_capacityStops;
        _state = STOPPING;
        setError("Log capacity reached; stopping before the storage reserve");
        return false;
    }
    Row& row = _queue[_tail];
    memcpy(row.text, line, length);
    row.text[length] = '\n';
    row.text[length + 1] = '\0';
    row.length = length + 1;
    _tail = (_tail + 1) % QUEUE_CAPACITY;
    ++_count;
    _queuedBytes += row.length;
    return true;
}

void FieldTestLog::requestStop() {
    if (_state == RECORDING) _state = STOPPING;
}

void FieldTestLog::measureIo(uint32_t startedUs) {
    const uint32_t elapsed = micros() - startedUs;
    if (elapsed > _maxWriteMicros) _maxWriteMicros = elapsed;
}

void FieldTestLog::failIo(const char* message) {
    // A partially written CSV tail is retained for diagnosis. Retrying could
    // duplicate a command/result row, so stop and count all unwritten rows.
    setError(message);
    ++_ioErrors;
    _dropped += _count;
    _head = _tail = _count = 0;
    _queuedBytes = 0;
    _dirty = false;
    if (_file) _file.close();
    _state = FAULT;
}

bool FieldTestLog::flushFile() {
    if (!_file) {
        failIo("Test file unexpectedly closed");
        return false;
    }
    _file.flush();
    if (_file.getWriteError()) {
        failIo("Test file flush failed; final rows may not be durable");
        return false;
    }
    _dirty = false;
    _lastFlushMs = millis();
    if (!refreshUsage()) {
        failIo("Could not refresh storage capacity after writing");
        return false;
    }
    return true;
}

void FieldTestLog::service(bool busIdle) {
    if (!busIdle || !active()) return;
    const uint32_t startedUs = micros();
    if (_count) {
        Row& row = _queue[_head];
        if (!budgetAllows(0, true) || !_file) {
            failIo("Storage reserve reached or test file closed unexpectedly");
            measureIo(startedUs);
            return;
        }
        const size_t bytes = _file.write(reinterpret_cast<const uint8_t*>(row.text), row.length);
        _writtenBytes += bytes;
        if (bytes != row.length || _file.getWriteError()) {
            failIo("Test file write failed or was partial; session stopped");
            measureIo(startedUs);
            return;
        }
        ++_writtenLines;
        _queuedBytes -= row.length;
        _head = (_head + 1) % QUEUE_CAPACITY;
        --_count;
        _dirty = true;
        // Account conservatively between flushes (allocation metadata is
        // covered by the 16 KiB reserve); no capacity queries in callbacks.
        _usedBytes += bytes;
    }
    if (_dirty && (millis() - _lastFlushMs >= 1000UL || (_state == STOPPING && !_count))) {
        if (!flushFile()) {
            measureIo(startedUs);
            return;
        }
    }
    if (_state == STOPPING && !_count) {
        _file.close();
        if (!refreshUsage()) _state = FAULT;
        else _state = STOPPED;
    }
    measureIo(startedUs);
}

bool FieldTestLog::formatStorage() {
    if (!_geometrySafe) {
        setError("Flash geometry is unsafe; formatting refused");
        return false;
    }
    if (active()) {
        setError("Stop the test before explicitly formatting storage");
        return false;
    }
    LittleFS.end();
    _mounted = false;
    if (!LittleFS.format()) {
        _state = UNAVAILABLE;
        setError("Explicit LittleFS format failed");
        return false;
    }
    _name[0] = '\0';
    _writtenBytes = _writtenLines = _queuedBytes = 0;
    _head = _tail = _count = 0;
    return begin(_geometrySafe);
}

bool FieldTestLog::removeFile(const char* path) {
    if (!_mounted || active()) {
        setError("Stop the test before removing a file");
        return false;
    }
    if (!validFilename(path)) {
        setError("Removal allowed only for /test-######.csv files");
        return false;
    }
    if (!LittleFS.exists(path) || !LittleFS.remove(path)) {
        setError("Requested test file was not found or removal failed");
        return false;
    }
    return refreshUsage();
}
