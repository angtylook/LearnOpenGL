#include "sweep_loader.h"
#include <array>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>
#include "base/filesystem.h"
#include "base/helper.h"
#include "glm/gtc/matrix_transform.hpp"

/* This function implements a parser for the "SWP" file format.  It's
 something Eugene came up with specifically for this assigment.
 
 A SWP file allows you to describe spline curves and swept surfaces.
 To specify a curve, you use the following syntax:
 
 TYPE NAME STEPS NUMPOINTS
 [ CONTROLPOINT ]
 [ CONTROLPOINT ]
 ...
 
 ---CURVES---
 
 TYPE can be Bez2, Bez3, Bsp2, Bsp3 which specify Bezier/Bspline
 curves in 2/3 dimensions.
 
 NAME is just a term that can be used later to refer to the curve.
 You can create an anonymous curve by giving '.' (period) as the
 name.
 
 STEPS controls how finely the curve is discretized.  Specifically,
 each cubic piece (not the whole curve) will be discretized into
 STEPS segments.
 
 NUMPOINTS indicates the number of control points.
 
 Each CONTROLPOINT is given as [ x y ] for 2D curves, and [ x y z ]
 for 3D curves.  Note that the square braces are required.
 
 In addition to these curves, you can specify circles as follows:
 
 circ NAME STEPS RADIUS
 
 The variables are self-explanatory.
 
 ---SURFACES---
 
 Surfaces of revolution are defined as follows:
 
 srev NAME STEPS PROFILE
 
 PROFILE is the name of a curve that previously occurred in the
 file.  This name *must* refer to a 2D curve (an error will be
 returned if a 3D curve is provided).
 
 Finally, generalized cylinders are defined as follows:
 
 gcyl NAME PROFILE SWEEP
 
 As with surfaces of revolution, PROFILE is the name of a 2D curve.
 SWEEP is the name of a 2D *or* 3D curve.
 */

namespace {

// read in dim-dimensional control points into a vector
std::vector<glm::vec3> readCps(std::istream& in, unsigned dim) {
    // number of control points
    unsigned n;
    in >> n;

    fprintf(stdout, "  %d cps\n", n);
    // vector of control points
    std::vector<glm::vec3> cps(n);

    char delim;
    float x;
    float y;
    float z;

    for (unsigned i = 0; i < n; ++i) {
        switch (dim) {
            case 2:
                in >> delim;
                in >> x;
                in >> y;
                cps[i] = glm::vec3(x, y, 0);
                in >> delim;
                break;
            case 3:
                in >> delim;
                in >> x;
                in >> y;
                in >> z;
                cps[i] = glm::vec3(x, y, z);
                in >> delim;
                break;
            default:
                fprintf(stderr, "dim<%d> control point not support.\n", dim);
        }
    }

    return cps;
}

// We're only implenting swept surfaces where the profile curve is
// flat on the xy-plane.  This is a check function.
bool checkFlat(const Curve& profile) {
    for (unsigned i = 0; i < profile.size(); i++) {
        CurvePoint point(profile.GetPoint(i));
        if (point.V[2] != 0.0 || point.T[2] != 0.0 || point.N[2] != 0.0)
            return false;
    }
    return true;
}

std::vector<glm::vec4> GenerateCanonicalBasis(unsigned steps) {
    std::vector<glm::vec4> canonical_basis(steps + 1);
    for (unsigned i = 0; i <= steps; ++i) {
        float t = (1.0f * i) / steps;
        glm::vec4 basis(1, t, t * t, t * t * t);
        canonical_basis[i] = basis;
    }
    return canonical_basis;
}
/*
void evalBezier(const std::vector<glm::vec3>& P, unsigned steps, Curve& curve);
void evalBspline(const std::vector<glm::vec3>& P, unsigned steps, Curve& curve);
void evalCircle(float radius, unsigned steps, Curve& curve);
void makeSurfRev(const Curve& profile, unsigned steps, Mesh& mesh);
void makeGenCyl(const Curve& profile, const Curve& sweep, Mesh& mesh);
 */
}  // namespace

