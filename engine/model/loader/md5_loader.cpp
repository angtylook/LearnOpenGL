#include "md5_loader.h"

#include <array>
#include <fstream>

#include "application.h"
#include "base/filesystem.h"
#include "base/string_util.h"
#include "material_manager.h"
#include "math/math_util.h"
#include "model/material.h"

using namespace engine;

namespace {
struct Weight {
    float weight = 0.0f;
    int bone_id = 0;
    glm::vec3 position;
};

// The JointInfo stores the information necessary to build the
// skeletons for each frame
struct JointInfo {
    std::string name;
    int parent;
    int flags;
    int start_index;
};
typedef std::vector<JointInfo> JointInfoList;

typedef std::vector<Joint> FrameBaseList;

struct FrameData {
    int id;
    std::vector<float> data;
};
typedef std::vector<FrameData> FrameDataList;

void PostProcessSkeleton(Animation& animation,
                         const JointInfoList& joint_infos,
                         const FrameBaseList& frame_bases,
                         const FrameData& frame_data) {
    Skeleton skeleton;

    for (unsigned int i = 0; i < joint_infos.size(); ++i) {
        unsigned int j = 0;

        const JointInfo& jointInfo = joint_infos[i];
        // Start with the base frame position and orientation.
        Joint animatedJoint = frame_bases[i];

        if (jointInfo.flags & 1)  // Pos.x
        {
            animatedJoint.translate.x =
                frame_data.data[jointInfo.start_index + j++];
        }
        if (jointInfo.flags & 2)  // Pos.y
        {
            animatedJoint.translate.y =
                frame_data.data[jointInfo.start_index + j++];
        }
        if (jointInfo.flags & 4)  // Pos.x
        {
            animatedJoint.translate.z =
                frame_data.data[jointInfo.start_index + j++];
        }
        if (jointInfo.flags & 8)  // Orient.x
        {
            animatedJoint.rotate.x =
                frame_data.data[jointInfo.start_index + j++];
        }
        if (jointInfo.flags & 16)  // Orient.y
        {
            animatedJoint.rotate.y =
                frame_data.data[jointInfo.start_index + j++];
        }
        if (jointInfo.flags & 32)  // Orient.z
        {
            animatedJoint.rotate.z =
                frame_data.data[jointInfo.start_index + j++];
        }

        ComputeQuatW(animatedJoint.rotate);

        // compute translate and rotate in object local space.
        if (jointInfo.parent >= 0)  // Has a parent joint
        {
            Joint parent_joint = skeleton.GetJoint(jointInfo.parent);
            // rotate to object local space
            glm::vec3 rot_pos = parent_joint.rotate * animatedJoint.translate;
            // translate to object local space
            animatedJoint.translate = parent_joint.translate + rot_pos;
            animatedJoint.rotate = parent_joint.rotate * animatedJoint.rotate;
            animatedJoint.rotate = glm::normalize(animatedJoint.rotate);
        }

        skeleton.AddJoint(animatedJoint, i);
    }

    animation.AddFrame(skeleton);
}

void PostProcessMesh(Mesh& mesh,
                     std::vector<Weight>& weights,
                     Skeleton& skeleton) {
    // process vertex data
    mesh.SetEnableVertexPosition(true);
    auto& vertex_data(mesh.GetVertexData());
    size_t vertex_size = vertex_data.size();
    for (size_t i = 0; i < vertex_size; ++i) {
        VertexData& data = vertex_data[i];
        for (int i = 0; i < kVertexBoneNumber; ++i) {
            int weight_index = data.bone_id[i];
            if (weight_index != -1) {
                Weight weight = weights[weight_index];
                data.bone_id[i] = weight.bone_id;
                data.weight[i] = weight.weight;
                data.position[i] = weight.position;
            }
        }
    }
    // general bind pose vertex position
    for (size_t i = 0; i < vertex_size; ++i) {
        glm::vec3 final_position(0);
        VertexData data = vertex_data[i];
        for (int i = 0; i < kVertexBoneNumber; ++i) {
            int bone_id = data.bone_id[i];
            if (bone_id != -1) {
                float weight = data.weight[i];
                glm::vec3 position = data.position[i];
                Joint joint = skeleton.GetJoint(bone_id);
                // Convert the weight position from Joint local space to object
                // space
                glm::vec3 rotated_position = joint.rotate * position;

                final_position += (joint.translate + rotated_position) * weight;
            }
        }
        mesh.GetVertexBuffer().push_back(final_position);
        mesh.GetNormalBuffer().push_back(glm::vec3(0));
    }
    // genenral normals
    auto& vertex_buffer(mesh.GetVertexBuffer());
    auto& normal_buffer(mesh.GetNormalBuffer());
    auto& index_buffer(mesh.GetIndexBuffer());
    size_t index_size = index_buffer.size();
    size_t tri_size = index_size / 3;
    for (size_t i = 0; i < tri_size; ++i) {
        size_t tri = 3 * i;
        glm::ivec3 triangle(index_buffer[tri], index_buffer[tri + 1],
                            index_buffer[tri + 2]);

        glm::vec3& v0 = vertex_buffer[triangle.x];
        glm::vec3& v1 = vertex_buffer[triangle.y];
        glm::vec3& v2 = vertex_buffer[triangle.z];

        glm::vec3 normal = glm::cross(v1 - v0, v2 - v0);

        normal_buffer[triangle.x] += normal;
        normal_buffer[triangle.y] += normal;
        normal_buffer[triangle.z] += normal;
    }
    for (size_t i = 0; i < normal_buffer.size(); ++i) {
        normal_buffer[i] = glm::normalize(normal_buffer[i]);
    }
}
}  // namespace

