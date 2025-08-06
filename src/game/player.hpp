#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Player
{
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    float yaw = -90.0f;
    float pitch = 0.0f;
    float speed = 10.0f;

    void forward(float delta);
    void backward(float delta);
    void left(float delta);
    void right(float delta);
    void upward(float delta);
    void downward(float delta);
    void look(float xoffset, float yoffset);
    glm::mat4 view();
};