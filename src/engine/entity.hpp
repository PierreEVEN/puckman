#pragma once
#include "sprite_sheet.hpp"

#include <array>

namespace pm
{
enum class EDirection : uint8_t
{
    Idle = 0,
    Up = 1,
    Left = 2,
    Down = 3,
    Right = 4,
    MAX = 4,
};

class Entity
{
public:
    Entity();

    void set_direction_sprite(const EDirection direction, const SpriteHandle& new_sprite);

    [[nodiscard]] double get_pos_x() const { return pos_x; }
    [[nodiscard]] double get_pos_y() const { return pos_y; }

    void set_direction(const EDirection new_direction);

    void draw();

    void set_position(const double x, const double y)
    {
        pos_x = x, pos_y = y;
    }

private:
    double pos_x;
    double pos_y;
    
    std::array<std::optional<SpriteHandle>, static_cast<size_t>(EDirection::MAX) + 1> direction_sprite;

    EDirection current_direction;
};
}
