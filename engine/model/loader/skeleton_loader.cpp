#include "skeleton_loader.h"
#include <array>
#include <fstream>
#include <sstream>
#include "base/filesystem.h"
#include "base/helper.h"
#include "obj_loader.h"

bool SkeletonLoader::Load(const std::string& path) {
    ObjLoader loader(GetModel());
    bool mesh_ok = loader.Load(path + ".obj");
    bool skel_ok = LoadSkeleton(path + ".skel");
    bool weight_ok = LoadWeight(path + ".attach");
    return mesh_ok && skel_ok && weight_ok;
}

bool SkeletonLoader::LoadSkeleton(const std::string& path) {
    if (!engine::fs::Exist(engine::fs::Path(path))) {
        fprintf(stderr, "load skeleton failed, file<%s> not exist.\n",
                path.c_str());
        return false;
    }

    std::ifstream file(engine::fs::GetRawPath(engine::fs::Path(path)).data());
    if (!file) {
        fprintf(stderr, "load skeleton failed, can not open file<%s>.\n",
                path.c_str());
        return false;
    }

    std::array<char, 1024> buffer;
    while (file.getline(buffer.data(), 1024)) {
        std::string line(std::string(buffer.data(), buffer.size()));
        std::stringstream ss(line);
        Joint joint;
        ss >> joint.translate[0] >> joint.translate[1] >> joint.translate[2];

        Bone& bone(GetModel().AllocBone());
        bone.SetIndex(static_cast<int>(GetModel().GetBoneSize()));

        int parent_bone_id = 0;
        ss >> parent_bone_id;
        bone.SetParent(parent_bone_id);

        if (parent_bone_id != -1) {
            Bone* parent = GetModel().GetBone(parent_bone_id);
            if (parent)
                parent->AddChild(bone.GetIndex());
        }

        GetModel().GetBindPoseSkeleton().AddJoint(joint, bone.GetIndex());
    }

    return true;
}

bool SkeletonLoader::LoadWeight(const std::string& path) {
    if (!engine::fs::Exist(engine::fs::Path(path))) {
        fprintf(stderr, "load weights failed, file<%s> not exist.\n",
                path.c_str());
        return false;
    }

    std::ifstream file(engine::fs::GetRawPath(engine::fs::Path(path)).data());
    if (!file) {
        fprintf(stderr, "load weights failed, can not open file<%s>.\n",
                path.c_str());
        return false;
    }
    Mesh* mesh(GetModel().GetMesh(0));
    auto& vertex_data = mesh->GetVertexData();

    std::array<char, 1024> buffer;
    while (file.getline(buffer.data(), 1024)) {
        std::string line(std::string(buffer.data(), buffer.size()));
        std::stringstream ss(line);
        std::vector<float> weights;
        float value;
        while (ss >> value) {
            weights.push_back(value);
        }
        // get the max
        VertexData data;
        for (int i = 0; i < kVertexBoneNumber; ++i) {
            float max_weight = 0.0f;
            int max_bone_id = -1;
            for (size_t bone_id = 0; bone_id < weights.size(); ++bone_id) {
                if (max_weight < weights[bone_id]) {
                    max_weight = weights[bone_id];
                    max_bone_id = bone_id;
                }
            }
            if (max_bone_id != -1) {
                data.bone_id[i] = max_bone_id + 1;
                data.weight[i] = max_weight;
                weights[max_bone_id] = -1;
            }
        }
        vertex_data.push_back(data);
    }

    return true;
}
