#include "frame_buffer.h"

FrameBuffer::FrameBuffer() : fbo_(0) {}

bool FrameBuffer::IsComplete() {
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

bool FrameBuffer::Init() {
    glGenFramebuffers(1, &fbo_);
    return fbo_ != 0;
}

void FrameBuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
}

void FrameBuffer::BindForRead() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_);
}

void FrameBuffer::BindForWrite() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_);
}

void FrameBuffer::AttachTexture(GLenum attachment, GLuint texture) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture,
                           0);
}

void FrameBuffer::AttachRenderBuffer(GLenum attachment, GLuint rbo) {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo);
}

FrameBuffer::~FrameBuffer() {
    Destroy();
}

void FrameBuffer::Destroy() {
    glDeleteFramebuffers(1, &fbo_);
}
