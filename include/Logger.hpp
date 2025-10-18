#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>

/**
 * @class Logger
 * @brief Singleton Logger class for logging information and errors.
 * 
 * This logger creates and writes to two separate log files: log.infos and log.errors.
 */
class Logger {
public:
    /**
     * @brief Retrieves the singleton instance of the Logger.
     * @return Reference to the singleton Logger instance.
     */
    static Logger& getInstance();

    // Deleted copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    /**
     * @brief Logs an error message to the console and log.errors file.
     * @param message The error message to log.
     */
    void logError(const std::string& message);

    /**
     * @brief Logs an informational message to the console and log.infos file.
     * @param message The info message to log.
     */
    void logInfo(const std::string& message);

private:
    Logger();
    ~Logger();

    std::mutex mutex;
    std::ofstream logFileInfos;
    std::ofstream logFileErrors;
};
