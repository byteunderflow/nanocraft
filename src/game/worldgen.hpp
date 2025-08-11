#pragma once

#include "world.hpp"

#include <PerlinNoise.hpp>

struct World;

struct WorldGenerator
{
    const siv::PerlinNoise noise;
    float32 scale = 0.01f;
    float32 height = 0.5f;

    WorldGenerator(siv::PerlinNoise::seed_type seed) : noise(seed) {};
    void generate(World &world) const;
    void generateChunk(World &world, const Chunks::Position &position) const;
};