bool MD5ModelLoader::Load(const std::string& path) {
    bool mesh_ok = LoadModel(path + ".md5mesh");
    bool anim_ok = LoadAnim(path + ".md5anim");
    if (anim_ok)
        GetModel().SetAnimation("default");
    return mesh_ok;
}

bool MD5ModelLoader::LoadModel(const std::string& path) {
    if (!engine::fs::Exist(path)) {
        fprintf(stderr, "MD5ModelLoader::LoadModel: failed to find file<%s>.\n",
                path.c_str());
        return false;
    }

    engine::fs::Path filePath(path);
    // store the parent path used for loading images relative to this file.
    engine::fs::Path parent_path = filePath.GetParentPath();

    std::ifstream file(engine::fs::GetRawPath(filePath).data());

    int64_t fileLength = engine::fs::GetFileLength(filePath);
    assert(fileLength > 0);

    BoneList& bones = GetModel().GetBoneList();
    MeshList& meshes = GetModel().GetMeshList();
    Skeleton& skeleton = GetModel().GetBindPoseSkeleton();
    bones.clear();
    meshes.clear();

    int num_joints(0);
    int num_meshes(0);

    std::string param;
    std::string junk;  // Read junk from the file
    file >> param;
    while (!file.eof()) {
        if (param == "MD5Version") {
            int version = 0;
            file >> version;
            fprintf(stdout, "MD5ModelLoader: load model version %d\n", version);
            if (version != 10) {
                fprintf(stdout,
                        "MD5ModelLoader: model version %d is unsupport.\n",
                        version);
            }
        } else if (param == "commandline") {
            std::array<char, 1024> cmd;
            file.getline(cmd.data(), 1024, '\n');
            fprintf(stdout, "MD5ModelLoader: load model cmd <%s>\n",
                    cmd.data());
        } else if (param == "numJoints") {
            file >> num_joints;
            bones.reserve(num_joints);
            fprintf(stdout,
                    "MD5ModelLoader: load model with %d joints(bones)\n",
                    num_joints);
        } else if (param == "numMeshes") {
            file >> num_meshes;
            meshes.reserve(num_meshes);
            fprintf(stdout, "MD5ModelLoader: load model with %d joints\n",
                    num_joints);
        } else if (param == "joints") {
            Joint joint;
            Bone bone;
            file >> junk;  // Read the '{' character
            int bone_parent_id = -1;
            std::string bone_name;
            for (int i = 0; i < num_joints; ++i) {
                // The first vector is the position of the joint in object local
                // space, and the second vector is the orientation of the joint
                // in object local space.
                file >> bone_name >> bone_parent_id >> junk >>
                    joint.translate.x >> joint.translate.y >>
                    joint.translate.z >> junk >> junk >> joint.rotate.x >>
                    joint.rotate.y >> joint.rotate.z >> junk;

                RemoveQuotes(bone_name);
                ComputeQuatW(joint.rotate);

                bone.SetIndex(i);
                bone.SetName(bone_name);
                bone.SetParent(bone_parent_id);
                if (bone_parent_id != -1) {
                    Bone& parent = bones[bone_parent_id];
                    parent.AddChild(bone.GetIndex());
                }
                bones.push_back(bone);
                skeleton.AddJoint(joint, i);
                // Ignore everything else on the line up to the end-of-line
                // character.
                engine::fs::IgnoreLine(file, fileLength);
            }
            file >> junk;  // Read the '}' character
        } else if (param == "mesh") {
            Mesh mesh;
            std::vector<Weight> weights;

            int numVerts, numTris, numWeights;

            file >> junk;  // Read the '{' character
            file >> param;
            while (param != "}") {  // Read until we get to the '}' character
                if (param == "shader") {
                    std::string texture;
                    file >> texture;
                    RemoveQuotes(texture);

                    engine::fs::Path shaderPath(texture);
                    engine::fs::Path texturePath;
                    if (shaderPath.HasParentPath()) {
                        texturePath = shaderPath;
                    } else {
                        texturePath = parent_path.Append(shaderPath);
                    }

                    if (!texturePath.HasExtension()) {
                        texturePath.ReplaceExtension(".tga");
                    }

                    std::unique_ptr<Material> material(
                        std::make_unique<Material>(texturePath.data()));
                    material->SetDiffuse(texturePath.data());
                    material->SetSpecular(texturePath.data());
                    material->SetShininess(32);
                    application->material_manager()->AddMaterial(
                        texturePath.data(), std::move(material));
                    mesh.SetMaterial(texturePath.data());

                    file.ignore(fileLength, '\n');
                } else if (param == "numverts") {
                    file >> numVerts;  // Read in the vertices
                    fprintf(stdout,
                            "MD5ModelLoader load mesh with %d numverts\n",
                            numVerts);
                    engine::fs::IgnoreLine(file, fileLength);
                    for (int i = 0; i < numVerts; ++i) {
                        glm::vec2 texcord;
                        VertexData vertex_data;
                        int start = 0;
                        int count = 0;
                        file >> junk >> junk >> junk  // vert vertIndex (
                            >> texcord.x >> texcord.y >> junk  //  s t )
                            >> start >> count;
                        engine::fs::IgnoreLine(file, fileLength);
                        for (int i = 0; i < count && i < kVertexBoneNumber;
                             ++i) {
                            vertex_data.bone_id[i] = start + i;
                            // weight to be filled later.
                        }

                        mesh.GetVertexData().push_back(vertex_data);
                        mesh.GetUVBuffer().push_back(texcord);
                    }
                } else if (param == "numtris") {
                    file >> numTris;
                    fprintf(stdout,
                            "MD5ModelLoader load mesh with %d numtris\n",
                            numTris);
                    engine::fs::IgnoreLine(file, fileLength);
                    for (int i = 0; i < numTris; ++i) {
                        glm::ivec3 tri;
                        file >> junk >> junk >> tri.x >> tri.y >> tri.z;

                        mesh.GetIndexBuffer().push_back(tri.x);
                        mesh.GetIndexBuffer().push_back(tri.y);
                        mesh.GetIndexBuffer().push_back(tri.z);

                        engine::fs::IgnoreLine(file, fileLength);
                    }
                } else if (param == "numweights") {
                    file >> numWeights;
                    fprintf(stdout,
                            "MD5ModelLoader load mesh with %d numWeights\n",
                            numWeights);
                    engine::fs::IgnoreLine(file, fileLength);
                    for (int i = 0; i < numWeights; ++i) {
                        Weight weight;
                        // The ��weightBias�� parameter is a ratio that
                        // determines how much of the joint��s orientation and
                        // position is applied to the vertex��s final position.
                        // The ��weightPosition�� parameter is a 3-component
                        // vector which describes the position of the weight in
                        // joint-local space and must be rotated by the joint��s
                        // orientation and added to the joint��s position before
                        // being applied to the final vertex position.
                        file >> junk >> junk >> weight.bone_id >>
                            weight.weight >> junk >> weight.position.x >>
                            weight.position.y >> weight.position.z >> junk;

                        engine::fs::IgnoreLine(file, fileLength);
                        weights.push_back(weight);
                    }
                } else {
                    engine::fs::IgnoreLine(file, fileLength);
                }

                file >> param;
            }
            // post process
            PostProcessMesh(mesh, weights, skeleton);
            meshes.push_back(mesh);
        }
        file >> param;
    }

    assert(bones.size() == num_joints);
    assert(skeleton.JointSize() == num_joints);
    assert(meshes.size() == num_meshes);

    return true;
}

