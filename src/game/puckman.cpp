#include "puckman.hpp"

#include "engine/logger.hpp"
#include "engine/terrain.hpp"

namespace pm
{
void Puckman::draw()
{
    const int new_points = points + get_terrain().eat(static_cast<int32_t>(std::round(get_pos_x() / 16)), static_cast<int32_t>(std::round(get_pos_y() / 16)));
    Character::draw();
    if (points != new_points)
    {
        points = new_points;
        DEBUG("{} Points", points);
    }
}
}
