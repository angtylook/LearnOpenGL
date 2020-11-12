#ifndef MODEL_LOADER_MD5_LOADER_H_
#define MODEL_LOADER_MD5_LOADER_H_

#include "model_loader.h"

class MD5ModelLoader : public ModelLoader {
public:
    using ModelLoader::ModelLoader;

public:
    virtual bool Load(const std::string& path) override;

private:
    bool LoadModel(const std::string& path);
    bool LoadAnim(const std::string& path);
};

#endif  // MODEL_LOADER_MD5_LOADER_H_
