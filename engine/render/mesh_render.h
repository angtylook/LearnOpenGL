#ifndef RENDER_MESH_RENDER_H_
#define RENDER_MESH_RENDER_H_

#include <memory>
#include <string>
#include <vector>
#include "GL/glew.h"
#include "base_render.h"

class Mesh;
class ModelNode;
class MaterialSetuper;
class MeshRender : public BaseRender {
public:
    MeshRender();
    virtual ~MeshRender();

public:
    virtual void Render(const std::string& opt_material) override;
    void UpdateMesh(Mesh* mesh);
    void Setup();
    void CleanUp();

private:
    GLuint vao_;
    GLuint vertex_buffer_;
    GLuint normal_buffer_;
    GLuint texture_buffer_;
    GLuint element_buffer_;
    bool has_texture_;
    bool has_normal_;
    GLsizei element_count_;
    std::string mesh_material_;
    std::unique_ptr<MaterialSetuper> material_setuper_;
};

#endif  // RENDER_MESH_RENDER_H_
