#ifndef LUA_MANAGER_IMPL_H_
#define LUA_MANAGER_IMPL_H_

#include <string>
#include "lua.hpp"
#include "lua_manager.h"

namespace engine {
class LuaManagerImpl : public LuaManager {
public:
    bool Initialize();

    void UnInitialize();

    void Update();

    virtual void RunFile(const std::string& file) override;

    virtual void RunChunk(const std::string& buffer) override;

private:
    lua_State* main_lua_state_{nullptr};
};

}  // namespace engine

#endif  // LUA_MANAGER_IMPL_H_