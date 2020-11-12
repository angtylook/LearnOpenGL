#ifndef SCENE_SCENE_NODE_H_
#define SCENE_SCENE_NODE_H_

#include <string>

#include "glm/glm.hpp"

enum SceneNodeType {
    Type_Scene = 0,
    Type_Model = 1,
    Type_Light = 2,
    Type_Text = 3,
    Type_Image = 4,
    Type_Axis = 5,
    Type_Camera = 6,
};

class SceneNode {
public:
    SceneNode(const std::string& name);
    virtual ~SceneNode(){};

public:
    std::string GetName();
    void SetName(const std::string& name);
    std::string GetShader();
    void SetShader(const std::string& shader);
    bool IsVisible();
    void SetVisible(bool visible);
    glm::mat4 GetTransfromMatrix();
    glm::vec3 GetScale();
    glm::vec3 GetTranslate();
    void Translate(const glm::vec3& translate);
    void Scale(const glm::vec3& scale);
    void Rotate(float angle, const glm::vec3& axis);
    virtual SceneNodeType GetNodeType() const;
    virtual bool IsRenderable();
    virtual void Render();
    virtual void Update();

private:
    bool visible_;
    bool update_transform_;
    std::string name_;
    glm::mat4 transform_;
    glm::vec3 position_;
    glm::vec3 scale_;
    glm::vec4 rotate_;
    std::string shader_;
};

template <typename To>
To ConvertTo(SceneNode* node) {
    if (std::is_base_of<SceneNode, To>::value &&
        std::is_convertible<SceneNode*, To>::value) {
        return static_cast<To>(node);
    } else {
        return nullptr;
    }
}

#endif  // SCENE_SCENE_NODE_H_
