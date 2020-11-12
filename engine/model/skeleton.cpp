#include "skeleton.h"

Skeleton::Skeleton() {}

const Joint& Skeleton::GetJoint(int bone_id) const {
    return joints_.at(bone_id);
}

void Skeleton::AddJoint(const Joint& joint, int bone_id) {
    joints_[bone_id] = joint;
}

size_t Skeleton::JointSize() const {
    return joints_.size();
}

void Skeleton::Reset() {
    joints_.clear();
}
