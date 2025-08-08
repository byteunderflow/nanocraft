#include "world.hpp"

#include <iostream>

std::unique_ptr<Chunk> World::generateChunk(int32 x, int32 z)
{
    const ChunkPosition position = {x, z};
    std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>(position);
    chunk->init();

    for (uint32 x = 0; x < Chunk::CHUNK_X; ++x)
    {
        for (uint32 z = 0; z < Chunk::CHUNK_Z; ++z)
        {
            for (uint32 y = 0; y < 1; ++y)
            {
                chunk->setBlock(x, y, z, Blocks::BLOCK_BEDROCK);
            }
        }
    }

    for (uint32 x = 0; x < Chunk::CHUNK_X; ++x)
    {
        for (uint32 z = 0; z < Chunk::CHUNK_Z; ++z)
        {
            for (uint32 y = 1; y < 31; ++y)
            {
                chunk->setBlock(x, y, z, Blocks::BLOCK_STONE);
            }
        }
    }

    for (uint32 x = 0; x < Chunk::CHUNK_X; ++x)
    {
        for (uint32 z = 0; z < Chunk::CHUNK_Z; ++z)
        {
            chunk->setBlock(x, 31, z, Blocks::BLOCK_GRASS);
        }
    }

    return chunk;
}

void World::generate()
{
    std::unique_ptr<Chunk> chunk = generateChunk(0, 0);
    chunks.push_back(std::move(chunk));
}

void World::update()
{
    player.update();
}