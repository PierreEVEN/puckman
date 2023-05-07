#pragma once
#include "direction.hpp"
#include "vector2.hpp"

#include <memory>
#include <vector>

namespace pm
{
class Terrain;

// Utility class used to find path on given terrain using A* algorithm (// not used anymore in current program)
class PathFinder
{
public:
    PathFinder(const std::shared_ptr<Terrain>& in_terrain);

    // Update path (only heavy if from and to have changed)
    bool find_path(const Vector2I& from, Vector2I to);

    // Advance in computed path and get direction to next path point
    // Return Direction::NONE when complete
    Direction direction_to_next_point(const Vector2I& current_location);

private:
    std::vector<Vector2I> actual_path;
    std::shared_ptr<Terrain> terrain;
};
}