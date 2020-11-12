#include "axis_node.h"

#include <vector>
#include "application.h"
#include "base/filesystem.h"
#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "scene_manager.h"
#include "shader_manager.h"

using namespace engine;
namespace {
void DrawAxis() {
    std::vector<GLfloat> g_vertex_buffer_data = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

    std::vector<GLfloat> g_color_buffer_data = {
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};
    // Wide lines have been deprecated. glLineWidth set to 5.000000. glLineWidth
    // with width greater than 1.0 will generate GL_INVALID_VALUE error in
    // future versions
    // glLineWidth(5.0f);

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data.size() * sizeof(GLfloat),
                 g_vertex_buffer_data.data(), GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, g_color_buffer_data.size() * sizeof(GLfloat),
                 g_color_buffer_data.data(), GL_STATIC_DRAW);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0,  // attribute 0. No particular reason for 0, but
                              // must match the layout in the shader.
                          3,         // size
                          GL_FLOAT,  // type
                          GL_FALSE,  // normalized?
                          0,         // stride
                          (void*)0   // array buffer offset
    );

    // 2nd attribute buffer : color
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(1,  // attribute 1. No particular reason for 0, but
                              // must match the layout in the shader.
                          3,         // size
                          GL_FLOAT,  // type
                          GL_FALSE,  // normalized?
                          0,         // stride
                          (void*)0   // array buffer offset
    );

    // Draw the lines !
    glDrawArrays(GL_LINES, 0, 6);  // 3 indices starting at 0 -> 1 triangle

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);

    // glLineWidth(1.0f);
}
}  // namespace

AxisNode::AxisNode(const std::string& name) : SceneNode(name) {
    glGenVertexArrays(1, &vertex_array_);
}

AxisNode::~AxisNode() {
    glDeleteVertexArrays(1, &vertex_array_);
}

SceneNodeType AxisNode::GetNodeType() const {
    return SceneNodeType::Type_Axis;
}

void AxisNode::Render() {
    assert(IsVisible());
    application->shader_manager()->UseShader("axis");
    glm::mat4 ProjectionMatrix =
        application->scene_manager()->GetCamera()->GetProjectMatrix();
    glm::mat4 ViewMatrix =
        application->scene_manager()->GetCamera()->GetViewMatrix();
    glm::mat4 ModelMatrix = GetTransfromMatrix();
    glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
    application->shader_manager()->SetShaderUniform("MVP", MVP);
    glBindVertexArray(vertex_array_);
    DrawAxis();
    glBindVertexArray(0);
}

void AxisNode::Update() {}
