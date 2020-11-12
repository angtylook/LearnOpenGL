#include "obj_loader.h"
#include <array>
#include <fstream>
#include <sstream>
#include "base/filesystem.h"
#include "base/string_util.h"

bool ObjLoader::Load(const std::string& path) {
    if (!engine::fs::Exist(engine::fs::Path(path))) {
        fprintf(stderr, "load object failed, file<%s> not exist.\n",
                path.c_str());
        return false;
    }

    std::ifstream file(engine::fs::GetRawPath(engine::fs::Path(path)).data());
    if (!file) {
        fprintf(stderr, "load object failed, can not open file<%s>.\n",
                path.c_str());
        return false;
    }

    Mesh& mesh(GetModel().AllocMesh());

    auto& vertex(mesh.GetVertexBuffer());
    auto& normal(mesh.GetNormalBuffer());
    auto& uv(mesh.GetUVBuffer());
    auto& tri(mesh.GetIndexBuffer());

    std::vector<glm::vec3> vertex_buffer;
    std::vector<glm::vec3> normal_buffer;
    std::vector<glm::vec2> uv_buffer;
    std::vector<glm::ivec3> tri_buffer;
    float x = 0;
    float y = 0;
    float z = 0;
    std::array<char, 1024> buffer;
    while (file.getline(buffer.data(), 1024)) {
        std::string line(std::string(buffer.data(), buffer.size()));
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            ss >> x >> y >> z;
            vertex_buffer.push_back(glm::vec3(x, y, z));
        } else if (type == "vn") {
            ss >> x >> y >> z;
            normal_buffer.push_back(glm::vec3(x, y, z));
        } else if (type == "vt") {
            ss >> x >> y;
            uv_buffer.push_back(glm::vec2(x, y));
        } else if (type == "f") {
            for (int i = 0; i < 3; ++i) {
                std::string raw;
                std::vector<std::string> vtn;
                ss >> raw;
                SplitString(raw, '/', vtn);
                while (vtn.size() < 3) {
                    vtn.push_back(std::string());
                }
                glm::ivec3 vec(0);
                if (!vtn[0].empty()) {
                    vec.x = std::stoi(vtn[0]);
                }
                if (!vtn[1].empty()) {
                    vec.y = std::stoi(vtn[1]);
                }
                if (!vtn[2].empty()) {
                    vec.z = std::stoi(vtn[2]);
                }
                tri_buffer.push_back(vec);
            }
        } else {
            fprintf(stderr, "objloader skip line: %s\n", line.c_str());
        }
    }

    // post process
    for (size_t i = 0; i < tri_buffer.size(); ++i) {
        glm::ivec3 vec(tri_buffer[i]);

        if (vec.x != 0) {
            glm::vec3 v = vertex_buffer[vec.x - 1];
            vertex.push_back(v);
        }
        if (vec.y != 0) {
            glm::vec2 vt = uv_buffer[vec.y - 1];
            uv.push_back(vt);
        }
        if (vec.z != 0) {
            glm::vec3 vn = normal_buffer[vec.z - 1];
            normal.push_back(vn);
        }
        tri.push_back(i);
    }
    if (normal.empty() && !vertex.empty() && !tri.empty()) {
        GenerateNormal(mesh);
    }
    return true;
}

void ObjLoader::GenerateNormal(Mesh& mesh) {
    auto& vertex_buffer(mesh.GetVertexBuffer());
    auto& normal_buffer(mesh.GetNormalBuffer());
    auto& index_buffer(mesh.GetIndexBuffer());
    normal_buffer.resize(vertex_buffer.size(), glm::vec3(0.0f, 0.0f, 0.0f));

    size_t index_size = index_buffer.size();
    size_t tri_size = index_size / 3;
    for (size_t i = 0; i < tri_size; ++i) {
        size_t tri = 3 * i;
        glm::ivec3 triangle(index_buffer[tri], index_buffer[tri + 1],
                            index_buffer[tri + 2]);

        glm::vec3& v0 = vertex_buffer[triangle.x];
        glm::vec3& v1 = vertex_buffer[triangle.y];
        glm::vec3& v2 = vertex_buffer[triangle.z];

        glm::vec3 normal = glm::cross(v1 - v0, v2 - v0);

        normal_buffer[triangle.x] += normal;
        normal_buffer[triangle.y] += normal;
        normal_buffer[triangle.z] += normal;
    }
    for (size_t i = 0; i < normal_buffer.size(); ++i) {
        normal_buffer[i] = glm::normalize(normal_buffer[i]);
    }
}