bool SweepLoader::Load(const std::string& path) {
    if (!engine::fs::Exist(engine::fs::Path(path))) {
        fprintf(stderr, "load sweep failed, file<%s> not exist.\n",
                path.c_str());
        return false;
    }

    std::ifstream in(engine::fs::GetRawPath(engine::fs::Path(path)).data());
    if (!in) {
        fprintf(stderr, "load sweep failed, can not open file<%s>.\n",
                path.c_str());
        return false;
    }

    std::vector<std::string> curveNames;
    std::vector<std::string> surfaceNames;

    std::string objType;

    // For looking up curve indices by name
    std::map<std::string, unsigned> curveIndex;

    // For looking up surface indices by name
    std::map<std::string, unsigned> surfaceIndex;

    // For storing dimension of curve
    std::vector<unsigned> dims;

    unsigned counter = 0;

    while (in >> objType) {
        fprintf(stdout, ">object %d\n", counter++);
        std::string objName;
        in >> objName;

        bool named = (objName != ".");

        std::vector<glm::vec3> cpsToAdd;

        if (curveIndex.find(objName) != curveIndex.end() ||
            surfaceIndex.find(objName) != surfaceIndex.end()) {
            fprintf(stderr, "error, [%s] already exists.\n", objName.c_str());
            return false;
        }

        unsigned steps;

        if (objType == "bez2") {
            in >> steps;
            fprintf(stdout, " reading bez2 [%s]\n", objName.c_str());
            evalBezier(cpsToAdd = readCps(in, 2), steps);
            curveNames.push_back(objName);
            dims.push_back(2);
            if (named)
                curveIndex[objName] = dims.size() - 1;
        } else if (objType == "bsp2") {
            fprintf(stdout, " reading bsp2 [%s]\n", objName.c_str());
            in >> steps;
            evalBspline(cpsToAdd = readCps(in, 2), steps);
            curveNames.push_back(objName);
            dims.push_back(2);
            if (named)
                curveIndex[objName] = dims.size() - 1;
        } else if (objType == "bez3") {
            fprintf(stdout, " reading bez3 [%s]\n", objName.c_str());
            in >> steps;
            evalBezier(cpsToAdd = readCps(in, 3), steps);
            curveNames.push_back(objName);
            dims.push_back(3);
            if (named)
                curveIndex[objName] = dims.size() - 1;

        } else if (objType == "bsp3") {
            fprintf(stdout, " reading bsp3 [%s]\n", objName.c_str());
            in >> steps;
            evalBspline(cpsToAdd = readCps(in, 3), steps);
            curveNames.push_back(objName);
            dims.push_back(3);
            if (named)
                curveIndex[objName] = dims.size() - 1;
        } else if (objType == "srev") {
            fprintf(stdout, " reading srev [%s]\n", objName.c_str());
            in >> steps;

            // Name of the profile curve
            std::string profName;
            in >> profName;

            fprintf(stdout, "  profile [%s]\n", profName.c_str());

            std::map<std::string, unsigned>::const_iterator it =
                curveIndex.find(profName);

            // Failure checks
            if (it == curveIndex.end()) {
                fprintf(stderr, "failed: [%s] doesn't exist!\n",
                        profName.c_str());
                return false;
            }
            if (dims[it->second] != 2) {
                fprintf(stderr, "failed: [%s] isn't 2d!\n", profName.c_str());
                return false;
            }

            // Make the surface
            Curve* curve = GetModel().GetCurve(it->second);
            if (curve)
                makeSurfRev(*curve, steps);
            surfaceNames.push_back(objName);
            if (named)
                surfaceIndex[objName] = surfaceNames.size() - 1;
        } else if (objType == "gcyl") {
            fprintf(stdout, " reading gcyl [%s]\n", objName.c_str());

            // Name of the profile curve and sweep curve
            std::string profName, sweepName;
            in >> profName >> sweepName;

            fprintf(stdout, "  profile [%s], sweep [%s]\n", profName.c_str(),
                    sweepName.c_str());

            std::map<std::string, unsigned>::const_iterator itP, itS;

            // Failure checks for profile
            itP = curveIndex.find(profName);

            if (itP == curveIndex.end()) {
                fprintf(stderr, "failed: [%s] doesn't exist!\n",
                        profName.c_str());
                return false;
            }

            if (dims[itP->second] != 2) {
                fprintf(stderr, "failed: [%s] isn't 2d!\n", profName.c_str());
                return false;
            }

            // Failure checks for sweep
            itS = curveIndex.find(sweepName);
            if (itS == curveIndex.end()) {
                fprintf(stderr, "failed: [%s] doesn't exist!\n",
                        sweepName.c_str());
                return false;
            }

            // Make the surface
            Curve* profile = GetModel().GetCurve(itP->second);
            Curve* sweep = GetModel().GetCurve(itS->second);
            if (profile && sweep)
                makeGenCyl(*profile, *sweep);
            surfaceNames.push_back(objName);
            if (named)
                surfaceIndex[objName] = surfaceNames.size() - 1;

        } else if (objType == "circ") {
            fprintf(stdout, " reading circ [%s]\n", objName.c_str());

            unsigned steps;
            float rad;
            in >> steps >> rad;
            fprintf(stdout, "  radius [%f]\n", rad);

            evalCircle(rad, steps);
            curveNames.push_back(objName);
            dims.push_back(2);
            if (named)
                curveIndex[objName] = dims.size() - 1;
        } else {
            fprintf(stdout, "failed: type %s unrecognized.\n", objType.c_str());
            return false;
        }
    }

    return true;
}

