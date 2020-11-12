#include "bone.h"

Bone::Bone() : parent_(0), index_(0) {}

Bone::Bone(const std::string& name, int index, int parent)
    : parent_(parent), index_(index), name_(name) {}

const std::string& Bone::GetName() {
    return name_;
}

void Bone::SetName(const std::string& name) {
    name_ = name;
}

int Bone::GetIndex() {
    return index_;
}

void Bone::SetIndex(int index) {
    index_ = index;
}

int Bone::GetParent() {
    return parent_;
}

void Bone::SetParent(int parent) {
    parent_ = parent;
}

const std::vector<int>& Bone::GetChildren() {
    return children_;
}

void Bone::AddChild(int index) {
    children_.push_back(index);
}
