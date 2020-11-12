//
// Created by jxunhappy on 2017/7/8.
//

#ifndef BASE_LOG_H_
#define BASE_LOG_H_

#include <string>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "third_party/tinyformat.h"

namespace engine {
namespace log {
namespace detail {
extern std::shared_ptr<spdlog::logger> loger;
}

template <typename... Args>
void verbose(const char* tag, const char* format, Args... args) {
    std::string msg = tinyformat::format(format, args...);
    detail::loger->trace("[{}] {}", tag, msg);
}

template <typename... Args>
void debug(const char* tag, const char* format, Args... args) {
    std::string msg = tinyformat::format(format, args...);
    detail::loger->debug("[{}] {}", tag, msg);
}

template <typename... Args>
void info(const char* tag, const char* format, Args... args) {
    std::string msg = tinyformat::format(format, args...);
    detail::loger->info("[{}] {}", tag, msg);
}

template <typename... Args>
void warn(const char* tag, const char* format, Args... args) {
    std::string msg = tinyformat::format(format, args...);
    detail::loger->warn("[{}] {}", tag, msg);
}

template <typename... Args>
void error(const char* tag, const char* format, Args... args) {
    std::string msg = tinyformat::format(format, args...);
    detail::loger->error("[{}] {}", tag, msg);
}

template <typename... Args>
void fatal(const char* tag, const char* format, Args... args) {
    std::string msg = tinyformat::format(format, args...);
    detail::loger->critical("[{}] {}", tag, msg);
}

template <typename... Args>
void verbose_if(bool cond, const char* tag, const char* format, Args... args) {
    if (cond) {
        std::string msg = tinyformat::format(format, args...);
        detail::loger->debug("[{}] {}", tag, msg);
    }
}

template <typename... Args>
void debug_if(bool cond, const char* tag, const char* format, Args... args) {
    if (cond) {
        std::string msg = tinyformat::format(format, args...);
        detail::loger->debug("[{}] {}", tag, msg);
    }
}

template <typename... Args>
void info_if(bool cond, const char* tag, const char* format, Args... args) {
    if (cond) {
        std::string msg = tinyformat::format(format, args...);
        detail::loger->info("[{}] {}", tag, msg);
    }
}

template <typename... Args>
void warn_if(bool cond, const char* tag, const char* format, Args... args) {
    if (cond) {
        std::string msg = tinyformat::format(format, args...);
        detail::loger->warn("[{}] {}", tag, msg);
    }
}

template <typename... Args>
void error_if(bool cond, const char* tag, const char* format, Args... args) {
    if (cond) {
        std::string msg = tinyformat::format(format, args...);
        detail::loger->error("[{}] {}", tag, msg);
    }
}

template <typename... Args>
void fatal_if(bool cond, const char* tag, const char* format, Args... args) {
    if (cond) {
        std::string msg = tinyformat::format(format, args...);
        detail::loger->critical("[{}] {}", tag, msg);
    }
}

bool Initialize();

}  // namespace log

}  // namespace engine

#endif  // BASE_LOG_H_
