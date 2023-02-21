#include "entity.hpp"

#include "logger.hpp"

pm::Entity::Entity()
    : pos_x(20), pos_y(20), direction_sprite({}), current_direction(EDirection::Idle)
{
}

void pm::Entity::set_direction_sprite(const EDirection direction, const SpriteHandle& new_sprite)
{
    direction_sprite[static_cast<uint8_t>(direction)] = new_sprite;
}

void pm::Entity::set_direction(const EDirection new_direction)
{
    current_direction = new_direction;
}

void pm::Entity::draw()
{
    auto sprite = direction_sprite[static_cast<uint8_t>(current_direction)];
    if (!sprite)
    {
        WARNING("missing sprite for direction %d", static_cast<uint8_t>(current_direction));
        return;
    }

    sprite->draw(SDL_Point{static_cast<int>(pos_x), static_cast<int>(pos_y)});
}
