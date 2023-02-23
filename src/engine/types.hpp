#pragma once

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

inline SDL_Point direction_to_vector(const EDirection direction)
{
    switch (direction)
    {
    case EDirection::Idle:
        return SDL_Point{0, 0};
    case EDirection::Up:
        return SDL_Point{0, 1};
    case EDirection::Left:
        return SDL_Point{-1, 0};
    case EDirection::Down:
        return SDL_Point{0, -1};
    case EDirection::Right:
        return SDL_Point{1, 0};
    default: ;
        return SDL_Point{0, 0};
    }
}

inline int32_t discrete_dot(const SDL_Point& a, const SDL_Point& b)
{
    return a.x * b.x + a.y * b.y;
}


}