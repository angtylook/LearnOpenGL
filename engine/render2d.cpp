#include "render2d.h"

#include "application.h"
#include "base/filesystem.h"
#include "base/window.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shader_manager.h"
#include "texture_manager.h"

namespace {
static const GLfloat vertices[] = {
    // Pos      // Tex
    0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};
}

namespace engine {
bool Render2D::Initilize() {
    glGenVertexArrays(1, &vertex_array_);
    glBindVertexArray(vertex_array_);
    // initilize buffer
    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // initilize vertex attribute array
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                          (GLvoid*)0);
    // glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    return true;
}

void Render2D::UnInitilize() {
    glDeleteBuffers(1, &vertex_buffer_);
    glDeleteVertexArrays(1, &vertex_array_);
    vertex_buffer_ = 0;
    vertex_array_ = 0;
}

bool Render2D::AddQuad(const Render2D::Quad& quad) {
    quads_.push_back(quad);
    return true;
}

void Render2D::RenderQuads() {
    glBindVertexArray(vertex_array_);
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    engine::math::ivec2 size(engine::application->GetWindowSize());
    GLfloat window_width = static_cast<GLfloat>(size.x);
    GLfloat window_height = static_cast<GLfloat>(size.y);
    glm::mat4 project =
        glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);

    for (auto quad : quads_) {
        glm::mat4 model = glm::translate(
            glm::mat4(1.0f), glm::vec3(quad.size.x, quad.size.y, 0.0f));
        model = glm::scale(model, glm::vec3(quad.size.z, quad.size.w, 1.0f));
        if (quad.type == Type_Text) {
            application->shader_manager()->UseShader("text");
            application->shader_manager()->SetShaderUniform("text_color",
                                                            quad.color);
            application->shader_manager()->SetShaderUniform(
                "background", glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
        } else {
            application->shader_manager()->UseShader("image");
            application->shader_manager()->SetShaderUniform("image_color",
                                                            quad.color);
        }
        application->shader_manager()->SetShaderUniform("MVP", project * model);
        if (quad.type == Type_Text) {
            glBindTexture(GL_TEXTURE_2D, application->texture_manager()
                                             ->GetCharacter(quad.texture[0])
                                             .TextureID);
        } else {
            glBindTexture(
                GL_TEXTURE_2D,
                application->texture_manager()->GetTexture(quad.texture));
        }
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    // clear
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
}

}
