#include "button.h"

#include "GLFW/glfw3.h"
#include "base/filesystem.h"
#include "graphics.h"

const int kButtonNormal = 0;
const int kButtonOver = 1;
const int kButtonDown = 2;
const int kButtonDisable = 3;

using namespace engine;
Button::Button(Desktop* desktop, Widget* parent) : Widget(desktop, parent) {}

Button::~Button() {}

void Button::OnDraw(Rect render_area) {
    if (textures_.empty()) {
        // fprintf(stderr, "button has no textures!");
        return;
    }

    if (engine::fs::Exist(textures_[current_texture_index_])) {
        Graphics::DrawImage(textures_[current_texture_index_], render_area);
    }
}

void Button::SetTexture(const std::string& texture) {
    if (texture.empty())
        return;
    if (!textures_.empty() && textures_[0] == texture)
        return;
    engine::fs::Path path(texture);
    textures_.clear();
    textures_.push_back(path.data());
    textures_.push_back(path.AddSubfix("over").data());
    textures_.push_back(path.AddSubfix("down").data());
    textures_.push_back(path.AddSubfix("disable").data());
}

bool Button::OnMouseButton(const engine::Event& event) {
    if (event.action == GLFW_PRESS)
        current_texture_index_ = kButtonDown;
    if (event.action == GLFW_RELEASE)
        current_texture_index_ = kButtonNormal;
    return true;
}

void Button::OnMouseEnter() {
    if (current_texture_index_ != kButtonDisable)
        current_texture_index_ = kButtonOver;
}

void Button::OnMouseExit() {
    if (current_texture_index_ != kButtonDisable)
        current_texture_index_ = kButtonNormal;
}

void Button::SetAttribute(const std::string& name, const std::string& value) {
    if (name.empty() || value.empty()) {
        fprintf(stderr, "name or value is empty\n");
        return;
    }
    Widget::SetAttribute(name, value);
}
