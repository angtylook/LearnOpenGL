#include "curve.h"

Curve::Curve() {}

std::vector<glm::vec3>& Curve::GetVertexBuffer() {
    return vertex_buffer_;
}

std::vector<glm::vec3>& Curve::GetNormalBuffer() {
    return normal_buffer_;
}

std::vector<glm::vec3>& Curve::GetBinormalBuffer() {
    return binormal_buffer_;
}

std::vector<glm::vec3>& Curve::GetTangentBuffer() {
    return tangent_buffer_;
}

std::vector<glm::vec3>& Curve::GetColorBuffer() {
    return color_buffer_;
}

std::vector<GLuint>& Curve::GetIndexBuffer() {
    return index_buffer_;
}

size_t Curve::size() const {
    return vertex_buffer_.size();
}

CurvePoint Curve::GetPoint(size_t i) const {
    CurvePoint point;
    if (i >= vertex_buffer_.size())
        return point;

    point.V = vertex_buffer_[i];
    point.T = tangent_buffer_[i];
    point.N = normal_buffer_[i];
    point.B = binormal_buffer_[i];
    return point;
}
