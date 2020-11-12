#include "camera.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "application.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace engine;

namespace {
// A yaw is rotation about the y-axis.
// counterclockwise begin from x-y plane.
const float kYaw = -90.0f;
// A pitch is rotation about the x-axis.
// counterclockwise begin from x-z plane.
const float kPitch = -30.0f;
// A pitch is a counterclockwise rotation about the y-axis.
// const float kRoll = 0.0f;
const float kSpeed = 0.5f;
const float kSensitivty = 0.05f;
const float kAspect = 45.0f;
const float kAspectFactor = 0.5f;
}  // namespace

Camera::Camera()
    : position_(0.0f, 100.0f, 300.0f)  // origin
    , front_(0.0f, -1.0f, -3.0f)       //-z
    , up_(0.0f, 1.0f, 0.0f)            // y
    , right_(0.0f, 0.0f, 0.0f)
    , world_up_(up_)
    , yaw_(kYaw)
    , pitch_(kPitch)
    , factor_movement_speed_(kSpeed)
    , factor_mouse_sensitivity_(kSensitivty)
    , aspect_(kAspect)
    , first_mouse_(true)
    , last_x(0.0)
    , last_y(0.0) {
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position_, position_ + front_, up_);
}

glm::mat4 Camera::GetProjectMatrix() {
    return glm::perspective(aspect_, 1024.0f / 768.0f, 0.1f, 1000.0f);
}

glm::mat4 Camera::GetMatrix() {
    return glm::mat4(glm::normalize(glm::vec4(right_, 0)),
                     glm::normalize(glm::vec4(up_, 0)),
                     glm::normalize(glm::vec4(front_, 0)),
                     glm::vec4(position_ + 10.0f * front_, 1.0f));
}

glm::vec3 Camera::GetPosition() const {
    return position_;
}

void Camera::Reset() {
    /*
    front_ = glm::normalize(glm::vec3(0, -1, -3));
    right_ = glm::normalize(glm::vec3(1, 0, 0));
    up_ = glm::normalize(world_up_);
    */
    yaw_ = -90;
    pitch_ = -30;
    position_ = glm::vec3(0, 100, 300);
    UpdateCameraVectors();
}

void Camera::ProcessKeyboard(Direction direction, float delta_time) {
    float velocity = factor_movement_speed_ * delta_time;
    if (direction == FORWARD) {
        position_ += front_ * velocity;
    }
    if (direction == BACKWARD) {
        position_ -= front_ * velocity;
    }
    if (direction == LEFT) {
        position_ -= right_ * velocity;
    }
    if (direction == RIGHT) {
        position_ += right_ * velocity;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset) {
    xoffset *= factor_mouse_sensitivity_;
    yoffset *= factor_mouse_sensitivity_;

    yaw_ += xoffset;
    pitch_ += yoffset;

    if (pitch_ > 89.0f) {
        pitch_ = 89.0f;
    }
    if (pitch_ < -89.0f) {
        pitch_ = -89.0f;
    }

    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    if (aspect_ >= 1.0f && aspect_ <= 45.0f) {
        aspect_ -= yoffset * kAspectFactor;
    }
    if (aspect_ <= 1.0f) {
        aspect_ = 1.0f;
    }
    if (aspect_ >= 45.0f) {
        aspect_ = 45.0f;
    }
}

void Camera::UpdateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);

    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));
}

bool Camera::ProcessEvent(const engine::Event& event) {
    if (event.type == engine::EventType::KEYBOARD &&
        event.action == GLFW_PRESS) {
        if (event.key == GLFW_KEY_W)
            ProcessKeyboard(Camera::FORWARD, application->GetDeltaTime());
        if (event.key == GLFW_KEY_S)
            ProcessKeyboard(Camera::BACKWARD, application->GetDeltaTime());
        if (event.key == GLFW_KEY_A)
            ProcessKeyboard(Camera::LEFT, application->GetDeltaTime());
        if (event.key == GLFW_KEY_D)
            ProcessKeyboard(Camera::RIGHT, application->GetDeltaTime());
        if (event.key == GLFW_KEY_SPACE)
            Reset();
        return true;
    }

    if (event.type == engine::EventType::POSITION) {
        if (first_mouse_) {
            last_x = event.xpos;
            last_y = event.ypos;
            first_mouse_ = false;
        }

        double xoffset = event.xpos - last_x;
        double yoffset = event.ypos - last_y;

        last_x = event.xpos;
        last_y = event.ypos;
        ProcessMouseMovement(static_cast<float>(xoffset),
                             static_cast<float>(yoffset));
        return true;
    }

    if (event.type == engine::EventType::SCROLL) {
        ProcessMouseScroll(static_cast<float>(event.yoffset));
        return true;
    }
    return false;
}
