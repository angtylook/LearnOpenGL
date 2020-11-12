#include "text_node.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "application.h"
#include "base/filesystem.h"
#include "base/window.h"

#include "shader_manager.h"
#include "texture_manager.h"

using namespace engine;
TextNode::TextNode(const std::string& name)
    : SceneNode(name)
    , x_(0.0f)
    , y_(0.0f)
    , scale_(1.0f)
    , color_(0.0f, 0.0f, 0.0f, 1.0f)
    , background_(1.0f, 1.0f, 1.0f, 0.0f) {
    glGenVertexArrays(1, &vertex_array_);
}

TextNode::~TextNode() {
    glDeleteVertexArrays(1, &vertex_array_);
}

SceneNodeType TextNode::GetNodeType() const {
    return SceneNodeType::Type_Text;
}

void TextNode::Render() {
    assert(IsVisible());
    glBindVertexArray(vertex_array_);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    engine::math::ivec2 size(engine::application->GetWindowSize());
    GLfloat window_width = static_cast<GLfloat>(size.x);
    GLfloat window_height = static_cast<GLfloat>(size.y);
    glm::mat4 mvp = glm::ortho(0.0f, window_width, window_height, 0.0f);
    // glm::mat4 mvp = glm::ortho(0.0f, window_width, 0.0f, window_height);
    // setup shader
    application->shader_manager()->UseShader("text");
    application->shader_manager()->SetShaderUniform("MVP", mvp);
    application->shader_manager()->SetShaderUniform("text_color", color_);
    application->shader_manager()->SetShaderUniform("background", background_);
    glActiveTexture(GL_TEXTURE0);
    // setup buffer
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    // the 2D quad requires 6 vertics(2 Triangle) of 4 floats, reserv memory.
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL,
                 GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    // draw
    GLfloat top = static_cast<GLfloat>(
        application->texture_manager()->GetCharacter('H').Bearing.y);
    GLfloat x = x_;
    GLfloat y = y_;
    for (auto c : text_) {
        Character ch = application->texture_manager()->GetCharacter(c);
        GLfloat xpos = x + ch.Bearing.x * scale_;
        GLfloat ypos = y + (top - ch.Bearing.y) * scale_;
        GLfloat w = ch.Size.x * scale_;
        GLfloat h = ch.Size.y * scale_;
        // update vertex buffer for each character
        GLfloat vertices[6][4] = {
            {xpos, ypos + h, 0.0, 1.0}, {xpos + w, ypos, 1.0, 0.0},
            {xpos, ypos, 0.0, 0.0},

            {xpos, ypos + h, 0.0, 1.0}, {xpos + w, ypos + h, 1.0, 1.0},
            {xpos + w, ypos, 1.0, 0.0}};
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of vertex buffer object memory
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.Advance >> 6) *
             scale_;  // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    // clear
    glDeleteBuffers(1, &vertex_buffer);
    glDisableVertexAttribArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glBindVertexArray(0);
}

void TextNode::Update() {
    // nothing to update
}

void TextNode::SetText(const std::string& text) {
    text_ = text;
}

void TextNode::SetColor(const glm::vec4& color) {
    color_ = color;
}
