#include "character.hpp"

#include "terrain.hpp"

namespace pm
{
void Character::draw()
{
    Entity::draw();
}

void Character::set_position(const double x, const double y)
{
    Entity::set_position(x, y);
    lin_pos_x = x;
    lin_pos_y = y;
}

void Character::set_look_direction(const EDirection new_direction)
{
    const auto current_dir = direction_to_vector(current_direction);
    const auto dot_result = discrete_dot(direction_to_vector(new_direction), current_dir);

    // Go forward or back
    if (dot_result != 0)
    {
        current_direction = new_direction;
        return Entity::set_look_direction(new_direction);
    }

    // Turn
    //@TODO 



}
}
