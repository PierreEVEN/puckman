#pragma once
#include "entity.hpp"

namespace pm
{
class Character : public Entity
{
public:
    Character(const std::shared_ptr<Terrain>& terrain)
        : Entity(terrain)
    {
    }

    void draw() override;

    void set_position(const double x, const double y) override;

    void set_look_direction(const EDirection new_direction) override;

private:

    double lin_pos_x;
    double lin_pos_y;
    EDirection current_direction = EDirection::Idle;
};
}
