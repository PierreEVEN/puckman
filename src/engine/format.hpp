#pragma once

#include <sstream>
#include <string>
#include <vector>

// std format replacement
#define format format_2

namespace std
{
inline std::vector<std::string> format_arguments()
{
    return {};
}

template <typename T, typename... Args> std::vector<std::string> format_arguments(T&& arg)
{
    std::stringstream arg_str;
    arg_str << arg;
    std::vector vec{arg_str.str()};
    return vec;
}

template <typename T, typename... Args> std::vector<std::string> format_arguments(T&& arg, Args&&...args)
{
    std::stringstream arg_str;
    arg_str << arg;
    auto vec = format_arguments(std::forward<Args>(args)...);
    vec.emplace_back(arg_str.str());
    return vec;
}

template <typename... Args> std::string format_2(const std::string& format, Args ...args)
{
    std::stringstream str;

    const std::vector<std::string> arguments = format_arguments(std::forward<Args>(args)...);
    size_t                         arg_index = arguments.size();
    bool                           skip      = false;

    for (const auto& chr : format)
    {
        switch (chr)
        {
        case '\\':
            if (skip)
            {
                str << chr;
                skip = false;
                break;
            }
            skip = true;
            break;
        case '{':
            if (skip)
            {
                str << chr;
                skip = false;
                break;
            }
            if (arg_index == 0)
                throw std::invalid_argument("Only " + std::to_string(arguments.size()) + " were provided, but more are required");
            str << arguments[--arg_index];
            break;
        case '}':
            if (skip)
            {
                str << chr;
                skip = false;
                break;
            }
            break;
        default:
            if (skip)
            {
                skip = false;
                break;
            }
            str << chr;
            break;
        }
    }
    return str.str();
}
} // namespace std