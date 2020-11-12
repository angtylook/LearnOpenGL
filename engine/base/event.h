#ifndef COMMON_EVENT_CONTAINER_H_
#define COMMON_EVENT_CONTAINER_H_

#include <deque>

namespace engine {
enum class EventType {
    UNKNOWN = 0,
    KEYBOARD = 1,
    POSITION = 2,
    SCROLL = 3,
    BUTTON = 4,
    NUMBER_OF_EVENT_TYPE,
};

struct Event {
    EventType type;
    // Key
    int key;  // GLFW key
    int action;
    int mods;
    int button;
    // Position
    double xpos;
    double ypos;
    // scroll
    double xoffset;
    double yoffset;

    Event();
};

}  // namespace engine

#endif  // COMMON_EVENT_CONTAINER_H_
