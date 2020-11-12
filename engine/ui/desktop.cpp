#include "desktop.h"

#include "GLFW/glfw3.h"
#include "base/xml.h"
#include "widget_builder.h"

const std::string kXmlNodeDesktop = "desktop";

namespace {
void BuildWidget(Widget* parent, engine::xml::Node* xml_node) {
    if (!xml_node)
        return;

    Widget* widget = WidgetBuilder::Build(parent, xml_node);
    if (widget) {
        parent->AddChild(widget);
        xml_node = xml_node->first_node();
        while (xml_node) {
            BuildWidget(widget, xml_node);
            xml_node = xml_node->next_sibling();
        }
    }
}
}  // namespace

// member function
Desktop::Desktop()
    : widget_count_(0)
    , hover_widget_(nullptr)
    , focus_widget_(nullptr)
    , capture_widget_(nullptr) {
    root_widget_.reset(new Widget(this));
    root_widget_->SetName("__root_widget");
}

Desktop::~Desktop() {}

bool Desktop::Load(const std::string& path) {
    engine::xml::Document doc;
    auto* xml_node = engine::xml::LoadXML(doc, path, kXmlNodeDesktop);
    if (!xml_node) {
        return false;
    }

    auto* name_attribute = xml_node->first_attribute("name");
    if (name_attribute)
        name_ = name_attribute->value();
    else
        fprintf(stderr, "warning, desktop with no name!!!");

    xml_node = xml_node->first_node();
    while (xml_node) {
        BuildWidget(root_widget_.get(), xml_node);
        xml_node = xml_node->next_sibling();
    }

    return true;
}

std::string Desktop::GetName() {
    return name_;
}

bool Desktop::ProcessEvent(const engine::Event& event) {
    if (event.type == engine::EventType::KEYBOARD) {
        if (event.key >= GLFW_KEY_SPACE && event.key <= GLFW_KEY_WORLD_2) {
            return root_widget_->ProcessKeyChar(event);
        } else if (event.key >= GLFW_KEY_ESCAPE && event.key <= GLFW_KEY_LAST) {
            return root_widget_->ProcessKeyButton(event);
        } else {
            fprintf(stderr, "unknown keyboard key: %u!\n", event.key);
            return false;
        }
    } else if (event.type == engine::EventType::BUTTON) {
        if (event.key >= GLFW_MOUSE_BUTTON_1 &&
            event.key <= GLFW_MOUSE_BUTTON_LAST) {
            return root_widget_->ProcessMouseButton(event);
        } else {
            fprintf(stderr, "unknown mouse key: %u!\n", event.key);
            return false;
        }
    } else if (event.type == engine::EventType::SCROLL) {
        return root_widget_->ProcessMouseScroll(event);
    } else if (event.type == engine::EventType::POSITION) {
        return root_widget_->ProcessMouseMove(event);
    } else {
        printf("unknown event type: %d!\n", event.type);
        return false;
    }
    // not process
    return false;
}

Widget* Desktop::GetFocusWidget() const {
    return focus_widget_;
}

void Desktop::SetFocusWidget(Widget* widget) {
    focus_widget_ = widget;
}

Widget* Desktop::GetCaptureWidget() const {
    return capture_widget_;
}

void Desktop::SetCaptureWidget(Widget* widget) {
    capture_widget_ = widget;
}

Widget* Desktop::GetHoverWidget() const {
    return hover_widget_;
}

void Desktop::SetHoverWidget(Widget* widget) {
    hover_widget_ = widget;
}

void Desktop::OnWidgetCreate(Widget* widget) {
    widgets_[widget->GetName()] = widget;
}

void Desktop::OnWidgetDelete(Widget* widget) {
    widgets_.erase(widget->GetName());
}

uint64_t Desktop::GetWidgetCount() const {
    return widget_count_;
}

Widget* Desktop::GetWidget(const std::string& name) const {
    auto iterator = widgets_.find(name);
    if (iterator != widgets_.end())
        return iterator->second;
    else
        return nullptr;
}

void Desktop::Update() {
    if (root_widget_) {
        root_widget_->Update();
    }
}

void Desktop::Render() {
    if (root_widget_) {
        root_widget_->Render();
    }
}
