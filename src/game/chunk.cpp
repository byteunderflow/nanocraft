#include "chunk.hpp"

void Chunk::setBlock(int x, int y, int z, Block block)
{
    blocks[x + y * CHUNK_X + z * CHUNK_X * CHUNK_Y] = block;
}

void Chunk::setType(int x, int y, int z, BlockType type)
{
    setBlockType(blocks[x + y * CHUNK_X + z * CHUNK_X * CHUNK_Y], type);
}

Block Chunk::getBlock(int x, int y, int z) const
{
    return blocks[x + y * CHUNK_X + z * CHUNK_X * CHUNK_Y];
}

BlockType Chunk::getType(int x, int y, int z) const
{
    return getBlockType(blocks[x + y * CHUNK_X + z * CHUNK_X * CHUNK_Y]);
}