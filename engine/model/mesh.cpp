#include "mesh.h"
#include "render/mesh_render.h"

Mesh::Mesh() : enable_vertex_position_(false) {}

std::vector<glm::vec3>& Mesh::GetVertexBuffer() {
    return vertex_buffer_;
}

std::vector<glm::vec3>& Mesh::GetNormalBuffer() {
    return normal_buffer_;
}

std::vector<glm::vec2>& Mesh::GetUVBuffer() {
    return texture_coordinate_;
}

std::vector<GLuint>& Mesh::GetIndexBuffer() {
    return index_buffer_;
}

std::string Mesh::GetMaterial() const {
    return material_;
}

void Mesh::SetMaterial(const std::string& material) {
    material_ = material;
}

std::vector<VertexData>& Mesh::GetVertexData() {
    return vertex_data_;
}

void Mesh::SetEnableVertexPosition(bool enable) {
    enable_vertex_position_ = enable;
}

bool Mesh::IsEnableVertexPosition() const {
    return enable_vertex_position_;
}

std::shared_ptr<MeshRender> Mesh::GetRender() {
    if (!mesh_render_) {
        mesh_render_.reset(new MeshRender());
        mesh_render_->UpdateMesh(this);
    }
    return mesh_render_;
}

void Mesh::UpdateMeshRender() {
    if (!mesh_render_) {
        mesh_render_.reset(new MeshRender());
    }
    mesh_render_->UpdateMesh(this);
}

VertexData::VertexData() {
    for (int i = 0; i < kVertexBoneNumber; ++i) {
        bone_id[i] = -1;
        weight[i] = 0.0f;
    }
}
