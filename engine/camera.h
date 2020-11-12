#ifndef CAMERA_H_
#define CAMERA_H_

#include "base/event.h"
#include "glm/glm.hpp"

class Camera {
public:
    enum Direction { FORWARD, BACKWARD, LEFT, RIGHT };

public:
    Camera();
    virtual ~Camera(){};
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectMatrix();
    glm::mat4 GetMatrix();
    //
    glm::vec3 GetPosition() const;
    // process input
    void ProcessKeyboard(Direction direction, float delta_time);
    void ProcessMouseMovement(float xoffset, float yoffset);
    void ProcessMouseScroll(float yoffset);
    //
    bool ProcessEvent(const engine::Event& event);

private:
    void UpdateCameraVectors();
    void Reset();

private:
    // camera attributes
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 world_up_;
    // eular angles
    float yaw_;
    float pitch_;
    // options
    float factor_movement_speed_;
    float factor_mouse_sensitivity_;
    float aspect_;
    //
    bool first_mouse_;
    double last_x;
    double last_y;
};

#endif  // CAMERA_H_
