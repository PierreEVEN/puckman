#pragma once
#include <string>
#include <chrono>
#include <random>

struct SDL_Window;
struct SDL_Surface;

namespace pm
{
class GamemodeBase
{
public:
    virtual void tick(double delta_seconds) {}
    virtual void draw() {}
private:
};


class Engine
{
public:
    template<typename Gamemode_T>
    static Engine& init(const std::string& app_name = "SDL_App", uint32_t width = 800, uint32_t height = 600)
    {
        auto& engine = init_internal(app_name, width, height);
        engine.gamemode = std::make_unique<Gamemode_T>();
        return engine;
    }

    static Engine& get();

    virtual ~Engine();

    [[nodiscard]] SDL_Window*                         get_window_handle() const { return window_handle; }
    [[nodiscard]] SDL_Surface*                        get_surface_handle() const { return surface_handle; }
    [[nodiscard]] double                              get_delta_second() const { return delta_second; }
    [[nodiscard]] std::chrono::steady_clock::duration get_world_time() const { return std::chrono::steady_clock::now() - start_time; }

    template<typename Gamemode_T = GamemodeBase>
    [[nodiscard]] Gamemode_T& get_gamemode() { return *static_cast<Gamemode_T*>(&*gamemode); }

    bool next_frame();

    void shutdown();

    
    int32_t random_int(int32_t min = INT32_MIN, int32_t max = INT32_MAX);
    double random_double(double min = 0.0, double max = 1.0);

    [[nodiscard]] bool is_init() const { return gamemode != nullptr; }

private:
    
    static Engine& init_internal(const std::string& app_name = "SDL_App", uint32_t width = 800, uint32_t height = 600);

    Engine(const std::string& app_name, uint32_t width, uint32_t height);

    SDL_Window*                           window_handle;
    SDL_Surface*                          surface_handle;
    bool                                  close;
    double                                delta_second;
    std::chrono::steady_clock::time_point last_frame_time;
    std::chrono::steady_clock::time_point start_time;
    std::unique_ptr<GamemodeBase>         gamemode;

    
    std::random_device random_device;
    std::mt19937 mt;
};
}
