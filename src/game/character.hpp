#pragma once
#include "engine/entity.hpp"

namespace pm
{
class Character : public Entity
{
public:
    Character(const std::shared_ptr<Terrain>& terrain)
        : Entity(terrain)
    {
    }

    void tick() override;

    virtual void set_position(const double x, const double y) override;

    void set_look_direction(const EDirection new_direction) override;

    [[nodiscard]] SDL_Point get_discrete_pos() const {return SDL_Point{static_cast<int>(std::round(lin_pos_x)), static_cast<int>(std::round(lin_pos_y))};}

    void set_velocity(double new_velocity) { velocity = new_velocity; }

private:
    double velocity = 3.0;
    double lin_pos_x;
    double lin_pos_y;
    EDirection current_direction = EDirection::Idle;
};
}
