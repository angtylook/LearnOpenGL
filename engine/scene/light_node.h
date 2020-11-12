#ifndef SCENE_LIGHT_NODE_H_
#define SCENE_LIGHT_NODE_H_

#include "model/light.h"
#include "model_node.h"

class LightNode : public ModelNode {
public:
    LightNode(const std::string& name);
    virtual ~LightNode();
    virtual SceneNodeType GetNodeType() const override;
    virtual void OnBeforeRender() override;
    void SetLight(const std::string& light);
    Light* GetLight();

private:
    std::string light_;
};

#endif  // SCENE_LIGHT_NODE_H_
