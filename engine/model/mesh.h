#ifndef MODEL_MESH_H_
#define MODEL_MESH_H_

#include <memory>
#include <string>
#include <vector>
#include "GL/glew.h"
#include "glm/glm.hpp"

const int kVertexBoneNumber = 4;

struct VertexData {
    int bone_id[kVertexBoneNumber];
    float weight[kVertexBoneNumber];
    glm::vec3 position[kVertexBoneNumber];
    VertexData();
};

class MeshRender;
class Mesh {
public:
    Mesh();

public:
    std::vector<glm::vec3>& GetVertexBuffer();
    std::vector<glm::vec3>& GetNormalBuffer();
    std::vector<glm::vec2>& GetUVBuffer();
    std::vector<GLuint>& GetIndexBuffer();
    std::vector<VertexData>& GetVertexData();
    void SetEnableVertexPosition(bool enable);
    bool IsEnableVertexPosition() const;

public:
    std::string GetMaterial() const;
    void SetMaterial(const std::string& matrial);
    void UpdateMeshRender();
    std::shared_ptr<MeshRender> GetRender();

private:
    // flags
    bool enable_vertex_position_;
    // for skin
    std::string material_;
    // vertex attribute
    std::vector<glm::vec3> vertex_buffer_;
    std::vector<glm::vec3> normal_buffer_;
    std::vector<glm::vec2> texture_coordinate_;
    std::vector<GLuint> index_buffer_;
    std::vector<VertexData> vertex_data_;
    // for render
    std::shared_ptr<MeshRender> mesh_render_;
};

typedef std::vector<Mesh> MeshList;

#endif  // MODEL_MESH_H_
