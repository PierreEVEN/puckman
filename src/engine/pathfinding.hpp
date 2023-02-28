#pragma once
#include <memory>
#include <SDL_rect.h>
#include <vector>

#include "types.hpp"

namespace pm
{
class Terrain;

class PathFinder
{
public:
    PathFinder(const std::shared_ptr<Terrain>& in_terrain);

    bool find_path(const SDL_Point& from, SDL_Point to);
    SDL_Point direction_to_next_point(const SDL_Point& current_location);

private:

    std::vector<SDL_Point> actual_path;

    std::shared_ptr<Terrain> terrain;
};
}