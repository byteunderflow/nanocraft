#pragma once

#include "texture.hpp"
#include "../game/block.hpp"
#include <glad/glad.h>

struct TextureCoordinates
{
    float u1;
    float v1;
    float u2;
    float v2;
};

struct Atlas
{
    static constexpr float TILE_WIDTH = 1.0f / 16.0f;
    static constexpr float TILE_HEIGHT = 1.0f / 16.0f;
    static constexpr GLenum FORMAT = GL_RGBA;

    Texture texture;
    void load(const char *path) const;
    void compute(BlockType type, BlockFace face, TextureCoordinates& coordinates) const;
    void bind();
    void unbind() const;
};