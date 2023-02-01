#pragma once
#include <memory>
#include <string>
#include <chrono>

struct SDL_Window;
struct SDL_Surface;

namespace pc
{
class Engine
{
public:
    static Engine& init(const std::string& app_name = "SDL_App", uint32_t width = 800, uint32_t height = 600);

    static Engine& get();

    virtual ~Engine();

    [[nodiscard]] SDL_Window*                         get_window_handle() const { return window_handle; }
    [[nodiscard]] SDL_Surface*                        get_surface_handle() const { return surface_handle; }
    [[nodiscard]] double                              get_delta_second() const { return delta_second; }
    [[nodiscard]] std::chrono::steady_clock::duration get_world_time() const { return std::chrono::steady_clock::now() - start_time; }

    bool next_frame();

    void shutdown();

private:
    Engine(const std::string& app_name, uint32_t width, uint32_t height);

    SDL_Window*                           window_handle;
    SDL_Surface*                          surface_handle;
    bool                                  close;
    double                                delta_second;
    std::chrono::steady_clock::time_point last_frame_time;
    std::chrono::steady_clock::time_point start_time;
};
}
