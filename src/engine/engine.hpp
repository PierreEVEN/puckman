#pragma once
#include <string>
#include <chrono>
#include <random>
#include <memory>

struct SDL_Window;
struct SDL_Surface;

namespace pm
{
// Extend this class to implement your one game mode.
class GamemodeBase
{
public:
    virtual ~GamemodeBase() = default;
    virtual void tick(double delta_seconds) {}
    virtual void draw() {}
};

/*
 * Root of the system.
 * Engine can be accessed anywhere using Engine::get()
 *
 * The goal of this class is to handle rendering and internal times
 * It also contains a reference to the instanced game mode.
 */
class Engine
{
public:

    // Initialize the engine. Gamemode_T should extend GamemodeBase and will be instanced once the engine will be initialized.
    template<typename Gamemode_T>
    static Engine& init(const std::string& app_name = "Empty Engine", uint32_t width = 800, uint32_t height = 600)
    {
        auto& engine = init_internal(app_name, width, height);
        engine.gamemode = std::make_unique<Gamemode_T>();
        return engine;
    }

    [[nodiscard]] bool is_init() const { return gamemode != nullptr; }

    // Get reference to instanced engine. WARNING : Engine::init() should have been called first
    static Engine& get();
    virtual ~Engine();

    // SDL handles
    [[nodiscard]] SDL_Window*                         get_window_handle() const { return window_handle; }
    [[nodiscard]] SDL_Surface*                        get_surface_handle() const { return surface_handle; }

    // Get elapsed time in seconds since last frame
    [[nodiscard]] double                              get_delta_second() const { return delta_second; }

    // Get elapsed time since game start
    [[nodiscard]] std::chrono::steady_clock::duration get_world_time() const { return std::chrono::steady_clock::now() - start_time; }

    // Get instanced gamemode.
    template<typename Gamemode_T = GamemodeBase>
    [[nodiscard]] Gamemode_T& get_gamemode() { return *static_cast<Gamemode_T*>(&*gamemode); }

    // Complete this frame and submit to display
    // return true while shutdown() have not been called
    bool next_frame();

    // Stop render loop and exit game.
    void shutdown();

    // Accurate randoms
    int32_t random_int(int32_t min = INT32_MIN, int32_t max = INT32_MAX);
    double random_double(double min = 0.0, double max = 1.0);

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

    // Random management
    std::random_device random_device;
    std::mt19937 mt;
};
}
