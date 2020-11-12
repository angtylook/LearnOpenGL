#include "ui_manager_impl.h"

#include <functional>
#include <map>
#include <memory>
#include "application.h"
#include "input_manager.h"

namespace engine {
bool UIManagerImpl::Initialize() {
    engine::application->input_manager()->RegisterHandler(
        engine::InputManager::Priority::UI, engine::EventType::KEYBOARD,
        std::bind(&UIManagerImpl::ProcessEvent, this, std::placeholders::_1));
    engine::application->input_manager()->RegisterHandler(
        engine::InputManager::Priority::UI, engine::EventType::POSITION,
        std::bind(&UIManagerImpl::ProcessEvent, this, std::placeholders::_1));
    engine::application->input_manager()->RegisterHandler(
        engine::InputManager::Priority::UI, engine::EventType::SCROLL,
        std::bind(&UIManagerImpl::ProcessEvent, this, std::placeholders::_1));
    current_desktop_ = "_uname_blank_internal_desktop";
    desktops_[current_desktop_] = std::make_unique<Desktop>();
    return true;
}

bool UIManagerImpl::UnInitialize() {
    engine::application->input_manager()->UnRegisterHandler(
        engine::InputManager::Priority::UI, engine::EventType::KEYBOARD);
    engine::application->input_manager()->UnRegisterHandler(
        engine::InputManager::Priority::UI, engine::EventType::POSITION);
    engine::application->input_manager()->UnRegisterHandler(
        engine::InputManager::Priority::UI, engine::EventType::SCROLL);
    return true;
}

void UIManagerImpl::Upate() {
    if (desktops_[current_desktop_]) {
        desktops_[current_desktop_]->Update();
    }
}

void UIManagerImpl::Render() {
    if (desktops_[current_desktop_]) {
        desktops_[current_desktop_]->Render();
    }
}

bool UIManagerImpl::ProcessEvent(const engine::Event& event) {
    if (desktops_[current_desktop_]) {
        return desktops_[current_desktop_]->ProcessEvent(event);
    }
    return false;
}

bool UIManagerImpl::LoadUI(const std::string& path) {
    std::unique_ptr<Desktop> desktop = std::make_unique<Desktop>();
    if (desktop->Load(path)) {
        desktops_[path] = std::move(desktop);
        return true;
    }
    return false;
}

bool UIManagerImpl::UnLoadUI(const std::string& path) {
    desktops_.erase(path);
    return true;
}

bool UIManagerImpl::SetCurrentUI(const std::string& path) {
    if (desktops_.find(path) == desktops_.end())
        return false;
    current_desktop_ = path;
    return true;
}

}  // namespace engine