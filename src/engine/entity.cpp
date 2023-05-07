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
    if (looking_direction == Direction::NONE)
        direction_sprite[Direction::NONE.index()]->reset_timer();
}

void pm::Entity::draw()
{
    if (hidden)
        return;
    auto sprite = direction_sprite[looking_direction.index()];
    if (!sprite)
    {
        WARNING("missing sprite for direction %d", looking_direction.index());
        return;
    }
    
    auto s = Cell::draw_scale;
    sprite->draw(get_absolute_discrete_pos() * static_cast<int32_t>(s), s, s);
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
