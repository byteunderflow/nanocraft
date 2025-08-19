#include "worldgen.hpp"

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

    for (uint32 x = 0; x < Chunks::Chunk::SIZE_X; ++x)
    {
        for (uint32 z = 0; z < Chunks::Chunk::SIZE_Z; ++z)
        {
            const float64 worldX =
                static_cast<float64>(position.x * static_cast<int32>(Chunks::Chunk::SIZE_X) + static_cast<int32>(x));
            const float64 worldZ =
                static_cast<float64>(position.z * static_cast<int32>(Chunks::Chunk::SIZE_Z) + static_cast<int32>(z));

            const float64 biomeValue = noise.noise2D_01(worldX * 0.005, worldZ * 0.005);
            Biome biome;
            if (biomeValue < 0.25)
                biome = BIOME_DESERT;
            else if (biomeValue < 0.5)
                biome = BIOME_PLAINS;
            else if (biomeValue < 0.75)
                biome = BIOME_MOUNTAINS;
            else
                biome = BIOME_OCEAN;

            const float64 worldY =
                std::floor(noise.noise2D_01(worldX * static_cast<float64>(scale), worldZ * static_cast<float64>(scale)) *
                           static_cast<float64>(BIOME_HEIGHT[biome]));
            const uint32 ground = static_cast<uint32>(worldY);

            for (uint32 y = 0; y < Chunks::Chunk::SIZE_Y; ++y)
            {
                if (y == 0)
                {
                    chunk->setBlock(x, y, z, Blocks::BLOCK_BEDROCK);
                    continue;
                }

                if (y < ground - 4)
                {
                    chunk->setBlock(x, y, z, Blocks::BLOCK_STONE);
                    continue;
                }

                if (y < ground)
                {
                    chunk->setBlock(x, y, z, biome == BIOME_DESERT || biome == BIOME_OCEAN ? Blocks::BLOCK_SAND : Blocks::BLOCK_DIRT);
                    continue;
                }

                if (y == ground)
                {
                    if (biome == BIOME_DESERT)
                        chunk->setBlock(x, y, z, Blocks::BLOCK_SAND);
                    else if (biome == BIOME_MOUNTAINS)
                        chunk->setBlock(x, y, z, Blocks::BLOCK_STONE);
                    else if (biome == BIOME_PLAINS)
                        chunk->setBlock(x, y, z, Blocks::BLOCK_GRASS);
                    else
                        chunk->setBlock(x, y, z, Blocks::BLOCK_WATER);
                    continue;
                }

                if (y == ground + 1 && (biome == BIOME_DESERT || biome == BIOME_PLAINS))
                {
                    if (biome == BIOME_PLAINS && std::rand() % 10 == 0)
                        chunk->setBlock(x, y, z, Blocks::BLOCK_PLANT);
                    else if (biome == BIOME_DESERT && std::rand() % 50 == 0)
                    {
                        chunk->setBlock(x, y, z, Blocks::BLOCK_CACTUS);
                        chunk->setBlock(x, y + 1, z, Blocks::BLOCK_CACTUS);
                    }
                    else
                        chunk->setBlock(x, y, z, Blocks::BLOCK_AIR);
                    continue;
                }

                chunk->setBlock(x, y, z, Blocks::BLOCK_AIR);
            }
        }
    }

    world.chunks[chunk->position] = std::move(chunk);
}