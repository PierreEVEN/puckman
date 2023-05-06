#pragma once
#include "vector2.hpp"
#include <vector>

namespace std
{
constexpr int  sign(auto a) { return a < 0 ? -1 : 1; }
constexpr auto clamp(auto val, auto min, auto max) { return val < min ? min : val > max ? max : val; }
}

namespace pm
{
struct Direction
{
public:
    Direction()
        : internal_dir(0, 0)
    {
    }

    Direction(Vector2I dir)
        : internal_dir(normalize(dir))
    {
    }

    Direction(int32_t x, int32_t y)
        : internal_dir(normalize(Vector2I{x, y}))
    {
    }

    [[nodiscard]] static const std::vector<Direction>& enumerate();

    [[nodiscard]] int8_t index() const
    {
        if (internal_dir == NONE.internal_dir)
            return 0;
        if (internal_dir == RIGHT.internal_dir)
            return 1;
        if (internal_dir == LEFT.internal_dir)
            return 2;
        if (internal_dir == DOWN.internal_dir)
            return 3;
        if (internal_dir == UP.internal_dir)
            return 4;
        return -1;
    }

    static const Direction RIGHT;
    static const Direction LEFT;
    static const Direction UP;
    static const Direction DOWN;
    static const Direction NONE;

    Direction& operator=(const Vector2I& in_dir)
    {
        internal_dir = normalize(in_dir);
        return *this;
    }

    Direction& operator=(const Vector2D& in_dir)
    {
        internal_dir = normalize(in_dir.rounded().cast<Vector2I>());
        return *this;
    }

    const Vector2I& operator*() const { return internal_dir; }
    const Vector2I* operator->() const { return &internal_dir; }


    [[nodiscard]] bool is_none() const { return internal_dir.x() == 0 && internal_dir.y() == 0; }
    [[nodiscard]] bool is_right() const { return internal_dir.x() == 1; }
    [[nodiscard]] bool is_left() const { return internal_dir.x() == -1; }
    [[nodiscard]] bool is_up() const { return internal_dir.y() == -1; }
    [[nodiscard]] bool is_down() const { return internal_dir.y() == 1; }

    [[nodiscard]] bool is_horizontal() const { return is_right() || is_left(); }
    [[nodiscard]] bool is_vertical() const { return is_up() || is_down(); }


    [[nodiscard]] Direction opposite() const;

    bool operator==(const Direction& other) const { return internal_dir == other.internal_dir; }

private:
    static Vector2I normalize(const Vector2I& input)
    {
        if (input.l1_length() == 0)
            return input;
        return std::abs(input.x()) > std::abs(input.y()) ? Vector2I{input.x() > 0 ? 1 : -1, 0} : Vector2I{0, input.y() > 0 ? 1 : -1};
    }

    Vector2I internal_dir;
};
}
