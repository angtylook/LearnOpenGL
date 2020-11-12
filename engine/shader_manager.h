#ifndef SHADER_MANAGER_H_
#define SHADER_MANAGER_H_

#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "base/micros.h"

namespace engine {
// class Shader;

class ShaderManager {
public:
    virtual bool CreateShader(const std::string& name,
                              const std::string& vetex_shader_path,
                              const std::string& fragment_shader_path) = 0;

    virtual void UseShader(const std::string& name) = 0;

    virtual const std::string& GetCurrentShaderName() const = 0;

    virtual GLuint GetCurrentShaderID() const = 0;

    virtual void DeleteShader(const std::string& name) = 0;

    virtual GLuint NameToShaderID(const std::string& name) const = 0;

    // matrix
    virtual void SetShaderUniform(const std::string& name,
                                  const glm::mat2& m) = 0;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::mat3& m) = 0;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::mat4& m) = 0;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::mat2x3& m) = 0;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::mat3x2& m) = 0;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::mat2x4& m) = 0;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::mat4x2& m) = 0;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::mat3x4& m) = 0;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::mat4x3& m) = 0;

    // float
    virtual void SetShaderUniform(const std::string& name, const GLfloat v) = 0;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::vec2& v) = 0;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::vec3& v) = 0;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::vec4& v) = 0;

    // int
    virtual void SetShaderUniform(const std::string& name, const GLint v) = 0;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::ivec2& v) = 0;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::ivec3& v) = 0;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::ivec4& v) = 0;

    // unsigned int
    virtual void SetShaderUniform(const std::string& name, const GLuint v) = 0;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::uvec2& v) = 0;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::uvec3& v) = 0;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::uvec4& v) = 0;

protected:
    ShaderManager() = default;
    virtual ~ShaderManager()= default;

private:
    DISALLOW_COPY_AND_ASSIGN(ShaderManager)
};

}  // namespace engine

#endif  // SHADER_MANAGER_H_
