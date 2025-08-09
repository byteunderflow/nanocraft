#include "world.hpp"

#include "../util/math.hpp"

void World::init()
{
    player = std::make_unique<Player>();
    player->init(this);
}

void World::generateChunk(const ChunkPosition &position)
{
    std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>(position);
    chunk->init();

    for (uint32 x = 0; x < Chunk::CHUNK_X; ++x)
    {
        for (uint32 z = 0; z < Chunk::CHUNK_Z; ++z)
        {
            for (uint32 y = 0; y < 1; ++y)
            {
                chunk->setBlock(x, y, z, Blocks::BLOCK_BEDROCK);
            }
        }
    }

    for (uint32 x = 0; x < Chunk::CHUNK_X; ++x)
    {
        for (uint32 z = 0; z < Chunk::CHUNK_Z; ++z)
        {
            for (uint32 y = 1; y < 31; ++y)
            {
                chunk->setBlock(x, y, z, Blocks::BLOCK_STONE);
            }
        }
    }

    for (uint32 x = 0; x < Chunk::CHUNK_X; ++x)
    {
        for (uint32 z = 0; z < Chunk::CHUNK_Z; ++z)
        {
            chunk->setBlock(x, 31, z, Blocks::BLOCK_GRASS);
        }
    }

    chunks[chunk->position] = std::move(chunk);
}

void World::generate()
{
    const int32 chunkX = static_cast<int32>(std::floor(player->position.x / static_cast<float32>(Chunk::CHUNK_X)));
    const int32 chunkZ = static_cast<int32>(std::floor(player->position.z / static_cast<float32>(Chunk::CHUNK_Z)));
    const int32 renderDistance = static_cast<int32>(player->settings.renderDistance);

    for (int32 x = chunkX - renderDistance; x <= chunkX + renderDistance; ++x)
    {
        for (int32 z = chunkZ - renderDistance; z <= chunkZ + renderDistance; ++z)
        {
            generateChunk({x, z});
        }
    }
}

void World::update()
{
}

Chunk *World::getChunkByChunkPosition(const ChunkPosition &position) const
{
    auto iterator = chunks.find(position);
    if (iterator != chunks.end())
        return iterator->second.get();
    return nullptr;
}

inline Chunk *World::getChunkByChunkPosition(int32 x, int32 z) const
{
    return getChunkByChunkPosition({x, z});
}

Chunk *World::getChunkByWorldPosition(int32 x, int32 z) const
{
    return getChunkByChunkPosition(
        Math::floorDiv(x, static_cast<int32>(Chunk::CHUNK_X)),
        Math::floorDiv(z, static_cast<int32>(Chunk::CHUNK_Z)));
}

Blocks::Block World::getBlock(int32 x, int32 y, int32 z) const
{
    Chunk *chunk = getChunkByWorldPosition(x, z);
    if (!chunk)
        return Blocks::BLOCK_AIR;

    int32 blockX = x % static_cast<int32>(Chunk::CHUNK_X);
    if (blockX < 0)
        blockX += Chunk::CHUNK_X;
    int32 blockZ = z % static_cast<int32>(Chunk::CHUNK_Z);
    if (blockZ < 0)
        blockZ += Chunk::CHUNK_Z;

    return chunk->getBlock(static_cast<uint32>(blockX), static_cast<uint32>(y), static_cast<uint32>(blockZ));
}