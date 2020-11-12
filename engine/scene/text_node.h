#ifndef SCENE_TEXT_NODE_H_
#define SCENE_TEXT_NODE_H_

#include <string>

#include <GL/glew.h>
#include "glm/glm.hpp"

#include "scene_node.h"

class TextNode : public SceneNode {
public:
    TextNode(const std::string& name);
    virtual ~TextNode();
    void SetText(const std::string& text);
    void SetColor(const glm::vec4& color);
    void SetBackgroundColor(const glm::vec4& color) { background_ = color; }
    void SetX(GLfloat x) { x_ = x; }
    void SetY(GLfloat y) { y_ = y; }
    void SetPosition(GLfloat x, GLfloat y) {
        x_ = x;
        y_ = y;
    }
    void SetScale(GLfloat scale) { scale_ = scale; }
    virtual SceneNodeType GetNodeType() const override;
    virtual bool IsRenderable() override { return !text_.empty(); }
    virtual void Render() override;
    virtual void Update() override;

private:
    GLfloat x_;
    GLfloat y_;
    GLfloat scale_;
    std::string text_;
    glm::vec4 color_;
    glm::vec4 background_;

private:
    GLuint vertex_array_;
};

#endif  // SCENE_TEXT_NODE_H_
