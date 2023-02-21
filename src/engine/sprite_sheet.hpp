#pragma once
#include <filesystem>
#include <SDL_rect.h>
#include <unordered_map>

struct SDL_Surface;

namespace pm
{
class SpriteHandle;
class SpriteSheet;

class SpriteInfo
{
    friend SpriteSheet;
public:
    SpriteInfo(const SDL_Rect& in_sprite_base_pos = {}, double in_animation_speed = 0.0, std::vector<SDL_Point> in_sprite_offsets = {})
        : animation_speed(in_animation_speed), sprite_base_pos(in_sprite_base_pos), sprite_offsets(std::move(in_sprite_offsets))
    {
    }

private:
    double                 animation_speed;
    SDL_Rect               sprite_base_pos;
    std::vector<SDL_Point> sprite_offsets;
};
}

template <>
struct std::formatter<::pm::SpriteHandle> : std::formatter<std::string>
{
    auto format(::pm::SpriteHandle s, format_context& ctx) const;
};

namespace pm
{
class SpriteHandle
{
    friend struct std::formatter<::pm::SpriteHandle>;
public:
    SpriteHandle(SpriteSheet* in_owner, std::string handle_name)
        : handle(std::move(handle_name)), owner(in_owner)
    {
    }

    SpriteHandle& draw(const SDL_Point& pos, double scale_x = 1.0, double scale_y = 1.0);
    SpriteHandle& set_paused(bool paused);

    SpriteHandle& operator=(const SpriteHandle& other) = default;
    SpriteHandle(const SpriteHandle& other)            = default;
    SpriteHandle(SpriteHandle&& other)                 = default;

    bool operator==(const SpriteHandle& other) const
    {
        return handle == other.handle;
    }

private:
    std::string  handle;
    SpriteSheet* owner;
};
}

inline auto std::formatter<pm::SpriteHandle, char>::format(pm::SpriteHandle s, format_context& ctx) const
{
    return formatter<string>::format(std::format("{}", s.handle), ctx);
}

template <>
struct std::hash<::pm::SpriteHandle>
{
    size_t operator()(const ::pm::SpriteHandle& c) const noexcept
    {
        return hash<string>()(std::format("{}", c));
    }
};

namespace pm
{
class SpriteSheet
{
public:
    SpriteSheet(const std::filesystem::path& sprite_sheet);

    SpriteHandle new_sprite(const std::string& name, SDL_Rect base_transform, double animation_speed = 1.0, const std::vector<SDL_Point>& offsets = {});

    void render_sprite(SpriteHandle sprite, SDL_Point pos, double scale_x = 1.0, double scale_y = 1.0);

    [[nodiscard]] std::optional<SpriteHandle> find_sprite_by_name(const std::string& name);

    void set_paused(bool in_paused)
    {
        paused = in_paused;
    }

private:
    double                                       internal_time = 0.0;
    std::chrono::steady_clock::time_point        last_time;
    bool                                         paused = false;
    std::unordered_map<SpriteHandle, SpriteInfo> sprite_map;
    SDL_Surface*                                 sprite_sheet_handle;
};
}
