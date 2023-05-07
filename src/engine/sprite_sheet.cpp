#include "sprite_sheet.hpp"

#include "engine.hpp"
#include "logger.hpp"

#include <SDL_surface.h>
#include <unordered_map>

namespace pm
{
static std::unordered_map<SpriteHandle, SpriteInfo> sprite_map;

void SpriteHandle::draw(const Vector2I& pos, double scale_x, double scale_y, SDL_Surface* surface_override) const
{
    if (!*this)
        FATAL("invalid handle");
    owner->render_sprite(*this, pos + Vector2I{0, 32}, scale_x, scale_y, surface_override);
}

SpriteHandle& SpriteHandle::set_paused(bool paused)
{
    if (!*this || !owner)
        FATAL("invalid handle");
    owner->set_paused(*this, paused);
    return *this;
}

bool SpriteHandle::is_paused() const
{
    if (!*this)
        FATAL("invalid handle");
    return owner->is_paused(*this);
}

void SpriteHandle::reset_timer() const
{
    if (!*this)
        FATAL("invalid handle");
    owner->reset_timer(*this);
}

SpriteSheet::SpriteSheet(const std::filesystem::path& sprite_sheet)
{
    sprite_sheet_handle = SDL_LoadBMP(sprite_sheet.string().c_str());
    SDL_SetColorKey(sprite_sheet_handle, true, 0);
}

SpriteHandle SpriteSheet::new_sprite(const std::string& name, SDL_Rect base_transform, double animation_speed, const std::vector<Vector2I>& offsets)
{
    SpriteHandle handle(this, name);
    if (sprite_map.contains(handle))
        ERROR("There already is a sprite using '{}' as handle", name);

    sprite_map[handle] = SpriteInfo(base_transform, animation_speed, offsets);
    return handle;
}

void SpriteSheet::render_sprite(const SpriteHandle& sprite, Vector2I pos, double scale_x, double scale_y, SDL_Surface* surface_override) const
{
    const auto info = sprite_map.find(sprite);
    if (info == sprite_map.end())
    {
        ERROR("Failed to find sprite with handle {}", sprite);
        return;
    }

    // Chose flip-book index based on world time and animation speed (0 is default offset)
    const auto elapsed = std::chrono::steady_clock::now() - info->second.last_time;
    if (!info->second.paused)
        info->second.internal_time += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()) / 1000000.0;
    info->second.last_time    = std::chrono::steady_clock::now();
    const size_t sprite_index = static_cast<int64_t>(info->second.internal_time * info->second.animation_speed) % (info->second.sprite_offsets.size() + 1);

    // If not 0, apply sub-sprite offset
    SDL_Rect selected_sprite = info->second.sprite_base_pos;
    if (sprite_index != 0)
    {
        selected_sprite.x += info->second.sprite_offsets[sprite_index - 1].x();
        selected_sprite.y += info->second.sprite_offsets[sprite_index - 1].y();
    }

    SDL_Rect new_coords{pos.x(), pos.y(), static_cast<int>(selected_sprite.w * scale_x), static_cast<int>(selected_sprite.h * scale_y)};
    SDL_BlitScaled(sprite_sheet_handle, &selected_sprite, surface_override ? surface_override : pm::Engine::get().get_surface_handle(), &new_coords);
}

void SpriteSheet::set_paused(SpriteHandle sprite, bool in_paused) const
{
    const auto info = sprite_map.find(sprite);
    if (info == sprite_map.end())
    {
        ERROR("Failed to find sprite with handle {}", sprite);
        return;
    }
    info->second.paused = in_paused;
}

bool SpriteSheet::is_paused(SpriteHandle sprite) const
{
    const auto info = sprite_map.find(sprite);
    if (info == sprite_map.end())
    {
        ERROR("Failed to find sprite with handle {}", sprite);
        return true;
    }
    return info->second.paused;
}

void SpriteSheet::reset_timer(SpriteHandle sprite) const
{
    const auto info = sprite_map.find(sprite);
    if (info == sprite_map.end())
    {
        ERROR("Failed to find sprite with handle {}", sprite);
    }
    info->second.last_time = std::chrono::steady_clock::now();
    info->second.internal_time = 0;
}

std::optional<SpriteHandle> SpriteSheet::find_sprite_by_name(const std::string& name)
{
    const auto found = sprite_map.find(SpriteHandle(nullptr, name));
    if (found == sprite_map.end())
        return {};

    return found->first;
}

SpriteHandle SpriteSheet::find_sprite_by_name_or_default(const std::string& name, SpriteHandle default_value)
{
    return find_sprite_by_name(name).value_or(default_value);
}
}
