#pragma once
#include "cell.hpp"
#include "engine/vector2.hpp"

#include <filesystem>

struct SDL_Surface;

namespace pm
{
/*
 * Representation of the level being played.
 *
 * This is the central place of authority for the state of the map,
 * including collectables: both pellet types, and items a.k.a "fruits".
 * It handles every interaction with it, including checking for tunnels.
 */

class Terrain
{
public:
    Terrain() = default;
    ~Terrain();

    void load_from_file(const std::filesystem::path& path);

    [[nodiscard]] Cell& get_cell(const Vector2I& pos);

    // Get the pixel length of one cell
    [[nodiscard]] int32_t get_unit_length() const
    {
        return unit_length;
    }

    [[nodiscard]] EItemType    get_level_item(int level) const;
    [[nodiscard]] SpriteHandle get_item_sprite_handle(EItemType item) const;

    void tick(double delta_time);
    void draw();

    // Determine if a cell is free to pass through and queryable
    [[nodiscard]] bool is_free(const Vector2I& pos, bool is_door_free = false) const
    {
        if (pos.x() < 0 || pos.y() < 0 || static_cast<uint32_t>(pos.x()) >= width || static_cast<uint32_t>(pos.y()) >= height)
            return false;

        const auto cell_type = grid[pos.x() + pos.y() * width].get_type();
        return cell_type != ECellType::Wall && (is_door_free || cell_type != ECellType::Door);
    }

    // Determine if a cell is a tunnel (no such cell stored, but accessible)
    [[nodiscard]] bool is_tunnel(const Vector2I& pos) const
    {
        // Two cells on either horizontal side of the terrain are considered accessible
        // to allow entities to be teleported to the other side of the level
        return (static_cast<uint32_t>(pos.x() + 2) + width) % (width + 2) >= width;
    }

    // Attempt to eat a collectable that might be at these coordinates
    void eat(const Vector2I& pos);

    // Remove the item a.k.a "fruit" if it's present
    void clear_fruit();

    [[nodiscard]] uint32_t get_width() const { return width; }
    [[nodiscard]] uint32_t get_height() const { return height; }

    [[nodiscard]] Vector2I closest_free_point(const Vector2I& location) const;

    // Set the color of all walls by setting the wall cache tint
    void set_wall_color(const Uint8 r, const Uint8 g, const Uint8 b) const;

    // Recover the terrain to the state it was in after being loaded
    void reset();

private:
    void update_position_and_walls();
    void update_sprite_handles();
    void create_wall_cache_surface();
    void free_wall_cache_surface();

    static const std::array<EItemType, 13>          level_items;
    static const std::unordered_map<EItemType, int> item_values;

    std::unordered_map<EItemType, SpriteHandle> item_sprite_handles;
    int32_t                                     unit_length               = 16;
    uint32_t                                    width                     = 0;
    uint32_t                                    height                    = 0;
    std::vector<Cell>                           grid                      = {};
    std::vector<Cell>                           initial_grid              = {};
    SDL_Surface*                                wall_cache_surface_handle = nullptr;
    int32_t                                     gum_count                 = 0;
    int32_t                                     initial_gum_count         = 0;
    double                                      item_timer                = 0;
};
}
