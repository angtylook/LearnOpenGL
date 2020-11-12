#include "gl_state.h"
#include <GL/glew.h>

GLState::GLState() {}

GLState::~GLState() {}

void GLState::BindDefaultFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
