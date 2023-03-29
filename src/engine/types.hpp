#pragma once

namespace std
{
constexpr int sign(auto a) { return a < 0 ? -1 : 1; }
constexpr auto clamp(auto val, auto min, auto max) { return val < min ? min : val > max ? max : val; }
}

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
        return SDL_Point{0, -1};
    case EDirection::Left:
        return SDL_Point{-1, 0};
    case EDirection::Down:
        return SDL_Point{0, 1};
    case EDirection::Right:
        return SDL_Point{1, 0};
    default: ;
        return SDL_Point{0, 0};
    }
}

SDL_Point operator*(const SDL_Point a, const auto b)
{
    return { static_cast<int>(a.x * b), static_cast<int>(a.y * b) };
}

SDL_Point& operator*=(SDL_Point& a, const auto b)
{
    return a = { static_cast<int>(a.x * b), static_cast<int>(a.y * b) };
}

inline int32_t discrete_dot(const SDL_Point& a, const SDL_Point& b)
{
    return a.x * b.x + a.y * b.y;
}

inline SDL_Point normalize(const SDL_Point& v)
{
    return std::abs(v.x) > std::abs(v.y) ? SDL_Point{std::clamp(v.x, -1, 1), 0} : SDL_Point{0, std::clamp(v.y, -1, 1)};
}

inline EDirection get_direction(const SDL_Point& v)
{
    if (v.x == 0 && v.y == 0)
        return EDirection::Idle;

    return std::abs(v.x) > std::abs(v.y) ? (v.x > 0 ? EDirection::Right : EDirection::Left) : (v.y > 0 ? EDirection::Down : EDirection::Up);
}

}


inline SDL_Point operator+(const SDL_Point& a, const SDL_Point& b)
{
    return {a.x + b.x, a.y + b.y};
}

inline SDL_Point operator-(const SDL_Point& a, const SDL_Point& b)
{
    return {a.x - b.x, a.y - b.y};
}

SDL_Point operator+(const SDL_Point& a, const auto& b)
{
    return {a.x + b, a.y + b};
}

SDL_Point operator-(const SDL_Point& a, const auto& b)
{
    return {a.x - b, a.y - b};
}

inline bool operator==(const SDL_Point& a, const SDL_Point& b)
{
    return a.x == b.x && a.y == b.y;
}

inline bool operator<(const SDL_Point& a, const SDL_Point& b)
{
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}

namespace std
{
template <> struct hash<SDL_Point>
{
    std::size_t operator()(const SDL_Point& p) const noexcept
    {
        return std::hash<int>()(p.x) + std::hash<int>()(p.y) * 2537;
    }
};
}
