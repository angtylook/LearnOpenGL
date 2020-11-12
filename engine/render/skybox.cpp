#include "skybox.h"

#include <array>
#include "SOIL2/SOIL2.h"
#include "application.h"
#include "base/filesystem.h"
#include "base/helper.h"
#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "scene_manager.h"
#include "shader_manager.h"

using namespace engine;

namespace {
const GLfloat vertices[] = {
    // cube point, above y
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
    // below y
    1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
    1.0f};

const GLuint indexs[]{// +x
                      0, 1, 5, 0, 5, 4,
                      // -x
                      3, 7, 6, 3, 6, 2,
                      // +y
                      0, 3, 2, 0, 2, 1,
                      // -y
                      4, 5, 6, 4, 6, 7,
                      // +z
                      0, 4, 7, 0, 7, 3,
                      // -z
                      1, 2, 6, 1, 6, 5};
const std::array<std::string, 6> subfixs = {{"right.jpg", "left.jpg", "top.jpg",
                                             "bottom.jpg", "back.jpg",
                                             "front.jpg"}};
}  // namespace

Skybox::Skybox() : vao_(0), vbo_(0), ebo_(0), texture_(0) {}

Skybox::~Skybox() {
    if (vao_ != 0) {
        glDeleteVertexArrays(1, &vao_);
        vao_ = 0;
    }
    if (vbo_ != 0) {
        glDeleteBuffers(1, &vbo_);
        vbo_ = 0;
    }
    if (ebo_ != 0) {
        glDeleteBuffers(1, &ebo_);
        ebo_ = 0;
    }
    if (texture_ != 0) {
        glDeleteTextures(1, &texture_);
        texture_ = 0;
    }
}

void Skybox::Init(const std::string& texture) {
    // begin vao
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    // vertex
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    // element
    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs,
                 GL_STATIC_DRAW);
    glBindVertexArray(0);
    // cube map
    SetCubeTexture(texture);
}

void Skybox::SetCubeTexture(const std::string& texture) {
    if (texture_ != 0) {
        glDeleteTextures(1, &texture_);
    }
    glGenTextures(1, &texture_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_);
    int width = 0;
    int height = 0;
    unsigned char* image = nullptr;
    for (GLuint i = 0; i < subfixs.size(); ++i) {
        engine::fs::Path path(texture);
        path = path.Append(subfixs[i]);

        image = SOIL_load_image(engine::fs::GetRawPath(path).data(), &width,
                                &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
                     height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
        image = nullptr;
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Skybox::Render() {
    glDepthMask(GL_FALSE);
    application->shader_manager()->UseShader("skybox");

    glm::mat4 ProjectionMatrix =
        application->scene_manager()->GetCamera()->GetProjectMatrix();
    application->shader_manager()->SetShaderUniform("project",
                                                    ProjectionMatrix);
    glm::mat4 ViewMatrix = glm::mat4(
        glm::mat3(application->scene_manager()->GetCamera()->GetViewMatrix()));
    application->shader_manager()->SetShaderUniform("view", ViewMatrix);
    application->shader_manager()->SetShaderUniform("model", glm::mat4(1.0f));
    glBindVertexArray(vbo_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_);
    application->shader_manager()->SetShaderUniform("skybox", 0);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glDepthMask(GL_TRUE);
}
