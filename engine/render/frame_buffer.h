#ifndef RENDER_FRAME_BUFFER_H_
#define RENDER_FRAME_BUFFER_H_

#include "GL/glew.h"
#include "base/micros.h"

class FrameBuffer {
public:
    FrameBuffer();
    ~FrameBuffer();

public:
    bool IsComplete();
    bool Init();
    void Bind();
    void BindForRead();
    void BindForWrite();

    void AttachTexture(GLenum attachment, GLuint texture);
    void AttachRenderBuffer(GLenum attachment, GLuint rbo);

    void Destroy();

private:
    GLuint fbo_;

private:
    DISALLOW_COPY_AND_ASSIGN(FrameBuffer)
};

#endif  // RENDER_FRAME_BUFFER_H_
