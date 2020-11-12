#ifndef MODEL_MATERIAL_H_
#define MODEL_MATERIAL_H_

#include <string>
#include "glm/glm.hpp"

class Material {
public:
    Material();
    Material(const std::string& name);
    ~Material();

    std::string GetName() const;
    void SetName(const std::string& name);

    float GetShininess() const;
    void SetShininess(float shininess);

    std::string GetAmbient() const;
    void SetAmbient(const std::string& ambient);
    glm::vec4 GetAmbientColor() const;
    void SetAmbientColor(const glm::vec4& ambient_color);
    bool HasAmbient() const;

    std::string GetDiffuse() const;
    void SetDiffuse(const std::string& diffuse);
    glm::vec4 GetDiffuseColor() const;
    void SetDiffuseColor(const glm::vec4& diffuse_color);
    bool HasDiffuse() const;

    std::string GetSpecular() const;
    void SetSpecular(const std::string& specular);
    glm::vec4 GetSpecularColor() const;
    void SetSpecularColor(const glm::vec4& specular_color);
    bool HasSpecular() const;

private:
    std::string name_;
    float shininess_;

    std::string ambient_;
    glm::vec4 ambient_color_;

    std::string diffuse_;
    glm::vec4 diffuse_color_;

    std::string specular_;
    glm::vec4 specular_color_;
};

#endif  // MODEL_MATERIAL_H_
