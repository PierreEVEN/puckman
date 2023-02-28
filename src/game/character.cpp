#include "character.hpp"

#include "engine/engine.hpp"
#include "engine/terrain.hpp"

namespace pm
{
void Character::draw()
{
    const auto   current_dir_vector = direction_to_vector(current_direction);
    const double step               = Engine::get().get_delta_second() * 3;

    if (current_direction == get_look_direction())
    {
        const auto next_x = std::round(lin_pos_x + current_dir_vector.x * 0.55);
        const auto next_y = std::round(lin_pos_y + current_dir_vector.y * 0.55);
        if (!get_terrain().is_free(SDL_Point{static_cast<int32_t>(next_x), static_cast<int32_t>(next_y)}))
        {
            lin_pos_x = std::round(lin_pos_x);
            lin_pos_y = std::round(lin_pos_y);
            pause_animation(true);
            Entity::set_position(lin_pos_x * 16, lin_pos_y * 16);
            Entity::draw();
            return;
        }

        lin_pos_x += current_dir_vector.x * step;
        lin_pos_y += current_dir_vector.y * step;
    }
    else
    {
        const double delta_x = std::round(lin_pos_x) - lin_pos_x;
        const double delta_y = std::round(lin_pos_y) - lin_pos_y;

        if (abs(delta_x) < step && abs(delta_y) < step)
        {
            current_direction         = get_look_direction();
            const auto new_dir_vector = direction_to_vector(current_direction);
            lin_pos_x                 = std::round(lin_pos_x) + delta_x + (step - (delta_x + delta_y)) * new_dir_vector.x;
            lin_pos_y                 = std::round(lin_pos_y) + delta_y + (step - (delta_x + delta_y)) * new_dir_vector.y;
        }
        else
        {
            lin_pos_x += step * std::sign(delta_x);
            lin_pos_y += step * std::sign(delta_y);
        }
    }

    Entity::set_position(lin_pos_x * 16, lin_pos_y * 16);
    Entity::draw();
}

void Character::set_position(const double x, const double y)
{
    Entity::set_position(x, y);
    lin_pos_x = x / 16;
    lin_pos_y = y / 16;
}

void Character::set_look_direction(const EDirection new_direction)
{
    const auto current_dir = direction_to_vector(current_direction);
    const auto dot_result  = discrete_dot(direction_to_vector(new_direction), current_dir);

    pause_animation(false);
    // Go forward or back
    if (dot_result != 0)
    {
        current_direction = new_direction;
        return Entity::set_look_direction(new_direction);
    }

    // Turn
    const auto next_dir = direction_to_vector(new_direction);
    const auto next_x   = std::round(lin_pos_x) + next_dir.x;
    const auto next_y   = std::round(lin_pos_y) + next_dir.y;
    if (!get_terrain().is_free(SDL_Point{static_cast<int32_t>(next_x), static_cast<int32_t>(next_y)}))
        return;

    Entity::set_look_direction(new_direction);
    if (current_direction == EDirection::Idle)
        current_direction = new_direction;
}
}
