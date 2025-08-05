#include "atlas.hpp"

void Atlas::load(const char *path) const
{
    texture.load(path, FORMAT);
}

void Atlas::compute(BlockType type, BlockFace face, TextureCoordinates& coordinates) const
{
    const int row = static_cast<int>(type);
    const int column = static_cast<int>(face);

    coordinates.u1 = column * TILE_WIDTH;
    coordinates.v2 = 1.0f - row * TILE_HEIGHT;
    coordinates.u2 = (column + 1) * TILE_WIDTH;
    coordinates.v1 = coordinates.v2 - TILE_HEIGHT;
}

void Atlas::bind()
{
    texture.bind(GL_TEXTURE0);
}

void Atlas::unbind() const
{
    texture.unbind();
}