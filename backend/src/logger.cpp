#include "logger.h"

Logger& Logger::instance()
{
    static Logger logger;
    return logger;
}

void Logger::init(const std::string& filePath)
{
    std::lock_guard<std::mutex> lock(mutex_);
    logFile_.open(filePath, std::ios::out | std::ios::app);
    if (logFile_.is_open())
    {
        log("Logger initialized.");
    }
    else
    {
        throw std::runtime_error("Failed to open log file: " + filePath);
    }
}

Logger::~Logger()
{
    if (logFile_.is_open())
    {
        log("Logger terminated.");
        logFile_.close();
    }
}

void Logger::log(const std::string& message)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (logFile_.is_open())
    {
        logFile_ << currentTimestamp() << " - " << message << std::endl;
    }
}

std::string Logger::currentTimestamp() const
{
    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  now.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&timeT), "%Y-%m-%d %H:%M:%S")
        << "." << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}
