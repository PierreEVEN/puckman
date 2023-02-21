#include "logger.hpp"

#include <iostream>
#include <memory>

namespace pm
{
std::unique_ptr<Logger> LOGGER_SINGLETON;

Logger& Logger::get()
{
    if (!LOGGER_SINGLETON)
        LOGGER_SINGLETON = std::make_unique<Logger>();

    return *LOGGER_SINGLETON;
}

void Logger::message(const ELogLevel level, const std::string& text, const std::optional<std::string>& file, const std::optional<std::string>& function, const std::optional<size_t>& line)
{
    std::lock_guard lock_guard(print_lock);
    switch (level)
    {
    case ELogLevel::DEBUG:
        std::cout << "[D] " << (function ? *function : "?") << "::" << (line ? std::to_string(*line) : "?") << " | ";
        break;
    case ELogLevel::INFO:
        std::cout << "[I] ";
        break;
    case ELogLevel::WARNING:
        std::cout << "[W] " << (function ? *function : "?") << "::" << (line ? std::to_string(*line) : "?") << " | ";
        break;
    case ELogLevel::ERROR:
        std::cout << "[E] " << (function ? *function : "?") << "::" << (line ? std::to_string(*line) : "?") << " | ";
        break;
    case ELogLevel::FATAL:
        std::cout << "[F] " << (function ? *function : "?") << "::" << (line ? std::to_string(*line) : "?") << " | ";
        break;
    }

    std::cout << text << std::endl;

    if (level == ELogLevel::ERROR || level == ELogLevel::FATAL)
        std::cout << "\t=>" << (file ? *file : "?") << std::endl;
}
}
