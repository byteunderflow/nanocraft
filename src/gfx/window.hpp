#pragma once

#define HEIGHT 540
#define WIDTH HEIGHT * 16 / 9
#define TITLE "Minecraft"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
};