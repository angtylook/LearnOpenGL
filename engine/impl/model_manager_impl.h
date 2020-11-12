#ifndef MODEL_MANAGER_IMPL_H_
#define MODEL_MANAGER_IMPL_H_

#include <map>
#include <string>
#include "base/micros.h"
#include "base/xml.h"
#include "model/loader/model_loader.h"
#include "model_manager.h"

namespace engine {
class ModelManagerImpl : public ModelManager {
public:
    ModelManagerImpl() = default;
    virtual ~ModelManagerImpl() = default;
public:
    bool Initialize();

    void UnInitialize();

    virtual Model* GetModel(const std::string& name) override;

    virtual bool LoadModel(const std::string& model_path,
                           const std::string& model_type,
                           const std::string& model_name) override;

    virtual bool LoadModelAsset(const std::string& path) override;

    virtual void Clear() override;

private:
    bool LoadModel(engine::xml::Node* xml_node);

    std::unique_ptr<ModelLoader> CreateLoader(Model* model);

private:
    std::map<std::string, std::unique_ptr<Model>> models_;

private:
    DISALLOW_COPY_AND_ASSIGN(ModelManagerImpl)
};

}  // namespace engine

#endif  // MODEL_MANAGER_IMPL_H_
