#include "game.hpp"

void Game::init()
{
    world.generate();
}

void Game::update()
{
    world.update();
}