#pragma once
#include "sprite_sheet.hpp"

#include <array>
#include "types.hpp"

namespace pm
{
class Terrain;

class Entity
{
public:
    Entity(const std::shared_ptr<Terrain>& terrain);

    void set_direction_sprite(const EDirection direction, const SpriteHandle& new_sprite);

    [[nodiscard]] double get_pos_x() const { return pos_x; }
    [[nodiscard]] double get_pos_y() const { return pos_y; }

    virtual void set_look_direction(const EDirection new_direction);
    [[nodiscard]] virtual EDirection get_look_direction() const
    {
        return looking_direction;
    }

    virtual void tick(){}

    virtual void draw();

    virtual void set_position(const double x, const double y)
    {
        pos_x = x, pos_y = y;
    }

    [[nodiscard]] Terrain& get_terrain() const { return *terrain; }

    void pause_animation(bool paused);

private:
    double pos_x;
    double pos_y;

    std::array<std::optional<SpriteHandle>, static_cast<size_t>(EDirection::MAX) + 1> direction_sprite;
    EDirection                                                                        looking_direction;
    std::shared_ptr<Terrain>                                                          terrain;
};
}
