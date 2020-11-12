#ifndef RENDER_GL_SHADER_H_
#define RENDER_GL_SHADER_H_

#include <GL/glew.h>

GLuint LoadShaders(const char* vertex_file_path,
                   const char* fragment_file_path);

#endif  // RENDER_GL_SHADER_H_
