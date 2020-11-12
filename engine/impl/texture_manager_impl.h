#ifndef TEXTURE_MANAGER_IMPL_H_
#define TEXTURE_MANAGER_IMPL_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <string>
#include "base/micros.h"
#include "texture_manager.h"

namespace engine {
class TextureManagerImpl : public TextureManager {
public:
    TextureManagerImpl() = default;
    virtual ~TextureManagerImpl() = default;

public:
    bool Initialize();

    void UnInitialize();

    virtual GLuint GetTexture(const std::string& path) override;

    virtual const Character& GetCharacter(GLchar c) override;

private:
    bool GenerateCharacter();

    void ClearCharacter();

private:
    std::map<std::string, GLuint> textures_;
    std::map<GLchar, Character> character_;

private:
    DISALLOW_COPY_AND_ASSIGN(TextureManagerImpl)
};

}  // namespace engine

#endif  // TEXTURE_MANAGER_H_
