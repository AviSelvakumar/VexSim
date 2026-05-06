#pragma once
#include <SDL2/SDL.h>
#include "SimState.hpp"
#include "Physics.hpp"

namespace sim {

class Renderer {
public:
    Renderer(SDL_Renderer* ren, int field_w, int field_h, const RobotConfig& cfg);
    void draw(const SimState& state);

private:
    SDL_Renderer* ren_;
    int fw_, fh_;
    RobotConfig cfg_;

    void drawBackground();
    void drawTileLines();
    void drawRobot(double x, double y, double heading);
    void drawHUD(const SimState& state, double x, double y, double heading);

    static void setColor(SDL_Renderer* r, uint8_t red, uint8_t g, uint8_t b, uint8_t a = 255);
};

} // namespace sim
