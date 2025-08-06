#pragma once

#include "block.hpp"

#define CHUNK_X 16
#define CHUNK_Y 256
#define CHUNK_Z 16
#define CHUNK_SIZE CHUNK_X * CHUNK_Y * CHUNK_Z

struct Chunk
{
    const int x;
    const int z;
    Block blocks[CHUNK_X * CHUNK_Y * CHUNK_Z] = {BlockType::AIR};

    Chunk(const int x, const int z) : x(x), z(z) {}
    void setBlock(int x, int y, int z, Block block);
    void setType(int x, int y, int z, BlockType type);
    Block getBlock(int x, int y, int z) const;
    BlockType getType(int x, int y, int z) const;
};