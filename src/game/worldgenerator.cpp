#include "worldgenerator.hpp"

void WorldGenerator::generate(Chunk &chunk)
{
    for (int x = 0; x < CHUNK_X; ++x)
    {
        for (int z = 0; z < CHUNK_Z; ++z)
        {
            for (int y = 0; y < 1; ++y)
            {
                chunk.setType(x, y, z, BlockType::BRICK);
            }
        }
    }

    for (int x = 0; x < CHUNK_X; ++x)
    {
        for (int z = 0; z < CHUNK_Z; ++z)
        {
            for (int y = 1; y < 20; ++y)
            {
                chunk.setType(x, y, z, BlockType::STONE);
            }
        }
    }

    for (int x = 0; x < CHUNK_X; ++x)
    {
        for (int z = 0; z < CHUNK_Z; ++z)
        {
            for (int y = 20; y < 21; ++y)
            {
                chunk.setType(x, y, z, BlockType::GRASS);
            }
        }
    }
}