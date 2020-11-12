#ifndef MATH_BOUNDING_BOX_H_
#define MATH_BOUNDING_BOX_H_

#include "glm/glm.hpp"

class BoundingBox {
public:
    BoundingBox();
    BoundingBox(const glm::vec3& min, const glm::vec3& max);
    void SetMin(const glm::vec3& min);
    void SetMax(const glm::vec3& max);
    glm::vec3 GetMin();
    glm::vec3 GetMax();

private:
    glm::vec3 min_;
    glm::vec3 max_;
};

#endif  // MATH_BOUNDING_BOX_H_