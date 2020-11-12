#include "image_node.h"

#include <vector>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "application.h"
#include "base/filesystem.h"
#include "base/window.h"
#include "shader_manager.h"
#include "texture_manager.h"

namespace {
static const GLfloat vertices[] = {
    // Pos      // Tex
    0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};
}

using namespace engine;

ImageNode::ImageNode(const std::string& name) : SceneNode(name) {
    glGenVertexArrays(1, &vertex_array_);
}

ImageNode::~ImageNode() {
    glDeleteVertexArrays(1, &vertex_array_);
}

SceneNodeType ImageNode::GetNodeType() const {
    return SceneNodeType::Type_Image;
}

void ImageNode::Render() {
    assert(IsVisible());

    engine::math::ivec2 size(engine::application->GetWindowSize());
    GLfloat window_width = static_cast<GLfloat>(size.x);
    GLfloat window_height = static_cast<GLfloat>(size.y);
    glm::mat4 project =
        glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
    glm::mat4 model = GetTransfromMatrix();
    //
    application->shader_manager()->UseShader("image");
    application->shader_manager()->SetShaderUniform("MVP", project * model);
    application->shader_manager()->SetShaderUniform(
        "image_color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    glBindVertexArray(vertex_array_);
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                          (GLvoid*)0);
    //
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, application->texture_manager()->GetTexture(
                                     "data/pic/cpp.jpg"));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // clear
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vertex_buffer);
    glDisableVertexAttribArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void ImageNode::Update() {
    // no update
}
