#include "state.hpp"

State State::state;

State::State() = default;
State::~State() = default;

void State::init()
{
    window = std::make_unique<Window>();
    window->create();

    renderer = std::make_unique<Renderer>();
    renderer->init();
    
    camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 1.0f));
}