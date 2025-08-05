#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera
{
    float yaw;
    float pitch;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    struct Settings
    {
        float speed;
    };
    Settings settings;

    void init();
    void moveForward(float delta);
    void moveBackward(float delta);
    void moveLeft(float delta);
    void moveRight(float delta);
    void moveUpward(float delta);
    void moveDownward(float delta);
    void move(float xoffset, float yoffset);
    void update(glm::mat4 &view);
};