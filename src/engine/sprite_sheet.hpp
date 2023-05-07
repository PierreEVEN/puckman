#pragma once

#include <filesystem>
#include <vector>
#include <SDL_rect.h>
#include <optional>

#include "format.hpp"
#include "vector2.hpp"

/*
 * Sprite system : sprite containers and handles
 */

struct SDL_Surface;

namespace pm
{
class SpriteHandle;
class SpriteSheet;

/*
 * Contains all runtime information about a sprite. There can be multiple instances of SpriteInfo for the same texture. (if it requires separated behaviors)
 */
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
    // Coordinates of the image on the sprite sheet
    SDL_Rect                              sprite_base_pos;
    // The animation system cycle through the different offset in the sprite sheet (in addition of default sprite location)
    std::vector<Vector2I>                 sprite_offsets;
    // Should we pause animation
    bool                                  paused        = false;
    // Internal animation time. Can be reset using SpriteHandle::reset_timer();
    double                                internal_time = 0.0;
    // Used to compute time delta between draw calls and update internal time.
    std::chrono::steady_clock::time_point last_time;
};
} // namespace pm

namespace pm
{
/*
 * Abstract representation of SpriteInfo. It's like a smart pointer (without internal reference counter)
 */
class SpriteHandle
{
    friend std::stringstream& operator<<(std::stringstream& stream, const SpriteHandle& s);

public:
    SpriteHandle(SpriteSheet* in_owner, std::string handle_name)
        : handle(std::move(handle_name)), owner(in_owner)
    {
    }

    // Always produce a null handle until it's owner have been set
    SpriteHandle(std::string handle_name)
        : handle(std::move(handle_name)), owner(nullptr)
    {
    }

    SpriteHandle()
        : owner(nullptr)
    {
    }

    // Draw onto given surface (or default surface if null). pos is the offset, (0,0) is top left corner.
    void               draw(const Vector2I& pos, double scale_x = 1.0, double scale_y = 1.0, SDL_Surface* surface_override = nullptr) const;

    // Pause animation cycle
    SpriteHandle&      set_paused(bool paused);
    [[nodiscard]] bool is_paused() const;

    // Reset animation timer to initial state
    void reset_timer() const;

    // Copy / affectation / comparison...
    SpriteHandle& operator=(const SpriteHandle& other) = default;
    SpriteHandle(const SpriteHandle& other) = default;
    SpriteHandle(SpriteHandle&& other)      = default;
    bool operator==(const SpriteHandle& other) const { return handle == other.handle; }

    // test if handle is valid. Ex : if (my_handle) // should be valid there
    operator bool() const
    {
        return owner;
    }

private:
    std::string  handle;
    SpriteSheet* owner;
};

// Used for debug purposes
inline std::stringstream& operator<<(std::stringstream& stream, const SpriteHandle& s)
{
    stream << s.handle.c_str();
    return stream;
}
} // namespace pm

// Hash
template <>
struct std::hash<::pm::SpriteHandle>
{
    size_t operator()(const ::pm::SpriteHandle& c) const noexcept
    {
        return hash<string>()(std::format_2("{}", c));
    }
};

// Use a given texture as a sprite sheet.
// Don't create multiple sprite sheet for the same textures.
// Generating sprite does not copy texture data. Having multiple sprites for the same image doesn't affect performances.
// WARNING : Sprite sheets are not meant to be destroyed before program completion.
namespace pm
{
class SpriteSheet
{
public:
    SpriteSheet(const std::filesystem::path& sprite_sheet);

    // Create a sprite from this sprite sheet. See SpriteInfos for more information.
    // Return a sprite handle referencing this sprite.
    // Sprite name should always be unique among all the game.
    // Creating a new sprite with a name already in use will update all sprites using this name. Be careful, it's not an intended behavior.
    SpriteHandle                                     new_sprite(const std::string& name, SDL_Rect base_transform, double animation_speed = 1.0, const std::vector<Vector2I>& offsets = {});
    [[nodiscard]] static std::optional<SpriteHandle> find_sprite_by_name(const std::string& name);
    [[nodiscard]] static SpriteHandle find_sprite_by_name_or_default(const std::string& name, SpriteHandle default_value=SpriteHandle{});

    // Draw sprite at location
    void render_sprite(const SpriteHandle& sprite, Vector2I pos, double scale_x = 1.0, double scale_y = 1.0, SDL_Surface* surface_override = nullptr) const;

    // Pause sprite animation
    void set_paused(SpriteHandle sprite, bool in_paused) const;
    [[nodiscard]] bool is_paused(SpriteHandle sprite) const;

    // Reset sprite internal time
    void               reset_timer(SpriteHandle sprite) const;

  private:
    // Sprite sheet image
    SDL_Surface* sprite_sheet_handle;
};
}
