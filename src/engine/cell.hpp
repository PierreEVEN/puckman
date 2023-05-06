#pragma once
#include "sprite_sheet.hpp"
#include "vector2.hpp"

#include <array>
#include <unordered_map>

namespace pm {

using WallMask = int;

const WallMask WALL_MASK_NORTH = 0b0001;
const WallMask WALL_MASK_WEST  = 0b0010;
const WallMask WALL_MASK_EAST  = 0b0100;
const WallMask WALL_MASK_SOUTH = 0b1000;

const WallMask WALL_MASK_FULL  = 0b1111;

enum class EItemType
{
    Cherry,
    Strawberry,
    Abricot,
    Apple,
    Wtfruit,
    Axe,
    Bell,
    Key
};

enum class ECellType
{
    Void,
    Wall,
    Gum,
    Item,
    BiGum,
    Door,
};

class Cell
{
public:
    static double draw_scale;

public:

    static Cell from_char(char chr);

    Cell();
    
    void set_pos(const Vector2I& in_pos);
    void set_item(EItemType in_item_type);
    void set_wall(WallMask in_wall_mask, WallMask in_wall_mask_neg=-1);
    void set_gum(bool big);
    void set_door();

    [[nodiscard]] ECellType get_type() const;
    void update_type(const ECellType new_type);

    void update_sprite_handle(
        std::unordered_map<ECellType, SpriteHandle> map_cell_type,
        std::unordered_map<EItemType, SpriteHandle> map_item_type,
        std::array<SpriteHandle, 16>& walls);
    void draw(int32_t terrain_unit_scale, SDL_Surface* surface_override=nullptr) const;

private:

    SpriteHandle sprite_handle;

    Vector2I pos{0, 0};

    ECellType type = ECellType::Void;

    union
    {
        EItemType item_type;
        struct {
            WallMask pos;
            WallMask neg;
        } wall_masks;
    };
};
}
