#pragma once
#include "character.hpp"
#include "engine/vector2.hpp"

namespace pm
{
class PathFinder;

enum class AiMode
{
    Spawned,
    ExitSpawn,
    Chase,
    Scatter,
    Frightened,
    GoHome,
};

class GhostBase : public Character
{
public:
    explicit GhostBase(const std::shared_ptr<Terrain>& terrain, std::shared_ptr<Character> in_target);
    virtual ~GhostBase();

    [[nodiscard]] virtual Vector2I target_player() const = 0;
    [[nodiscard]] virtual Vector2I scatter_target() const = 0;
    [[nodiscard]] virtual Vector2I home_location() const = 0;
    [[nodiscard]] virtual double compute_speed_percent() const;

    void tick() override;
    void draw() override;

    void reset() override;

protected:
    void on_search_new_dir();

    // simulate overflow error from original pacman game
    static Vector2I dir_with_overflow_error(const Direction& in_dir)
    {
        return in_dir.is_up() ? Vector2I{-1, -1} : *in_dir;
    }

    const std::shared_ptr<PathFinder> pathfinder;
    std::shared_ptr<Character>        target;
    AiMode                            mode;
    Vector2I                          last_cell = {};

private:
    void on_frightened();
    void on_scatter();
    void on_chase();
    SpriteHandle frightened_sprite;
    SpriteHandle frightened_sprite_flash;
    SpriteHandle eyes_down_sprite;
    SpriteHandle eyes_up_sprite;
    SpriteHandle eyes_right_sprite;
    SpriteHandle eyes_left_sprite;
};

class Blinky : public GhostBase
{
public:
    Blinky(const std::shared_ptr<Terrain>& terrain, std::shared_ptr<Character> in_target);

    [[nodiscard]] Vector2I target_player() const override;
    [[nodiscard]] Vector2I scatter_target() const override;
    [[nodiscard]] Vector2I home_location() const override;

private:
};


class Pinky : public GhostBase
{
public:
    Pinky(const std::shared_ptr<Terrain>& terrain, std::shared_ptr<Character> in_target);

    [[nodiscard]] Vector2I target_player() const override;
    [[nodiscard]] Vector2I scatter_target() const override;
    [[nodiscard]] Vector2I home_location() const override;

private:
};



class Inky : public GhostBase
{
public:
    Inky(const std::shared_ptr<Terrain>& terrain, std::shared_ptr<Character> in_target, std::shared_ptr<Character> blinky_ref);

    [[nodiscard]] Vector2I target_player() const override;
    [[nodiscard]] Vector2I scatter_target() const override;
    [[nodiscard]] Vector2I home_location() const override;

private:
    std::shared_ptr<Character> blinky;
};



class Clyde : public GhostBase
{
public:
    Clyde(const std::shared_ptr<Terrain>& terrain, std::shared_ptr<Character> in_target);

    [[nodiscard]] Vector2I target_player() const override;
    [[nodiscard]] Vector2I scatter_target() const override;
    [[nodiscard]] Vector2I home_location() const override;

private:
};
}
