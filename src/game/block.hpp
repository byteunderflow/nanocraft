#pragma once

enum BlockFace
{
    FRONT,
    BACK,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

enum BlockType : unsigned char
{
    AIR,
    GRASS,
    STONE,
    BRICK
};

using Block = unsigned char;
using BlockMask = unsigned char;

constexpr BlockMask BLOCK_MASK_TYPE = 15;
constexpr BlockMask BLOCK_MASK_SOLID = 16;
constexpr BlockMask BLOCK_MASK_TRANSPARENT = 32;

void setBlockType(Block &block, BlockType type);
void setBlockFlag(Block &block, BlockMask flag);
BlockType getBlockType(Block block);
bool getBlockFlag(Block block, BlockMask flag);