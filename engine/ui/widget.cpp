#include "widget.h"

#include <algorithm>
#include "base/string_util.h"
#include "desktop.h"
#include "flags.h"
#include "graphics.h"

using namespace engine;

// member function
Widget::Widget(Desktop* desktop, Widget* parent)
    : desktop_(desktop), parent_(parent), flags_(0) {
    name_.append("un_name_");
    name_.append(std::to_string(desktop_->GetWidgetCount()));
    SetFlags(kVisible);
    SetFlags(kEnable);
}

Widget::~Widget() {}

// event
bool Widget::SkipAllEvent() {
    return !HasFlags(kVisible);
}

bool Widget::SkipSelfEvent() {
    return !HasFlags(kEnable);
}

// left, right mouse button
bool Widget::ProcessMouseButton(const engine::Event& event) {
    if (SkipAllEvent()) {
        return false;
    }
    // forward to children
    for (auto iterator = children_.begin(); iterator != children_.end();
         ++iterator) {
        Widget* child = iterator->get();
        if (child &&
            child->Contains(static_cast<float>(event.xpos),
                            static_cast<float>(event.ypos)) &&
            child->OnMouseButton(event)) {
            return true;
        }
    }

    if (SkipSelfEvent()) {
        return false;
    }

    return OnMouseButton(event);
}
// mouse pointer position
bool Widget::ProcessMouseMove(const engine::Event& event) {
    if (SkipAllEvent()) {
        return false;
    }
    Widget* last_hover_widget = desktop_->GetHoverWidget();
    Widget* widget = GetWidgetByPoint(static_cast<float>(event.xpos),
                                      static_cast<float>(event.ypos));
    // process last hover widget
    if (last_hover_widget && widget != last_hover_widget) {
        last_hover_widget->OnMouseExit();
    }
    if (!widget || widget->SkipAllEvent()) {
        return false;
    }

    desktop_->SetHoverWidget(widget);
    // process mouse enter
    widget->OnMouseEnter();
    if (widget->SkipSelfEvent()) {
        return false;
    } else {
        return widget->OnMouseMove(static_cast<float>(event.xpos),
                                   static_cast<float>(event.ypos));
    }

    return false;
}
// mouse scroll wheel
bool Widget::ProcessMouseScroll(const engine::Event& event) {
    if (SkipAllEvent()) {
        return false;
    }
    // forward to children
    for (auto iterator = children_.begin(); iterator != children_.end();
         ++iterator) {
        Widget* child = iterator->get();
        if (child &&
            child->Contains(static_cast<float>(event.xpos),
                            static_cast<float>(event.ypos)) &&
            child->OnMouseWheel()) {
            return true;
        }
    }

    if (SkipSelfEvent()) {
        return false;
    }

    return OnMouseWheel();
}
// RETURN, CTRL, ALT, ESC etc
bool Widget::ProcessKeyButton(const engine::Event& event) {
    if (SkipAllEvent()) {
        return false;
    }
    // forward to children
    for (auto iterator = children_.begin(); iterator != children_.end();
         ++iterator) {
        Widget* child = iterator->get();
        if (child &&
            child->Contains(static_cast<float>(event.xpos),
                            static_cast<float>(event.ypos)) &&
            child->OnKeyButton()) {
            return true;
        }
    }

    if (SkipSelfEvent()) {
        return false;
    }

    return OnKeyButton();
}
// char as space, alphabet, etc
bool Widget::ProcessKeyChar(const engine::Event& event) {
    if (SkipAllEvent()) {
        return false;
    }
    // todo: forward to children
    for (auto iterator = children_.begin(); iterator != children_.end();
         ++iterator) {
        Widget* child = iterator->get();
        if (child &&
            child->Contains(static_cast<float>(event.xpos),
                            static_cast<float>(event.ypos)) &&
            child->OnKeyChar()) {
            return true;
        }
    }

    if (SkipSelfEvent()) {
        return false;
    }

    return OnKeyChar();
}

// getter and setter
void Widget::SetFlags(uint32_t flag) {
    flags_ = flags_ | flag;
}

void Widget::ClearFlags(uint32_t flag) {
    flags_ &= ~flag;
}

bool Widget::HasFlags(uint32_t flag) const {
    return (flags_ & flag) != 0x0;
}

// size, position and layout
void Widget::SetSize(float x, float y, float w, float h) {
    render_area_ = Rect(x, y, w, h);
}

Point Widget::GetOrigin() const {
    return render_area_.Origin();
}

Size Widget::GetSize() const {
    return render_area_.Size();
}

Rect Widget::GetRenderRect() const {
    return render_area_;
}

void Widget::Move(float dx, float dy) {
    render_area_.Move(dx, dy);
}

