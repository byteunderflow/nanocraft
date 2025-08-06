#pragma once

#include "worldgenerator.hpp"
#include "player.hpp"
#include "chunk.hpp"

#include <vector>

struct World
{
    WorldGenerator generator;
    Player player;
    std::vector<std::unique_ptr<Chunk>> chunks;

    struct Settings
    {
    };
    Settings settings;

    std::unique_ptr<Chunk> generateChunk(int x, int z);
    void generate();
    void update();
};