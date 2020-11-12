//
// Created by jxunhappy on 2017/7/1.
//

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <chrono>
#include <memory>
#include <string>
#include "base/window.h"
#include "math/vec.h"

namespace engine {
class InputManager;
class ShaderManager;
class TextureManager;

class MaterialManager;

class ModelManager;

class LightManager;

class SceneManager;

class UIManager;
class LuaManager;
class Application {
public:
    class Delegate {
    public:
        Delegate(){};

        virtual ~Delegate(){};

        // return the resource directory.
        virtual std::string GetResourceRoot() = 0;

        virtual engine::math::ivec2 GetWindowSize() = 0;

        virtual std::string GetWindowTitle() = 0;

        virtual bool Initialize(Application* application) = 0;

        virtual void WillEnter() = 0;

        virtual void Update() = 0;

        virtual bool Done() = 0;

        virtual void WillWindowClose() = 0;

        virtual void OnExit() = 0;
    };

public:
    Application(Delegate* delegate);

    ~Application();

public:
    int Run();

    engine::math::ivec2 GetWindowSize();

    // time delta in ms
    float GetDeltaTime();

public:
    InputManager* input_manager();

    ShaderManager* shader_manager();

    TextureManager* texture_manager();

    MaterialManager* material_manager();

    ModelManager* model_manager();

    LightManager* light_manager();

    SceneManager* scene_manager();

    UIManager* ui_manager();

    LuaManager* lua_manager();

private:
    bool Initialize();
    void UnInitialize();

    void OnWindowSize(int width, int height);

private:
    Delegate* delegate_;
    Window window_;
    std::chrono::steady_clock::time_point last_time_point_;
    std::chrono::steady_clock::duration delta_time_duration_;

private:
    struct Context;
    std::unique_ptr<Context> context_;
};

// temp for access
extern Application* application;

}  // namespace engine

#endif  // APPLICATION_H
