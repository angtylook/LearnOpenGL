#include "texture_manager_impl.h"

#include <GL/glew.h>
#include <map>
#include <vector>
#include "SOIL2/SOIL2.h"
#include "ft2build.h"
#include FT_FREETYPE_H

#include "base/filesystem.h"

namespace engine {

GLuint TextureManagerImpl::GetTexture(const std::string& path) {
    if (textures_.find(path) != textures_.end()) {
        return textures_[path];
    }

    GLuint texture = SOIL_load_OGL_texture(engine::fs::GetRawPath(path).data(),
                                           SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
                                           SOIL_FLAG_MIPMAPS);
    if (texture != 0)
        textures_[path] = texture;

    return texture;
}

const Character& TextureManagerImpl::GetCharacter(GLchar c) {
    if (character_.find(c) != character_.end()) {
        return character_[c];
    } else {
        fprintf(stderr, "character(%c) not found.", c);
        return character_[' '];
    }
}

bool TextureManagerImpl::GenerateCharacter() {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        fprintf(stderr, "could not init freetype libaray.\n");
        return false;
    }

    FT_Face face;
    if (FT_New_Face(
            ft,
            engine::fs::GetRawPath(engine::fs::Path("data/fonts/arial.ttf"))
                .data(),
            0, &face)) {
        fprintf(stderr, "could not init freetype libaray.\n");
        return false;
    }
    // setting the width to zero lets the face dynamically
    // calculate the width based on the given height.
    FT_Set_Pixel_Sizes(face, 0, 48);
    // disable byte-alignment restriction
    GLint alignment;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (GLubyte c = 0; c < 128; c++) {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            fprintf(stderr, "failed to load glyph(%c).\n", c);
            continue;
        }
        // generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                     face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x),
        };
        character_.insert(std::make_pair(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    return true;
}

void TextureManagerImpl::ClearCharacter() {
    if (character_.empty())
        return;

    std::vector<GLuint> textures;
    for (auto it = character_.begin(); it != character_.end(); ++it) {
        textures.push_back(it->second.TextureID);
    }
    glDeleteTextures(textures.size(), textures.data());
}

bool TextureManagerImpl::Initialize() {
    return GenerateCharacter();
}

void TextureManagerImpl::UnInitialize() {
    ClearCharacter();
    if (textures_.empty())
        return;

    std::vector<GLuint> textures;
    for (auto it = textures_.begin(); it != textures_.end(); ++it) {
        textures.push_back(it->second);
    }
    glDeleteTextures(textures.size(), textures.data());
}

}  // namespace engine