//
// Created by jxunhappy on 2017/7/1.
//

#include "app.h"
#include "light_manager.h"
#include "material_manager.h"
#include "model_manager.h"
#include "scene/world.h"
#include "ui_manager.h"

namespace {
const int kWindowWidth = 1024;
const int kWindowHeight = 768;
const char kWindowTitle[] = "Lotus Example";

#ifdef __APPLE__
const char kFileRoot[] = "/path/to/src/directory";
#elif _WIN32
const char kFileRoot[] = "D:/src/LearnGL";
#endif
}  // namespace

App::App() : done_(false), application_(nullptr) {}

App::~App() {}

engine::math::ivec2 App::GetWindowSize() {
    return engine::math::ivec2(kWindowWidth, kWindowHeight);
}

std::string App::GetResourceRoot() {
    return std::string(kFileRoot);
}

std::string App::GetWindowTitle() {
    return std::string(kWindowTitle);
}

bool App::Initialize(engine::Application* application) {
    // load resource
    application->material_manager()->LoadMaterialAsset("data/material.xml");
    application->model_manager()->LoadModelAsset("data/model.xml");
    application->light_manager()->LoadLightAsset("data/light.xml");
    // load world
    World world;
    world.Load("data/world.xml");
    application->ui_manager()->LoadUI("data/ui.xml");
    application->ui_manager()->SetCurrentUI("data/ui.xml");
    return true;
}

void App::WillEnter() {
    printf("App::WinEnter\n");
}

void App::Update() {
    printf("App::Update\n");
}

bool App::Done() {
    return done_;
}

void App::WillWindowClose() {
    done_ = true;
}

void App::OnExit() {
    printf("App::OnExit\n");
}
