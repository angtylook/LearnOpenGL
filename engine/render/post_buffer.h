#ifndef RENDER_POST_BUFFER_H_
#define RENDER_POST_BUFFER_H_

#include "GL/glew.h"
#include "base/micros.h"

class PostBuffer {
public:
    PostBuffer();
    ~PostBuffer();

public:
    bool Init(int width, int height);
    GLuint GetTexure();
    GLuint GetRenderBuffer();
    void Render();

private:
    bool InitRender(int width, int height);
    bool InitData();

private:
    GLuint vao_;
    GLuint vbo_;
    GLuint texture_;
    GLuint rbo_;

private:
    DISALLOW_COPY_AND_ASSIGN(PostBuffer)
};

#endif  // RENDER_POST_BUFFER_H_
