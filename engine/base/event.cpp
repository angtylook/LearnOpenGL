#include "event.h"

#include <GLFW/glfw3.h>

namespace engine {
Event::Event()
    : type(EventType::UNKNOWN)
    , key(GLFW_KEY_UNKNOWN)
    , action(-1)
    , mods(0)
    , button(-1)
    , xpos(0.0)
    , ypos(0.0)
    , xoffset(0.0)
    , yoffset(0.0) {}

}  // namespace engine