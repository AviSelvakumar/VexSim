#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <SDL2/SDL.h>
#include "sim/SimState.hpp"
#include "sim/Physics.hpp"
#include "sim/Renderer.hpp"
#include "sim/InputHandler.hpp"
#include "sim/Scheduler.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <vector>

// ─── CLI arg parsing ──────────────────────────────────────────────────────────

static std::vector<int> parsePorts(const char* s) {
    std::vector<int> out;
    std::stringstream ss(s);
    std::string tok;
    while (std::getline(ss, tok, ',')) {
        int v = std::stoi(tok);
        if (v != 0) out.push_back(v);
    }
    return out;
}

static const char* argval(int argc, char** argv, const char* flag, const char* def) {
    for (int i = 1; i + 1 < argc; ++i)
        if (std::strcmp(argv[i], flag) == 0) return argv[i + 1];
    return def;
}

static sim::RobotConfig buildConfig(int argc, char** argv) {
    // Helpers — convert to the values we need
    auto dbl  = [&](const char* flag, double def) {
        return std::stod(argval(argc, argv, flag, std::to_string(def).c_str()));
    };
    auto ports = [&](const char* flag, const char* def) -> std::array<int,4> {
        auto v = parsePorts(argval(argc, argv, flag, def));
        std::array<int,4> a = {0,0,0,0};
        for (int i = 0; i < (int)v.size() && i < 4; ++i) a[i] = v[i];
        return a;
    };

    sim::RobotConfig cfg;

    // Physical — convert inches to pixels (1 in = 5 px)
    double wheel_radius_in        = dbl("--wheel-radius",    1.625);
    double track_width_in         = dbl("--track-width",     12.0);
    double tracking_wheel_radius  = dbl("--tracking-radius", 1.0);
    double robot_width_in         = dbl("--robot-width",     8.0);
    double robot_height_in        = dbl("--robot-height",    8.0);

    cfg.wheel_radius_px           = wheel_radius_in        * 5.0;
    cfg.track_width_px            = track_width_in         * 5.0;
    cfg.tracking_wheel_radius_px  = tracking_wheel_radius  * 5.0;
    cfg.robot_half_w              = (robot_width_in  / 2.0) * 5.0;
    cfg.robot_half_h              = (robot_height_in / 2.0) * 5.0;

    cfg.max_rpm             = dbl("--max-rpm",      600.0);
    cfg.gear_ratio          = dbl("--gear-ratio",   1.0);
    cfg.gear_friction_coeff = dbl("--gear-friction", 0.0);
    cfg.field_w    = 720.0;
    cfg.field_h    = 720.0;

    // Inertia — tau = m * v_max² / (4 * N * P)
    // v_max uses the effective wheel RPM (motor RPM × gear ratio)
    double mass_kg         = dbl("--mass",         10.0);
    double drive_motors    = dbl("--drive-motors",  6.0);
    double r_m             = (cfg.wheel_radius_px / 5.0) * 0.0254;
    double v_max           = (cfg.max_rpm * cfg.gear_ratio / 60.0) * 2.0 * M_PI * r_m;
    cfg.accel_time_constant = (mass_kg * v_max * v_max) / (4.0 * drive_motors * 11.0);

    // Motor ports
    cfg.left_ports  = ports("--left-ports",  "4,5,6");
    cfg.right_ports = ports("--right-ports", "1,2,3");

    // Tracking wheel ports (0 = disabled)
    auto iarg = [&](const char* flag, int def) {
        return std::stoi(argval(argc, argv, flag, std::to_string(def).c_str()));
    };
    cfg.tracking_left_port  = iarg("--tracking-left",  0);
    cfg.tracking_right_port = iarg("--tracking-right", 0);
    cfg.tracking_mid_port   = iarg("--tracking-mid",   0);

    return cfg;
}

// ─── main ─────────────────────────────────────────────────────────────────────

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    constexpr int FIELD_W = 720;
    constexpr int FIELD_H = 720;
    constexpr int WIN_H   = FIELD_H + 80;

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

    sim::RobotConfig cfg = buildConfig(argc, argv);

    // Pre-set reversed flags from signed port numbers so Physics and stubs
    // agree before initialize() runs.  Negative port = reversed motor.
    for (int port : cfg.left_ports) {
        if (port == 0) break;
        if (port < 0) sim::SimState::get().motors[-port].reversed.store(true);
    }
    for (int port : cfg.right_ports) {
        if (port == 0) break;
        if (port < 0) sim::SimState::get().motors[-port].reversed.store(true);
    }

    // Start pose — coordinates in inches from field center (LemLib convention:
    // X right, Y up, heading 0° = North/up, clockwise-positive).
    // Converted to SDL pixels: SDL_y is flipped, sim heading 0 = East (+X).
    {
        auto dbl = [&](const char* flag, double def) {
            return std::stod(argval(argc, argv, flag, std::to_string(def).c_str()));
        };
        double sx  = dbl("--start-x",       0.0);   // inches from center
        double sy  = dbl("--start-y",       0.0);
        double shd = dbl("--start-heading", 0.0);   // degrees, 0=North CW+

        const double S = FIELD_W / 140.41;
        auto& state = sim::SimState::get();
        std::lock_guard<std::mutex> lock(state.pose_mutex);
        state.pose.x       = FIELD_W / 2.0 + sx * S;
        state.pose.y       = FIELD_H / 2.0 - sy * S;          // flip Y
        state.pose.heading = (shd - 90.0) * M_PI / 180.0;     // to sim radians
        state.pose.heading_accumulated = 0.0;
    }

    sim::Physics     physics(cfg);
    sim::Renderer    renderer(sdl_ren, FIELD_W, FIELD_H, cfg);
    sim::InputHandler input;

    printf("VEX Sim ready. Press F6 to start driving, F5 for autonomous.\n");
    printf("Controls: WASD = left stick, Arrow keys = right stick\n");
    printf("Buttons: L=L1  ;=L2  P=R1  [=R2  1=X  2=B  3=Y  4=A\n");

    sim::Scheduler::get().start();

    uint32_t last_ticks = SDL_GetTicks();

    while (sim::SimState::get().running.load()) {
        uint32_t now = SDL_GetTicks();
        double dt    = (now - last_ticks) / 1000.0;
        last_ticks   = now;
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
    std::quick_exit(0);
}
