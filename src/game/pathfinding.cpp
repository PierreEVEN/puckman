#include "pathfinding.hpp"

#include "engine/logger.hpp"
#include "terrain.hpp"

#include <unordered_set>

namespace pm
{
PathFinder::PathFinder(const std::shared_ptr<Terrain>& in_terrain)
    : terrain(in_terrain)
{
    if (!in_terrain)
        FATAL("invalid terrain");
}

bool PathFinder::find_path(const Vector2I& from, Vector2I to)
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

    std::unordered_set study_points = {from};
    const std::vector<Vector2I>  neighbors    = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    distance_map[from.x() + from.y() * width] = std::make_pair(0, from);

    const auto index_of    = [width](const Vector2I&         pt) { return pt.x() + pt.y() * width; };
    const auto point_valid = [width, height](const Vector2I& pt) { return pt.x() >= 0 && pt.y() >= 0 && pt.x() < static_cast<int>(width) && pt.y() < static_cast<int>(height); };

    // A* body
    do
    {
        const auto old_sp = study_points;
        study_points.clear();
        for (const auto& study_point : old_sp)
        {
            for (const auto& n : neighbors)
            {
                const auto tested_neighbor = study_point + n;
                if (tested_neighbor == to)
                {
                    distance_map[index_of(to)] = std::make_pair(distance_map[index_of(study_point)].first + 1, study_point);
                    // Find path to root;
                    Vector2I current_rewind_pos = to;

                    while (current_rewind_pos != from)
                    {
                        actual_path.emplace_back(current_rewind_pos);

                        Vector2I min_dir     = {};
                        uint32_t   min_dir_val = distance_map[index_of(current_rewind_pos)].first;
                        for (const auto neighbor : neighbors)
                        {
                            const auto rev_test_point = current_rewind_pos + neighbor;
                            if (!point_valid(rev_test_point))
                                continue;

                            if (distance_map[index_of(rev_test_point)].first < min_dir_val)
                            {
                                min_dir_val = distance_map[index_of(rev_test_point)].first;
                                min_dir     = distance_map[index_of(rev_test_point)].second;
                            }
                        }
                        if (min_dir == Vector2I{})
                        {
                            ERROR("failed to rewind pathfinding pos");
                            actual_path.clear();
                            return false;
                        }
                        current_rewind_pos = min_dir;
                    }
                    actual_path.emplace_back(from);
                    return true;
                }
                if (terrain->is_free(tested_neighbor))
                {
                    const auto min = std::min(distance_map[index_of(tested_neighbor)].first, distance_map[index_of(study_point)].first + 1);
                    if (distance_map[index_of(tested_neighbor)].first > min)
                    {
                        distance_map[index_of(tested_neighbor)] = std::make_pair(min, study_point);
                        study_points.insert(tested_neighbor);
                    }
                }
            }
        }
    } while (!study_points.empty());

    return false;
}

Direction PathFinder::direction_to_next_point(const Vector2I& current_location)
{
    if (actual_path.empty())
        return Direction::NONE;

    if (current_location == actual_path.back())
        actual_path.pop_back();
    
    if (actual_path.empty())
        return Direction::NONE;
    
    return {actual_path.back() - current_location};
}
}
