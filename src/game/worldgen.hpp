#pragma once

#include "world.hpp"

#include <PerlinNoise.hpp>

struct World;

enum Biome : uint8
{
    BIOME_PLAINS,
    BIOME_DESERT,
    BIOME_MOUNTAINS,
    BIOME_OCEAN,

    BIOME_COUNT
};

constexpr uint32 BIOME_HEIGHT[BIOME_COUNT] = {
    Chunks::Chunk::SIZE_Y / 3,
    Chunks::Chunk::SIZE_Y / 3,
    Chunks::Chunk::SIZE_Y - 1,
    Chunks::Chunk::SIZE_Y / 4};

struct WorldGenerator
{
    const siv::PerlinNoise::seed_type seed;
    const siv::PerlinNoise noise;
    float32 scale = 0.01f;

    WorldGenerator(siv::PerlinNoise::seed_type seed) : seed(seed), noise(seed) {};
    void generate(World &world) const;
    void generateChunk(World &world, const Chunks::Position &position) const;
};