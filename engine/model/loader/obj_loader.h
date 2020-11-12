#ifndef MODEL_LOADER_OBJ_LOADER_H_
#define MODEL_LOADER_OBJ_LOADER_H_

#include <vector>
#include "model_loader.h"

class ObjLoader : public ModelLoader {
public:
    using ModelLoader::ModelLoader;

public:
    virtual bool Load(const std::string& path) override;

private:
    void GenerateNormal(Mesh& mesh);
};

#endif  // MODEL_LOADER_OBJ_LOADER_H_
