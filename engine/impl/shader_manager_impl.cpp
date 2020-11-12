#include "shader_manager_impl.h"

#include <GL/glew.h>
#include <map>
#include "base/filesystem.h"
#include "glm/gtc/type_ptr.hpp"
#include "render/gl_shader.h"

namespace {
const size_t kShaderNumber = 8;
struct ShaderConfig {
    const char* name;
    const char* vertex;
    const char* fragment;
};
const ShaderConfig shader_config[kShaderNumber] = {
    {"model", "data/shader/model.vertexshader",
     "data/shader/model.fragmentshader"},
    {"axis", "data/shader/axis.vertexshader",
     "data/shader/axis.fragmentshader"},
    {"text", "data/shader/text.vertexshader",
     "data/shader/text.fragmentshader"},
    {"image", "data/shader/image.vertexshader",
     "data/shader/image.fragmentshader"},
    {"light", "data/shader/light.vertexshader",
     "data/shader/light.fragmentshader"},
    {"camera", "data/shader/camera.vertexshader",
     "data/shader/camera.fragmentshader"},
    {"post", "data/shader/post.vertexshader",
     "data/shader/post.fragmentshader"},
    {"skybox", "data/shader/skybox.vertexshader",
     "data/shader/skybox.fragmentshader"}};
}  // namespace

namespace engine {
bool ShaderManagerImpl::CreateShader(const std::string& name,
                                     const std::string& vetex_shader_path,
                                     const std::string& fragment_shader_path) {
    GLuint shader =
        LoadShaders(vetex_shader_path.c_str(), fragment_shader_path.c_str());
    if (shader != 0) {
        if (shaders_.find(name) != shaders_.end()) {
            DeleteShader(name);
        }
        shaders_[name] = shader;
        return true;
    } else {
        fprintf(stderr,
                "create shader: %s failed with vertex: %s and fragment: %s\n",
                name.c_str(), vetex_shader_path.c_str(),
                fragment_shader_path.c_str());
        return false;
    }
}

void ShaderManagerImpl::UseShader(const std::string& name) {
    if (name == current_shader_) {
        return;
    }
    if (shaders_.find(name) != shaders_.end()) {
        current_shader_ = name;
        glUseProgram(shaders_[name]);
    } else {
        fprintf(stderr, "use shader(%s) failed!\n", name.c_str());
    }
}

const std::string& ShaderManagerImpl::GetCurrentShaderName() const {
    return current_shader_;
}

GLuint ShaderManagerImpl::GetCurrentShaderID() const {
    return NameToShaderID(current_shader_);
}

void ShaderManagerImpl::DeleteShader(const std::string& name) {
    if (shaders_.find(name) != shaders_.end()) {
        glDeleteProgram(shaders_[name]);
        shaders_.erase(name);
    }
}

GLuint ShaderManagerImpl::NameToShaderID(const std::string& name) const {
    auto it = shaders_.find(name);
    if (it == shaders_.end()) {
        fprintf(stderr, "shader(%s) not found.", name.c_str());
        return 0;
    }
    return it->second;
}

bool ShaderManagerImpl::Initialize() {
    for (size_t i = 0; i < kShaderNumber; ++i) {
        CreateShader(shader_config[i].name, shader_config[i].vertex,
                     shader_config[i].fragment);
    }
    return true;
}

void ShaderManagerImpl::UnInitialize() {
    for (auto shader : shaders_) {
        glDeleteProgram(shader.second);
    }
    shaders_.clear();
}

GLint ShaderManagerImpl::GetUniformLocation(const std::string& shader,
                                            const std::string& name) {
    GLuint shader_id = NameToShaderID(shader);
    if (shader_id == 0) {
        fprintf(stderr,
                "can not get uniform loction with invild shader id:%u\n",
                shader_id);
        return -1;
    }

    GLuint uniform_id = glGetUniformLocation(shader_id, name.c_str());
    if (uniform_id == GL_INVALID_VALUE || uniform_id == GL_INVALID_OPERATION) {
        fprintf(stderr,
                "set shader(%s) uniform(%s) failed. uniform not found.\n",
                current_shader_.c_str(), name.c_str());
    }

    return uniform_id;
}

// matrix
void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const glm::mat2& m) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniformMatrix2fv(uniform_id, 1, false, glm::value_ptr(m));
}

void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const glm::mat3& m) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniformMatrix3fv(uniform_id, 1, false, glm::value_ptr(m));
}

void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const glm::mat4& m) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniformMatrix4fv(uniform_id, 1, false, glm::value_ptr(m));
}

void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const glm::mat2x3& m) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniformMatrix2x3fv(uniform_id, 1, false, glm::value_ptr(m));
}

void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const glm::mat3x2& m) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniformMatrix3x2fv(uniform_id, 1, false, glm::value_ptr(m));
}

void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const glm::mat2x4& m) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniformMatrix2x4fv(uniform_id, 1, false, glm::value_ptr(m));
}

void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const glm::mat4x2& m) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniformMatrix4x2fv(uniform_id, 1, false, glm::value_ptr(m));
}

void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const glm::mat3x4& m) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniformMatrix3x4fv(uniform_id, 1, false, glm::value_ptr(m));
}

void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const glm::mat4x3& m) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniformMatrix4x3fv(uniform_id, 1, false, glm::value_ptr(m));
}

// float
void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const GLfloat v) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniform1fv(uniform_id, 1, &v);
}

void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const glm::vec2& v) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniform2fv(uniform_id, 1, glm::value_ptr(v));
}

void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const glm::vec3& v) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniform3fv(uniform_id, 1, glm::value_ptr(v));
}

void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const glm::vec4& v) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniform4fv(uniform_id, 1, glm::value_ptr(v));
}

// int
void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const GLint v) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniform1iv(uniform_id, 1, &v);
}

void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const glm::ivec2& v) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniform2iv(uniform_id, 1, glm::value_ptr(v));
}

void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const glm::ivec3& v) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniform3iv(uniform_id, 1, glm::value_ptr(v));
}

void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const glm::ivec4& v) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniform4iv(uniform_id, 1, glm::value_ptr(v));
}

// unsigned int
void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const GLuint v) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniform1uiv(uniform_id, 1, &v);
}

void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const glm::uvec2& v) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniform2uiv(uniform_id, 1, glm::value_ptr(v));
}

void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const glm::uvec3& v) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniform3uiv(uniform_id, 1, glm::value_ptr(v));
}

void ShaderManagerImpl::SetShaderUniform(const std::string& name,
                                         const glm::uvec4& v) {
    GLint uniform_id = GetUniformLocation(GetCurrentShaderName(), name);
    if (uniform_id != -1)
        glUniform4uiv(uniform_id, 1, glm::value_ptr(v));
}

}  // namespace engine