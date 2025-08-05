#include "camera.hpp"

void Camera::init()
{
    settings.speed = 5.0f;
    yaw = -90.0f;
    pitch = 0.0f;
    position = glm::vec3(0.0f, 0.0f, 3.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::moveForward(float delta)
{
    position += delta * settings.speed * front;
}

void Camera::moveBackward(float delta)
{
    position -= delta * settings.speed * front;
}

void Camera::moveLeft(float delta)
{
    position -= glm::normalize(glm::cross(front, up)) * delta * settings.speed;
}

void Camera::moveRight(float delta)
{
    position += glm::normalize(glm::cross(front, up)) * delta * settings.speed;
}

void Camera::moveUpward(float delta)
{
    position += up * delta * settings.speed;
}

void Camera::moveDownward(float delta)
{
    position -= up * delta * settings.speed;
}

void Camera::move(float xoffset, float yoffset)
{
    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
}

void Camera::update(glm::mat4 &view)
{
    view = glm::lookAt(position, position + front, up);
}