void SweepLoader::evalBezier(const std::vector<glm::vec3>& P, unsigned steps) {
    Curve& curve(GetModel().AllocCurve());
    // Check
    if (P.size() < 4 || P.size() % 3 != 1) {
        fprintf(stderr,
                "evalBezier must be called with 3n+1 control points.\n");
        return;
    }

    auto& vertex(curve.GetVertexBuffer());
    auto& normal(curve.GetNormalBuffer());
    auto& binormal(curve.GetBinormalBuffer());
    auto& tangent(curve.GetTangentBuffer());

    std::vector<glm::vec4> canonical_basis(GenerateCanonicalBasis(steps));

    glm::mat4 spline_matrix(1, -3, 3, -1, 0, 3, -6, 3, 0, 0, 3, -3, 0, 0, 0, 1);

    spline_matrix = glm::transpose(spline_matrix);

    glm::mat4 TMatrix(-3, 6, -3, 0, 3, -12, 9, 0, 0, 6, -9, 0, 0, 0, 3, 0);

    TMatrix = glm::transpose(TMatrix);

    int piece = P.size() / 3;
    for (int i = 0; i < piece; ++i) {
        int cps = 3 * i;
        glm::vec4 p1(P[cps], 1);
        glm::vec4 p2(P[cps + 1], 1);
        glm::vec4 p3(P[cps + 2], 1);
        glm::vec4 p4(P[cps + 3], 1);
        glm::mat4 geo(p1, p2, p3, p4);

        // first point
        // glm::vec3 B0(glm::vec3::cross((p2 - p1).xyz(), (p3 - p1).xyz()));
        glm::vec3 B0(0, 0, 1);
        CurvePoint point;
        glm::vec4 pos(geo * (spline_matrix * canonical_basis[0]));
        point.V = glm::vec3(pos);

        glm::vec4 T(geo * (TMatrix * canonical_basis[0]));
        point.T = glm::normalize(glm::vec3(T));

        point.B = B0;
        point.N = glm::normalize(glm::cross(point.B, point.T));
        point.B = glm::normalize(glm::cross(point.T, point.N));

        vertex.push_back(point.V);
        normal.push_back(point.N);
        binormal.push_back(point.B);
        tangent.push_back(point.T);

        for (size_t j = 1; j < canonical_basis.size(); ++j) {
            CurvePoint point;
            glm::vec4 pos(geo * (spline_matrix * canonical_basis[j]));
            point.V = glm::vec3(pos);

            glm::vec4 T(geo * (TMatrix * canonical_basis[j]));
            point.T = glm::normalize(glm::vec3(T));
            point.B = binormal.back();
            point.N = glm::normalize(glm::cross(point.B, point.T));
            point.B = glm::normalize(glm::cross(point.T, point.N));

            vertex.push_back(point.V);
            normal.push_back(point.N);
            binormal.push_back(point.B);
            tangent.push_back(point.T);
        }
    }
}

