#ifndef UI_BUTTON_H_
#define UI_BUTTON_H_

#include "widget.h"

class Button : public Widget {
public:
    Button(Desktop* desktop, Widget* parent);
    virtual ~Button();

public:
    virtual void OnDraw(Rect render_area) override;
    virtual void SetTexture(const std::string& texture) override;
    // event
    virtual bool OnMouseButton(const engine::Event& event) override;
    virtual void OnMouseEnter() override;
    virtual void OnMouseExit() override;
    virtual void SetAttribute(const std::string& name,
                              const std::string& value) override;

private:
    int current_texture_index_;
    std::vector<std::string> textures_;
};

#endif  // UI_BUTTON_H_
