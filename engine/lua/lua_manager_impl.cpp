#include "lua_manager_impl.h"
#include "base/filesystem.h"

namespace engine {
bool LuaManagerImpl::Initialize() {
    main_lua_state_ = luaL_newstate();
    luaL_openlibs(main_lua_state_);
    return true;
}

void LuaManagerImpl::UnInitialize() {
    lua_close(main_lua_state_);
}

void LuaManagerImpl::Update() {}

void LuaManagerImpl::RunFile(const std::string& file) {
    if (!engine::fs::Exist(engine::fs::Path(file))) {
        fprintf(stderr, "lua file: \"%s\" is not exist.\n", file.c_str());
        return;
    }

    std::string buffer = engine::fs::GetFileContent(engine::fs::Path(file));
    if (buffer.empty()) {
        fprintf(stderr, "lua file: \"%s\" is empty.\n", file.c_str());
        return;
    }

    RunChunk(buffer);
}

void LuaManagerImpl::RunChunk(const std::string& buffer) {
    if (buffer.empty()) {
        fprintf(stderr, "the buffer is empty.\n");
        return;
    }

    int error = luaL_loadstring(main_lua_state_, buffer.c_str());
    if (error != 0) {
        fprintf(stderr, "load lua error:\n %s\n",
                lua_tostring(main_lua_state_, -1));
        return;
    }
    // lua_State*, args, ret args, error function
    error = lua_pcall(main_lua_state_, 0, 0, 0);
    if (error != 0) {
        fprintf(stderr, "run lua error:\n %s\n",
                lua_tostring(main_lua_state_, -1));
        return;
    }
}

}  // namespace engine