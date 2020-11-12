#ifndef UI_MANAGER_H_
#define UI_MANAGER_H_

#include <string>
#include "base/micros.h"

namespace engine {
/**
 * manager desktop the top widget and dispatch
 * event to current active widget
 * load and save ui
 */
class UIManager {
public:
    virtual bool LoadUI(const std::string& path) = 0;

    virtual bool UnLoadUI(const std::string& path) = 0;

    virtual bool SetCurrentUI(const std::string& path) = 0;

protected:
    UIManager() = default;
    virtual ~UIManager()= default;

private:
    DISALLOW_COPY_AND_ASSIGN(UIManager)
};

}  // namespace engine

#endif  // UI_MANAGER_H_
