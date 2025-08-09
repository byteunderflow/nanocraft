#pragma once

#include "../util/types.hpp"

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
    constexpr Mask MASK_SOLID = 16;

    constexpr Block BLOCK_AIR = Type::AIR;
    constexpr Block BLOCK_GRASS = Type::GRASS | MASK_SOLID;
    constexpr Block BLOCK_STONE = Type::STONE | MASK_SOLID;
    constexpr Block BLOCK_BRICK = Type::BRICK | MASK_SOLID;
    constexpr Block BLOCK_BEDROCK = Type::BEDROCK | MASK_SOLID;

    inline void setType(Block &block, Type type)
    {
        block &= ~MASK_TYPE;
        block |= static_cast<Mask>(type);
    }

    inline bool isSolid(Block block)
    {
        return static_cast<bool>(block & MASK_SOLID);
    }

    inline Type getType(Block block)
    {
        return static_cast<Type>(block & MASK_TYPE);
    }
}