#include "input_manager_impl.h"

#include <GLFW/glfw3.h>

namespace engine {
bool InputManagerImpl::Initialize() {
    return true;
}

void InputManagerImpl::UnInitialize() {
    ui_handlers_.clear();
    other_handlers_.clear();
}

void InputManagerImpl::PollEvents() {
    glfwPollEvents();
}

void InputManagerImpl::PushEvent(const Event& event) {
    events_.PushEvent(event);
}

void InputManagerImpl::ProcessInput() {
    while (!events_.IsEmpty()) {
        Event event = events_.PopEvent();
        switch (event.type) {
            case EventType::KEYBOARD:
                ProcessKeyboardEvent(event);
                break;
            case EventType::POSITION:
                ProcessPositionEvent(event);
                break;
            case EventType::SCROLL:
                ProcessScrollEvent(event);
                break;
            case EventType::BUTTON:
                ProcessButtonEvent(event);
                break;
            default:
                printf("event type unexpect!\n");
                break;
        }
    }
}

void InputManagerImpl::ProcessKeyboardEvent(const Event& event) {
    auto& ui_handlers = ui_handlers_[event.type];
    for (auto& handler : ui_handlers) {
        if (handler(event)) {
            return;
        }
    }

    auto& other_handlers = other_handlers_[event.type];
    for (auto& handler : other_handlers) {
        if (handler(event)) {
            return;
        }
    }
}

void InputManagerImpl::ProcessPositionEvent(const Event& event) {
    auto& ui_handlers = ui_handlers_[event.type];
    for (auto& handler : ui_handlers) {
        if (handler(event)) {
            return;
        }
    }

    auto& other_handlers = other_handlers_[event.type];
    for (auto& handler : other_handlers) {
        if (handler(event)) {
            return;
        }
    }
}

void InputManagerImpl::ProcessScrollEvent(const Event& event) {
    auto& ui_handlers = ui_handlers_[event.type];
    for (auto& handler : ui_handlers) {
        if (handler(event)) {
            return;
        }
    }

    auto& other_handlers = other_handlers_[event.type];
    for (auto& handler : other_handlers) {
        if (handler(event)) {
            return;
        }
    }
}

void InputManagerImpl::ProcessButtonEvent(const Event& event) {
    auto& ui_handlers = ui_handlers_[event.type];
    for (auto& handler : ui_handlers) {
        if (handler(event)) {
            return;
        }
    }

    auto& other_handlers = other_handlers_[event.type];
    for (auto& handler : other_handlers) {
        if (handler(event)) {
            return;
        }
    }
}

void InputManagerImpl::RegisterHandler(Priority priority,
                                       EventType type,
                                       const EventHandler& handler) {
    if (priority == Priority::UI) {
        ui_handlers_[type].push_back(handler);
    } else {
        other_handlers_[type].push_back(handler);
    }
}

void InputManagerImpl::UnRegisterHandler(Priority priority, EventType type) {
    if (priority == Priority::UI) {
        ui_handlers_[type].clear();
    } else {
        other_handlers_[type].clear();
    }
}

EventContainer::EventContainer() {}

EventContainer::~EventContainer() {}

void EventContainer::PushEvent(const Event& event) {
    events_.push_back(event);
}

Event EventContainer::PopEvent() {
    Event e = events_.front();
    events_.pop_front();
    return e;
}

bool EventContainer::IsEmpty() const {
    return events_.empty();
}

}  // namespace engine
