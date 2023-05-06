#include "entity.hpp"

#include "logger.hpp"

pm::Entity::Entity(std::shared_ptr<Terrain> new_terrain)
    : direction_sprite({}), looking_direction(Direction::NONE), terrain(std::move(new_terrain))
{
}

void pm::Entity::set_direction_sprite(const Direction direction, const SpriteHandle& new_sprite)
{
    direction_sprite[direction.index()] = new_sprite;
    direction_sprite[direction.index()]->set_paused(true);
}

void pm::Entity::set_look_direction(const Direction new_direction)
{
    looking_direction = new_direction;
}

void pm::Entity::draw()
{
    auto sprite = direction_sprite[looking_direction.index()];
    if (!sprite)
    {
        WARNING("missing sprite for direction %d", looking_direction.index());
        return;
    }

    sprite->draw(SDL_Point{get_absolute_discrete_pos().x(), get_absolute_discrete_pos().y()});
}

void pm::Entity::pause_animation(bool paused)
{
    for (auto& anim : direction_sprite)
        if (anim)
            anim->set_paused(paused);
}

bool pm::Entity::is_at_intersection() const
{
    if (looking_direction.is_vertical())
        if (terrain->is_free(get_cell_discrete_pos() + Vector2I{1, 0}) || terrain->is_free(get_cell_discrete_pos() - Vector2I{1, 0}))
            return true;
    if (looking_direction.is_horizontal())
        if (terrain->is_free(get_cell_discrete_pos() + Vector2I{0, 1}) || terrain->is_free(get_cell_discrete_pos() - Vector2I{0, 1}))
            return true;
    return false;
}
