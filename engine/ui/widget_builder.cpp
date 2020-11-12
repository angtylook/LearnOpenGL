#include "widget_builder.h"

#include "base/xml.h"
#include "button.h"
#include "label.h"
#include "widget.h"

Widget* WidgetBuilder::Build(Widget* parent, engine::xml::Node* node) {
    if (!node || !parent)
        return nullptr;
    std::string name(node->name());
    Widget* widget = nullptr;
    if (name == "widget") {
        widget = new Widget(parent->GetDesktop(), parent);
    } else if (name == "label") {
        widget = new Label(parent->GetDesktop(), parent);
    } else if (name == "button") {
        widget = new Button(parent->GetDesktop(), parent);
    } else {
        fprintf(stderr, "unknown widget type: %s\n", name.c_str());
    }

    if (!widget)
        return widget;

    auto* attribute = node->first_attribute();
    while (attribute) {
        widget->SetAttribute(attribute->name(), attribute->value());
        attribute = attribute->next_attribute();
    }

    return widget;
}
