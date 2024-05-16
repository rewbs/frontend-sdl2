#ifndef FILE_MONITOR_H
#define FILE_MONITOR_H

#include <iostream>
#include <functional>
#include <thread>
#include <chrono>
#include <atomic>
#include <sys/stat.h>

class FileMonitor {
public:
    FileMonitor(const std::string& path, std::function<void()> callback, std::chrono::milliseconds interval);
    ~FileMonitor();
    bool poll();

private:
    std::string path_;
    std::function<void()> callback_;
    std::chrono::milliseconds interval_;
    std::thread monitorThread_;
    std::atomic<bool> running_;
    time_t lastModifiedTime_;

    void monitor();
    std::time_t getLastModifiedTime(const std::string& path);
};

#endif // FILE_MONITOR_H