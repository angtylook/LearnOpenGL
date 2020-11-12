#ifndef TEXTURE_MANAGER_H_
#define TEXTURE_MANAGER_H_

#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "base/micros.h"

namespace engine {
struct Character {
    GLuint TextureID;    // ID handle of the glyph texture
    glm::ivec2 Size;     // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;      // Offset to advance to next glyph
};

class TextureManager {
public:
    virtual GLuint GetTexture(const std::string& path) = 0;

    virtual const Character& GetCharacter(GLchar c) = 0;

protected:
    TextureManager() = default;
    virtual ~TextureManager()= default;

private:
    DISALLOW_COPY_AND_ASSIGN(TextureManager)
};

}  // namespace engine

#endif  // TEXTURE_MANAGER_H_
