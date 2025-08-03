#pragma once

#include <memory>
#include "window.hpp"
#include "renderer.hpp"

struct State
{
    static State state;

public:
    std::unique_ptr<Window> window;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Camera> camera;

    State();
    ~State();
    void init();
};