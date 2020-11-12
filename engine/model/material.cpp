#include "material.h"

Material::Material() : Material("unname") {}

Material::Material(const std::string& name) : name_(name), shininess_(1.0f) {}

Material::~Material() {}

std::string Material::GetName() const {
    return name_;
}

void Material::SetName(const std::string& name) {
    name_ = name;
}

float Material::GetShininess() const {
    return shininess_;
}

void Material::SetShininess(float shininess) {
    shininess_ = shininess;
}

std::string Material::GetAmbient() const {
    return ambient_;
}

void Material::SetAmbient(const std::string& ambient) {
    ambient_ = ambient;
}

glm::vec4 Material::GetAmbientColor() const {
    return ambient_color_;
}

void Material::SetAmbientColor(const glm::vec4& ambient_color) {
    ambient_color_ = ambient_color;
}

bool Material::HasAmbient() const {
    return !ambient_.empty();
}

std::string Material::GetDiffuse() const {
    return diffuse_;
}

void Material::SetDiffuse(const std::string& diffuse) {
    diffuse_ = diffuse;
}

glm::vec4 Material::GetDiffuseColor() const {
    return diffuse_color_;
}

void Material::SetDiffuseColor(const glm::vec4& diffuse_color) {
    diffuse_color_ = diffuse_color;
}

bool Material::HasDiffuse() const {
    return !diffuse_.empty();
}

std::string Material::GetSpecular() const {
    return specular_;
}

void Material::SetSpecular(const std::string& specular) {
    specular_ = specular;
}

glm::vec4 Material::GetSpecularColor() const {
    return specular_color_;
}

void Material::SetSpecularColor(const glm::vec4& specular_color) {
    specular_color_ = specular_color;
}

bool Material::HasSpecular() const {
    return !specular_.empty();
}
