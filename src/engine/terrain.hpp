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

    [[nodiscard]] int32_t get_unit_length() const
    {
        return unit_length;
    }

    void draw();

    [[nodiscard]] bool is_free(const SDL_Point& pos, bool is_door_free = false) const
    {
        if (pos.x < 0 || pos.y < 0 || static_cast<uint32_t>(pos.x) >= width || static_cast<uint32_t>(pos.y) >= height)
            return false;

        const auto cell_type = grid[pos.x + pos.y * width].get_type();
        return cell_type != ECellType::Wall && (is_door_free || cell_type != ECellType::Door);
    }

    [[nodiscard]] bool is_tunnel(const SDL_Point& pos) const
    {
        return (static_cast<uint32_t>(pos.x + 2) + width) % (width + 2) >= width;
    }

    [[nodiscard]] int eat(const int32_t x, const int32_t y);

    [[nodiscard]] uint32_t get_width() const { return width; }
    [[nodiscard]] uint32_t get_height() const { return height; }

    [[nodiscard]] SDL_Point closest_free_point(const SDL_Point& location) const;
    
    void set_wall_color(const Uint8 r, const Uint8 g, const Uint8 b) const;

private:
    void update_position_and_walls();
    void update_sprite_handles();
    void create_wall_cache_surface();
    void free_wall_cache_surface();

private:
    int32_t           unit_length = 16;
    uint32_t          width;
    uint32_t          height;
    std::vector<Cell> grid;
    SDL_Surface* wall_cache_surface_handle = nullptr;
};
}
