#ifndef SCENE_MODEL_NODE_H_
#define SCENE_MODEL_NODE_H_

#include "scene_node.h"

class Mesh;
class Model;
class ModelNode : public SceneNode {
public:
    ModelNode(const std::string& name);
    virtual ~ModelNode();
    void SetModel(const std::string& model);
    Model* GetModel();
    void SetMaterial(const std::string& material);
    std::string GetMaterial() const;
    virtual SceneNodeType GetNodeType() const override;
    virtual bool IsRenderable() override;
    ;
    virtual void Render() override;
    virtual void Update() override;
    virtual void OnBeforeRender();
    virtual void OnAfterRender();

private:
    std::string model_;
    std::string material_;
};

#endif  // SCENE_MODEL_NODE_H_
