//
// Created by jxunhappy on 2017/7/1.
//

#include "application.h"
#include <thread>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "base/log.h"
#include "engine.h"
#include "impl/input_manager_impl.h"
#include "impl/light_manager_impl.h"
#include "impl/material_manager_impl.h"
#include "impl/model_manager_impl.h"
#include "impl/scene_manager_impl.h"
#include "impl/shader_manager_impl.h"
#include "impl/texture_manager_impl.h"
#include "impl/ui_manager_impl.h"
#include "lua/lua_manager_impl.h"

namespace engine {

const std::chrono::milliseconds kFrameDurationMs(16);

struct Application::Context {
    InputManagerImpl input_manager;
    ShaderManagerImpl shader_manager;
    TextureManagerImpl texture_manager;
    MaterialManagerImpl material_manager;
    ModelManagerImpl model_manager;
    LightManagerImpl light_manager;
    SceneManagerImpl scene_manager;
    UIManagerImpl ui_manager;
    LuaManagerImpl lua_manager;

    bool Initialize() {
        if (!input_manager.Initialize())
            return false;
        if (!shader_manager.Initialize())
            return false;
        if (!texture_manager.Initialize())
            return false;
        if (!model_manager.Initialize())
            return false;
        if (!scene_manager.Initialize())
            return false;
        if (!ui_manager.Initialize())
            return false;
        if (!lua_manager.Initialize())
            return false;
        return true;
    }

    void UnInitialize() {
        lua_manager.UnInitialize();
        ui_manager.UnInitialize();
        scene_manager.UnInitialize();
        model_manager.UnInitialize();
        texture_manager.UnInitialize();
        shader_manager.UnInitialize();
        input_manager.UnInitialize();
    }
};

Application* application = nullptr;
Application::Application(Delegate* delegate)
    : delegate_(delegate), context_(new Context()) {
    application = this;
}

int Application::Run() {
    if (!Initialize()) {
        return -1;
    }
    //
    if (!delegate_->Initialize(this)) {
        return -1;
    }
    delegate_->WillEnter();

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;
    last_time_point_ = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(kFrameDurationMs);
    while (!delegate_->Done()) {
        start_time = std::chrono::steady_clock::now();
        delta_time_duration_ = start_time - last_time_point_;
        last_time_point_ = start_time;

        context_->input_manager.PollEvents();
        // update input
        context_->input_manager.ProcessInput();
        // delegate
        delegate_->Update();
        // update ui
        context_->ui_manager.Upate();
        // update scene
        context_->scene_manager.Update();
        // draw
        // clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // render
        context_->scene_manager.RenderScene();
        context_->ui_manager.Render();

        if (window_.ShouldClose()) {
            delegate_->WillWindowClose();
        }
        window_.SwapBuffers();

        end_time = std::chrono::steady_clock::now();
        if (end_time - start_time < kFrameDurationMs) {
            std::this_thread::sleep_for(end_time - start_time);
        }
    }
    delegate_->OnExit();
    UnInitialize();
    return 0;
}

Application::~Application() {}

bool Application::Initialize() {
    if (!log::Initialize())
        return false;
    if (!engine::InitGLFW())
        return false;

    engine::fs::Init(delegate_->GetResourceRoot());
    // create window
    window_.SetSizeHandler(std::bind(&Application::OnWindowSize, this,
                                     std::placeholders::_1,
                                     std::placeholders::_1));
    window_.SetEventHandler(std::bind(&InputManagerImpl::PushEvent,
                                      &context_->input_manager,
                                      std::placeholders::_1));
    auto sz = delegate_->GetWindowSize();
    std::string title = delegate_->GetWindowTitle();
    if (!window_.Create(sz.x, sz.y, title)) {
        fprintf(stderr, "can not create window.\n");
        return false;
    }
    window_.MakeContextCurrent();
    // opengl
    if (!engine::InitOpenGLLibrary())
        return false;
    // context
    if (!context_->Initialize())
        return false;

    return true;
}

void Application::UnInitialize() {
    context_->UnInitialize();
    window_.Destroy();
    glfwTerminate();
}

void Application::OnWindowSize(int width, int height) {
    // notify subsystem to handle this
}

engine::math::ivec2 Application::GetWindowSize() {
    return engine::math::ivec2(window_.GetWidth(), window_.GetHeight());
}

float Application::GetDeltaTime() {
    return static_cast<float>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            delta_time_duration_)
            .count());
}

InputManager* Application::input_manager() {
    return &context_->input_manager;
}

ShaderManager* Application::shader_manager() {
    return &context_->shader_manager;
}

TextureManager* Application::texture_manager() {
    return &context_->texture_manager;
}

MaterialManager* Application::material_manager() {
    return &context_->material_manager;
}

ModelManager* Application::model_manager() {
    return &context_->model_manager;
}

LightManager* Application::light_manager() {
    return &context_->light_manager;
}

SceneManager* Application::scene_manager() {
    return &context_->scene_manager;
}

UIManager* Application::ui_manager() {
    return &context_->ui_manager;
}

LuaManager* Application::lua_manager() {
    return &context_->lua_manager;
}

}  // namespace engine
