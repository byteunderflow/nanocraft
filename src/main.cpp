#include "gfx/window.hpp"

constexpr int HEIGHT = 540;
constexpr int WIDTH = HEIGHT * 16 / 9;
constexpr const char* TITLE = "nanocraft";

int main()
{
    Window window;
    window.create(WIDTH, HEIGHT, TITLE);
    window.run();

    return EXIT_SUCCESS;
}