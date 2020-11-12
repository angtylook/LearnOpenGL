#ifndef INPUT_MANAGER_IMPL_H_
#define INPUT_MANAGER_IMPL_H_

#include <deque>
#include <functional>
#include <map>
#include <vector>

#include "base/event.h"
#include "base/micros.h"
#include "input_manager.h"

namespace engine {

class EventContainer {
public:
    EventContainer();

    ~EventContainer();

    void PushEvent(const Event& event);

    Event PopEvent();

    bool IsEmpty() const;

private:
    std::deque<Event> events_;
};

class InputManagerImpl : public InputManager {
public:
    InputManagerImpl() = default;
    virtual ~InputManagerImpl() = default;

public:
    // interface
    bool Initialize();

    void UnInitialize();

    void PollEvents();

    void PushEvent(const Event& event);

    void ProcessInput();

    void RegisterHandler(Priority priority,
                         EventType type,
                         const EventHandler& handler) override;

    virtual void UnRegisterHandler(Priority priority, EventType type) override;

private:
    void ProcessKeyboardEvent(const Event& event);

    void ProcessPositionEvent(const Event& event);

    void ProcessScrollEvent(const Event& event);

    void ProcessButtonEvent(const Event& event);

private:
    typedef std::vector<EventHandler> HandlerVector;
    typedef std::map<EventType, HandlerVector> HandlerMap;

    HandlerMap ui_handlers_;
    HandlerMap other_handlers_;
    EventContainer events_;

private:
    DISALLOW_COPY_AND_ASSIGN(InputManagerImpl)
};

}  // namespace engine

#endif  // INPUT_MANAGER_H_
