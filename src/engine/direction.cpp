#include "direction.hpp"

namespace pm
{
const Direction Direction::RIGHT = {1, 0};
const Direction Direction::LEFT  = {-1, 0};
const Direction Direction::UP    = {0, -1};
const Direction Direction::DOWN  = {0, 1};
const Direction Direction::NONE  = {0, 0};

static const auto enumeration = std::vector{Direction::RIGHT, Direction::LEFT, Direction::UP, Direction::DOWN};

const std::vector<Direction>& Direction::enumerate()
{
    return enumeration;
}

Direction Direction::opposite() const
{
    if (internal_dir.x() == 1)
        return LEFT;
    if (internal_dir.x() == -1)
        return RIGHT;
    if (internal_dir.y() == 1)
        return UP;
    if (internal_dir.y() == -1)
        return DOWN;
    return NONE;
}
}
