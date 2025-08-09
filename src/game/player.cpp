#include "player.hpp"

void Player::init(World *_world)
{
    world = _world;
    position = glm::vec3(0.0f, 32.0f + settings.height, 0.0f);
}

void Player::update()
{
}

void Player::move(Movement movement, float32 delta)
{
    glm::vec3 destination;
    switch (movement)
    {
    case Movement::FORWARD:
        destination = position + delta * settings.speed * glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));
        break;
    case Movement::BACKWARD:
        destination = position - delta * settings.speed * glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));
        break;
    case Movement::LEFT:
        destination = position - glm::normalize(glm::cross(direction, up)) * delta * settings.speed;
        break;
    case Movement::RIGHT:
        destination = position + glm::normalize(glm::cross(direction, up)) * delta * settings.speed;
        break;
    case Movement::UPWARD:
        destination = position + up * delta * settings.speed;
        break;
    case Movement::DOWNWARD:
        destination = position - up * delta * settings.speed;
        break;
    }

    const int32 x = static_cast<int32>(destination.x);
    const int32 y = static_cast<int32>(destination.y - settings.height);
    const int32 z = static_cast<int32>(destination.z);

    if (y < 0 || y > static_cast<int32>(Chunk::CHUNK_Y))
        return;

    chunk = world->getChunkByWorldPosition(x, z);
    if (!chunk)
    {
        position = destination;
        return;
    }

    const Blocks::Block block = world->getBlock(x, y, z);
    if (!Blocks::isSolid(block))
    {
        position = destination;
    }
}

void Player::look(float32 xOffset, float32 yOffset)
{
    yaw += xOffset;
    pitch += yOffset;

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