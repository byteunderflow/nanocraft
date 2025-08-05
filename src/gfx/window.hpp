#pragma once

#include "mouse.hpp"
#include "camera.hpp"
#include "renderer.hpp"

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

struct Window
{
    GLFWwindow *handle;
    std::unique_ptr<Mouse> mouse;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Renderer> renderer;

    struct Settings
    {
        bool vsync = true;
    };
    Settings settings;

    struct State
    {
        bool paused = false;
    };
    State state;

    struct Time
    {
        float lasttime = 0.0;
        float delta = 0.0f;

        float lastframe = 0.0;
        int frames;
        int fps;
    };
    Time time;

    ~Window();
    void create(int width, int height, const char *title);
    void run();
    void pause();
    void resume();
    void input();
    void move(double x, double y);
};