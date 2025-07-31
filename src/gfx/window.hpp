#pragma once

// #define HEIGHT 540
// #define WIDTH HEIGHT * 16 / 9
#define HEIGHT 512
#define WIDTH 512
#define TITLE "nanocraft"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "renderer.hpp"

class Window
{
public:
    Window();
    ~Window();
    void create();
    void run();

private:
    GLFWwindow *handle;
    int width;
    int height;
    std::unique_ptr<Renderer> renderer;
};