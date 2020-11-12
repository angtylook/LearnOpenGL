#include "scene_manager_impl.h"

#include <memory>
#include "application.h"
#include "base/filesystem.h"
#include "base/window.h"
#include "input_manager.h"
#include "model/light.h"
#include "render/gl_state.h"
#include "scene/light_node.h"
#include "shader_manager.h"

namespace engine {
bool SceneManagerImpl::Initialize() {
    engine::application->input_manager()->RegisterHandler(
        engine::InputManager::Priority::SCENE, engine::EventType::KEYBOARD,
        std::bind(&Camera::ProcessEvent, GetCamera(), std::placeholders::_1));
    engine::application->input_manager()->RegisterHandler(
        engine::InputManager::Priority::SCENE, engine::EventType::POSITION,
        std::bind(&Camera::ProcessEvent, GetCamera(), std::placeholders::_1));
    engine::application->input_manager()->RegisterHandler(
        engine::InputManager::Priority::SCENE, engine::EventType::SCROLL,
        std::bind(&Camera::ProcessEvent, GetCamera(), std::placeholders::_1));
    //
    SetupPostRender();
    //
    skybox_.Init("data/learngl/skybox");
    render2d_.Initilize();
    return true;
}

bool SceneManagerImpl::SetupPostRender() {
    auto sz = engine::application->GetWindowSize();
    post_buffer_.Init(sz.x, sz.y);
    frame_buffer_.Init();
    frame_buffer_.Bind();
    frame_buffer_.AttachTexture(GL_COLOR_ATTACHMENT0, post_buffer_.GetTexure());
    frame_buffer_.AttachRenderBuffer(GL_DEPTH_STENCIL_ATTACHMENT,
                                     post_buffer_.GetRenderBuffer());
    GLState::BindDefaultFrameBuffer();
    return frame_buffer_.IsComplete();
}

void SceneManagerImpl::UnInitialize() {
    engine::application->input_manager()->UnRegisterHandler(
        engine::InputManager::Priority::SCENE, engine::EventType::KEYBOARD);
    engine::application->input_manager()->UnRegisterHandler(
        engine::InputManager::Priority::SCENE, engine::EventType::POSITION);
    engine::application->input_manager()->UnRegisterHandler(
        engine::InputManager::Priority::SCENE, engine::EventType::SCROLL);
    Clear();
    render2d_.UnInitilize();
}

void SceneManagerImpl::Clear() {
    nodes_.clear();
}

void SceneManagerImpl::Update() {
    for (auto& node : nodes_) {
        node.second->Update();
    }
}

void SceneManagerImpl::RenderPass() {
    auto sz = engine::application->GetWindowSize();
    glViewport(0, 0, sz.x, sz.y);
    // render skybox
    skybox_.Render();
    // render scene
    // not model node
    for (auto& node : nodes_) {
        if (node.second->IsVisible() && node.second->IsRenderable() &&
            node.second->GetNodeType() != SceneNodeType::Type_Model) {
            // fprintf(stdout, "render node: %s\n",
            // node.second->GetName().c_str());
            node.second->Render();
        }
    }
    // set light
    application->shader_manager()->UseShader("model");
    application->shader_manager()->SetShaderUniform("view_position",
                                                    GetCamera()->GetPosition());
    light_setuper_.Reset();
    for (auto& node : nodes_) {
        if (node.second->GetNodeType() == SceneNodeType::Type_Light) {
            Light* light =
                static_cast<LightNode*>(node.second.get())->GetLight();
            if (light) {
                switch (light->GetType()) {
                    case Light_Point:
                        light_setuper_.SetupLightPont(light);
                        break;
                    case Light_Direct:
                        light_setuper_.SetupLightDirect(light);
                        break;
                    case Light_Spot:
                        light_setuper_.SetupLightSpot(light);
                        break;
                    default:
                        fprintf(stderr,
                                "can not setup light with type unknown.\n");
                        break;
                }
            }
        }
    }
    light_setuper_.AfterSetup();
    // render model node
    for (auto& node : nodes_) {
        if (node.second->IsVisible() && node.second->IsRenderable() &&
            node.second->GetNodeType() == SceneNodeType::Type_Model) {
            // fprintf(stdout, "render node: %s\n",
            // node.second->GetName().c_str());
            node.second->Render();
        }
    }
}

void SceneManagerImpl::RenderScene() {
    // normal rendering
    frame_buffer_.Bind();
    // glCheckError();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    RenderPass();
    // render post effect.
    GLState::BindDefaultFrameBuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    post_buffer_.Render();
    // render ui
    render2d_.RenderQuads();
}

Camera* SceneManagerImpl::GetCamera() {
    return &camera_;
}

Render2D* SceneManagerImpl::GetRender2D() {
    return &render2d_;
}

void SceneManagerImpl::AddSceneNode(std::unique_ptr<SceneNode> node) {
    assert(!node->GetName().empty());
    if (nodes_.find(node->GetName()) == nodes_.end()) {
        nodes_.insert(std::make_pair(node->GetName(), std::move(node)));
    }
}

void SceneManagerImpl::RemoveSceneNode(const std::string& name) {
    if (nodes_.find(name) != nodes_.end()) {
        nodes_.erase(name);
    }
}

SceneNode* SceneManagerImpl::GetSceneNode(const std::string& name) {
    if (nodes_.find(name) != nodes_.end()) {
        return nodes_[name].get();
    }
    return nullptr;
}

}  // namespace engine
