#include "block.hpp"

namespace Blocks
{
    void setType(Block &block, Type type)
    {
        block &= ~MASK_TYPE;
        block |= static_cast<Mask>(type);
    }

    Type getType(Block block)
    {
        return static_cast<Type>(block & MASK_TYPE);
    }
}