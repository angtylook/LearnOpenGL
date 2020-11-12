#ifndef UI_MANAGER_IMPL_H_
#define UI_MANAGER_IMPL_H_

#include <map>
#include <string>
#include "base/micros.h"
#include "ui/desktop.h"
#include "ui_manager.h"

namespace engine {

class UIManagerImpl : public UIManager {
public:
    using DesktopPtr = std::unique_ptr<Desktop>;
    using DesktopContainer = std::map<std::string, DesktopPtr>;

public:
    bool Initialize();

    bool UnInitialize();

    void Upate();

    void Render();

public:
    virtual bool LoadUI(const std::string& path) override;

    virtual bool UnLoadUI(const std::string& path) override;

    virtual bool SetCurrentUI(const std::string& path) override;

public:
    bool ProcessEvent(const engine::Event& event);

private:
    std::string current_desktop_;
    DesktopContainer desktops_;
};

}  // namespace engine

#endif  // UI_MANAGER_H_