void SweepLoader::evalBspline(const std::vector<glm::vec3>& P, unsigned steps) {
    Curve& curve(GetModel().AllocCurve());
    // Check
    if (P.size() < 4) {
        fprintf(stderr,
                "evalBspline must be called with 4 or more control points.\n");
        return;
    }

    auto& vertex(curve.GetVertexBuffer());
    auto& normal(curve.GetNormalBuffer());
    auto& binormal(curve.GetBinormalBuffer());
    auto& tangent(curve.GetTangentBuffer());

    std::vector<glm::vec4> canonical_basis(GenerateCanonicalBasis(steps));

    glm::mat4 spline_matrix(1, -3, 3, -1, 4, 0, -6, 3, 1, 3, 3, -3, 0, 0, 0, 1);
    spline_matrix = glm::transpose(spline_matrix);

    spline_matrix /= 6;

    glm::mat4 TMatrix(-1, 2, -1, 0, 0, -4, 3, 0, 1, 2, -3, 0, 0, 0, 1, 0);
    TMatrix = glm::transpose(TMatrix);

    TMatrix /= 2;

    // glm::vec3 B0(glm::normalize(glm::cross((p2 - p1), (p3 - p1)));
    glm::vec3 B0(0, 0, 1);
    for (size_t i = 0; i < P.size() - 3; ++i) {
        glm::vec4 p1(P[i], 1);
        glm::vec4 p2(P[i + 1], 1);
        glm::vec4 p3(P[i + 2], 1);
        glm::vec4 p4(P[i + 3], 1);
        glm::mat4 geo(p1, p2, p3, p4);

        if (!binormal.empty()) {
            B0 = binormal.back();
        }

        CurvePoint point;
        glm::vec4 pos(geo * (spline_matrix * canonical_basis[0]));
        point.V = glm::vec3(pos);

        glm::vec4 T(geo * (TMatrix * canonical_basis[0]));
        point.T = glm::normalize(glm::vec3(T));

        point.B = B0;
        point.N = glm::normalize(glm::cross(point.B, point.T));
        point.B = glm::normalize(glm::cross(point.T, point.N));

        vertex.push_back(point.V);
        normal.push_back(point.N);
        binormal.push_back(point.B);
        tangent.push_back(point.T);

        for (size_t j = 1; j < canonical_basis.size(); ++j) {
            CurvePoint point;
            glm::vec4 pos(geo * (spline_matrix * canonical_basis[j]));
            point.V = glm::vec3(pos);

            glm::vec4 T(geo * (TMatrix * canonical_basis[j]));
            point.T = glm::normalize(glm::vec3(T));

            point.B = binormal.back();
            point.N = glm::normalize(glm::cross(point.B, point.T));
            point.B = glm::normalize(glm::cross(point.T, point.N));

            vertex.push_back(point.V);
            normal.push_back(point.N);
            binormal.push_back(point.B);
            tangent.push_back(point.T);
        }
    }

    // check closed
    if (vertex.size() > 2 && vertex.front() == vertex.back() &&
        tangent.front() == tangent.back()) {
        if (normal.front() != normal.back())
            fprintf(stderr, "detect closed curve break.\n");
    }
}

