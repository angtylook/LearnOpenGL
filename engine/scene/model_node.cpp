#include "model_node.h"

#include "glm/gtc/matrix_transform.hpp"

#include "application.h"
#include "camera.h"
#include "model/model.h"
#include "model_manager.h"
#include "render/mesh_render.h"
#include "scene_manager.h"
#include "shader_manager.h"

using namespace engine;
ModelNode::ModelNode(const std::string& name)
    : SceneNode(name), material_("white") {
    SetShader("model");
}

ModelNode::~ModelNode() {}

inline bool ModelNode::IsRenderable() {
    return !model_.empty();
}

void ModelNode::Render() {
    assert(IsVisible());
    Model* model = application->model_manager()->GetModel(model_);
    if (model) {
        OnBeforeRender();
        size_t mesh_size = model->GetMeshSize();
        for (size_t i = 0; i < mesh_size; ++i) {
            Mesh* mesh = model->GetMesh(i);
            if (mesh && mesh->GetRender()) {
                mesh->GetRender()->Render(GetMaterial());
            }
        }
        OnAfterRender();
    }
}

void ModelNode::OnBeforeRender() {
    // mvp matrix
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

void ModelNode::OnAfterRender() {}

void ModelNode::Update() {
    Model* model = application->model_manager()->GetModel(model_);
    if (model) {
        model->Update();
    }
}

void ModelNode::SetModel(const std::string& model) {
    model_ = model;
}

void ModelNode::SetMaterial(const std::string& material) {
    material_ = material;
}

std::string ModelNode::GetMaterial() const {
    return material_;
}

SceneNodeType ModelNode::GetNodeType() const {
    return SceneNodeType::Type_Model;
}

Model* ModelNode::GetModel() {
    return application->model_manager()->GetModel(model_);
}
