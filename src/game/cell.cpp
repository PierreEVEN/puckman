#include "cell.hpp"

#include "engine/logger.hpp"
#include "engine/vector2.hpp"

namespace pm
{
double Cell::draw_scale = 1.;

void Cell::set_pos(const Vector2I& in_pos)
{
    pos = in_pos;
}

void Cell::set_item(EItemType in_item_type)
{
    type      = ECellType::Item;
    item_type = in_item_type;
}

void Cell::set_wall(WallMask in_wall_mask, WallMask in_wall_mask_neg)
{
    type       = ECellType::Wall;
    wall_masks = {in_wall_mask, in_wall_mask_neg < 0 ? wall_masks.neg : in_wall_mask_neg};
}

void Cell::set_gum(bool big)
{
    type = big ? ECellType::BiGum : ECellType::Gum;
}

void Cell::set_door()
{
    type = ECellType::Door;
}

ECellType Cell::get_type() const
{
    return type;
}

EItemType Cell::get_item() const
{
    if (type != ECellType::Item)
        FATAL("this cell is not an item");
    return item_type;
}

void Cell::update_type(const ECellType new_type)
{
    type = new_type;
}

void Cell::update_sprite_handle(SpriteHandle new_sprite_handle)
{
    sprite_handle = new_sprite_handle;
}

void Cell::update_sprite_handle(
    const std::unordered_map<ECellType, SpriteHandle>& cell_sprite_handles,
    const std::unordered_map<EItemType, SpriteHandle>& item_sprite_handles,
    const std::array<SpriteHandle, 16>&                wall_sprite_handles)
{
    switch (type)
    {
    case ECellType::Wall:
        sprite_handle = wall_sprite_handles[wall_masks.pos & ~wall_masks.neg];
        break;
    case ECellType::Item:
        sprite_handle = item_sprite_handles.find(item_type)->second;
        break;
    default:
        sprite_handle = cell_sprite_handles.find(type)->second;
        break;
    }
}

void Cell::draw(int32_t terrain_unit_scale, SDL_Surface* surface_override) const
{
    if (sprite_handle)
    {
        Vector2I draw_pos{pos};
        draw_pos *= terrain_unit_scale * static_cast<int32_t>(draw_scale);
        const double ds = draw_scale * (1 + (type == ECellType::Wall));
        sprite_handle.draw(draw_pos, ds, ds, surface_override);
    }
}

Cell Cell::from_char(char chr)
{
    Cell cell;
    switch (chr)
    {
    case '.':
        return cell;
    case '#':
        cell.set_wall(WALL_MASK_FULL, 0);
        break;
    case '^':
        cell.set_wall(WALL_MASK_FULL, WALL_MASK_SOUTH);
        break;
    case '<':
        cell.set_wall(WALL_MASK_FULL, WALL_MASK_EAST);
        break;
    case '>':
        cell.set_wall(WALL_MASK_FULL, WALL_MASK_WEST);
        break;
    case 'v':
        cell.set_wall(WALL_MASK_FULL, WALL_MASK_NORTH);
        break;
    case 'o':
        cell.set_gum(false);
        break;
    case 'G':
        cell.set_gum(true);
        break;
    case '-':
        cell.set_door();
        break;
    default:
        cell.set_item(EItemType::Abricot);
    }
    return cell;
}
}
