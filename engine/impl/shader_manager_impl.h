#ifndef SHADER_MANAGER_IMPL_H_
#define SHADER_MANAGER_IMPL_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <string>
#include "base/micros.h"
#include "shader_manager.h"

namespace engine {
// class Shader;
class ShaderManagerImpl : public ShaderManager {
public:
    ShaderManagerImpl() = default;
    virtual ~ShaderManagerImpl() = default;
public:
    bool Initialize();

    void UnInitialize();

    // interface
    virtual bool CreateShader(const std::string& name,
                              const std::string& vetex_shader_path,
                              const std::string& fragment_shader_path) override;

    virtual void UseShader(const std::string& name) override;

    virtual const std::string& GetCurrentShaderName() const override;

    virtual GLuint GetCurrentShaderID() const override;

    virtual void DeleteShader(const std::string& name) override;

    virtual GLuint NameToShaderID(const std::string& name) const override;

    // matrix
    virtual void SetShaderUniform(const std::string& name,
                                  const glm::mat2& m) override;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::mat3& m) override;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::mat4& m) override;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::mat2x3& m) override;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::mat3x2& m) override;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::mat2x4& m) override;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::mat4x2& m) override;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::mat3x4& m) override;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::mat4x3& m) override;

    // float
    virtual void SetShaderUniform(const std::string& name,
                                  const GLfloat v) override;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::vec2& v) override;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::vec3& v) override;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::vec4& v) override;

    // int
    virtual void SetShaderUniform(const std::string& name,
                                  const GLint v) override;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::ivec2& v) override;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::ivec3& v) override;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::ivec4& v) override;

    // unsigned int
    virtual void SetShaderUniform(const std::string& name,
                                  const GLuint v) override;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::uvec2& v) override;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::uvec3& v) override;

    virtual void SetShaderUniform(const std::string& name,
                                  const glm::uvec4& v) override;

private:
    GLint GetUniformLocation(const std::string& shader,
                             const std::string& name);

private:
    std::string current_shader_;
    std::map<std::string, GLuint> shaders_;

private:
    DISALLOW_COPY_AND_ASSIGN(ShaderManagerImpl)
};

}  // namespace engine

#endif  // SHADER_MANAGER_H_
