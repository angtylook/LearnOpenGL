#ifndef MODEL_CURVE_H_
#define MODEL_CURVE_H_

#include <vector>

#include "GL/glew.h"
#include "glm/glm.hpp"

struct CurvePoint {
    glm::vec3 V;
    glm::vec3 T;
    glm::vec3 B;
    glm::vec3 N;
};

class Curve {
public:
    Curve();
    std::vector<glm::vec3>& GetVertexBuffer();
    std::vector<glm::vec3>& GetNormalBuffer();
    std::vector<glm::vec3>& GetBinormalBuffer();
    std::vector<glm::vec3>& GetTangentBuffer();
    std::vector<glm::vec3>& GetColorBuffer();
    std::vector<GLuint>& GetIndexBuffer();

    size_t size() const;
    CurvePoint GetPoint(size_t i) const;

private:
    // for rendering
    std::vector<glm::vec3> vertex_buffer_;
    std::vector<glm::vec3> normal_buffer_;
    std::vector<glm::vec3> binormal_buffer_;
    std::vector<glm::vec3> tangent_buffer_;
    std::vector<glm::vec3> color_buffer_;
    std::vector<GLuint> index_buffer_;
};

typedef std::vector<Curve> CurveList;

#endif  // MODEL_CURVE_H_
