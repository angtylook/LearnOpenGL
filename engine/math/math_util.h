#ifndef MATH_MATH_UTIL_H_
#define MATH_MATH_UTIL_H_

#include "glm/gtc/quaternion.hpp"

// Computes the W component of the quaternion based on the X, Y, and Z
// components. This method assumes the quaternion is of unit length.
void ComputeQuatW(glm::quat& quat);

#endif  // MATH_MATH_UTIL_H_
