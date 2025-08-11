#pragma once

#include <glm/glm.hpp>

#include "world.hpp"

struct World;
struct Chunk;

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
    World *world;
    Chunks::Chunk *chunk;

    struct Settings
    {
        float32 height = 2.0f;
        float32 speed = 20.0f;
        uint32 renderDistance = 8;
    };
    Settings settings;

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    float32 yaw = -90.0f;
    float32 pitch = 0.0f;

    void init(World *world);
    void update();
    void move(Movement movement, float32 delta);
    void look(float32 xOffset, float32 yOffset);
};