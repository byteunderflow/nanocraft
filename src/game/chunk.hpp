#pragma once

#include "block.hpp"

#include <glad/glad.h>

#include <cstddef>
#include <functional>

namespace Chunks
{
    struct Position
    {
        const int32 x, z;

        constexpr Position(const int32 x, const int32 z) : x(x), z(z) {}

        bool operator==(const Position &position) const noexcept
        {
            return x == position.x && z == position.z;
        }
    };

    struct PositionHash
    {
        std::size_t operator()(const Position &position) const noexcept
        {
            std::size_t h1 = std::hash<int32>()(position.x);
            std::size_t h2 = std::hash<int32>()(position.z);
            return h1 ^ (h2 << 1);
        }
    };

    struct Vertex
    {
        GLfloat x, y, z, u, v;
    };

    struct Mesh
    {
        static constexpr const Vertex VERTICES[6][4] = {
            {
                {-0.5f, 0.5f, 0.5f, 0.0f, 0.0f}, // Top left
                {0.5f, 0.5f, 0.5f, 0.0f, 0.0f},  // Top right
                {0.5f, -0.5f, 0.5f, 0.0f, 0.0f}, // Bottom right
                {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f} // Bottom left
            },

            {
                {-0.5f, 0.5f, -0.5f, 0.0f, 0.0f}, // Top left
                {0.5f, 0.5f, -0.5f, 0.0f, 0.0f},  // Top right
                {0.5f, -0.5f, -0.5f, 0.0f, 0.0f}, // Bottom right
                {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f} // Bottom left
            },

            {
                {-0.5f, 0.5f, -0.5f, 0.0f, 0.0f}, // Top left
                {-0.5f, 0.5f, 0.5f, 0.0f, 0.0f},  // Top right
                {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f}, // Bottom right
                {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f} // Bottom left
            },

            {
                {0.5f, 0.5f, 0.5f, 0.0f, 0.0f},   // Top left
                {0.5f, 0.5f, -0.5f, 0.0f, 0.0f},  // Top right
                {0.5f, -0.5f, -0.5f, 0.0f, 0.0f}, // Bottom right
                {0.5f, -0.5f, 0.5f, 0.0f, 0.0f}   // Bottom left
            },

            {
                {-0.5f, 0.5f, -0.5f, 0.0f, 0.0f}, // Top left
                {0.5f, 0.5f, -0.5f, 0.0f, 0.0f},  // Top right
                {0.5f, 0.5f, 0.5f, 0.0f, 0.0f},   // Bottom right
                {-0.5f, 0.5f, 0.5f, 0.0f, 0.0f}   // Bottom left
            },

            {
                {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f}, // Top left
                {0.5f, -0.5f, -0.5f, 0.0f, 0.0f},  // Top right
                {0.5f, -0.5f, 0.5f, 0.0f, 0.0f},   // Bottom right
                {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f}   // Bottom left
            }};

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        void addFace(uint32 x, uint32 y, uint32 z, Blocks::Type type, Blocks::Face face);
    };

    struct Chunk
    {
        static constexpr uint32 SIZE_X = 16;
        static constexpr uint32 SIZE_Y = 128;
        static constexpr uint32 SIZE_Z = 16;
        static constexpr uint32 SIZE = SIZE_X * SIZE_Y * SIZE_Z;

        const Position position;
        Blocks::Block blocks[SIZE];
        std::unique_ptr<Mesh> mesh;

        constexpr Chunk(const Position position) : position(position) {};
        void init();
        void update();
        void render();
        void setBlock(uint32 x, uint32 y, uint32 z, Blocks::Block block);
        Blocks::Block getBlock(uint32 x, uint32 y, uint32 z) const;
    };
}