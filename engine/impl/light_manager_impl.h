#ifndef LIGHT_MANAGER_IMPL_H_
#define LIGHT_MANAGER_IMPL_H_

#include <map>
#include <memory>
#include <string>
#include "base/micros.h"
#include "base/xml.h"
#include "light_manager.h"

namespace engine {

class LightManagerImpl : public LightManager {
public:
    LightManagerImpl() = default;
    virtual ~LightManagerImpl() = default;

public:
    virtual Light* GetLight(const std::string& light) override;

    virtual bool LoadLightAsset(const std::string& path) override;

private:
    bool LoadLight(xml::Node* xml_node);

private:
    std::map<std::string, std::unique_ptr<Light>> lights_;

private:
    DISALLOW_COPY_AND_ASSIGN(LightManagerImpl)
};

}  // namespace engine

#endif  // LIGHT_MANAGER_IMPL_H_
