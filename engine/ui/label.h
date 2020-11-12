#ifndef UI_LABEL_H_
#define UI_LABEL_H_

#include "widget.h"

class Label : public Widget {
public:
    Label(Desktop* desktop, Widget* parent);
    virtual ~Label();

public:
    void SetText(const std::string& text);
    const std::string& GetText() const;
    virtual void OnDraw(Rect render_area) override;
    virtual void SetAttribute(const std::string& name,
                              const std::string& value) override;

private:
    std::string text_;
};

#endif  // UI_LABEL_H_
