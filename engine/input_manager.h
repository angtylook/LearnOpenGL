#ifndef INPUT_MANAGER_H_
#define INPUT_MANAGER_H_

#include <functional>

#include "base/event.h"
#include "base/micros.h"

namespace engine {
class InputManager {
public:
    enum class Priority { UI = 0, SCENE = 1, PRIORITY_END };

    using EventHandler = std::function<bool(const engine::Event&)>;

public:
    virtual void RegisterHandler(Priority priority,
                                 EventType type,
                                 const EventHandler& handler) = 0;

    virtual void UnRegisterHandler(Priority priority, EventType type) = 0;

protected:
    InputManager() = default;
    virtual ~InputManager() = default;

private:
    DISALLOW_COPY_AND_ASSIGN(InputManager)
};

}  // namespace engine

#endif  // INPUT_MANAGER_H_
