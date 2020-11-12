#include "light.h"

Light::Light() {
    Reset();
}

Light::Light(const std::string& name) {
    Reset();
    name_ = name;
}

Light::~Light() {}

void Light::Reset() {
    type_ = Light_Invalid;
    glm::vec3 zero(0.0f);
    glm::vec4 one(1.0f);

    position_ = zero;
    direction_ = zero;
    inner_cut_ = 0;
    outer_cut_ = 0;
    // point, spot
    constant_ = 0;
    linear_ = 0;
    quadratic_ = 0;

    // common
    ambient_ = one;
    diffuse_ = one;
    specular_ = one;
}

LightType Light::GetType() const {
    return type_;
}

void Light::SetType(LightType type) {
    type_ = type;
}

std::string Light::GetName() const {
    return name_;
}

void Light::SetName(const std::string& name) {
    name_ = name;
}

// color
glm::vec4 Light::GetAmbient() const {
    return ambient_;
}

void Light::SetAmbient(const glm::vec4& ambient) {
    ambient_ = ambient;
}

glm::vec4 Light::GetDiffuse() const {
    return diffuse_;
}

void Light::SetDiffuse(const glm::vec4& diffuse) {
    diffuse_ = diffuse;
}

glm::vec4 Light::GetSpecular() const {
    return specular_;
}

void Light::SetSpecular(const glm::vec4& specular) {
    specular_ = specular;
}

// position, direction.
glm::vec3 Light::GetPosition() const {
    return position_;
}

void Light::SetPosition(const glm::vec3& position) {
    position_ = position;
}

glm::vec3 Light::GetDirection() const {
    return direction_;
}

void Light::SetDirection(const glm::vec3& direction) {
    direction_ = direction;
}

// for spot light
float Light::GetInnerCut() const {
    return inner_cut_;
}

void Light::SetInnerCut(float inner_cut) {
    inner_cut_ = inner_cut;
}

float Light::GetOuterCut() const {
    return outer_cut_;
}

void Light::SetOuterCut(float outer_cut) {
    outer_cut_ = outer_cut;
}

// for point, spot light
float Light::GetConstant() const {
    return constant_;
}

void Light::SetConstant(float constant) {
    constant_ = constant;
}

float Light::GetLinear() const {
    return linear_;
}

void Light::SetLinear(float linear) {
    linear_ = linear;
}

float Light::GetQuadratic() const {
    return quadratic_;
}

void Light::SetQuadratic(float quadratic) {
    quadratic_ = quadratic;
}

LightType GetLightType(const std::string& value) {
    if (value.empty() || value == "invalid") {
        return Light_Invalid;
    }
    if (value == "point") {
        return Light_Point;
    }
    if (value == "direction") {
        return Light_Direct;
    }
    if (value == "spot") {
        return Light_Spot;
    }
    return Light_Invalid;
}

std::string GetLightTypeString(LightType type) {
    switch (type) {
        case Light_Invalid:
            return "invalid";
            break;
        case Light_Point:
            return "point";
            break;
        case Light_Direct:
            return "direction";
            break;
        case Light_Spot:
            return "spot";
        default:
            return "invalid";
            break;
    }
}
