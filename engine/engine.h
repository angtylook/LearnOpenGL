//
// Created by jxunhappy on 2017/6/30.
//

#ifndef ENGINE_H_
#define ENGINE_H_

#include <string>

#include "base/filesystem.h"

namespace engine {
bool InitFileSystem(const std::string& base_directory);

bool InitGLFW();

bool InitOpenGLLibrary();
}  // namespace engine

#endif  // ENGINE_H
