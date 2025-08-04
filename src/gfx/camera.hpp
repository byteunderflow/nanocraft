#pragma once

#include "window.hpp"

class Window;

class Camera
{
public:
    float speed;
    float yaw;
    float pitch;
    glm::mat4 view;

    Camera(const Window *window) : window(window) {};
    ~Camera() = default;
    void init();
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void moveUpward();
    void moveDownward();
    void move();
    void update();

private:
    const Window *window;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
};