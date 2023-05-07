#pragma once

#include <cstdint>
#include <cmath>
#include <iosfwd>

namespace pm
{
// Template class to define a 2 dimension vector of any numeric type.
template <typename T>
class Vector2
{
public:
    Vector2() = default;

    Vector2(T a)
        : pos_x(a), pos_y(a)
    {
    }

    Vector2(T x, T y)
        : pos_x(x), pos_y(y)
    {
    }

    // Accessors
    T& x() { return pos_x; }
    T& y() { return pos_y; }

    [[nodiscard]] const T& x() const { return pos_x; }
    [[nodiscard]] const T& y() const { return pos_y; }

    // Operators
    Vector2 operator+(const Vector2& other) const { return {pos_x + other.pos_x, pos_y + other.pos_y}; }
    Vector2 operator-(const Vector2& other) const { return {pos_x - other.pos_x, pos_y - other.pos_y}; }
    Vector2 operator*(const Vector2& other) const { return {pos_x * other.pos_x, pos_y * other.pos_y}; }
    Vector2 operator/(const Vector2& other) const { return {pos_x / other.pos_x, pos_y / other.pos_y}; }
    Vector2 operator%(const Vector2& other) const { return {pos_x % other.pos_x, pos_y % other.pos_y}; }

    Vector2 operator+(const T& other) const { return {pos_x + other, pos_y + other}; }
    Vector2 operator-(const T& other) const { return {pos_x - other, pos_y - other}; }
    Vector2 operator*(const T& other) const { return {pos_x * other, pos_y * other}; }
    Vector2 operator/(const T& other) const { return {pos_x / other, pos_y / other}; }
    Vector2 operator%(const T& other) const { return {pos_x % other, pos_y % other}; }

    Vector2& operator+=(const Vector2& other) { pos_x += other.pos_x; pos_y += other.pos_y; return *this; }
    Vector2& operator-=(const Vector2& other) { pos_x -= other.pos_x; pos_y -= other.pos_y; return *this; }
    Vector2& operator*=(const Vector2& other) { pos_x *= other.pos_x; pos_y *= other.pos_y; return *this; }
    Vector2& operator/=(const Vector2& other) { pos_x /= other.pos_x; pos_y /= other.pos_y; return *this; }
    Vector2& operator%=(const Vector2& other) { pos_x %= other.pos_x; pos_y %= other.pos_y; return *this; }

    Vector2& operator+=(const T& other) { pos_x += other; pos_y += other; return *this; }
    Vector2& operator-=(const T& other) { pos_x -= other; pos_y -= other; return *this; }
    Vector2& operator*=(const T& other) { pos_x *= other; pos_y *= other; return *this; }
    Vector2& operator/=(const T& other) { pos_x /= other; pos_y /= other; return *this; }
    Vector2& operator%=(const T& other) { pos_x %= other; pos_y %= other; return *this; }

    bool operator==(const Vector2& other) const { return pos_x == other.pos_x && pos_y == other.pos_y; }

    // Utilities
    [[nodiscard]] T       length() const { return static_cast<T>(std::sqrt(static_cast<double>(pos_x * pos_x + pos_y * pos_y))); }
    [[nodiscard]] T       l1_length() const { return std::max(std::abs(pos_x), std::abs(pos_y)); }
    [[nodiscard]] Vector2 normalized() const { return *this / length(); }
    [[nodiscard]] T       dot(const Vector2& other) const { return pos_x * other.pos_x + pos_y * other.pos_y; }
    [[nodiscard]] Vector2 rounded() const { return {static_cast<T>(std::round(pos_x)), static_cast<T>(std::round(pos_y))}; }

    using InternalType = T;

    // Cast this vector2 into another kind vector2
    template <typename U>
    [[nodiscard]] Vector2<typename U::InternalType> cast() const
    {
        return Vector2<typename U::InternalType>(static_cast<typename U::InternalType>(pos_x), static_cast<typename U::InternalType>(pos_y));
    }

private:
    T pos_x = T(0), pos_y = T(0);
};

// Common vector2 usages
using Vector2D = Vector2<double>;
using Vector2I = Vector2<int32_t>;

// Used for printing and debug purpose
template<typename T>
std::stringstream& operator<<(std::stringstream& stream, const Vector2<T>& s)
{
    stream << s.x() << ", " << s.y();
    return stream;
}

}

// Hash operators
namespace std
{
template <> struct hash<pm::Vector2D>
{
    std::size_t operator()(const pm::Vector2D& p) const noexcept
    {
        return std::hash<double>()(p.x()) + std::hash<double>()(p.y()) * 2537;
    }
};

template <> struct hash<pm::Vector2I>
{
    std::size_t operator()(const pm::Vector2I& p) const noexcept
    {
        return std::hash<int>()(p.x()) + std::hash<int>()(p.y()) * 2537;
    }
};
}
