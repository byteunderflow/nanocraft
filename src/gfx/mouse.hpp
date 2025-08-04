#pragma once

class Mouse
{
public:
    float sensitivity = 0.2f;
    float xoffset = 0.0f;
    float yoffset = 0.0f;

    Mouse(float x, float y) : lastx(x), lasty(y) {};
    ~Mouse() = default;
    void update(float x, float y);

private:
    bool first = true;
    float lastx;
    float lasty;
};