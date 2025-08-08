#pragma once

#include "chunk.hpp"
#include "player.hpp"

#include <vector>

struct World
{
    Player player;
    std::vector<std::unique_ptr<Chunk>> chunks;

    struct Settings
    {
        uint64 seed;
    };
    Settings settings;

    std::unique_ptr<Chunk> generateChunk(int32 x, int32 z);
    void generate();
    void update();
};