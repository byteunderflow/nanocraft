#include "world.hpp"

std::unique_ptr<Chunk> World::generateChunk(int x, int z)
{
    std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>(x, z);
    generator.generate(*chunk);
    return chunk;
}

void World::generate()
{
    std::unique_ptr<Chunk> chunk = generateChunk(0, 0);
    chunks.push_back(std::move(chunk));
}

void World::update()
{
}