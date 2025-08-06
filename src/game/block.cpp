#include "block.hpp"

void setBlockType(Block &block, BlockType type)
{
    block &= ~BLOCK_MASK_TYPE;
    block |= static_cast<BlockMask>(type);
}

void setBlockFlag(Block &block, BlockMask flag)
{
    block |= flag;
}

BlockType getBlockType(Block block)
{
    return static_cast<BlockType>(block & BLOCK_MASK_TYPE);
}

bool getBlockFlag(Block block, BlockMask flag)
{
    return static_cast<bool>(block & flag);
}