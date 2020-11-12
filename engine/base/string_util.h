#ifndef COMMON_STRING_UTIL_H_
#define COMMON_STRING_UTIL_H_

#include <string>
#include <vector>
#include "glm/glm.hpp"

// Remove the quotes from a string
void RemoveQuotes(std::string& str);

void SplitString(const std::string& s, char delim, std::vector<std::string>& v);
std::string TrimWhiteSpace(const std::string& str);

glm::vec2 ReadVec2(const std::string& s, float def = 1.0f);
glm::vec3 ReadVec3(const std::string& s, float def = 1.0f);
glm::vec4 ReadVec4(const std::string& s, float def = 1.0f);

#endif  // COMMON_STRING_UTIL_H_
