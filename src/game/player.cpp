#include "player.hpp"

void Player::update()
{
    chunkX = static_cast<int32>(std::floor(position.x / 16.0f));
    chunkY = static_cast<int32>(std::floor(position.z / 16.0f));
}

void Player::move(Movement movement, float32 delta)
{
    switch (movement)
    {
    case Movement::FORWARD:
        position += delta * speed * glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));
        break;
    case Movement::BACKWARD:
        position -= delta * speed * glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));
        break;
    case Movement::LEFT:
        position -= glm::normalize(glm::cross(direction, up)) * delta * speed;
        break;
    case Movement::RIGHT:
        position += glm::normalize(glm::cross(direction, up)) * delta * speed;
        break;
    case Movement::UPWARD:
        position += up * delta * speed;
        break;
    case Movement::DOWNWARD:
        position -= up * delta * speed;
        break;
    }
}

void Player::look(float32 xoffset, float32 yoffset)
{
    yaw += xoffset;
    pitch += yoffset;

    if (yaw > 360.0f)
    {
        yaw -= 360.0f;
    }
    if (yaw < -360.0f)
    {
        yaw += 360.0f;
    }

    if (pitch > 90.0f)
    {
        pitch = 89.9f;
    }
    if (pitch < -90.0f)
    {
        pitch = -89.9f;
    }

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction = glm::normalize(direction);
}