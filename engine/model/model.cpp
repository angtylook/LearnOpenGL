#include "model.h"
#include "base/filesystem.h"

Model::Model() : current_anim_(nullptr) {}

Model::Model(const std::string& path, const std::string& model_type)
    : model_path_(path), model_type_(model_type), current_anim_(nullptr) {}

const std::string& Model::GetPath() {
    return model_path_;
}

const std::string& Model::GetType() {
    return model_type_;
}

void Model::Update() {
    if (current_anim_) {
        current_anim_->Update();
        UpdateMesh();
    }
}

void Model::UpdateMesh() {
    const Skeleton& skeleton = current_anim_->GetCurrentSkeleton();
    for (size_t i = 0; i < meshs_.size(); ++i) {
        auto& vertex_data = meshs_[i].GetVertexData();
        auto& vertex = meshs_[i].GetVertexBuffer();
        auto& normal = meshs_[i].GetNormalBuffer();
        vertex.clear();

        for (unsigned int i = 0; i < vertex_data.size(); ++i) {
            glm::vec3 pos = glm::vec3(0);
            normal[i] = glm::vec3(0);
            const VertexData& data = vertex_data[i];

            for (int j = 0; j < kVertexBoneNumber; ++j) {
                if (data.bone_id[j] != -1) {
                    // const Weight& weight = mesh.m_Weights[vert.m_StartWeight
                    // + j];
                    const Joint& joint = skeleton.GetJoint(data.bone_id[j]);
                    // const MD5Animation::SkeletonJoint& joint =
                    // skel.m_Joints[weight.m_JointID];

                    glm::vec3 rot_pos = joint.rotate * data.position[j];
                    pos += (joint.translate + rot_pos) * data.weight[j];
                }
            }
            vertex.push_back(pos);
        }
        // genenral normals
        auto& index_buffer(meshs_[i].GetIndexBuffer());
        size_t index_size = index_buffer.size();
        size_t tri_size = index_size / 3;
        for (size_t k = 0; k < tri_size; ++k) {
            size_t tri = 3 * k;
            glm::ivec3 triangle(index_buffer[tri], index_buffer[tri + 1],
                                index_buffer[tri + 2]);

            glm::vec3& v0 = vertex[triangle.x];
            glm::vec3& v1 = vertex[triangle.y];
            glm::vec3& v2 = vertex[triangle.z];

            glm::vec3 normal_vec = glm::cross(v1 - v0, v2 - v0);

            normal[triangle.x] += normal_vec;
            normal[triangle.y] += normal_vec;
            normal[triangle.z] += normal_vec;
        }
        for (size_t i = 0; i < normal.size(); ++i) {
            normal[i] = glm::normalize(normal[i]);
        }
        meshs_[i].UpdateMeshRender();
    }
}

Bone& Model::AllocBone() {
    bones_.emplace_back(Bone());
    return bones_.back();
}

BoneList& Model::GetBoneList() {
    return bones_;
}

Bone* Model::GetBone(size_t bone_id) {
    if (bone_id < bones_.size())
        return &bones_[bone_id];
    else
        return nullptr;
}

size_t Model::GetBoneSize() const {
    return bones_.size();
}

Skeleton& Model::GetBindPoseSkeleton() {
    return bind_pose_skeleton_;
}

Mesh& Model::AllocMesh() {
    meshs_.emplace_back(Mesh());
    return meshs_.back();
}

Mesh* Model::GetMesh(size_t index) {
    if (index < meshs_.size()) {
        return &meshs_[index];
    } else {
        return nullptr;
    }
}

MeshList& Model::GetMeshList() {
    return meshs_;
}

size_t Model::GetMeshSize() const {
    return meshs_.size();
}

Animation* Model::GetAnimation(const std::string& name) {
    if (animations_.find(name) != animations_.end()) {
        fprintf(stderr, "Animation: %s in model(%s) not found!\n", name.c_str(),
                model_path_.c_str());
        return &animations_[name];
    }
    return nullptr;
}

Animation& Model::AllocAnimation(const std::string& name) {
    animations_[name].SetName(name);
    return animations_[name];
}

Animation* Model::GetCurrentAnimation() {
    return current_anim_;
}
void Model::SetAnimation(const std::string& name) {
    current_anim_name_ = name;
    current_anim_ = GetAnimation(current_anim_name_);
}

size_t Model::GetAnimationSize() const {
    return animations_.size();
}

// curve
Curve& Model::AllocCurve() {
    curves_.emplace_back(Curve());
    return curves_.back();
}

Curve* Model::GetCurve(size_t index) {
    if (index < curves_.size())
        return &curves_[index];
    else
        return nullptr;
}

CurveList& Model::GetCurveList() {
    return curves_;
}

size_t Model::GetCurveSize() const {
    return curves_.size();
}
