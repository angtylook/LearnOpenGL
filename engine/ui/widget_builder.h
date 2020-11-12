#ifndef UI_WIDGET_BUILDER_H_
#define UI_WIDGET_BUILDER_H_

#include "rapidxml/rapidxml.hpp"

class Widget;
class WidgetBuilder {
public:
    static Widget* Build(Widget* parent, rapidxml::xml_node<char>* node);
};

#endif  // UI_WIDGET_BUILDER_H_
