#pragma once
#include "cell.hpp"
#include "engine/vector2.hpp"

#include <filesystem>

struct SDL_Surface;

namespace pm
{
class Terrain
{
public:
    Terrain() = default;
    ~Terrain();

    void load_from_file(const std::filesystem::path& path);

    [[nodiscard]] Cell& get_cell(const Vector2I& pos);

    [[nodiscard]] int32_t get_unit_length() const
    {
        return unit_length;
    }

    [[nodiscard]] EItemType    get_level_item(int level) const;
    [[nodiscard]] SpriteHandle get_item_sprite_handle(EItemType item) const;

    void tick(double delta_time);
    void draw();

    [[nodiscard]] bool is_free(const Vector2I& pos, bool is_door_free = false) const
    {
        if (pos.x() < 0 || pos.y() < 0 || static_cast<uint32_t>(pos.x()) >= width || static_cast<uint32_t>(pos.y()) >= height)
            return false;

        const auto cell_type = grid[pos.x() + pos.y() * width].get_type();
        return cell_type != ECellType::Wall && (is_door_free || cell_type != ECellType::Door);
    }

    [[nodiscard]] bool is_tunnel(const Vector2I& pos) const
    {
        return (static_cast<uint32_t>(pos.x() + 2) + width) % (width + 2) >= width;
    }

    void eat(const Vector2I& pos);

    [[nodiscard]] uint32_t get_width() const { return width; }
    [[nodiscard]] uint32_t get_height() const { return height; }

    [[nodiscard]] Vector2I closest_free_point(const Vector2I& location) const;

    void set_wall_color(const Uint8 r, const Uint8 g, const Uint8 b) const;

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
