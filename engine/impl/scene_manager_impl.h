#ifndef SCENE_MANAGER_IMPL_H_
#define SCENE_MANAGER_IMPL_H_

#include <map>
#include <memory>
#include <string>

#include "base/micros.h"
#include "camera.h"
#include "render/frame_buffer.h"
#include "render/light_setuper.h"
#include "render/post_buffer.h"
#include "render/skybox.h"
#include "render2d.h"
#include "scene/scene_node.h"
#include "scene_manager.h"

namespace engine {
class SceneManagerImpl : public SceneManager {
public:
    SceneManagerImpl() = default;
    virtual ~SceneManagerImpl() = default;

public:
    // interface
    bool Initialize();

    void UnInitialize();

    void Clear();

    void Update();

    void RenderScene();

    virtual Camera* GetCamera() override;

    virtual Render2D* GetRender2D() override;

    virtual void AddSceneNode(std::unique_ptr<SceneNode> node) override;

    virtual void RemoveSceneNode(const std::string& name) override;

    virtual SceneNode* GetSceneNode(const std::string& name) override;

private:
    void RenderPass();

    bool SetupPostRender();

private:
    Camera camera_;
    LightSetuper light_setuper_;
    FrameBuffer frame_buffer_;
    PostBuffer post_buffer_;
    Skybox skybox_;
    Render2D render2d_;
    std::map<std::string, std::unique_ptr<SceneNode>> nodes_;

private:
    DISALLOW_COPY_AND_ASSIGN(SceneManagerImpl)
};

}  // namespace engine

#endif  // SCENE_MANAGER_H_
