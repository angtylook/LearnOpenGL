#include "graphics.h"

#include <vector>
#include "application.h"
#include "glm/gtc/matrix_transform.hpp"
#include "render2d.h"
#include "scene_manager.h"
#include "texture_manager.h"

namespace engine {
// static
void Graphics::DrawText(const std::string& text,
                        float x,
                        float y,
                        const glm::vec4& color,
                        float scale /*= 1.0*/) {
    Render2D* render2d = application->scene_manager()->GetRender2D();
    Render2D::Quad quad;
    quad.type = Render2D::Type_Text;
    quad.texture.push_back(' ');  // reserve one char
    quad.color = color;
    // for coordinate calculate
    GLfloat top = static_cast<GLfloat>(
        application->texture_manager()->GetCharacter('H').Bearing.y);
    for (auto c : text) {
        quad.texture[0] = c;
        // set coordinates
        Character ch = application->texture_manager()->GetCharacter(c);
        quad.size = glm::vec4(x + ch.Bearing.x * scale,
                              y + (top - ch.Bearing.y) * scale,
                              ch.Size.x * scale, ch.Size.y * scale);
        x += (ch.Advance >> 6) *
             scale;  // Bitshift by 6 to get value in pixels (2^6 = 64)
        render2d->AddQuad(quad);
    }
}

// static
void Graphics::DrawImage(const std::string& image_path,
                         float x,
                         float y,
                         float width,
                         float height) {
    Render2D* render2d = application->scene_manager()->GetRender2D();
    Render2D::Quad quad;
    quad.type = Render2D::Type_Image;
    quad.texture = image_path;
    quad.size = glm::vec4(x, y, width, height);
    render2d->AddQuad(quad);
}

void Graphics::DrawImage(const std::string& image_path, const Rect& rect) {
    DrawImage(image_path, rect.Left(), rect.Top(), rect.Width(), rect.Height());
}

}
