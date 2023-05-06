#pragma once
#include "direction.hpp"
#include "sprite_sheet.hpp"
#include "terrain.hpp"
#include "vector2.hpp"

#include <array>

namespace pm
{
class Terrain;

class Entity
{
public:
    Entity(std::shared_ptr<Terrain> terrain);

    void set_direction_sprite(const Direction direction, const SpriteHandle& new_sprite);

    virtual void set_look_direction(const Direction new_direction);

    [[nodiscard]] virtual Direction get_look_direction() const
    {
        return looking_direction;
    }

    virtual void tick()
    {
    }

    virtual void draw();

    void set_absolute_linear_position(const Vector2D& new_pos) { linear_pos = new_pos; }
    void set_absolute_discrete_position(const Vector2I& new_pos) { linear_pos = new_pos.cast<Vector2D>(); }
    void set_cell_discrete_pos(const Vector2I& new_pos) { linear_pos = new_pos.cast<Vector2D>() * terrain->get_unit_length(); }
    void set_cell_linear_pos(const Vector2D& new_pos) { linear_pos = new_pos * terrain->get_unit_length(); }

    [[nodiscard]] Vector2I        get_absolute_discrete_pos() const { return {static_cast<int32_t>(linear_pos.x()), static_cast<int32_t>(linear_pos.y())}; }
    [[nodiscard]] const Vector2D& get_absolute_linear_pos() const { return linear_pos; }
    [[nodiscard]] Vector2I        get_cell_discrete_pos() const { return (linear_pos / terrain->get_unit_length()).rounded().cast<Vector2I>(); }
    [[nodiscard]] Vector2D        get_cell_linear_pos() const { return linear_pos / terrain->get_unit_length(); }


    [[nodiscard]] Terrain& get_terrain() const { return *terrain; }

    void pause_animation(bool paused);

    [[nodiscard]] bool is_at_intersection() const;

    void hide(bool in_hide) { hidden = in_hide; }

    virtual void reset() {}

  protected:
    bool hidden = false;

  private:
    Vector2D linear_pos;

    std::array<std::optional<SpriteHandle>, 5> direction_sprite;
    Direction                                  looking_direction;
    std::shared_ptr<Terrain>                   terrain;
};
}