void Widget::MoveTo(float x, float y) {
    render_area_.MoveTo(x, y);
}

bool Widget::Contains(float x, float y) const {
    return render_area_.Contains(Point(x, y));
}

Widget* Widget::GetWidgetByPoint(float x, float y) {
    if (!Contains(x, y))
        return nullptr;

    for (auto iterator = children_.begin(); iterator != children_.end();
         ++iterator) {
        Widget* child = iterator->get();
        if (child && child->Contains(x, y)) {
            return child->GetWidgetByPoint(x, y);
        }
    }

    return this;
}

Desktop* Widget::GetDesktop() const {
    return desktop_;
}

Widget* Widget::GetParent() const {
    return parent_;
}

void Widget::SetParent(Widget* parent) {
    if (parent_)
        parent_->RemoveChild(this);
    parent_ = parent;
}

void Widget::AddChild(Widget* child) {
    if (!child) {
        return;
    }
    child->SetParent(this);
    children_.emplace_back(child);
    children_names_[child->GetName()] = child;
}

void Widget::RemoveChild(Widget* child, bool del) {
    if (!child)
        return;

    auto iterator =
        std::find_if(children_.begin(), children_.end(),
                     [child](const std::unique_ptr<Widget>& w) -> bool {
                         return w.get() == child;
                     });
    if (iterator != children_.end()) {
        children_names_.erase(child->GetName());
        if (!del) {
            iterator->release();
        }
        children_.erase(iterator);
    }
}

Widget* Widget::GetChild(const std::string& name) const {
    auto iterator = children_names_.find(name);
    if (iterator != children_names_.end())
        return iterator->second;
    else
        return nullptr;
}

bool Widget::HasChild(const std::string& name) const {
    return children_names_.find(name) != children_names_.end();
}

size_t Widget::GetChildCount() const {
    return children_.size();
}

// update and render
void Widget::Update() {
    for (auto iterator = children_.begin(); iterator != children_.end();
         ++iterator) {
        Widget* child = iterator->get();
        if (child) {
            child->Update();
        }
    }
}

void Widget::Render() {
    if (HasFlags(kVisible)) {
        OnDraw(render_area_);
    }
    for (auto iterator = children_.begin(); iterator != children_.end();
         ++iterator) {
        Widget* child = iterator->get();
        if (child && child->HasFlags(kVisible)) {
            child->Render();
        }
    }
}

std::string Widget::GetTexture() const {
    return texture_;
}

void Widget::SetTexture(const std::string& texture) {
    texture_ = texture;
}

void Widget::SetAttribute(const std::string& name, const std::string& value) {
    if (name.empty() || value.empty()) {
        fprintf(stderr, "name or value is empty\n");
        return;
    }
    if (name == "name") {
        name_ = value;
    } else if (name == "texture") {
        texture_ = value;
    } else if (name == "x") {
        render_area_.SetLeft(std::stof(value));
    } else if (name == "y") {
        render_area_.SetTop(std::stof(value));
    } else if (name == "width") {
        render_area_.SetWidth(std::stof(value));
    } else if (name == "height") {
        render_area_.SetHeight(std::stof(value));
    } else if (name == "comment") {
        comment_ = value;
    } else if (name == "color") {
        color_ = ReadVec4(value);
    } else if (name == "visible") {
        if (value == "false")
            ClearFlags(kVisible);
        else
            SetFlags(kVisible);
    } else {
        fprintf(stderr, "unknown attribute: %s\n", name.c_str());
    }
}

std::string Widget::GetName() const {
    return name_;
}

void Widget::SetName(const std::string& name) {
    name_ = name;
}

std::string Widget::GetComment() const {
    return comment_;
}

void Widget::SetComment(const std::string& comment) {
    comment_ = comment;
}

// event callback default implements
// callback
// focus
void Widget::OnFocus() {}

void Widget::OnLoseFocus() {}

// capture
void Widget::OnCapture() {}

void Widget::OnLoseCapture() {}

// mouse
bool Widget::OnMouseButton(const engine::Event& event) {
    return false;
}

bool Widget::OnMouseMove(float x, float y) {
    return false;
}

bool Widget::OnMouseWheel() {
    return false;
}

void Widget::OnMouseEnter() {}

void Widget::OnMouseExit() {}

// keyboard
bool Widget::OnKeyButton() {
    return false;
}

bool Widget::OnKeyChar() {
    return false;
}

// others
void Widget::OnSize() {}

void Widget::OnDraw(Rect render_area) {
    if (texture_.empty()) {
        return;
    }
    Graphics::DrawImage(texture_, render_area);
}

void Widget::SetColor(const glm::vec4& color) {
    color_ = color;
}

const glm::vec4& Widget::GetColor() const {
    return color_;
}
