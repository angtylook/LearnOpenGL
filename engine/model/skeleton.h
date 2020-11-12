#ifndef MODEL_SKELETON_H_
#define MODEL_SKELETON_H_

#include <map>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

struct Joint {
    glm::vec3 translate;
    glm::quat rotate;
};

class Skeleton {
public:
    Skeleton();
    const Joint& GetJoint(int bone_id) const;
    void AddJoint(const Joint& joint, int bone_id);
    size_t JointSize() const;
    void Reset();

private:
    std::map<int, Joint> joints_;
};

typedef std::vector<Skeleton> SkeletonList;

#endif  // MODEL_SKELETON_H_
