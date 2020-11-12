#ifndef LIGHT_MANAGER_H_
#define LIGHT_MANAGER_H_

#include <string>
#include "base/micros.h"

class Light;
namespace engine {
class LightManager {
public:
    virtual Light* GetLight(const std::string& light) = 0;

    virtual bool LoadLightAsset(const std::string& path) = 0;

protected:
    LightManager() = default;
    virtual ~LightManager() = default;

private:
    DISALLOW_COPY_AND_ASSIGN(LightManager)
};

}  // namespace engine

#endif  // LIGHT_MANAGER_H_
