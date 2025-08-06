#pragma once

#include <memory>

#include "chunk.hpp"

struct WorldGenerator
{
    struct Settings
    {

    };
    Settings settings;

    void generate(Chunk &chunk);
};