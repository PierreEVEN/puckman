#include "ghost.hpp"

namespace pm
{
void Ghost::draw()
{
    if (rand() % 100 == 0)
        set_look_direction((EDirection)(rand() % 4 + 1));

    Character::draw();
}
}
