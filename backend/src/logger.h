#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <memory>
#include <chrono>
#include <iomanip>
#include <sstream>

/**
 * @brief A simple thread-safe file logger.
 *
 * This logger class was developed by ChatGPT specifically for the lunar lander simulation project.
 * It supports logging messages with timestamps to a file in a thread-safe manner.
 * The logger can be used from multiple threads, such as Qt worker threads.
 */
class Logger
{
public:
    /**
     * @brief Get the singleton instance of the Logger.
     *
     * @return Logger& Reference to the logger instance.
     */
    static Logger& instance();

    /**
     * @brief Initialize the logger with a file path.
     *
     * @param filePath Path to the log file.
     */
    void init(const std::string& filePath);

    /**
     * @brief Log a message to the file with timestamp.
     *
     * @param message The message to log.
     */
    void log(const std::string& message);

private:
    Logger() = default; // private constructor for singleton
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::ofstream logFile_;
    std::mutex mutex_;

    std::string currentTimestamp() const;
};
