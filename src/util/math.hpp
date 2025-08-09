#pragma once

#include "types.hpp"

namespace Math
{
    inline int32 floorDiv(int32 a, int32 b)
    {
        int32 q = a / b;
        if ((a ^ b) < 0 && (q * b != a))
            --q;
        return q;
    }
}