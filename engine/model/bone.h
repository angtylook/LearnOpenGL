#ifndef MODEL_BONE_H_
#define MODEL_BONE_H_

#include <string>
#include <vector>

class Bone {
public:
    Bone();
    Bone(const std::string& name, int index, int parent);
    const std::string& GetName();
    void SetName(const std::string& name);
    int GetIndex();
    void SetIndex(int index);
    int GetParent();
    void SetParent(int parent);
    const std::vector<int>& GetChildren();
    void AddChild(int index);
    // not implements
    // void RemoveChild(int index);
private:
    int parent_;
    int index_;
    std::string name_;
    std::vector<int> children_;
};

typedef std::vector<Bone> BoneList;

#endif  // MODEL_BONE_H_
