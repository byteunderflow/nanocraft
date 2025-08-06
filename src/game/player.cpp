#include "player.hpp"

void Player::forward(float delta)
{
    position += delta * speed * direction;
}

void Player::backward(float delta)
{
    position -= delta * speed * direction;
}

void Player::left(float delta)
{
    position -= glm::normalize(glm::cross(direction, up)) * delta * speed;
}

void Player::right(float delta)
{
    position += glm::normalize(glm::cross(direction, up)) * delta * speed;
}

void Player::upward(float delta)
{
    position += up * delta * speed;
}

void Player::downward(float delta)
{
    position -= up * delta * speed;
}

void Player::look(float xoffset, float yoffset)
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

glm::mat4 Player::view()
{
    return glm::lookAt(position, position + direction, up);
}