void SweepLoader::evalCircle(float radius, unsigned steps) {
    // Preallocate a curve with steps+1 CurvePoints
    Curve& curve(GetModel().AllocCurve());

    auto& vertex(curve.GetVertexBuffer());
    auto& normal(curve.GetNormalBuffer());
    auto& binormal(curve.GetBinormalBuffer());
    auto& tangent(curve.GetTangentBuffer());
    // Fill it in counterclockwise
    for (unsigned i = 0; i <= steps; ++i) {
        // step from 0 to 2pi
        float t = 2.0f * glm::pi<float>() * float(i) / steps;

        // Initialize position
        // We're pivoting counterclockwise around the y-axis
        vertex.push_back(radius * glm::vec3(cos(t), sin(t), 0));

        // Tangent vector is first derivative
        tangent.push_back(glm::vec3(-sin(t), cos(t), 0));

        // Normal vector is second derivative
        normal.push_back(glm::vec3(-cos(t), -sin(t), 0));

        // Finally, binormal is facing up.
        binormal.push_back(glm::vec3(0, 0, 1));
    }
}

void SweepLoader::makeSurfRev(const Curve& profile, unsigned steps) {
    if (!checkFlat(profile)) {
        fprintf(stderr, "surfRev profile curve must be flat on xy plane.\n");
        return;
    }
    Mesh& mesh(GetModel().AllocMesh());
    auto& vertex(mesh.GetVertexBuffer());
    auto& normal(mesh.GetNormalBuffer());
    auto& index(mesh.GetIndexBuffer());
    for (unsigned step = 0; step <= steps; ++step) {
        glm::mat4 M(1.0f);
        M = glm::rotate(M, step * (2 * glm::pi<float>() / steps),
                        glm::vec3(0.0f, 1.0f, 0.0f));
        for (size_t c = 0; c < profile.size(); ++c) {
            CurvePoint point(profile.GetPoint(c));
            glm::vec4 v(point.V, 1);
            glm::vec3 n(point.N);
            v = M * v;
            n = -(glm::mat3(M) * n);

            vertex.push_back(glm::vec3(v));
            normal.push_back(glm::normalize(n));
        }
    }
    // triangle
    size_t psize = profile.size();
    for (unsigned step = 1; step <= steps; ++step) {
        for (size_t c = 0; c < psize - 1; ++c) {
            std::array<GLuint, 3> tri;
            tri[0] = step * psize + c;
            tri[1] = (step - 1) * psize + c;
            tri[2] = (step - 1) * psize + c + 1;
            index.insert(index.end(), tri.begin(), tri.end());

            tri[1] = tri[2];
            tri[2] = tri[0] + 1;
            index.insert(index.end(), tri.begin(), tri.end());
        }
    }
}

void SweepLoader::makeGenCyl(const Curve& profile, const Curve& sweep) {
    if (!checkFlat(profile)) {
        fprintf(stderr, "genCyl profile curve must be flat on xy plane.\n");
        return;
    }
    Mesh& mesh(GetModel().AllocMesh());
    auto& vertex(mesh.GetVertexBuffer());
    auto& normal(mesh.GetNormalBuffer());
    auto& index(mesh.GetIndexBuffer());

    size_t ssize = sweep.size();
    size_t psize = profile.size();
    for (size_t i = 0; i < ssize; ++i) {
        CurvePoint cpoint(sweep.GetPoint(i));
        glm::mat4 M(glm::vec4(cpoint.N, 0), glm::vec4(cpoint.B, 0),
                    glm::vec4(cpoint.T, 0), glm::vec4(cpoint.V, 1));
        for (size_t j = 0; j < psize; ++j) {
            CurvePoint point(profile.GetPoint(j));
            glm::vec4 v(point.V, 1);
            glm::vec3 t(point.T);
            v = M * v;
            t = glm::mat3(M) * t;
            glm::vec3 n(glm::normalize(glm::cross(t, cpoint.T)));

            vertex.push_back(glm::vec3(v));
            normal.push_back(n);
        }
    }

    // triangle
    for (size_t step = 1; step <= ssize; ++step) {
        for (size_t c = 0; c < psize - 1; ++c) {
            std::array<GLuint, 3> tri;
            tri[0] = (step % ssize) * psize + c;
            tri[1] = (step - 1) * psize + c;
            tri[2] = (step - 1) * psize + c + 1;
            index.insert(index.end(), tri.begin(), tri.end());
            tri[1] = tri[2];
            tri[2] = tri[0] + 1;
            index.insert(index.end(), tri.begin(), tri.end());
        }
    }
}
