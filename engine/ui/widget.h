#ifndef UI_WIDGET_H_
#define UI_WIDGET_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "base/event.h"
#include "glm/glm.hpp"
#include "math/rect.h"

// base ui class
class Desktop;
class Widget {
public:
    //
    Widget(Desktop* desktop, Widget* parent = nullptr);
    virtual ~Widget();
    // script
    // not implements

    // flags
    void SetFlags(uint32_t flag);
    void ClearFlags(uint32_t flag);
    bool HasFlags(uint32_t flag) const;

    // size, position and layout
    void SetSize(float x, float y, float w, float h);
    Point GetOrigin() const;
    Size GetSize() const;
    Rect GetRenderRect() const;
    void Move(float dx, float dy);
    void MoveTo(float x, float y);
    bool Contains(float x, float y) const;
    // if widget contains point(x,y) return this
    // widget or one of it's child else return nullptr
    Widget* GetWidgetByPoint(float x, float y);

    // parent and children
    Desktop* GetDesktop() const;
    Widget* GetParent() const;
    // remove from old parent and set it parent
    void SetParent(Widget* parent);
    // add child at back will take ownership
    void AddChild(Widget* child);
    // remove child and delete it except del is false
    void RemoveChild(Widget* child, bool del = true);
    Widget* GetChild(const std::string& name) const;
    bool HasChild(const std::string& name) const;
    size_t GetChildCount() const;

    // callback
    // focus
    virtual void OnFocus();
    virtual void OnLoseFocus();
    // capture
    virtual void OnCapture();
    virtual void OnLoseCapture();
    // mouse
    virtual bool OnMouseButton(const engine::Event& event);
    virtual bool OnMouseMove(float x, float y);
    virtual bool OnMouseWheel();
    virtual void OnMouseEnter();
    virtual void OnMouseExit();
    // keyboard
    virtual bool OnKeyButton();
    virtual bool OnKeyChar();
    // others
    virtual void OnSize();
    virtual void OnDraw(Rect render_area);

    // event
    virtual bool ProcessMouseButton(const engine::Event& event);
    virtual bool ProcessMouseMove(const engine::Event& event);
    virtual bool ProcessMouseScroll(const engine::Event& event);
    virtual bool ProcessKeyButton(const engine::Event& event);
    virtual bool ProcessKeyChar(const engine::Event& event);

    // update and render
    virtual void Update();
    virtual void Render();

    // rendering
    std::string GetTexture() const;
    virtual void SetTexture(const std::string& texture);
    void SetColor(const glm::vec4& color);
    const glm::vec4& GetColor() const;

    // serial
    // not implements

    // build
    virtual void SetAttribute(const std::string& name,
                              const std::string& value);

    // others
    std::string GetName() const;
    // the name should be unique
    void SetName(const std::string& name);
    std::string GetComment() const;
    void SetComment(const std::string& comment);

private:
    bool SkipAllEvent();
    bool SkipSelfEvent();

private:
    // Desktop
    Desktop* desktop_;
    // widget's parent pointer, weak.
    Widget* parent_;
    // store child widget pointer, own it.
    std::vector<std::unique_ptr<Widget>> children_;
    // map widget's name to pointer for fast search
    std::map<std::string, Widget*> children_names_;
    // flags for widget's state or other property
    // can be represent as true or false
    uint32_t flags_;
    // widget's name
    std::string name_;
    // render texture
    std::string texture_;
    // render color
    glm::vec4 color_;
    // render rect
    Rect render_area_;
    // debug purpose
    std::string comment_;
};

#endif  // UI_WIDGET_H_
