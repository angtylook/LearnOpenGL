#include "label.h"
#include "graphics.h"

using namespace engine;

Label::Label(Desktop* desktop, Widget* parent) : Widget(desktop, parent) {}

Label::~Label() {}

void Label::SetText(const std::string& text) {
    text_ = text;
}

const std::string& Label::GetText() const {
    return text_;
}

void Label::OnDraw(Rect render_area) {
    Graphics::DrawText(text_, render_area.Left(), render_area.Top(),
                       GetColor());
}

void Label::SetAttribute(const std::string& name, const std::string& value) {
    if (name.empty() || value.empty()) {
        fprintf(stderr, "name or value is empty\n");
        return;
    }

    if (name == "text") {
        text_ = value;
    } else {
        Widget::SetAttribute(name, value);
    }
}
