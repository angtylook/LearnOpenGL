#ifndef RENDER_GL_DEBUG_H_
#define RENDER_GL_DEBUG_H_

#include "GL/glew.h"

GLenum glCheckError_(const char* file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void GLAPIENTRY gl_debug_output(GLenum source,
                                GLenum type,
                                GLenum id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam);

#endif  // RENDER_GL_DEBUG_H_
