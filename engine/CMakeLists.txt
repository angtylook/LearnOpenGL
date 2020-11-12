cmake_minimum_required(VERSION 3.10)
project(engine)

include(src.cmake)

add_library(engine STATIC
        ${base}
        ${impl}
        ${lua}
        ${math}
        ${model}
        ${model_loader}
        ${render}
        ${scene}
        ${ui}
        ${root})

target_include_directories(engine
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${RAPIDXML_INCLUDE_DIRS}
        PRIVATE ${LUA_INCLUDE_DIR})

target_link_libraries(engine
        PRIVATE ${OPENGL_LIBRARY}
        PRIVATE ${CORE_FOUNDATION_FRAMEWORK}
        PRIVATE glfw
        PRIVATE GLEW::GLEW
        PRIVATE soil2
        PRIVATE freetype
        PRIVATE ${LUA_LIBRARIES}
        PRIVATE spdlog::spdlog spdlog::spdlog_header_only
        PUBLIC glm
        PUBLIC third_party)
