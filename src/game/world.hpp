#pragma once

#include "chunk.hpp"
#include "player.hpp"

#include <unordered_map>

struct Player;

struct World
{
    std::unique_ptr<Player> player;
    std::unordered_map<ChunkPosition, std::unique_ptr<Chunk>, ChunkPositionHash> chunks;

    struct Settings
    {
        uint64 seed;
    };
    Settings settings;

    void init();
    void generateChunk(const ChunkPosition &position);
    void generate();
    void update();
    Chunk *getChunkByChunkPosition(const ChunkPosition &position) const;
    Chunk *getChunkByChunkPosition(int32, int32) const;
    Chunk *getChunkByWorldPosition(int32 x, int32 z) const;
    Blocks::Block getBlock(int32 x, int32 y, int32 z) const;
};