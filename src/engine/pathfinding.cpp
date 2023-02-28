#include "pathfinding.hpp"

#include "logger.hpp"
#include "terrain.hpp"

#include <unordered_set>

namespace pm
{
PathFinder::PathFinder(const std::shared_ptr<Terrain>& in_terrain)
{
    if (!in_terrain)
        FATAL("invalid terrain");
}


bool PathFinder::find_path(const SDL_Point& from, SDL_Point to)
{
    to = terrain->closest_free_point(to);

    // Using same path than last search
    if (!actual_path.empty() && from == actual_path.front() && to == actual_path.back())
        return true;

    // Rebuilding new path
    actual_path.clear();

    // Ensure source is not in a wall
    if (!terrain->is_free(from))
        return false;

    // Case source = target
    if (from == to)
    {
        actual_path = {to};
        return true;
    }

    const uint32_t width  = terrain->get_width();
    const uint32_t height = terrain->get_height();

    std::vector distance_map(width * height, std::make_pair(UINT32_MAX, to));

    std::unordered_set<SDL_Point>           study_points = {from};
    const std::vector<SDL_Point> neighbors    = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    distance_map[from.x + from.y * width] = std::make_pair(0, from);

    do
    {
        const auto old_sp = study_points;
        study_points.clear();
        for (const auto& study_point : old_sp)
        {
            for (const auto& n : neighbors)
            {
                const auto tested_point = study_point + n;
                if (tested_point == to)
                {
                    // @TODO : Find path to root;
                    return true;
                }
                if (terrain->is_free(tested_point))
                {
                    const auto index = tested_point.x + tested_point.y * width;
                    const auto min   = std::min(distance_map[index].first, distance_map[study_point.x + study_point.y * width].first);
                    if (distance_map[index].first > min)
                    {
                        distance_map[index] = std::make_pair(min, study_point);
                        study_points.insert(tested_point);
                    }
                }
            }
        }
    } while (!study_points.empty());

    return false;
}

SDL_Point PathFinder::direction_to_next_point(const SDL_Point& current_location)
{
    if (actual_path.empty())
        return {0, 0};

    if (current_location == actual_path.back())
        actual_path.pop_back();
    
    if (actual_path.empty())
        return {0, 0};

    return normalize(actual_path.back() - current_location);
}
}
