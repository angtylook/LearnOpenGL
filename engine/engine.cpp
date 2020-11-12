//
// Created by jxunhappy on 2017/6/30.
//

#include "engine.h"
#include <GL/glew.h>
#include <stdio.h>
#include "GLFW/glfw3.h"
#include "render/gl_debug.h"

namespace engine {
namespace detail {
void OnGLFWError(int error, const char* description) {
    fprintf(stderr, "error: %d, %s\n", error, description);
}
}  // namespace detail

bool InitFileSystem(const std::string& base_directory) {
    return engine::fs::Init(base_directory);
}

bool InitGLFW() {
    // Initialize GLFW
    glfwSetErrorCallback(detail::OnGLFWError);
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return false;
    }
    return true;
}

bool InitOpenGLLibrary() {
    // Initialize GLEW
    glewExperimental = true;  // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return false;
    }
    if (GLEW_ARB_debug_output) {
        fprintf(stdout, "OpenGL debug output support.\n");
        glDebugMessageCallbackARB(gl_debug_output, NULL);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    }

    char* GL_version = (char*)glGetString(GL_VERSION);
    char* GL_vendor = (char*)glGetString(GL_VENDOR);
    char* GL_renderer = (char*)glGetString(GL_RENDERER);
    fprintf(stdout, "GL_version: %s, GL_vendor: %s, GL_renderer: %s.\n",
            GL_version, GL_vendor, GL_renderer);

    return true;
}
}  // namespace engine