#ifndef COMMON_WINDOW_H_
#define COMMON_WINDOW_H_

#include <functional>
#include <string>
#include "event.h"
#include "micros.h"

struct GLFWwindow;
namespace engine {
class Window {
public:
    using EventHandler = std::function<void(const Event&)>;
    using SizeHandler = std::function<void(int, int)>;

public:
    Window();

    ~Window();

    void MakeContextCurrent();

    bool ShouldClose();

    void SwapBuffers();

    bool Create(int width, int height, const std::string& title);

    void Destroy();

    GLFWwindow* GetWindow();

    void OnEvent(const Event& event);

    // callback
    void OnSize(int width, int height);

    int GetHeight() { return height_; };

    int GetWidth() { return width_; };

    void SetEventHandler(const EventHandler& handler);

    void SetSizeHandler(const SizeHandler& handler);

private:
    int width_;
    int height_;
    GLFWwindow* window_;
    EventHandler event_handler_;
    SizeHandler size_handler_;

private:
    DISALLOW_COPY_AND_ASSIGN(Window)
};

}  // namespace engine

#endif  // COMMON_WINDOW_H_
