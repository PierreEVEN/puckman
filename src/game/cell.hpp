#pragma once
#include "engine/sprite_sheet.hpp"
#include "engine/vector2.hpp"

#include <array>
#include <unordered_map>

namespace pm
{
/*
 * This is the representation of a cell of pacman game's terrain
 */

enum class ECellType
{
    Void,
    Wall,
    Gum,
    Item,
    BiGum,
    Door,
};

// Specialized item type informations
enum class EItemType
{
    Cherry,
    Strawberry,
    Abricot,
    Apple,
    Grapes,
    Galaxian,
    Bell,
    Key
};

class Cell
{
public:
    static double draw_scale;

    using WallMask = int32_t;
    static constexpr WallMask WALL_MASK_NORTH = 0b0001;
    static constexpr WallMask WALL_MASK_WEST  = 0b0010;
    static constexpr WallMask WALL_MASK_EAST  = 0b0100;
    static constexpr WallMask WALL_MASK_SOUTH = 0b1000;
    static constexpr WallMask WALL_MASK_FULL  = 0b1111;

public:

    // Create a cell based on the character that represents it
    [[nodiscard]] static Cell from_char(char chr);

    Cell() = default;

    void set_pos(const Vector2I& in_pos);

    // Set the cell to be of a certain type, with extra relevant information
    void set_item(EItemType in_item_type);
    void set_wall(WallMask in_wall_mask, WallMask in_wall_mask_neg = -1);
    void set_gum(bool big);
    void set_door();

    [[nodiscard]] ECellType get_type() const;
    [[nodiscard]] EItemType get_item() const;
    void                    update_type(const ECellType new_type);

    // Update the cell's sprite handle to be the provided one
    // (called update instead of set for consistency with the other overload)
    void update_sprite_handle(SpriteHandle new_sprite_handle);

    // Update the cell's sprite handle based on its nature
    // from the provided data structures
    void update_sprite_handle(
        const std::unordered_map<ECellType, SpriteHandle>& cell_sprite_handles,
        const std::unordered_map<EItemType, SpriteHandle>& item_sprite_handles,
        const std::array<SpriteHandle, 16>&                wall_sprite_handles);

    // Draw the cell to the default or provided surface
    void draw(int32_t terrain_unit_scale, SDL_Surface* surface_override = nullptr) const;

private:
    SpriteHandle sprite_handle = {};

    Vector2I pos{0, 0};

    ECellType type = ECellType::Void;

    struct DualWallMask
    {
        WallMask pos;
        WallMask neg;
    };

    EItemType        item_type = EItemType::Cherry;
    DualWallMask     wall_masks;
};
}
