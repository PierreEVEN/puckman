#pragma once
#include "direction.hpp"
#include "vector2.hpp"

#include <memory>
#include <vector>

namespace pm
{
class Terrain;

class PathFinder
{
public:
    PathFinder(const std::shared_ptr<Terrain>& in_terrain);

    bool find_path(const Vector2I& from, Vector2I to);
    Direction direction_to_next_point(const Vector2I& current_location);

private:

    std::vector<Vector2I> actual_path;

    std::shared_ptr<Terrain> terrain;
};
}