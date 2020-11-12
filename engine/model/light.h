#ifndef MODEL_LIGHT_H_
#define MODEL_LIGHT_H_

#include <string>
#include "glm/glm.hpp"

enum LightType {
    Light_Invalid = 0,
    Light_Point = 1,
    Light_Direct = 2,
    Light_Spot = 3
};

class Light {
public:
    Light();
    Light(const std::string& name);
    ~Light();

public:
    LightType GetType() const;
    void SetType(LightType type);
    std::string GetName() const;
    void SetName(const std::string& name);
    // color
    glm::vec4 GetAmbient() const;
    void SetAmbient(const glm::vec4& ambient);
    glm::vec4 GetDiffuse() const;
    void SetDiffuse(const glm::vec4& diffuse);
    glm::vec4 GetSpecular() const;
    void SetSpecular(const glm::vec4& specular);
    // position, direction.
    glm::vec3 GetPosition() const;
    void SetPosition(const glm::vec3& position);
    glm::vec3 GetDirection() const;
    void SetDirection(const glm::vec3& direction);
    // for spot light
    float GetInnerCut() const;
    void SetInnerCut(float inner_cut);
    float GetOuterCut() const;
    void SetOuterCut(float outer_cut);
    // for point, spot light
    float GetConstant() const;
    void SetConstant(float constant);
    float GetLinear() const;
    void SetLinear(float linear);
    float GetQuadratic() const;
    void SetQuadratic(float quadratic);

private:
    void Reset();

private:
    /*
     * type: 0 invalid, 1 point, 2 direction, 3 spotlight
     */
    LightType type_;  // point, direction, spotlight
    std::string name_;
    // point, spot
    glm::vec3 position_;
    // direction, spot
    glm::vec3 direction_;
    // spot
    float inner_cut_;
    float outer_cut_;

    // point, spot
    float constant_;
    float linear_;
    float quadratic_;

    // common
    glm::vec4 ambient_;
    glm::vec4 diffuse_;
    glm::vec4 specular_;
};

LightType GetLightType(const std::string& value);
std::string GetLightTypeString(LightType type);

#endif  // MODEL_LIGHT_H_
