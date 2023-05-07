#pragma once
#include "character.hpp"
#include "engine/vector2.hpp"

namespace pm
{
class PathFinder;

enum class AiMode
{
    // Just spawned, in this state until the first tick
    Spawned,
    // Should exit spawn (allowed to go through doors)
    ExitSpawn,
    // Chase player
    Chase,
    // Go to corresponding corner point
    Scatter,
    // Randomly move (and may be eaten by player)
    Frightened,
    // Move back to home (allowed to go through doors)
    GoHome,
};

/**
 * This is the base class to handle Ghosts behaviors
 */
class GhostBase : public Character
{
public:
    explicit GhostBase(const std::shared_ptr<Terrain>& terrain, std::shared_ptr<Character> in_target);
    virtual  ~GhostBase();

    // Implement in children classes to specialize behavior for each different ghost.
    [[nodiscard]] virtual Vector2I target_player() const = 0;
    [[nodiscard]] virtual Vector2I scatter_target() const = 0;
    [[nodiscard]] virtual Vector2I home_location() const = 0;
    [[nodiscard]] virtual double   compute_speed_percent() const;

    void tick() override;
    void draw() override;

    void reset() override;

protected:
    // Call when mode is changed or we reached a new cell (maybe an intersection)
    void on_search_new_dir();

    // simulate overflow error from original pacman game
    static Vector2I dir_with_overflow_error(const Direction& in_dir)
    {
        return in_dir.is_up() ? Vector2I{-1, -1} : *in_dir;
    }

    // Player
    std::shared_ptr<Character> target;

    // Current behavior
    AiMode                     mode;

    // Last cell we were, used to call on_search_new_dir once per cell.
    Vector2I                   last_cell = {};

private:
    // Bound to Gamemode events
    void         on_frightened();
    void         on_scatter();
    void         on_chase();

    // Sprites that replace defaults one in some situations
    SpriteHandle frightened_sprite;
    SpriteHandle frightened_sprite_flash;
    SpriteHandle eyes_down_sprite;
    SpriteHandle eyes_up_sprite;
    SpriteHandle eyes_right_sprite;
    SpriteHandle eyes_left_sprite;
};
}
