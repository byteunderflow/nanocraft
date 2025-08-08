#pragma once

#include "block.hpp"

struct ChunkPosition
{
    int32 x;
    int32 z;
};

struct Chunk
{
    static constexpr uint32 CHUNK_X = 16;
    static constexpr uint32 CHUNK_Y = 128;
    static constexpr uint32 CHUNK_Z = 16;
    static constexpr uint32 CHUNK_SIZE = CHUNK_X * CHUNK_Y * CHUNK_Z;

    const ChunkPosition position;
    Blocks::Block blocks[CHUNK_X * CHUNK_Y * CHUNK_Z];

    Chunk(const ChunkPosition position) : position(position) {};
    void init();
    void update();
    void setBlock(uint32 x, uint32 y, uint32 z, Blocks::Block block);
    Blocks::Block getBlock(uint32 x, uint32 y, uint32 z) const;
};