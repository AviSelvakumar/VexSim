#define _USE_MATH_DEFINES
#include <cmath>
#include <SDL2/SDL.h>
#include "sim/SimState.hpp"
#include "sim/Physics.hpp"
#include "sim/Renderer.hpp"
#include "sim/InputHandler.hpp"
#include "sim/Scheduler.hpp"
#include <algorithm>
#include <cstdio>

// ─────────────────────────────────────────────────────────────────────────────
// Robot configuration — edit these to match your robot's port layout
// ─────────────────────────────────────────────────────────────────────────────
// ─────────────────────────────────────────────────────────────────────────────
// Scaling guide:
//   The field is 720x720 px representing a 12ft x 12ft VEX field.
//   1 tile = 120 px = 24 inches  →  1 inch ≈ 5 px  (1 cm ≈ 2 px)
//
//   Set wheel_radius_px  = physical_wheel_radius_inches * 5.0
//   Set track_width_px   = physical_track_width_inches  * 5.0
//   LemLib TrackingWheel diameter should equal physical_wheel_diameter_inches.
// ─────────────────────────────────────────────────────────────────────────────
static sim::RobotConfig makeRobotConfig() {
    sim::RobotConfig cfg;

    // ── Physical dimensions (1 in = 5 px) ────────────────────────────────────
    // 3.25" diameter drive wheels → radius = 1.625" → ~8px
    cfg.wheel_radius_px  = 8.0;
    // 12" track width → 60px
    cfg.track_width_px   = 60.0;
    cfg.max_rpm          = 600.0;  // 600 RPM blue cartridge

    cfg.field_w          = 720.0;
    cfg.field_h          = 720.0;
    cfg.robot_half_w     = 20.0;
    cfg.robot_half_h     = 20.0;

    // ── Drive motor ports ─────────────────────────────────────────────────────
    // Match these to your robot/main.cpp motor port numbers.
    cfg.left_ports       = {4, 5, 6, 0};
    cfg.right_ports      = {1, 2, 3, 0};

    // ── Dedicated tracking wheel ports (optional) ─────────────────────────────
    // If your robot uses pros::Rotation sensors on separate ports for odometry,
    // set these so the physics engine updates those encoder values.
    // Leave at 0 to use drive motor encoders only (odometry via Motor::get_position).
    //
    // Example: LemLib with 2.75" tracking wheels on ports 7 & 8:
    //   cfg.tracking_left_port       = 7;
    //   cfg.tracking_right_port      = 8;
    //   cfg.tracking_wheel_radius_px = 2.75 / 2.0 * 5.0;  // 6.875 px
    cfg.tracking_left_port       = 16;
    cfg.tracking_right_port      = 0;
    cfg.tracking_mid_port        = 17;     // always 0 for diff-drive
    cfg.tracking_wheel_radius_px = (2.00 / 2.0) * 5.0; // 2.75" wheel (unused when ports = 0)

    return cfg;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    constexpr int FIELD_W = 720;
    constexpr int FIELD_H = 720;
    constexpr int WIN_H   = FIELD_H + 80; // extra for HUD

    SDL_Window* window = SDL_CreateWindow(
        "VEX V5 Simulator  |  F5=Auto  F6=Drive  F7=Disable  ESC=Quit",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        FIELD_W, WIN_H,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* sdl_ren = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!sdl_ren) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    sim::RobotConfig cfg = makeRobotConfig();

    // Set robot starting position to field center, facing "north" (up on screen)
    {
        auto& state = sim::SimState::get();
        std::lock_guard<std::mutex> lock(state.pose_mutex);
        state.pose.x       = FIELD_W / 2.0;
        state.pose.y       = FIELD_H / 2.0;
        state.pose.heading = -M_PI / 2.0; // -90° = facing up (+Y is down in SDL)
        state.pose.heading_accumulated = 0.0;
    }

    sim::Physics     physics(cfg);
    sim::Renderer    renderer(sdl_ren, FIELD_W, FIELD_H, cfg);
    sim::InputHandler input;

    printf("VEX Sim ready. Press F6 to start driving, F5 for autonomous.\n");
    printf("Controls: WASD = left stick, Arrow keys = right stick\n");
    printf("Buttons: L=L1  ;=L2  P=R1  [=R2  1=X  2=B  3=Y  4=A\n");

    // Launch robot code thread (calls initialize() immediately)
    sim::Scheduler::get().start();

    uint32_t last_ticks = SDL_GetTicks();

    while (sim::SimState::get().running.load()) {
        uint32_t now = SDL_GetTicks();
        double dt    = (now - last_ticks) / 1000.0;
        last_ticks   = now;
        // Cap dt to avoid spiral-of-death after focus loss
        dt = std::min(dt, 0.05);

        if (!input.poll(sim::SimState::get())) break;

        physics.step(dt);
        renderer.draw(sim::SimState::get());
    }

    sim::SimState::get().running.store(false);
    sim::Scheduler::get().shutdown();

    SDL_DestroyRenderer(sdl_ren);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
