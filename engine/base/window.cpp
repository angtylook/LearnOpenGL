#include "window.h"

#include <cassert>
#include <map>
#include "GLFW/glfw3.h"

//----------Input Event Handler-------------//
namespace {
// translate system event to engine event
class WindowEventHandler {
public:
    // xpos the new x-coordinate, in screen coordinates
    // ypos the new y-coordinate, in screen coordinates
    // upper-left corner of the content area of the window
    static void OnPosition(GLFWwindow* window, int xpos, int ypos) {
        
    }

    // width the new width in screen coordinates of the window
    // height the new height in screen coordinates of the window
    static void OnSize(GLFWwindow* window, int width, int height) {
        if (windows_.find(window) != windows_.end()) {
            windows_[window]->OnSize(width, height);
        }
    }
    
    // window the window that the user attempted to close
    static void OnClose(GLFWwindow* window) {

    }
    
    // window the window whose content needs to be refreshed
    static void OnRefresh(GLFWwindow* window) {

    }

    // callback declare
    static void OnKey(GLFWwindow* window,
                      int key,
                      int scancode,
                      int action,
                      int mods) {
        if (windows_.find(window) != windows_.end()) {
            engine::Event event;
            event.type = engine::EventType::KEYBOARD;
            event.key = key;
            event.action = action;
            event.mods = mods;
            //
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GL_TRUE);
                return;
            }

            windows_[window]->OnEvent(event);
        }
    }

    static void OnCursorPosition(GLFWwindow* window, double xpos, double ypos) {
        if (windows_.find(window) != windows_.end()) {
            engine::Event event;
            event.type = engine::EventType::POSITION;
            event.xpos = xpos;
            event.ypos = ypos;
            //
            windows_[window]->OnEvent(event);
        }
    }

    static void OnCursorEnter(GLFWwindow* window, int entered) {
        if (windows_.find(window) != windows_.end()) {
            // windows_[window];
        }
    }

    static void OnMouseScroll(GLFWwindow* window,
                              double xoffset,
                              double yoffset) {
        if (windows_.find(window) != windows_.end()) {
            engine::Event event;
            event.type = engine::EventType::SCROLL;
            event.xoffset = xoffset;
            event.yoffset = yoffset;
            //
            windows_[window]->OnEvent(event);
        }
    }

    static void OnMouseButton(GLFWwindow* window,
                              int button,
                              int action,
                              int mods) {
        if (windows_.find(window) != windows_.end()) {
            engine::Event event;
            event.type = engine::EventType::BUTTON;
            event.button = button;
            event.action = action;
            event.mods = mods;
            glfwGetCursorPos(window, &event.xpos, &event.ypos);
            //
            windows_[window]->OnEvent(event);
        }
    }

    static void Register(GLFWwindow* glfw_window, engine::Window* window) {
        windows_[glfw_window] = window;
    }

    static void UnRegister(GLFWwindow* glfw_window) {
        windows_.erase(glfw_window);
    }

private:
    static std::map<GLFWwindow*, engine::Window*> windows_;
};

std::map<GLFWwindow*, engine::Window*> WindowEventHandler::windows_;
}  // namespace

namespace engine {
//---------Window----------------------//
Window::Window() : window_(nullptr) {}

Window::~Window() {}

bool Window::Create(int width, int height, const std::string& title) {
    assert(!window_);
    //
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
    // Open a window and create its OpenGL context
    window_ = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window_) {
        return false;
    }
    // set window
    // glfwSetWindowPosCallback(window_, &Wind)
    glfwSetWindowSizeCallback(window_, &WindowEventHandler::OnSize);
    glfwSetCursorPosCallback(window_, &WindowEventHandler::OnCursorPosition);
    glfwSetScrollCallback(window_, &WindowEventHandler::OnMouseScroll);
    glfwSetCursorEnterCallback(window_, &WindowEventHandler::OnCursorEnter);
    glfwSetMouseButtonCallback(window_, &WindowEventHandler::OnMouseButton);
    glfwSetKeyCallback(window_, &WindowEventHandler::OnKey);

    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    width_ = width;
    height_ = height;
    WindowEventHandler::Register(window_, this);
    return true;
}

void Window::Destroy() {
    if (window_) {
        WindowEventHandler::UnRegister(window_);
        window_ = nullptr;
    }
}

void Window::MakeContextCurrent() {
    assert(window_);
    glfwMakeContextCurrent(window_);
}

bool Window::ShouldClose() {
    assert(window_);
    return !!glfwWindowShouldClose(window_);
}

void Window::SwapBuffers() {
    assert(window_);
    glfwSwapBuffers(window_);
}

GLFWwindow* Window::GetWindow() {
    return window_;
}

void Window::OnEvent(const Event& event) {
    if (event_handler_)
        event_handler_(event);
}

void Window::OnSize(int width, int height) {
    width_ = width;
    height_ = height;
    if (size_handler_)
        size_handler_(width_, height_);
}

void Window::SetEventHandler(const Window::EventHandler& handler) {
    event_handler_ = handler;
}

void Window::SetSizeHandler(const Window::SizeHandler& handler) {
    size_handler_ = handler;
}

}  // namespace engine