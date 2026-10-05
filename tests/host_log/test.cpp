// Host invariant tests, not a substitute for ESP8266 flash/bus timing tests.
// Build: g++ -std=c++17 -Wall -Wextra -Werror -Itests/host_log -Isrc
// (same command) tests/host_log/test.cpp src/FieldTestLog.cpp -o /tmp/field-test-log
// Run: /tmp/field-test-log
#include "FieldTestLog.h"
#include <cassert>
#include <iostream>

FakeFS LittleFS;
uint32_t fakeMillis = 0;
uint32_t millis() { return fakeMillis; }
uint32_t micros() { static uint32_t us = 0; return ++us; }

static void reset() { LittleFS = FakeFS(); fakeMillis = 0; }
static void addFile(const char* name, const std::string& content) {
    auto node = std::make_shared<FakeNode>();
    node->data = content;
    LittleFS.files[name] = node;
}

int main() {
    reset();
    {
        FieldTestLog log;
        assert(!log.begin(false));
        assert(!log.formatStorage());
        assert(LittleFS.ioCalls == 0);
    }
    reset();
    {
        LittleFS.mountOk = false;
        FieldTestLog log;
        assert(!log.begin());
        assert(!LittleFS.autoFormat);
        assert(LittleFS.formatCalls == 0);
        assert(log.state() == FieldTestLog::UNAVAILABLE);
        assert(log.formatStorage());
        assert(LittleFS.formatCalls == 1);
    }
    reset();
    {
        addFile("/test-000041.csv", "old\n");
        addFile("/something-999999.csv", "unrelated\n");
        FieldTestLog log;
        assert(log.begin());
        assert(log.start("event,value"));
        assert(std::string(log.currentName()) == "/test-000042.csv");
        const uint32_t before = LittleFS.ioCalls;
        assert(log.appendCsvLine("read,42"));
        log.service(false);
        assert(LittleFS.ioCalls == before); // append and bus-busy service do no IO.
        assert(log.queued() == 2);
        assert(!log.formatStorage());
        assert(!log.removeFile("/test-000041.csv"));
        log.requestStop();
        assert(log.appendCsvLine("command_done,100", true));
        log.service(true);
        assert(log.queued() == 2); // Exactly one row written per service.
        log.service(true);
        assert(log.state() == FieldTestLog::STOPPING);
        log.service(true);
        assert(log.state() == FieldTestLog::STOPPED);
        assert(log.writtenLines() == 3);
        assert(log.maxWriteMicros() > 0);
        assert(LittleFS.files.at("/test-000042.csv")->data == "event,value\nread,42\ncommand_done,100\n");
        assert(LittleFS.files.at("/test-000041.csv")->data == "old\n");
        assert(!log.removeFile("/../test-000041.csv"));
        assert(log.removeFile("/test-000041.csv"));
        assert(log.start("event,value"));
        assert(std::string(log.currentName()) == "/test-000043.csv");
        log.requestStop();
        log.service(true);
    }
    reset();
    {
        FieldTestLog log;
        assert(log.begin());
        assert(log.start("event"));
        const uint32_t before = LittleFS.ioCalls;
        for (int i = 1; i < FieldTestLog::QUEUE_CAPACITY; ++i)
            assert(log.appendCsvLine("queued"));
        assert(!log.appendCsvLine("overflow"));
        assert(log.dropped() == 1);
        assert(!log.canAcceptCommand());
        assert(LittleFS.ioCalls == before);
        log.service(true);
        assert(log.queued() == 7);
        assert(!log.appendCsvLine(std::string(511, 'x').c_str()));
        assert(!log.appendCsvLine("bad\nrow"));
        assert(!log.appendCsvLine(nullptr));
        assert(log.dropped() == 4);
        log.requestStop();
        while (log.active()) log.service(true);
    }
    reset();
    {
        FieldTestLog log;
        assert(log.begin());
        assert(log.start("event"));
        assert(log.appendCsvLine("result"));
        LittleFS.writeLimit = 2;
        log.service(true);
        assert(log.state() == FieldTestLog::FAULT);
        assert(log.dropped() == 2);
        assert(log.ioErrors() == 1);
        assert(log.queued() == 0);
        assert(log.size() == 2);
        assert(LittleFS.files.at(log.currentName())->data == "ev");
        assert(!log.canAcceptCommand());
    }
    reset();
    {
        FieldTestLog log;
        assert(log.begin());
        assert(log.start("event"));
        log.service(true);
        fakeMillis = 1000;
        LittleFS.flushOk = false;
        log.service(true);
        assert(log.state() == FieldTestLog::FAULT);
        assert(std::string(log.lastError()).find("flush failed") != std::string::npos);
    }
    reset();
    {
        FieldTestLog log;
        assert(log.begin());
        assert(log.start("event"));
        log.service(true);
        const std::string largeRow(510, 'x');
        while (log.appendCsvLine(largeRow.c_str())) log.service(true);
        assert(log.state() == FieldTestLog::STOPPING);
        assert(log.capacityStops() == 1);
        assert(log.size() < FieldTestLog::MAX_FILE_BYTES);
        assert(log.appendCsvLine("command_result_still_fits", true));
        log.service(true);
        assert(log.state() == FieldTestLog::STOPPED);
        assert(log.availableBytes() >= FieldTestLog::STORAGE_RESERVE_BYTES);
    }
    reset();
    {
        addFile("/test-000001.csv", std::string(LittleFS.total - 25200, 'a'));
        FieldTestLog log;
        assert(log.begin());
        assert(log.start("event"));
        log.service(true);
        const std::string largeRow(510, 'x');
        while (log.appendCsvLine(largeRow.c_str())) log.service(true);
        assert(log.state() == FieldTestLog::STOPPING);
        assert(log.appendCsvLine("reserved_command_result", true));
        log.service(true);
        assert(log.state() == FieldTestLog::STOPPED);
        assert(log.availableBytes() >= FieldTestLog::STORAGE_RESERVE_BYTES);
        assert(log.size() < 1000);
    }
    reset();
    {
        addFile("/test-999999.csv", "preserved");
        FieldTestLog log;
        assert(log.begin());
        assert(!log.start("event"));
        assert(LittleFS.files.at("/test-999999.csv")->data == "preserved");
    }
    std::cout << "FieldTestLog host invariants: PASS\n";
}
