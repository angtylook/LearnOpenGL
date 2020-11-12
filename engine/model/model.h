#ifndef MODEL_MODEL_H_
#define MODEL_MODEL_H_

#include <vector>
#include "animation.h"
#include "bone.h"
#include "curve.h"
#include "mesh.h"

class Model {
public:
    Model();
    Model(const std::string& path, const std::string& model_type);
    virtual ~Model() {}
    const std::string& GetPath();
    const std::string& GetType();

    void Update();

    // bone
    Bone& AllocBone();
    BoneList& GetBoneList();
    Bone* GetBone(size_t bone_id);
    size_t GetBoneSize() const;
    // skeleton
    Skeleton& GetBindPoseSkeleton();
    // mesh
    Mesh& AllocMesh();
    Mesh* GetMesh(size_t index);
    MeshList& GetMeshList();
    size_t GetMeshSize() const;
    // animation
    Animation& AllocAnimation(const std::string& name);
    Animation* GetAnimation(const std::string& name);
    Animation* GetCurrentAnimation();
    void SetAnimation(const std::string& name);
    size_t GetAnimationSize() const;
    // curve
    Curve& AllocCurve();
    Curve* GetCurve(size_t index);
    CurveList& GetCurveList();
    size_t GetCurveSize() const;

private:
    void UpdateMesh();

private:
    std::string model_path_;
    std::string model_type_;
    // render
    CurveList curves_;
    MeshList meshs_;
    // skeleton
    BoneList bones_;
    Skeleton bind_pose_skeleton_;
    // animation
    std::string current_anim_name_;
    Animation* current_anim_;
    AnimationMap animations_;
};

#endif  // MODEL_MODEL_H_
