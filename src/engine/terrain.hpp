#pragma once
#include "cell.hpp"
#include <filesystem>

struct SDL_Surface;

namespace pm
{
class Terrain
{
public:
    Terrain();
    ~Terrain();

    void load_from_file(const std::filesystem::path& path);

    [[nodiscard]] Cell& get_cell(const uint32_t x, const uint32_t y);

    void draw();

    [[nodiscard]] bool is_free(const int32_t x, const int32_t y, bool is_door_free=false) const
    {
        if (x < 0 || y < 0 || static_cast<uint32_t>(x) >= width || static_cast<uint32_t>(y) >= height)
            return false;

        const auto cell_type = grid[x + y * width].get_type();
        return cell_type != ECellType::Wall && (is_door_free || cell_type != ECellType::Door);
    }

    [[nodiscard]] int eat(const int32_t x, const int32_t y);

private:
    void update_position_and_walls();
    void update_sprite_handles();
    void create_wall_cache_surface();
    void free_wall_cache_surface();

private:
    uint32_t          width;
    uint32_t          height;
    std::vector<Cell> grid;
    SDL_Surface* wall_cache_surface_handle = nullptr;
};
}
