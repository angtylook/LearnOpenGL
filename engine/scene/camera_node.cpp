#include "camera_node.h"

#include <GL/glew.h>
#include "application.h"
#include "base/filesystem.h"
#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "model/model.h"
#include "scene_manager.h"
#include "shader_manager.h"

using namespace engine;
CameraNode::CameraNode(const std::string& name) : ModelNode(name) {
    SetShader("camera");
}

CameraNode::~CameraNode() {}

SceneNodeType CameraNode::GetNodeType() const {
    return SceneNodeType::Type_Camera;
}

void CameraNode::OnBeforeRender() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    application->shader_manager()->UseShader(GetShader());
    glm::mat4 ProjectionMatrix =
        application->scene_manager()->GetCamera()->GetProjectMatrix();
    application->shader_manager()->SetShaderUniform("project",
                                                    ProjectionMatrix);
    glm::mat4 ViewMatrix =
        application->scene_manager()->GetCamera()->GetViewMatrix();
    application->shader_manager()->SetShaderUniform("view", ViewMatrix);
    glm::mat4 ModelMatrix =
        application->scene_manager()->GetCamera()->GetMatrix();
    application->shader_manager()->SetShaderUniform("model", ModelMatrix);
}

void CameraNode::OnAfterRender() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
};
