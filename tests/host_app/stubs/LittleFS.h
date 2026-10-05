#pragma once
#include "Arduino.h"
#include <algorithm>
#include <limits>
#include <map>
#include <memory>
#include <vector>

struct FSInfo { size_t totalBytes = 0, usedBytes = 0; };
struct LittleFSConfig { bool autoFormat = true; void setAutoFormat(bool v) { autoFormat = v; } };
struct FakeNode { std::string data; bool writeError = false; };
struct FakeFS;
extern FakeFS LittleFS;
class File {
public:
    File() = default;
    explicit File(std::shared_ptr<FakeNode> node) : node(node) {}
    explicit operator bool() const { return static_cast<bool>(node); }
    size_t write(const uint8_t* bytes, size_t length);
    void flush();
    void close();
    size_t size() const { return node ? node->data.size() : 0; }
    bool getWriteError() const { return node && node->writeError; }
private:
    std::shared_ptr<FakeNode> node;
};
class Dir {
public:
    explicit Dir(std::vector<std::pair<std::string, size_t>> entries) : entries(entries) {}
    bool next() { return ++index < static_cast<int>(entries.size()); }
    String fileName() const { return String(entries.at(index).first); }
    size_t fileSize() const { return entries.at(index).second; }
private:
    std::vector<std::pair<std::string, size_t>> entries;
    int index = -1;
};
struct FakeFS {
    bool flushOk = true;
    uint32_t ioCalls = 0;
    size_t writeLimit = std::numeric_limits<size_t>::max();
    std::map<std::string, std::shared_ptr<FakeNode>> files;
    void setConfig(const LittleFSConfig&) {}
    bool begin() { ++ioCalls; return true; }
    void end() { ++ioCalls; }
    bool format() { ++ioCalls; files.clear(); return true; }
    bool info(FSInfo& info) {
        ++ioCalls; info.totalBytes = 256 * 1024; info.usedBytes = 0;
        for (const auto& entry : files) info.usedBytes += entry.second->data.size();
        return true;
    }
    Dir openDir(const char*) {
        ++ioCalls; std::vector<std::pair<std::string, size_t>> entries;
        for (const auto& e : files) entries.push_back({e.first, e.second->data.size()});
        return Dir(entries);
    }
    bool exists(const char* path) { ++ioCalls; return files.count(path); }
    File open(const char* path, const char* mode) {
        ++ioCalls;
        if (mode[0] == 'r') return files.count(path) ? File(files.at(path)) : File();
        auto node = std::make_shared<FakeNode>(); files[path] = node; return File(node);
    }
    File open(const String& path, const char* mode) { return open(path.c_str(), mode); }
    bool remove(const char* path) { ++ioCalls; return files.erase(path); }
};
inline size_t File::write(const uint8_t* bytes, size_t length) {
    ++LittleFS.ioCalls; size_t n = std::min(length, LittleFS.writeLimit);
    node->data.append(reinterpret_cast<const char*>(bytes), n);
    if (n < length) node->writeError = true;
    return n;
}
inline void File::flush() { ++LittleFS.ioCalls; if (!LittleFS.flushOk) node->writeError = true; }
inline void File::close() { ++LittleFS.ioCalls; node.reset(); }
