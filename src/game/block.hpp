#pragma once

#include "../util/types.hpp"

namespace Blocks
{
    enum Face : uint8
    {
        FACE_FRONT,
        FACE_BACK,
        FACE_LEFT,
        FACE_RIGHT,
        FACE_TOP,
        FACE_BOTTOM,

        FACE_PLANT1,
        FACE_PLANT2,

        FACE_COUNT
    };

    enum Type : uint8
    {
        TYPE_AIR,
        TYPE_GRASS,
        TYPE_STONE,
        TYPE_BRICK,
        TYPE_BEDROCK,
        TYPE_PLANT,
        TYPE_SAND,
        TYPE_CACTUS,
        TYPE_DIRT,
        TYPE_WATER
    };

    using Block = uint8;
    using Mask = uint8;

    constexpr Mask MASK_TYPE = 15;
    constexpr Mask MASK_SOLID = 16;
    constexpr Mask MASK_PLANT = 32;

    constexpr Block BLOCK_AIR = TYPE_AIR;
    constexpr Block BLOCK_GRASS = TYPE_GRASS | MASK_SOLID;
    constexpr Block BLOCK_STONE = TYPE_STONE | MASK_SOLID;
    constexpr Block BLOCK_BRICK = TYPE_BRICK | MASK_SOLID;
    constexpr Block BLOCK_BEDROCK = TYPE_BEDROCK | MASK_SOLID;
    constexpr Block BLOCK_PLANT = TYPE_PLANT | MASK_PLANT;
    constexpr Block BLOCK_SAND = TYPE_SAND | MASK_SOLID;
    constexpr Block BLOCK_CACTUS = TYPE_CACTUS;
    constexpr Block BLOCK_DIRT = TYPE_DIRT | MASK_SOLID;
    constexpr Block BLOCK_WATER = TYPE_WATER;

    inline void setType(Block &block, Type type)
    {
        block &= ~MASK_TYPE;
        block |= static_cast<Mask>(type);
    }

    inline bool isSolid(Block block)
    {
        return static_cast<bool>(block & MASK_SOLID);
    }

    inline bool isPlant(Block block)
    {
        return static_cast<bool>(block & MASK_PLANT);
    }

    inline Type getType(Block block)
    {
        return static_cast<Type>(block & MASK_TYPE);
    }
}