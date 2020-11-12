#ifndef MODEL_MANAGER_H_
#define MODEL_MANAGER_H_

#include <string>
#include "base/micros.h"

class Model;
namespace engine {
class ModelManager {
public:
    ModelManager() = default;
    virtual ~ModelManager() = default;

public:
    virtual Model* GetModel(const std::string& name) = 0;

    virtual bool LoadModel(const std::string& model_path,
                           const std::string& model_type,
                           const std::string& model_name) = 0;

    virtual bool LoadModelAsset(const std::string& path) = 0;

    virtual void Clear() = 0;

private:
    DISALLOW_COPY_AND_ASSIGN(ModelManager)
};

}  // namespace engine

#endif  // MODEL_MANAGER_H_
