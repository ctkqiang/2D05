#include "logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

Logger::Logger() {
    currentLogFile = "app.log";
    logFile.open(currentLogFile, std::ios::app);
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::setLogFile(const std::string& filename) {
    if (logFile.is_open()) {
        logFile.close();
    }
    currentLogFile = filename;
    logFile.open(currentLogFile, std::ios::app);
}

std::string Logger::getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string Logger::getLevelString(LogLevel level) {
    switch (level) {
        case INFO:    return "INFO";
        case WARNING: return "WARNING";
        case ERROR:   return "ERROR";
        case DEBUG:   return "DEBUG";
        default:      return "UNKNOWN";
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    std::string timestamp = getTimestamp();
    std::string levelStr = getLevelString(level);
    
    std::string logMessage = timestamp + " [" + levelStr + "] " + message + "\n";
    
    if (logFile.is_open()) {
        logFile << logMessage;
        logFile.flush();
    }
    
    std::cout << logMessage;
}