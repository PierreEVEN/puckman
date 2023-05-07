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
    
    void set_look_direction(const Direction new_direction) override;    
    void set_velocity(double new_velocity) { velocity = new_velocity; }
protected:
    bool go_through_doors = false;
private:
    double velocity = 75.0;
    Direction current_direction = Direction::NONE;
};
}
