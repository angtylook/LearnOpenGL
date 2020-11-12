#include "post_buffer.h"

#include "application.h"
#include "base/window.h"
#include "glm/gtc/matrix_transform.hpp"
#include "shader_manager.h"

namespace {
static const GLfloat vertices[] = {
    // Pos      // Tex
    0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

    0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
};
}

using namespace engine;

PostBuffer::PostBuffer() : vao_(0), vbo_(0), texture_(0), rbo_(0) {}

PostBuffer::~PostBuffer() {
    if (vao_ != 0) {
        glDeleteVertexArrays(1, &vao_);
        vao_ = 0;
    }
    if (vbo_ != 0) {
        glDeleteBuffers(1, &vbo_);
        vbo_ = 0;
    }
    if (texture_ != 0) {
        glDeleteTextures(1, &texture_);
        texture_ = 0;
    }
    if (rbo_ != 0) {
        glDeleteRenderbuffers(1, &rbo_);
        rbo_ = 0;
    }
}

bool PostBuffer::Init(int width, int height) {
    return InitData() && InitRender(width, height);
}

bool PostBuffer::InitData() {
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    // initilize buffer
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // initilize vertex attribute array
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    return true;
}

bool PostBuffer::InitRender(int width, int height) {
    // color texture
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // depth and stencil
    glGenRenderbuffers(1, &rbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    return texture_ != 0 && rbo_ != 0;
}

GLuint PostBuffer::GetTexure() {
    return texture_;
}

GLuint PostBuffer::GetRenderBuffer() {
    return rbo_;
}

void PostBuffer::Render() {
    glBindVertexArray(vao_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, GetTexure());
    glDisable(GL_DEPTH_TEST);

    engine::math::ivec2 size(engine::application->GetWindowSize());
    GLfloat window_width = static_cast<GLfloat>(size.x);
    GLfloat window_height = static_cast<GLfloat>(size.y);
    glm::mat4 project =
        glm::ortho(0.0f, window_width, 0.0f, window_height, -1.0f, 1.0f);
    glm::mat4 model(1.0f);
    model = glm::scale(model, glm::vec3(window_width, window_height, 1.0f));
    application->shader_manager()->UseShader("post");
    application->shader_manager()->SetShaderUniform("project", project);
    application->shader_manager()->SetShaderUniform("model", model);
    application->shader_manager()->SetShaderUniform("image", 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // clear
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
}
