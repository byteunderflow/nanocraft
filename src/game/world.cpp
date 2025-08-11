#include "world.hpp"

#include "../util/math.hpp"

#include <unordered_set>

void World::init()
{
    generator = std::make_unique<WorldGenerator>(1337);

    player = std::make_unique<Player>();
    player->init(this);
}

void World::generate()
{
    generator->generate(*this);

    player->position = glm::vec3(0.0f, 50.0f, 0.0f);
}

void World::update()
{
    const int32 chunkX =
        static_cast<int32>(std::floor(player->position.x / static_cast<float32>(Chunks::Chunk::SIZE_X)));
    const int32 chunkZ =
        static_cast<int32>(std::floor(player->position.z / static_cast<float32>(Chunks::Chunk::SIZE_Z)));
    const int32 renderDistance = static_cast<int32>(player->settings.renderDistance);

    std::unordered_set<Chunks::Position, Chunks::PositionHash> range;
    for (int32 x = chunkX - renderDistance; x <= chunkX + renderDistance; ++x)
    {
        for (int32 z = chunkZ - renderDistance; z <= chunkZ + renderDistance; ++z)
        {
            const Chunks::Position position{x, z};
            range.insert(position);

            if (chunks.find(position) == chunks.end())
                generator->generateChunk(*this, position);
        }
    }

    for (auto iterator = chunks.begin(); iterator != chunks.end(); )
    {
        if (range.find(iterator->first) == range.end())
        {
            iterator = chunks.erase(iterator);
        }
        else
        {
            ++iterator;
        }
    }

    player->update();
}

Chunks::Chunk *World::getChunkByChunkPosition(const Chunks::Position &position) const
{
    auto iterator = chunks.find(position);
    if (iterator != chunks.end())
        return iterator->second.get();
    return nullptr;
}

inline Chunks::Chunk *World::getChunkByChunkPosition(int32 x, int32 z) const
{
    return getChunkByChunkPosition({x, z});
}

Chunks::Chunk *World::getChunkByWorldPosition(int32 x, int32 z) const
{
    return getChunkByChunkPosition(
        Math::floorDiv(x, static_cast<int32>(Chunks::Chunk::SIZE_X)),
        Math::floorDiv(z, static_cast<int32>(Chunks::Chunk::SIZE_Z)));
}

Blocks::Block World::getBlock(int32 x, int32 y, int32 z) const
{
    Chunks::Chunk *chunk = getChunkByWorldPosition(x, z);
    if (!chunk)
        return Blocks::BLOCK_AIR;

    int32 blockX = x % static_cast<int32>(Chunks::Chunk::SIZE_X);
    if (blockX < 0)
        blockX += Chunks::Chunk::SIZE_X;
    int32 blockZ = z % static_cast<int32>(Chunks::Chunk::SIZE_Z);
    if (blockZ < 0)
        blockZ += Chunks::Chunk::SIZE_Z;

    return chunk->getBlock(
        static_cast<uint32>(blockX),
        static_cast<uint32>(y),
        static_cast<uint32>(blockZ));
}