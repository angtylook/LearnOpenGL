#ifndef RENDER_GL_STATE_H_
#define RENDER_GL_STATE_H_

#include "base/micros.h"

class GLState {
public:
    GLState();
    ~GLState();

public:
    static void BindDefaultFrameBuffer();

private:
private:
    DISALLOW_COPY_AND_ASSIGN(GLState)
};

#endif  // RENDER_GL_STATE_H_
