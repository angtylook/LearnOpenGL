#find OpenGL support
find_package(OpenGL REQUIRED)
if (APPLE)
    find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
endif ()

find_path(RAPIDXML_INCLUDE_DIRS "rapidxml/rapidxml.hpp")

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(soil2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
include(FindLua)
find_package(Lua REQUIRED)

add_library(third_party INTERFACE)
target_include_directories(third_party INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
