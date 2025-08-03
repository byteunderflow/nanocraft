#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    glm::vec3 position;

    Camera(glm::vec3 position) : position(position) {};
    ~Camera() = default;
};