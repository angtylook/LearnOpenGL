#include "string_util.h"

#include <vector>
#include "glm/glm.hpp"

// Remove the quotes from a string
void RemoveQuotes(std::string& str) {
    size_t n = std::string::npos;
    while ((n = str.find('\"')) != std::string::npos)
        str.erase(n, 1);
}

void SplitString(const std::string& s,
                 char delim,
                 std::vector<std::string>& v) {
    auto i = 0;
    auto pos = s.find(delim);
    while (pos != std::string::npos) {
        v.push_back(s.substr(i, pos - i));
        i = ++pos;
        pos = s.find(delim, pos);
    }
    if (pos == std::string::npos)
        v.push_back(s.substr(i, s.length()));
}

std::string TrimWhiteSpace(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

glm::vec2 ReadVec2(const std::string& s, float def) {
    glm::vec2 v(def);
    std::vector<std::string> components;
    SplitString(TrimWhiteSpace(s), ' ', components);
    for (size_t i = 0; i < components.size() && i < 2; ++i) {
        v[i] = std::stof(components[i]);
    }

    return v;
}

glm::vec3 ReadVec3(const std::string& s, float def) {
    glm::vec3 v(def);
    std::vector<std::string> components;
    SplitString(TrimWhiteSpace(s), ' ', components);
    for (size_t i = 0; i < components.size() && i < 3; ++i) {
        v[i] = std::stof(components[i]);
    }

    return v;
}

glm::vec4 ReadVec4(const std::string& s, float def) {
    glm::vec4 v(def);
    std::vector<std::string> components;
    SplitString(TrimWhiteSpace(s), ' ', components);
    for (size_t i = 0; i < components.size() && i < 4; ++i) {
        v[i] = std::stof(components[i]);
    }

    return v;
}
