cmake_minimum_required(VERSION 3.10)

# set(CMAKE_TOOLCHAIN_FILE "D:/devtools/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(lotus)

#options
option(BUILD_SHARED_LIBS "Build library as shared" ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_TESTING OFF)
set(CMAKE_CXX_STANDARD 17)

#output dir
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#directory limits
if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
  message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
  message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
  message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif ()

if (WIN32)
  #windows 7
  add_compile_options(/wd4005 /wd4068 /wd4244 /wd4800 /wd4201)
  add_definitions(/D_WIN32_WINNT=0x0601 /DWIN32_LEAN_AND_MEAN=1 /D_CRT_SECURE_NO_WARNINGS)
endif()

#dependencies
#add_subdirectory(third_party)
include(third_party/CMakeLists.txt)
add_subdirectory(engine)

add_executable(lotus example.cpp app.h app.cpp)
target_link_libraries(lotus PRIVATE engine)
