#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "renderer.hpp"

#include "../game/game.hpp"

struct Window
{
    GLFWwindow *handle;
    int width = 0;
    int height = 0;

    Renderer *renderer;
    Game *game;

    struct State
    {
        bool paused = false;
    };
    State state;

    struct Time
    {
        float lasttime = 0.0f;
        float delta = 0.0f;

        float lastframe = 0.0f;
        int frames = 0;
        int fps = 0;
    };
    Time time;

    struct Mouse
    {
        bool enter = true;
        float lastx = 0.0f;
        float lasty = 0.0f;

        struct Settings
        {
            float sensitivity = 0.2f;
        };
        Settings settings;
    };
    Mouse mouse;

    struct Settings
    {
        bool vsync = true;
    };
    Settings settings;

    ~Window();
    void create(int width, int height, const char *title);
    void run();
    void pause();
    void resume();
    void input();
    void move(double x, double y);
};