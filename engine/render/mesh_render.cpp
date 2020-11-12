#include "mesh_render.h"
#include "camera.h"
#include "material_setuper.h"
#include "model/mesh.h"
#include "scene/model_node.h"

MeshRender::MeshRender()
    : vao_(0)
    , vertex_buffer_(0)
    , normal_buffer_(0)
    , texture_buffer_(0)
    , element_buffer_(0)
    , has_texture_(false)
    , has_normal_(false)
    , element_count_(0)
    , material_setuper_(new MaterialSetuper()) {
    glGenVertexArrays(1, &vao_);
    Setup();
}

MeshRender::~MeshRender() {
    CleanUp();
    glDeleteVertexArrays(1, &vao_);
}

void MeshRender::Setup() {}

void MeshRender::CleanUp() {
    if (vertex_buffer_ != 0) {
        glDeleteBuffers(1, &vertex_buffer_);
        vertex_buffer_ = 0;
    }
    if (normal_buffer_ != 0) {
        glDeleteBuffers(1, &normal_buffer_);
        normal_buffer_ = 0;
    }
    if (texture_buffer_ != 0) {
        glDeleteBuffers(1, &texture_buffer_);
        texture_buffer_ = 0;
    }
    if (element_buffer_ != 0) {
        glDeleteBuffers(1, &element_buffer_);
        element_buffer_ = 0;
    }
}

void MeshRender::Render(const std::string& opt_material) {
    glBindVertexArray(vao_);
    // matrial
    material_setuper_->Reset();
    material_setuper_->SetHasUV(has_texture_);
    material_setuper_->SetHasNormal(has_normal_);

    if (!mesh_material_.empty()) {
        material_setuper_->SetupMaterial(mesh_material_);
    } else {
        material_setuper_->SetupMaterial(opt_material);
    }
    // draw
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_);
    glDrawElements(GL_TRIANGLES, element_count_, GL_UNSIGNED_INT, (void*)0);
    // reset to default
    glBindVertexArray(0);
}

void MeshRender::UpdateMesh(Mesh* mesh) {
    // reset buffer
    CleanUp();
    // create buffer
    has_normal_ = !mesh->GetNormalBuffer().empty();
    has_texture_ = !mesh->GetUVBuffer().empty();
    mesh_material_ = mesh->GetMaterial();
    /*
      Vertex Array Object(VAO)
      VBO Binds and Attribute Pointer
      States will be stored in VAO
    */
    glBindVertexArray(vao_);
    auto& vertex(mesh->GetVertexBuffer());
    // vertex
    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(glm::vec3), &vertex[0],
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);

    // normal
    if (has_normal_) {
        auto& normal(mesh->GetNormalBuffer());
        glGenBuffers(1, &normal_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_);
        glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(glm::vec3),
                     &normal[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        // 2nd attribute buffer : normal
        glEnableVertexAttribArray(1);
    }

    // texture
    if (has_texture_) {
        auto& texture(mesh->GetUVBuffer());
        glGenBuffers(1, &texture_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, texture_buffer_);
        glBufferData(GL_ARRAY_BUFFER, texture.size() * sizeof(glm::vec2),
                     &texture[0], GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        // 3rd attribute buffer : texture coordinate
        glEnableVertexAttribArray(2);
    }
    // element
    auto& tri(mesh->GetIndexBuffer());
    element_count_ = tri.size();
    glGenBuffers(1, &element_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_count_ * sizeof(GLuint),
                 &tri[0], GL_STATIC_DRAW);

    // reset to default
    glBindVertexArray(0);
}
