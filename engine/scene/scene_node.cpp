#include "scene_node.h"

#include <cstdio>

#include "base/filesystem.h"
#include "glm/gtc/matrix_transform.hpp"

SceneNode::SceneNode(const std::string& name)
    : visible_(true)
    , update_transform_(false)
    , name_(name)
    , transform_(1.0f)
    , position_(0.0f)
    , scale_(1.0f)
    , rotate_(0.0f) {}

std::string SceneNode::GetName() {
    return name_;
}

void SceneNode::SetName(const std::string& name) {
    name_ = name;
}

std::string SceneNode::GetShader() {
    return shader_;
}

void SceneNode::SetShader(const std::string& shader) {
    shader_ = shader;
}

bool SceneNode::IsVisible() {
    return visible_;
}

void SceneNode::SetVisible(bool visible) {
    visible_ = visible;
}

bool SceneNode::IsRenderable() {
    return false;
}

void SceneNode::Render() {
    fprintf(stderr, "SceneNode can not be rendered!");
}

void SceneNode::Update() {
    fprintf(stderr, "SceneNode can not be updated!");
}

glm::mat4 SceneNode::GetTransfromMatrix() {
    if (update_transform_) {
        transform_ = glm::mat4(1.0f);
        if (rotate_[3] != 0)
            transform_ =
                glm::rotate(transform_, rotate_[3], glm::vec3(rotate_));
        transform_ = glm::translate(transform_, position_);
        transform_ = glm::scale(transform_, scale_);
        update_transform_ = false;
    }
    return transform_;
}

void SceneNode::Translate(const glm::vec3& translate) {
    position_ = translate;
    update_transform_ = true;
}

glm::vec3 SceneNode::GetTranslate() {
    return position_;
}

void SceneNode::Scale(const glm::vec3& scale) {
    scale_ = scale;
    update_transform_ = true;
}

glm::vec3 SceneNode::GetScale() {
    return scale_;
}

void SceneNode::Rotate(float angle, const glm::vec3& axis) {
    rotate_ = glm::vec4(glm::normalize(axis), glm::radians(angle));
    update_transform_ = true;
}

SceneNodeType SceneNode::GetNodeType() const {
    return SceneNodeType::Type_Scene;
}
