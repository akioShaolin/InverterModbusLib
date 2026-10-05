#pragma once
#include "Arduino.h"
#include <algorithm>
#include <limits>
#include <map>
#include <memory>
#include <vector>

struct FSInfo { size_t totalBytes = 0, usedBytes = 0; };
struct LittleFSConfig {
    bool autoFormat = true;
    void setAutoFormat(bool value) { autoFormat = value; }
};
struct FakeNode { std::string data; bool writeError = false; };
struct FakeFS;
extern FakeFS LittleFS;

class File {
public:
    File() = default;
    explicit File(std::shared_ptr<FakeNode> node) : _node(node) {}
    explicit operator bool() const { return static_cast<bool>(_node); }
    size_t write(const uint8_t* data, size_t length);
    void flush();
    void close();
    bool getWriteError() const { return _node && _node->writeError; }
private:
    std::shared_ptr<FakeNode> _node;
};
class Dir {
public:
    Dir() = default;
    explicit Dir(std::vector<std::string> paths) : _paths(paths) {}
    bool next() { return ++_index < static_cast<int>(_paths.size()); }
    String fileName() const { return String(_paths.at(_index)); }
private:
    std::vector<std::string> _paths;
    int _index = -1;
};
struct FakeFS {
    bool mountOk = true, infoOk = true, flushOk = true, autoFormat = true;
    uint32_t ioCalls = 0, formatCalls = 0;
    size_t total = 256 * 1024;
    size_t writeLimit = std::numeric_limits<size_t>::max();
    std::map<std::string, std::shared_ptr<FakeNode>> files;
    void setConfig(const LittleFSConfig& config) { autoFormat = config.autoFormat; }
    bool begin() { ++ioCalls; return mountOk; }
    void end() { ++ioCalls; }
    bool format() { ++ioCalls; ++formatCalls; files.clear(); mountOk = true; return true; }
    bool info(FSInfo& info) {
        ++ioCalls;
        if (!infoOk) return false;
        info.totalBytes = total;
        for (const auto& entry : files) info.usedBytes += entry.second->data.size();
        return true;
    }
    Dir openDir(const char*) {
        ++ioCalls;
        std::vector<std::string> paths;
        for (const auto& entry : files) paths.push_back(entry.first);
        return Dir(paths);
    }
    bool exists(const char* path) { ++ioCalls; return files.count(path); }
    File open(const char* path, const char*) {
        ++ioCalls;
        auto node = std::make_shared<FakeNode>();
        files[path] = node;
        return File(node);
    }
    bool remove(const char* path) { ++ioCalls; return files.erase(path); }
};
inline size_t File::write(const uint8_t* data, size_t length) {
    ++LittleFS.ioCalls;
    const size_t count = std::min(length, LittleFS.writeLimit);
    _node->data.append(reinterpret_cast<const char*>(data), count);
    if (count < length) _node->writeError = true;
    return count;
}
inline void File::flush() {
    ++LittleFS.ioCalls;
    if (!LittleFS.flushOk) _node->writeError = true;
}
inline void File::close() { ++LittleFS.ioCalls; _node.reset(); }
