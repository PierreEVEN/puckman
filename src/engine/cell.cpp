#include "cell.hpp"

#include "logger.hpp"
#include "types.hpp"

namespace pm
{

double Cell::draw_scale = 1.;

Cell::Cell() : type(ECellType::Void)
{}

void Cell::set_pos(const SDL_Point& in_pos)
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
    type      = ECellType::Wall;
    wall_masks = { in_wall_mask, in_wall_mask_neg < 0 ? wall_masks.neg : in_wall_mask_neg };
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

void Cell::update_type(const ECellType new_type)
{
    type = new_type;
}

void Cell::update_sprite_handle(
        std::unordered_map<ECellType, SpriteHandle> map_cell_type,
        std::unordered_map<EItemType, SpriteHandle> map_item_type,
        std::array<SpriteHandle, 16>& walls)
{
    switch (type)
    {
    case ECellType::Wall:
        sprite_handle = walls[wall_masks.pos & ~wall_masks.neg];
        break;
    case ECellType::Item:
        sprite_handle = map_item_type[item_type];
        break;
    default:
        sprite_handle = map_cell_type[type];
        break;
    }
}

void Cell::draw() const
{
    if (sprite_handle){
        SDL_Point draw_pos{pos};
        draw_pos *= 16. * draw_scale;
        const double ds = draw_scale * (1 + (type == ECellType::Wall));
        sprite_handle.draw(draw_pos, ds, ds);
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
