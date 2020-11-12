//
// Created by jxunhappy on 2017/7/8.
//

#include <stdio.h>

#include "log.h"
namespace engine {
namespace log {
namespace detail {
std::shared_ptr<spdlog::logger> loger;
}

bool Initialize() {
    detail::loger = spdlog::stdout_color_mt("console");
    if (detail::loger) {
        detail::loger->set_level(spdlog::level::trace);
        info("Engine", "log system initialized!");
        return true;
    }
    fprintf(stderr, "Initialize log system failed.");
    return false;
}

}  // namespace log
}  // namespace engine
