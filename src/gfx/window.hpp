#pragma once

#include "mouse.hpp"
#include "camera.hpp"
#include "renderer.hpp"

#include <GLFW/glfw3.h>

struct Window
{
    GLFWwindow *handle;
    std::unique_ptr<Mouse> mouse;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Renderer> renderer;
    float delta;

    ~Window();
    void create(int width, int height, const char *title);
    void run();
    void input();
    void move(double x, double y);
};