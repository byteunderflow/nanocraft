#include "game.hpp"

void Game::init()
{
    world.init();
    world.generate();
}

void Game::update()
{
    world.update();
}