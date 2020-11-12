#ifndef SCENE_IMAGE_NODE_H_
#define SCENE_IMAGE_NODE_H_

#include <string>

#include <GL/glew.h>

#include "scene_node.h"

class ImageNode : public SceneNode {
public:
    ImageNode(const std::string& name);
    virtual ~ImageNode();
    virtual SceneNodeType GetNodeType() const override;
    virtual bool IsRenderable() override { return true; };
    virtual void Render() override;
    virtual void Update() override;

private:
    GLuint vertex_array_;
};

#endif  // SCENE_IMAGE_NODE_H_
