#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

class Logger {
   public:
    enum LogLevel { INFO, WARNING, ERROR, DEBUG };

    static Logger& getInstance();
    void log(LogLevel level, const std::string& message);
    void setLogFile(const std::string& filename);

   private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::ofstream logFile;
    std::string currentLogFile;
    std::string getTimestamp();
    std::string getLevelString(LogLevel level);
};

#endif