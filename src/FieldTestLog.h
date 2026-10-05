#pragma once

#include <Arduino.h>
#include <LittleFS.h>

// A bounded RAM queue keeps filesystem access out of Modbus callbacks. All
// synchronous methods (begin/start/format/remove) require the caller to have
// stopped the test and made the bus idle. service() enforces that for writes.
class FieldTestLog {
public:
    enum State : uint8_t { UNAVAILABLE, STOPPED, RECORDING, STOPPING, FAULT };
    static constexpr uint8_t QUEUE_CAPACITY = 8;
    static constexpr size_t MAX_LINE_BYTES = 512; // Includes newline and NUL.
    static constexpr uint32_t MAX_FILE_BYTES = 192UL * 1024UL;
    static constexpr uint32_t STORAGE_RESERVE_BYTES = 16UL * 1024UL;
    static constexpr uint32_t COMMAND_HEADROOM_BYTES = 8UL * 1024UL;

    bool begin(bool geometrySafe = true);
    bool start(const char* csvHeader);
    // line must contain one CSV row without newline, max MAX_LINE_BYTES - 2.
    // critical rows (command results/session end) may use reserved headroom
    // and may be queued during STOPPING. No filesystem calls occur here.
    bool appendCsvLine(const char* line, bool critical = false);
    void service(bool busIdle);
    void requestStop();
    bool formatStorage();
    bool removeFile(const char* path);

    bool active() const { return _state == RECORDING || _state == STOPPING; }
    bool mounted() const { return _mounted; }
    bool canRecord() const;
    bool canAcceptCommand() const;
    State state() const { return _state; }
    const char* stateName() const;
    const char* currentName() const { return _name; }
    const char* lastError() const { return _error; }
    uint8_t queued() const { return _count; }
    uint32_t dropped() const { return _dropped; }
    uint32_t ioErrors() const { return _ioErrors; }
    uint32_t capacityStops() const { return _capacityStops; }
    uint32_t size() const { return _writtenBytes; }
    uint32_t writtenLines() const { return _writtenLines; }
    uint32_t maxWriteMicros() const { return _maxWriteMicros; }
    uint32_t capacityBytes() const { return _capacityBytes; }
    uint32_t usedBytes() const { return _usedBytes; }
    uint32_t availableBytes() const {
        return _capacityBytes > _usedBytes ? _capacityBytes - _usedBytes : 0;
    }
    static bool validFilename(const char* path);

private:
    struct Row { char text[MAX_LINE_BYTES]; uint16_t length; };
    Row _queue[QUEUE_CAPACITY]{};
    File _file;
    State _state = UNAVAILABLE;
    bool _mounted = false;
    bool _geometrySafe = false;
    bool _dirty = false;
    uint8_t _head = 0, _tail = 0, _count = 0;
    uint32_t _queuedBytes = 0, _writtenBytes = 0, _writtenLines = 0;
    uint32_t _dropped = 0, _ioErrors = 0, _capacityStops = 0;
    uint32_t _maxWriteMicros = 0, _lastFlushMs = 0;
    uint32_t _capacityBytes = 0, _usedBytes = 0;
    char _name[24]{};
    char _error[112]{};

    void setError(const char* message);
    void failIo(const char* message);
    bool refreshUsage();
    bool budgetAllows(uint32_t bytes, bool critical) const;
    bool flushFile();
    void measureIo(uint32_t startedUs);
};
