#pragma once

#include "../types.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UPWARD,
    DOWNWARD
};

struct Player
{
    uint32 chunkX = 0;
    uint32 chunkY = 0;
    glm::vec3 position = glm::vec3(0.0f, 32.0f, 0.0f);
    glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    float32 yaw = -90.0f;
    float32 pitch = 0.0f;
    
    float32 speed = 10.0f;

    void update();
    void move(Movement movement, float32 delta);
    void look(float32 xoffset, float32 yoffset);
};