#pragma once

struct Mouse
{
    bool first = true;
    float lastx;
    float lasty;
    float xoffset = 0.0f;
    float yoffset = 0.0f;

    struct Settings
    {
        float sensitivity = 0.2f;
    };
    Settings settings;

    Mouse(float x, float y) : lastx(x), lasty(y) {};
    void update(float x, float y);
};