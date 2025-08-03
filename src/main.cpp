#include <cstdlib>
#include "gfx/state.hpp"

int main()
{
    State::state.init();
    State::state.window->run();

    return EXIT_SUCCESS;
}