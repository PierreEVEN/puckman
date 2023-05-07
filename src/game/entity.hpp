#pragma once
#include "engine/direction.hpp"
#include "engine/sprite_sheet.hpp"
#include "terrain.hpp"
#include "engine/vector2.hpp"

#include <array>

namespace pm
{
class Terrain;

/*
 * Main class.
 * Extend this to implement an object that will move on a given terrain.
 * An entity have a position and a look direction and will draw the corresponding sprite.
 */
class Entity
{
public:
    Entity(std::shared_ptr<Terrain> terrain);
    virtual ~Entity() = default;

    // Set the sprite used for the given direction. All the 5 directions (including NONE) should be initialized.
    void set_direction_sprite(const Direction direction, const SpriteHandle& new_sprite);

    virtual void draw();

    virtual void tick()
    {
    }

    /*
     * There are different kind of coordinates :
     * - Absolute mean a position in pixel on screen (1 unit per pixel)
     * - Cell mean a position in grid space (1 unit per cell)
     * - Discrete give the corresponding rounded position to closest integer value
     * - Linear give the raw position (or eventually in cell space)
     */

    void set_absolute_linear_position(const Vector2D& new_pos) { linear_pos = new_pos; }
    void set_absolute_discrete_position(const Vector2I& new_pos) { linear_pos = new_pos.cast<Vector2D>(); }
    void set_cell_discrete_pos(const Vector2I& new_pos) { linear_pos = new_pos.cast<Vector2D>() * terrain->get_unit_length(); }
    void set_cell_linear_pos(const Vector2D& new_pos) { linear_pos = new_pos * terrain->get_unit_length(); }

    [[nodiscard]] Vector2I        get_absolute_discrete_pos() const { return {static_cast<int32_t>(linear_pos.x()), static_cast<int32_t>(linear_pos.y())}; }
    [[nodiscard]] const Vector2D& get_absolute_linear_pos() const { return linear_pos; }
    [[nodiscard]] Vector2I        get_cell_discrete_pos() const { return (linear_pos / terrain->get_unit_length()).rounded().cast<Vector2I>(); }
    [[nodiscard]] Vector2D        get_cell_linear_pos() const { return linear_pos / terrain->get_unit_length(); }

    // Set entity's direction. It is mainly used to select which sprite to render when draw is called.
    virtual void                    set_look_direction(const Direction new_direction);
    [[nodiscard]] virtual Direction get_look_direction() const { return looking_direction; }

    // Return true if one of the neighbors cells is free
    [[nodiscard]] bool     is_at_intersection() const;
    [[nodiscard]] Terrain& get_terrain() const { return *terrain; }

    // Hide entity
    void hide(bool in_hide) { hidden = in_hide; }

    // Reset entity to it's default state and position when game start
    virtual void reset()
    {
    }

    // Pause animations
    void pause_animation(bool paused);
protected:
    bool hidden = false;

private:
    std::array<std::optional<SpriteHandle>, 5> direction_sprite;
    Direction                                  looking_direction;
    std::shared_ptr<Terrain>                   terrain;

    // Entity position is internally handled using double coordinates.
    Vector2D linear_pos;
};
}
