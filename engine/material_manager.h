#ifndef MATERIAL_MANAGER_H_
#define MATERIAL_MANAGER_H_

#include <memory>
#include <string>
#include "base/micros.h"

class Material;

namespace engine {
class MaterialManager {
public:
    virtual Material* GetMaterial(const std::string& material) = 0;

    virtual bool AddMaterial(const std::string& material_name,
                             std::unique_ptr<Material> material) = 0;

    virtual bool LoadMaterialAsset(const std::string& path) = 0;

protected:
    MaterialManager() = default;
    virtual ~MaterialManager() = default;

private:
    DISALLOW_COPY_AND_ASSIGN(MaterialManager)
};

}  // namespace engine

#endif  // MATERIAL_MANAGER_H_
