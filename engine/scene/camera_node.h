#ifndef SCENE_CAMERA_NODE_H_
#define SCENE_CAMERA_NODE_H_

#include "model_node.h"

class Mesh;
class CameraNode : public ModelNode {
public:
    CameraNode(const std::string& name);
    virtual ~CameraNode();
    virtual SceneNodeType GetNodeType() const override;
    virtual void OnBeforeRender() override;
    virtual void OnAfterRender() override;
};

#endif  // SCENE_CAMERA_NODE_H_
