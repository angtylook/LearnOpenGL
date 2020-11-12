#ifndef MATERIAL_MANAGER_IMPL_H_
#define MATERIAL_MANAGER_IMPL_H_

#include <map>
#include <memory>
#include <string>
#include "base/micros.h"
#include "base/xml.h"
#include "material_manager.h"
#include "model/material.h"

namespace engine {
class MaterialManagerImpl : public MaterialManager {
public:
    MaterialManagerImpl() = default;
    virtual ~MaterialManagerImpl() = default;
public:
    virtual Material* GetMaterial(const std::string& material) override;

    virtual bool AddMaterial(const std::string& material_name,
                             std::unique_ptr<Material> material) override;

    virtual bool LoadMaterialAsset(const std::string& path) override;

private:
    bool LoadMaterial(engine::xml::Node* xml_node);

private:
    std::map<std::string, std::unique_ptr<Material>> materials_;

private:
    DISALLOW_COPY_AND_ASSIGN(MaterialManagerImpl)
};

}  // namespace engine

#endif  // MATERIAL_MANAGER_H_
