#include "chunk.hpp"

void Chunk::init()
{
    for (uint32 i = 0; i < CHUNK_SIZE; ++i)
        blocks[i] = Blocks::BLOCK_AIR;
}

void Chunk::update()
{

}

void Chunk::setBlock(uint32 x, uint32 y, uint32 z, Blocks::Block block)
{
    blocks[x + y * CHUNK_X + z * CHUNK_X * CHUNK_Y] = block;
}

Blocks::Block Chunk::getBlock(uint32 x, uint32 y, uint32 z) const
{
    return blocks[x + y * CHUNK_X + z * CHUNK_X * CHUNK_Y];
}