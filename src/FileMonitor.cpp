#include "FileMonitor.h"

// Dumb implementation of a file monitor that checks the last modified time of a file every interval
// milliseconds and calls a callback if the file has been modified.
// We could do something smarter like using inotify on Linux or ReadDirectoryChangesW on Windows but
// this is just a simple way to keep it cross-platform.
//
// TODO: Monitoring on separate thread is disabled. Trying to force a preset reload from 
// a new thread causes crashes.
FileMonitor::FileMonitor(const std::string& path, std::function<void()> callback, std::chrono::milliseconds interval)
    : path_(path), callback_(callback), interval_(interval), running_(true) {
    // monitorThread_ = std::thread([this]() { this->monitor(); });
}

FileMonitor::~FileMonitor() {
    running_.store(false);
    // if (monitorThread_.joinable()) {
    //     monitorThread_.join();
    // }
}

void FileMonitor::monitor() {
    auto lastModifiedTime = getLastModifiedTime(path_);

    while (running_.load()) {
        std::this_thread::sleep_for(interval_);
        auto currentModifiedTime = getLastModifiedTime(path_);
        std::cout << "Checking file: " << path_ << std::endl;
        if (currentModifiedTime != lastModifiedTime) {
            std::cout << "Modification found" << std::endl;
            lastModifiedTime = currentModifiedTime;
            callback_();
        }
    }
}

bool FileMonitor::poll() {
    //std::cout << "Polling file: " << path_ << std::endl;
    auto currentModifiedTime = getLastModifiedTime(path_);
    if (currentModifiedTime != lastModifiedTime_) {
        lastModifiedTime_ = currentModifiedTime;
        callback_();
        return true;
    }
    
    return false;
}

std::time_t FileMonitor::getLastModifiedTime(const std::string& path) {
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) == 0) {
        return fileInfo.st_mtime;
    } else {
        return 0;
    }
}