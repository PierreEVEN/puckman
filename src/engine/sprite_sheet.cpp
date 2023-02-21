#include "sprite_sheet.hpp"

#include "engine.hpp"
#include "logger.hpp"

#include <SDL_surface.h>

namespace pm
{
SpriteHandle& SpriteHandle::draw(const SDL_Point& pos, double scale_x, double scale_y)
{
    owner->render_sprite(*this, pos, scale_x, scale_y);
    return *this;
}

SpriteSheet::SpriteSheet(const std::filesystem::path& sprite_sheet)
{
    sprite_sheet_handle = SDL_LoadBMP(sprite_sheet.string().c_str());
    SDL_SetColorKey(sprite_sheet_handle, true, 0);
}

SpriteHandle SpriteSheet::new_sprite(const std::string& name, SDL_Rect base_transform, double animation_speed, const std::vector<SDL_Point>& offsets)
{
    SpriteHandle handle(this, name);
    sprite_map[handle] = SpriteInfo(base_transform, animation_speed, offsets);
    return handle;
}

void SpriteSheet::render_sprite(SpriteHandle sprite, SDL_Point pos, double scale_x, double scale_y)
{
    const auto info = sprite_map.find(sprite);
    if (info == sprite_map.end())
    {
        ERROR("Failed to find sprite with handle {}", sprite);
        return;
    }

    // Chose flip-book index based on world time and animation speed (0 is default offset)

    const auto   last         = std::chrono::steady_clock::now() - last_time;
    const double time         = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - last_time).count()) / 1000000.0;
    const size_t sprite_index = static_cast<int64_t>(time * info->second.animation_speed) % (info->second.sprite_offsets.size() + 1);

    // If not 0, apply sub-sprite offset
    SDL_Rect selected_sprite = info->second.sprite_base_pos;
    if (sprite_index != 0)
    {
        selected_sprite.x += info->second.sprite_offsets[sprite_index - 1].x;
        selected_sprite.y += info->second.sprite_offsets[sprite_index - 1].y;
    }

    SDL_Rect new_coords{pos.x, pos.y, static_cast<int>(selected_sprite.w * scale_x), static_cast<int>(selected_sprite.h * scale_y)};
    SDL_BlitScaled(sprite_sheet_handle, &selected_sprite, pm::Engine::get().get_surface_handle(), &new_coords);
}

std::optional<SpriteHandle> SpriteSheet::find_sprite_by_name(const std::string& name)
{
    const auto found = sprite_map.find(SpriteHandle(this, name));
    if (found == sprite_map.end())
        return {};

    return SpriteHandle(this, name);
}
}



