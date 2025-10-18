#include "Logger.hpp"

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() {
    logFileInfos.open("log.infos", std::ios::app);
    logFileErrors.open("log.errors", std::ios::app);
    if (!logFileInfos.is_open() || !logFileErrors.is_open()) {
        std::cerr << "ERROR: Unable to open log files." << std::endl;
    }
}

Logger::~Logger() {
    if (logFileInfos.is_open()) {
        logFileInfos.close();
    }
    if (logFileErrors.is_open()) {
        logFileErrors.close();
    }
}

void Logger::logError(const std::string& message) {
    std::lock_guard<std::mutex> guard(mutex);
    std::string fullMessage = "ERROR: " + message;
    std::cerr << fullMessage << std::endl;
    if (logFileErrors.is_open()) {
        logFileErrors << fullMessage << std::endl;
    }
}

void Logger::logInfo(const std::string& message) {
    std::lock_guard<std::mutex> guard(mutex);
    std::string fullMessage = "INFO: " + message;
    std::cout << fullMessage << std::endl;
    if (logFileInfos.is_open()) {
        logFileInfos << fullMessage << std::endl;
    }
}
