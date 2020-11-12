#include "bounding_box.h"

BoundingBox::BoundingBox() : min_(0), max_(0) {}

BoundingBox::BoundingBox(const glm::vec3& min, const glm::vec3& max)
    : min_(min), max_(max) {}

void BoundingBox::SetMin(const glm::vec3& min) {
    min_ = min;
}

void BoundingBox::SetMax(const glm::vec3& max) {
    max_ = max;
}

glm::vec3 BoundingBox::GetMin() {
    return min_;
}

glm::vec3 BoundingBox::GetMax() {
    return max_;
}