bool MD5ModelLoader::LoadAnim(const std::string& path) {
    engine::fs::Path filePath(path);
    if (!engine::fs::Exist(filePath)) {
        fprintf(stderr, "MD5ModelLoader::LoadAnim Failed to find file<%s>\n",
                path.c_str());
        return false;
    }

    std::string param;
    std::string junk;  // Read junk from the file

    std::ifstream file(engine::fs::GetRawPath(filePath).data());
    int64_t fileLength = engine::fs::GetFileLength(filePath);
    if (fileLength <= 0) {
        fprintf(stderr, "MD5ModelLoader::LoadAnim Failed to load file<%s>\n",
                path.c_str());
        return false;
    }

    Animation& anim = GetModel().AllocAnimation("default");

    JointInfoList joint_infos;
    FrameBaseList frame_bases;

    int num_frames = 0;
    int num_joints = 0;
    int num_animated_components = 0;
    file >> param;

    while (!file.eof()) {
        if (param == "MD5Version") {
            int version = 0;
            file >> version;
            fprintf(stdout, "MD5ModelLoader: load animation version %d\n",
                    version);
            if (version != 10) {
                fprintf(stdout,
                        "MD5ModelLoader: animation version %d is unsupport.\n",
                        version);
            };
        } else if (param == "commandline") {
            std::array<char, 1024> cmd;
            file.getline(cmd.data(), 1024, '\n');
            fprintf(stdout, "MD5ModelLoader: load animation cmd<%s>\n",
                    cmd.data());
        } else if (param == "numFrames") {
            file >> num_frames;
            file.ignore(fileLength, '\n');
            anim.ReserveAnimationFrames(num_frames);
            fprintf(stdout, "MD5ModelLoader: load animation frames <%d>\n",
                    num_frames);
        } else if (param == "numJoints") {
            file >> num_joints;
            file.ignore(fileLength, '\n');
            fprintf(stdout, "MD5ModelLoader: load animation joints <%d>\n",
                    num_joints);
        } else if (param == "frameRate") {
            int frame_rate = 0;
            file >> frame_rate;
            anim.SetFrameRate(frame_rate);
            file.ignore(fileLength, '\n');
            fprintf(stdout, "MD5ModelLoader: load animation frame rate <%d>\n",
                    frame_rate);
        } else if (param == "numAnimatedComponents") {
            file >> num_animated_components;
            file.ignore(fileLength, '\n');
            fprintf(stdout,
                    "MD5ModelLoader: load animation animated components <%d>\n",
                    num_animated_components);
        } else if (param == "hierarchy") {
            file >> junk;  // read in the '{' character
            for (int i = 0; i < num_joints; ++i) {
                JointInfo joint;
                file >> joint.name >> joint.parent >> joint.flags >>
                    joint.start_index;
                RemoveQuotes(joint.name);
                joint_infos.push_back(joint);
                file.ignore(fileLength, '\n');
            }
            file >> junk;  // read in the '}' character
        } else if (param == "bounds") {
            file >> junk;  // read in the '{' character
            file.ignore(fileLength, '\n');
            for (int i = 0; i < num_frames; ++i) {
                glm::vec3 min;
                glm::vec3 max;
                file >> junk;  // read in the '(' character
                file >> min.x >> min.y >> min.z;
                file >> junk >> junk;  // read in the ')' and '(' characters.
                file >> max.x >> max.y >> max.z;

                BoundingBox bound(min, max);
                anim.AddBoundingBox(bound);
                file.ignore(fileLength, '\n');
            }

            file >> junk;  // read in the '}' character
            file.ignore(fileLength, '\n');
        } else if (param == "baseframe") {
            file >> junk;  // read in the '{' character
            file.ignore(fileLength, '\n');

            for (int i = 0; i < num_joints; ++i) {
                Joint frame_base;
                file >> junk;
                file >> frame_base.translate.x >> frame_base.translate.y >>
                    frame_base.translate.z;
                file >> junk >> junk;
                file >> frame_base.rotate.x >> frame_base.rotate.y >>
                    frame_base.rotate.z;
                file.ignore(fileLength, '\n');
                frame_bases.push_back(frame_base);
            }
            file >> junk;  // read in the '}' character
            file.ignore(fileLength, '\n');
        } else if (param == "frame") {
            FrameData frame;
            file >> frame.id >> junk;  // Read in the '{' character
            file.ignore(fileLength, '\n');

            for (int i = 0; i < num_animated_components; ++i) {
                float frame_data;
                file >> frame_data;
                frame.data.push_back(frame_data);
            }

            // Build a skeleton for this frame
            PostProcessSkeleton(anim, joint_infos, frame_bases, frame);

            file >> junk;  // Read in the '}' character
            file.ignore(fileLength, '\n');
        }

        file >> param;
    }  // while ( !file.eof )

    assert(joint_infos.size() == num_joints);
    assert(frame_bases.size() == num_joints);
    assert(anim.GetAnimationFrames() == num_frames);

    return true;
}
