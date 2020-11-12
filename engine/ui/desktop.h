#ifndef UI_DESKTOP_H_
#define UI_DESKTOP_H_

#include "base/event.h"
#include "widget.h"

// Desktop base ui class
// for create widget from xml
// for comunicate with other external system
// for focus hover and capture
class Desktop {
public:
    //
    Desktop();
    virtual ~Desktop();
    // load
    bool Load(const std::string& path);
    std::string GetName();

    // focus, hover and capture
    Widget* GetFocusWidget() const;
    void SetFocusWidget(Widget* widget);
    Widget* GetCaptureWidget() const;
    void SetCaptureWidget(Widget* widget);
    Widget* GetHoverWidget() const;
    void SetHoverWidget(Widget* widget);
    // event
    bool ProcessEvent(const engine::Event& event);

    // widget supoort
    void OnWidgetCreate(Widget* widget);
    void OnWidgetDelete(Widget* widget);
    uint64_t GetWidgetCount() const;
    Widget* GetWidget(const std::string& name) const;
    // update and render
    void Update();
    void Render();

private:
    std::string name_;
    // widget's count, mainly use for rename
    // a widget with an exist name or name
    // a widget with no name.
    uint64_t widget_count_;
    // current hover widget
    Widget* hover_widget_;
    // current focus widget
    Widget* focus_widget_;
    // current capture widget_;
    Widget* capture_widget_;
    // root widget contains all widgets
    std::unique_ptr<Widget> root_widget_;
    // map widget's name to widget pointer
    // for fast search and find
    std::map<std::string, Widget*> widgets_;
};

#endif  // UI_DESKTOP_H_
