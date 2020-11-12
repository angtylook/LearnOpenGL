#ifndef SCENE_AXIS_NODE_H_
#define SCENE_AXIS_NODE_H_

#include <GL/glew.h>
#include <string>
#include "scene_node.h"

class AxisNode : public SceneNode {
public:
    AxisNode(const std::string& name);
    virtual ~AxisNode();
    virtual SceneNodeType GetNodeType() const override;
    virtual bool IsRenderable() override { return true; };
    virtual void Render() override;
    virtual void Update() override;

private:
    GLuint vertex_array_;
};

#endif  // SCENE_AXIS_NODE_H_
