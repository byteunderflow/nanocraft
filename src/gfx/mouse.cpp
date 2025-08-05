#include "mouse.hpp"

void Mouse::update(float x, float y)
{
    if (first)
    {
        lastx = x;
        lasty = y;
        first = false;
    }

    xoffset = x - lastx;
    yoffset = lasty - y;
    lastx = x;
    lasty = y;
    xoffset *= settings.sensitivity;
    yoffset *= settings.sensitivity;
}