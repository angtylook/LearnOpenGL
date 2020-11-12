#ifndef TEXT_RENDER_H_
#define TEXT_RENDER_H_

#include <GL/glew.h>
#include <string>
#include <vector>
#include "base/micros.h"
#include "glm/glm.hpp"

namespace engine {
class Render2D {
public:
    Render2D() = default;
    ~Render2D() = default;

public:
    enum QuadType { Type_Text = 0, Type_Image = 1, Type_Invalid };

    struct Quad {
        QuadType type;
        std::string texture;
        glm::vec4 size;
        glm::vec4 color;
        Quad() : type(Type_Invalid), color(1.0f, 1.0f, 1.0f, 1.0f) {}
    };

public:
    bool Initilize();
    void UnInitilize();

public:
    bool AddQuad(const Quad& quad);
    void RenderQuads();

private:
    GLuint vertex_array_{0};
    GLuint vertex_buffer_{0};
    std::vector<Quad> quads_;

private:
    DISALLOW_COPY_AND_ASSIGN(Render2D)
};
}  // namespace engine
#endif  // TEXT_RENDER_H_
