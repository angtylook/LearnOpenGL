#include "light_node.h"

#include "glm/gtc/matrix_transform.hpp"

#include "application.h"
#include "camera.h"
#include "light_manager.h"
#include "model/model.h"
#include "scene_manager.h"
#include "shader_manager.h"

using namespace engine;
LightNode::LightNode(const std::string& name) : ModelNode(name) {
    SetShader("light");
}

LightNode::~LightNode() {}

SceneNodeType LightNode::GetNodeType() const {
    return SceneNodeType::Type_Light;
}

void LightNode::OnBeforeRender() {
    application->shader_manager()->UseShader(GetShader());
    glm::mat4 ProjectionMatrix =
        application->scene_manager()->GetCamera()->GetProjectMatrix();
    application->shader_manager()->SetShaderUniform("project",
                                                    ProjectionMatrix);
    glm::mat4 ViewMatrix =
        application->scene_manager()->GetCamera()->GetViewMatrix();
    application->shader_manager()->SetShaderUniform("view", ViewMatrix);
    glm::mat4 ModelMatrix = GetTransfromMatrix();
    application->shader_manager()->SetShaderUniform("model", ModelMatrix);
}

void LightNode::SetLight(const std::string& light) {
    light_ = light;
}

Light* LightNode::GetLight() {
    return application->light_manager()->GetLight(light_);
}
