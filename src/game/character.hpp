#pragma once
#include "game/entity.hpp"

namespace pm
{
/*
 * The character class handles movements with velocity inside terrain
 */
class Character : public Entity
{
public:
    Character(const std::shared_ptr<Terrain>& terrain)
        : Entity(terrain)
    {
    }

    void tick() override;

    // Set the desired look direction.
    // If desired direction is free, the entity will look at the required direction, then move
    // until it reaches the center of the intersection before continuing in the desired direction
    void set_look_direction(const Direction new_direction) override;

    // Change the character velocity (in pixel / second)
    void set_velocity(double new_velocity) { velocity = new_velocity; }
protected:
    // is this character allowed to pass through doors
    bool go_through_doors = false;
private:
    double velocity = 75.0;
    Direction current_direction = Direction::NONE;
};
}
