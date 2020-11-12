#ifndef MODEL_LOADER_SKELETON_LOADER_H_
#define MODEL_LOADER_SKELETON_LOADER_H_

#include "model_loader.h"

class SkeletonLoader : public ModelLoader {
public:
    using ModelLoader::ModelLoader;

public:
    virtual bool Load(const std::string& path) override;

private:
    bool LoadSkeleton(const std::string& path);
    bool LoadWeight(const std::string& path);
};

#endif  // MODEL_LOADER_SKELETON_LOADER_H_
