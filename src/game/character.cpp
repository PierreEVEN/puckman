#include "character.hpp"

#include "pacman_gamemode.hpp"
#include "engine/engine.hpp"
#include "engine/terrain.hpp"

namespace pm
{
void Character::tick()
{
    if (Engine::get().get_gamemode<PacmanGamemode>().stop_movements())
    {
        Entity::tick();
        return;
    }


    const auto   current_dir_vector = *current_direction;
    const double step               = Engine::get().get_delta_second() * velocity;
    const auto&  terrain            = get_terrain();

    if (current_direction == get_look_direction())
    {
        // Move forward in current direction
        const auto next = get_cell_discrete_pos() + current_dir_vector;

        // Predict cell in next frame
        const auto next_absolute = (get_cell_linear_pos() + current_dir_vector.cast<Vector2D>() * (0.5 + step)).rounded().cast<Vector2I>();

        // Hit wall
        if (!terrain.is_free(next_absolute, go_through_doors) && !terrain.is_tunnel(next_absolute))
        {
            pause_animation(true);
            set_cell_discrete_pos(get_cell_discrete_pos());
            Entity::tick();
            return;
        }

        // Move forward
        set_cell_linear_pos(get_cell_linear_pos() + current_dir_vector.cast<Vector2D>() * step);

        if (terrain.is_tunnel(next))
        {
            const int32_t width = terrain.get_width();
            if (get_cell_discrete_pos().x() < -1.0)
                set_cell_discrete_pos({width, get_cell_discrete_pos().y()});
            else if (get_cell_discrete_pos().x() > width)
                set_cell_discrete_pos({0, get_cell_discrete_pos().y()});
        }
    }
    else
    {
        // Interpolate to cell center before changing direction
        const auto cell_delta = get_cell_discrete_pos().cast<Vector2D>() - get_cell_linear_pos();

        // to far from cell center
        if (cell_delta.l1_length() < step)
        {
            // Teleport to cell center, and add remaining delta in new direction
            current_direction         = get_look_direction();
            const auto new_dir_vector = *get_look_direction();
            set_cell_linear_pos(get_cell_discrete_pos().cast<Vector2D>() + cell_delta + new_dir_vector.cast<Vector2D>() * (step - (cell_delta.x() + cell_delta.y())));
        }
        else
        {
            // Move to cell center
            set_cell_linear_pos(get_cell_linear_pos() + Vector2D{step * std::sign(cell_delta.x()), step * std::sign(cell_delta.y())});
        }
    }

    Entity::tick();
}

void Character::set_look_direction(const Direction new_direction)
{
    if (new_direction == Direction::NONE)
    {
        Entity::set_look_direction(new_direction);
        current_direction = new_direction;
    }

    const auto dot_result = new_direction->dot(*current_direction);

    pause_animation(false);
    // Go forward or back
    if (dot_result != 0)
    {
        current_direction = new_direction;
        return Entity::set_look_direction(new_direction);
    }

    // Allow turn if cell is free
    if (!get_terrain().is_free(get_cell_discrete_pos() + *new_direction, go_through_doors))
        return;

    Entity::set_look_direction(new_direction);
    if (current_direction.is_none())
        current_direction = new_direction;
}
}
