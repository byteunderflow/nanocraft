#include "worldgen.hpp"

#include <iostream>

void WorldGenerator::generate(World &world) const
{
    const int32 chunkX =
        static_cast<int32>(std::floor(world.player->position.x / static_cast<float32>(Chunks::Chunk::SIZE_X)));
    const int32 chunkZ =
        static_cast<int32>(std::floor(world.player->position.z / static_cast<float32>(Chunks::Chunk::SIZE_Z)));
    const int32 renderDistance = static_cast<int32>(world.player->settings.renderDistance);

    for (int32 x = chunkX - renderDistance; x <= chunkX + renderDistance; ++x)
        for (int32 z = chunkZ - renderDistance; z <= chunkZ + renderDistance; ++z)
            generateChunk(world, {x, z});
}

void WorldGenerator::generateChunk(World &world, const Chunks::Position &position) const
{
    std::unique_ptr<Chunks::Chunk> chunk = std::make_unique<Chunks::Chunk>(position);
    chunk->init();

    for (uint32 x = 0; x < Chunks::Chunk::SIZE_X; ++x)
    {
        for (uint32 z = 0; z < Chunks::Chunk::SIZE_Z; ++z)
        {
            const float64 worldX =
                static_cast<float64>(position.x * static_cast<int32>(Chunks::Chunk::SIZE_X) + static_cast<int32>(x));
            const float64 worldZ =
                static_cast<float64>(position.z * static_cast<int32>(Chunks::Chunk::SIZE_Z) + static_cast<int32>(z));
            const float64 worldY =
                std::floor(noise.noise2D_01(
                               worldX * static_cast<float64>(scale),
                               worldZ * static_cast<float64>(scale)) *
                           static_cast<float64>(Chunks::Chunk::SIZE_Y) * static_cast<float64>(height));

            for (uint32 y = 0; y < static_cast<uint32>(worldY); ++y)
                chunk->setBlock(x, y, z, Blocks::BLOCK_STONE);
            chunk->setBlock(x, static_cast<uint32>(worldY), z, Blocks::BLOCK_GRASS);
        }
    }

    world.chunks[chunk->position] = std::move(chunk);
}