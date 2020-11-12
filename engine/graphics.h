#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <string>
#include "glm/glm.hpp"
#include "math/rect.h"

namespace engine {
class Graphics {
public:
    static void DrawText(const std::string& text,
                         float x,
                         float y,
                         const glm::vec4& color,
                         float scale = 1.0);
    static void DrawImage(const std::string& image_path,
                          float x,
                          float y,
                          float width,
                          float height);
    static void DrawImage(const std::string& image_path, const Rect& rect);
};
}  // namespace engine
#endif  // GRAPHICS_H_
