#pragma once

#include <filesystem>
#include <vector>
#include <SDL_rect.h>
#include <optional>

#include "format.hpp"
#include "vector2.hpp"

struct SDL_Surface;

namespace pm
{
class SpriteHandle;
class SpriteSheet;

class SpriteInfo
{
    friend SpriteSheet;
public:
    SpriteInfo(const SDL_Rect& in_sprite_base_pos = {}, double in_animation_speed = 0.0, std::vector<Vector2I> in_sprite_offsets = {})
        : animation_speed(in_animation_speed), sprite_base_pos(in_sprite_base_pos), sprite_offsets(std::move(in_sprite_offsets))
    {
    }

private:
    double                                animation_speed;
    SDL_Rect                              sprite_base_pos;
    std::vector<Vector2I>                 sprite_offsets;
    bool                                  paused        = false;
    double                                internal_time = 0.0;
    std::chrono::steady_clock::time_point last_time;
};
} // namespace pm

namespace pm
{
class SpriteHandle
{
    friend std::stringstream& operator<<(std::stringstream& stream, const SpriteHandle& s);

public:
    SpriteHandle(SpriteSheet* in_owner, std::string handle_name)
        : handle(std::move(handle_name)), owner(in_owner)
    {
    }

    SpriteHandle(std::string handle_name)
        : handle(std::move(handle_name)), owner(nullptr)
    {
    }

    SpriteHandle()
        : owner(nullptr)
    {
    }

    void               draw(const Vector2I& pos, double scale_x = 1.0, double scale_y = 1.0, SDL_Surface* surface_override = nullptr) const;
    SpriteHandle&      set_paused(bool paused);
    [[nodiscard]] bool is_paused() const;

    SpriteHandle& operator=(const SpriteHandle& other) = default;
    SpriteHandle(const SpriteHandle& other) = default;
    SpriteHandle(SpriteHandle&& other)      = default;

    bool operator==(const SpriteHandle& other) const
    {
        return handle == other.handle;
    }

    operator bool() const
    {
        return owner;
    }

private:
    std::string  handle;
    SpriteSheet* owner;
};

inline std::stringstream& operator<<(std::stringstream& stream, const SpriteHandle& s)
{
    stream << s.handle.c_str();
    return stream;
}
} // namespace pm

template <>
struct std::hash<::pm::SpriteHandle>
{
    size_t operator()(const ::pm::SpriteHandle& c) const noexcept
    {
        return hash<string>()(std::format_2("{}", c));
    }
};

namespace pm
{
class SpriteSheet
{
public:
    SpriteSheet(const std::filesystem::path& sprite_sheet);

    SpriteHandle new_sprite(const std::string& name, SDL_Rect base_transform, double animation_speed = 1.0, const std::vector<Vector2I>& offsets = {});

    void render_sprite(const SpriteHandle& sprite, Vector2I pos, double scale_x = 1.0, double scale_y = 1.0, SDL_Surface* surface_override = nullptr) const;

    [[nodiscard]] static std::optional<SpriteHandle> find_sprite_by_name(const std::string& name);

    void set_paused(SpriteHandle sprite, bool in_paused) const;

    [[nodiscard]] bool is_paused(SpriteHandle sprite) const;
private:
    SDL_Surface* sprite_sheet_handle;
};
}
