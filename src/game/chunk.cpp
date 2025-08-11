#include "chunk.hpp"

namespace Chunks
{
    void Mesh::addFace(uint32 x, uint32 y, uint32 z, Blocks::Type type, Blocks::Face face)
    {
        constexpr float32 width = 1.0f / 16.0f;
        constexpr float32 height = 1.0f / 16.0f;
        const uint8 row = static_cast<uint8>(type) - 1;
        const uint8 column = static_cast<uint8>(face);
        const float32 u1 = column * width;
        const float32 v2 = 1.0f - row * height;
        const float32 u2 = (column + 1) * width;
        const float32 v1 = v2 - height;

        const GLuint index = static_cast<GLuint>(vertices.size());

        Vertex vertex1 = VERTICES[face][0];
        vertex1.x += x;
        vertex1.y += y;
        vertex1.z += z;
        vertex1.u = u1;
        vertex1.v = v2;

        Vertex vertex2 = VERTICES[face][1];
        vertex2.x += x;
        vertex2.y += y;
        vertex2.z += z;
        vertex2.u = u2;
        vertex2.v = v2;

        Vertex vertex3 = VERTICES[face][2];
        vertex3.x += x;
        vertex3.y += y;
        vertex3.z += z;
        vertex3.u = u2;
        vertex3.v = v1;

        Vertex vertex4 = VERTICES[face][3];
        vertex4.x += x;
        vertex4.y += y;
        vertex4.z += z;
        vertex4.u = u1;
        vertex4.v = v1;

        vertices.push_back(vertex1);
        vertices.push_back(vertex2);
        vertices.push_back(vertex3);
        vertices.push_back(vertex4);

        indices.push_back(index + 0);
        indices.push_back(index + 1);
        indices.push_back(index + 2);
        indices.push_back(index + 2);
        indices.push_back(index + 3);
        indices.push_back(index + 0);
    }

    void Chunk::init()
    {
        for (uint32 i = 0; i < SIZE; ++i)
            blocks[i] = Blocks::BLOCK_AIR;
    }

    void Chunk::update()
    {
    }

    void Chunk::render()
    {
        if (mesh)
            return;

        mesh = std::make_unique<Mesh>();
        for (uint32 x = 0; x < SIZE_X; ++x)
        {
            for (uint32 y = 0; y < SIZE_Y; ++y)
            {
                for (uint32 z = 0; z < SIZE_Z; ++z)
                {
                    const Blocks::Block block = getBlock(x, y, z);
                    if (block == Blocks::BLOCK_AIR)
                        continue;

                    const Blocks::Type type = Blocks::getType(block);

                    // Optimization: Face culling (render only visible faces)
                    if (z + 1 == SIZE_Z || !Blocks::isSolid(getBlock(x, y, z + 1)))
                        mesh->addFace(x, y, z, type, Blocks::Face::FRONT);
                    if (z == 0 || !Blocks::isSolid(getBlock(x, y, z - 1)))
                        mesh->addFace(x, y, z, type, Blocks::Face::BACK);
                    if (x == 0 || !Blocks::isSolid(getBlock(x - 1, y, z)))
                        mesh->addFace(x, y, z, type, Blocks::Face::LEFT);
                    if (x + 1 == SIZE_X || !Blocks::isSolid(getBlock(x + 1, y, z)))
                        mesh->addFace(x, y, z, type, Blocks::Face::RIGHT);
                    if (y + 1 == SIZE_Y || !Blocks::isSolid(getBlock(x, y + 1, z)))
                        mesh->addFace(x, y, z, type, Blocks::Face::TOP);
                    if (y == 0 || !Blocks::isSolid(getBlock(x, y - 1, z)))
                        mesh->addFace(x, y, z, type, Blocks::Face::BOTTOM);
                }
            }
        }
    }

    void Chunk::setBlock(uint32 x, uint32 y, uint32 z, Blocks::Block block)
    {
        blocks[x + y * SIZE_X + z * SIZE_X * SIZE_Y] = block;
        mesh.reset();
    }

    Blocks::Block Chunk::getBlock(uint32 x, uint32 y, uint32 z) const
    {
        return blocks[x + y * SIZE_X + z * SIZE_X * SIZE_Y];
    }
}