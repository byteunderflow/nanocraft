#pragma once

#define HEIGHT 540
#define WIDTH HEIGHT * 16 / 9
#define TITLE "nanocraft"

#include "mouse.hpp"
#include "renderer.hpp"

#include <GLFW/glfw3.h>

class Mouse;
class Camera;
class Renderer;

class Window
{
public:
    std::unique_ptr<Mouse> mouse;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Renderer> renderer;
    int width;
    int height;
    float delta;

    Window() = default;
    ~Window();
    void create();
    void run();

private:
    GLFWwindow *handle;

    void input();
    void move(double x, double y);
};