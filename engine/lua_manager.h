#ifndef LUA_MANAGER_H_
#define LUA_MANAGER_H_

#include <string>

#include "base/micros.h"

namespace engine {
class LuaManager {
public:
    virtual void RunFile(const std::string& file) = 0;

    virtual void RunChunk(const std::string& buffer) = 0;

protected:
    LuaManager() = default;
    virtual ~LuaManager() = default;

private:
    DISALLOW_COPY_AND_ASSIGN(LuaManager)
};

}  // namespace engine

#endif  // LUA_MANAGER_H_