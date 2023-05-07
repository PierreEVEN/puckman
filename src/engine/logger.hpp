#pragma once

#include <string>
#include <mutex>
#include <optional>
#include <engine/format.hpp>

// Multiplateform logger utility

// Helper macro - Usages examples :
// INFO("my log");
// WARNING("this is a warning of {}", value);
// FATAL("Program termination");

#if defined(_MSC_VER)
#define DEBUG(format_str, ...) ::pm::Logger::get().message(::pm::ELogLevel::DEBUG, std::format_2(format_str, __VA_ARGS__), __FILE__, ##__FUNCTION__, __LINE__)
#define INFO(format_str, ...) ::pm::Logger::get().message(::pm::ELogLevel::INFO, std::format_2(format_str, __VA_ARGS__), __FILE__, ##__FUNCTION__, __LINE__)
#define WARNING(format_str, ...) ::pm::Logger::get().message(::pm::ELogLevel::WARNING, std::format_2(format_str, __VA_ARGS__), __FILE__, ##__FUNCTION__, __LINE__)
#define ERROR(format_str, ...) ::pm::Logger::get().message(::pm::ELogLevel::ERROR, std::format_2(format_str, __VA_ARGS__), __FILE__, ##__FUNCTION__, __LINE__)
#if !defined(NDEBUG) || defined(_DEBUG)
#define FATAL(format_str, ...) do { ::pm::Logger::get().message(::pm::ELogLevel::FATAL, std::format_2(format_str, __VA_ARGS__), __FILE__, ##__FUNCTION__, __LINE__); __debugbreak(); exit(EXIT_FAILURE); } while (0)
#else
#define FATAL(format_str, ...) do { ::pm::Logger::get().message(::pm::ELogLevel::FATAL, std::format(format_str, __VA_ARGS__), __FILE__, ##__FUNCTION__, __LINE__); exit(EXIT_FAILURE); } while (0)
#endif
#elif (defined(__GNUC__) || defined(__GNUG__)) && !defined(__INTEL_COMPILER)
#define DEBUG(format_str, ...) ::pm::Logger::get().message(::pm::ELogLevel::DEBUG, std::format_2(format_str, ##__VA_ARGS__), __FILE__, __FUNCTION__, __LINE__)
#define INFO(format_str, ...) ::pm::Logger::get().message(::pm::ELogLevel::INFO, std::format_2(format_str, ##__VA_ARGS__), __FILE__, __FUNCTION__, __LINE__)
#define WARNING(format_str, ...) ::pm::Logger::get().message(::pm::ELogLevel::WARNING, std::format_2(format_str, ##__VA_ARGS__), __FILE__, __FUNCTION__, __LINE__)
#define ERROR(format_str, ...) ::pm::Logger::get().message(::pm::ELogLevel::ERROR, std::format_2(format_str, ##__VA_ARGS__), __FILE__, __FUNCTION__, __LINE__)
#define FATAL(format_str, ...) do { ::pm::Logger::get().message(::pm::ELogLevel::FATAL, std::format_2(format_str, ##__VA_ARGS__), __FILE__, __FUNCTION__, __LINE__); exit(EXIT_FAILURE); } while (0)
#elif defined(__clang__)
#define DEBUG(format_str, ...) ::pm::Logger::get().message(::pm::ELogLevel::DEBUG, std::format_2(format_str __VA_OPT__(,) __VA_ARGS__), __FILE__, ##__FUNCTION__, __LINE__)
#define INFO(format_str, ...) ::pm::Logger::get().message(::pm::ELogLevel::INFO, std::format_2(format_str __VA_OPT__(,) __VA_ARGS__), __FILE__, ##__FUNCTION__, __LINE__)
#define WARNING(format_str, ...) ::pm::Logger::get().message(::pm::ELogLevel::WARNING, std::format_2(format_str __VA_OPT__(,) __VA_ARGS__), __FILE__, ##__FUNCTION__, __LINE__)
#define ERROR(format_str, ...) ::pm::Logger::get().message(::pm::ELogLevel::ERROR, std::format_2(format_str __VA_OPT__(,) __VA_ARGS__), __FILE__, ##__FUNCTION__, __LINE__)
#define FATAL(format_str, ...) do { ::pm::Logger::get().message(::pm::ELogLevel::FATAL, std::format_2(format_str __VA_OPT__(,) __VA_ARGS__), __FILE__, ##__FUNCTION__, __LINE__); exit(EXIT_FAILURE); } while (0)
#endif

namespace pm
{
enum class ELogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class Logger
{
public:
    static Logger& get();

    // Add a new log message
    void message(const ELogLevel level, const std::string& text, const std::optional<std::string>& file= {}, const std::optional<std::string>& function = {}, const std::optional<size_t>& line= {});

private:
    // Safety for parallel printing
    std::mutex print_lock;
};
}
