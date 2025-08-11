#pragma once

#include "chunk.hpp"
#include "player.hpp"
#include "worldgen.hpp"

#include <unordered_map>

struct Player;
struct WorldGenerator;

struct World
{
    std::unique_ptr<Player> player;
    std::unique_ptr<WorldGenerator> generator;
    std::unordered_map<Chunks::Position, std::unique_ptr<Chunks::Chunk>, Chunks::PositionHash> chunks;

    void init();
    void generate();
    void update();
    Chunks::Chunk *getChunkByChunkPosition(const Chunks::Position &position) const;
    Chunks::Chunk *getChunkByChunkPosition(int32, int32) const;
    Chunks::Chunk *getChunkByWorldPosition(int32 x, int32 z) const;
    Blocks::Block getBlock(int32 x, int32 y, int32 z) const;
};