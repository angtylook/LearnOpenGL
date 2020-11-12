#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include <memory>
#include <string>

#include "base/micros.h"

class Camera;

class SceneNode;

class Render2D;

namespace engine {
class SceneManager {
public:
    virtual Camera* GetCamera() = 0;

    virtual Render2D* GetRender2D() = 0;

    virtual void AddSceneNode(std::unique_ptr<SceneNode> node) = 0;

    virtual void RemoveSceneNode(const std::string& name) = 0;

    virtual SceneNode* GetSceneNode(const std::string& name) = 0;

protected:
    SceneManager() = default;
    virtual ~SceneManager()= default;

private:
    DISALLOW_COPY_AND_ASSIGN(SceneManager)
};

}  // namespace engine

#endif  // SCENE_MANAGER_H_
