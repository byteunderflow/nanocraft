#pragma once

#define HEIGHT 540
#define WIDTH HEIGHT * 16 / 9
#define TITLE "nanocraft"

#include "renderer.hpp"

class Window
{
public:
    Window() = default;
    ~Window();
    void create();
    void run();

private:
    GLFWwindow *handle;
    std::unique_ptr<Renderer> renderer;
};