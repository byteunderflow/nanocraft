#pragma once

#include "../types.hpp"

namespace Blocks
{
    enum Face : uint8
    {
        FRONT,
        BACK,
        LEFT,
        RIGHT,
        TOP,
        BOTTOM
    };

    enum Type : uint8
    {
        AIR,
        GRASS,
        STONE,
        BRICK,
        BEDROCK
    };

    using Block = uint8;
    using Mask = uint8;

    constexpr Mask MASK_TYPE = 15;
    constexpr Mask MASK_TRANSPARENT = 16;

    constexpr Block BLOCK_AIR = Type::AIR | MASK_TRANSPARENT;
    constexpr Block BLOCK_GRASS = Type::GRASS;
    constexpr Block BLOCK_STONE = Type::STONE;
    constexpr Block BLOCK_BRICK = Type::BRICK;
    constexpr Block BLOCK_BEDROCK = Type::BEDROCK;

    void setType(Block &block, Type type);
    Type getType(Block block);
}