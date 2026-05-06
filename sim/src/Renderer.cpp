#define _USE_MATH_DEFINES
#include "sim/Renderer.hpp"
#include <cmath>
#include <cstdio>
#include <array>

namespace sim {

Renderer::Renderer(SDL_Renderer* ren, int field_w, int field_h, const RobotConfig& cfg)
    : ren_(ren), fw_(field_w), fh_(field_h), cfg_(cfg) {}

void Renderer::setColor(SDL_Renderer* r, uint8_t red, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(r, red, g, b, a);
}

void Renderer::drawBackground() {
    setColor(ren_, 34, 34, 34);
    SDL_Rect bg = {0, 0, fw_, fh_ + 80};
    SDL_RenderFillRect(ren_, &bg);
}

void Renderer::drawTileLines() {
    setColor(ren_, 80, 80, 80);
    int tile = fw_ / 6; // 120px
    for (int i = 0; i <= 6; ++i) {
        SDL_RenderDrawLine(ren_, i * tile, 0, i * tile, fh_);
        SDL_RenderDrawLine(ren_, 0, i * tile, fw_, i * tile);
    }
    // Field border
    setColor(ren_, 160, 160, 160);
    SDL_Rect border = {0, 0, fw_, fh_};
    SDL_RenderDrawRect(ren_, &border);
}

void Renderer::drawRobot(double x, double y, double heading) {
    double hw = cfg_.robot_half_w;
    double hh = cfg_.robot_half_h;

    // Four corners of robot in local space
    double cx[4] = {-hw, hw,  hw, -hw};
    double cy[4] = {-hh, -hh, hh,  hh};

    double cosH = std::cos(heading);
    double sinH = std::sin(heading);

    // Rotate and translate to world space
    SDL_Point pts[5];
    for (int i = 0; i < 4; ++i) {
        pts[i].x = static_cast<int>(x + cx[i] * cosH - cy[i] * sinH);
        pts[i].y = static_cast<int>(y + cx[i] * sinH + cy[i] * cosH);
    }
    pts[4] = pts[0]; // close the rectangle

    // Fill robot body
    setColor(ren_, 220, 220, 220);
    // Draw filled using horizontal scanlines between rotated edges (simplified: draw outline)
    SDL_RenderDrawLines(ren_, pts, 5);

    // Draw nose indicator (line from center toward front = +Y in local space → heading direction)
    setColor(ren_, 255, 80, 80);
    int nx = static_cast<int>(x + (-sinH) * hh);  // local +Y is "forward" (up when heading=-pi/2)
    int ny = static_cast<int>(y + ( cosH) * hh);
    // Wait — forward in local space is -Y (up on screen), local top edge midpoint
    // With heading=-pi/2 (facing up): cosH=0, sinH=-1
    // local top midpoint: (0, -hh) → world: (x + 0*0 - (-hh)*(-1), y + 0*(-1) + (-hh)*0) = (x-hh, y)
    // That's not right. Let's use: nose is along local +X direction in robot frame
    // local +X → world: (cos(heading), sin(heading))
    int fx = static_cast<int>(x + cosH * hw);
    int fy = static_cast<int>(y + sinH * hw);
    SDL_RenderDrawLine(ren_, static_cast<int>(x), static_cast<int>(y), fx, fy);

    // Center dot
    setColor(ren_, 255, 255, 0);
    SDL_Rect dot = {static_cast<int>(x) - 2, static_cast<int>(y) - 2, 5, 5};
    SDL_RenderFillRect(ren_, &dot);
}

void Renderer::drawHUD(const SimState& state, double x, double y, double heading) {
    // We can't render fonts without SDL_ttf, so we draw simple colored bars
    // representing motor voltages instead.

    // HUD background
    setColor(ren_, 20, 20, 20);
    SDL_Rect hudBg = {0, fh_, fw_, 80};
    SDL_RenderFillRect(ren_, &hudBg);

    // Motor voltage bars (ports 1–10)
    for (int port = 1; port <= 10; ++port) {
        int voltage = state.motors[port].voltage.load(); // -127..127
        int barMaxH = 34;
        int barW    = fw_ / 10 - 4;
        int barX    = (port - 1) * (fw_ / 10) + 2;

        // Background bar
        setColor(ren_, 50, 50, 50);
        SDL_Rect bg = {barX, fh_ + 4, barW, barMaxH * 2};
        SDL_RenderFillRect(ren_, &bg);

        int center = fh_ + 4 + barMaxH;
        int barH   = static_cast<int>((std::abs(voltage) / 127.0) * barMaxH);

        if (voltage > 0) {
            setColor(ren_, 80, 200, 80);
            SDL_Rect bar = {barX, center - barH, barW, barH};
            SDL_RenderFillRect(ren_, &bar);
        } else if (voltage < 0) {
            setColor(ren_, 200, 80, 80);
            SDL_Rect bar = {barX, center, barW, barH};
            SDL_RenderFillRect(ren_, &bar);
        }

        // Centerline
        setColor(ren_, 120, 120, 120);
        SDL_RenderDrawLine(ren_, barX, center, barX + barW, center);
    }

    // Comp mode indicator
    CompMode mode = state.comp_mode.load();
    if (mode == CompMode::AUTONOMOUS) {
        setColor(ren_, 255, 165, 0);
    } else if (mode == CompMode::OPCONTROL) {
        setColor(ren_, 80, 200, 80);
    } else {
        setColor(ren_, 150, 150, 150);
    }
    SDL_Rect modeRect = {fw_ - 14, fh_ + 4, 10, 10};
    SDL_RenderFillRect(ren_, &modeRect);
}

void Renderer::draw(const SimState& state) {
    double px, py, heading;
    {
        std::lock_guard<std::mutex> lock(state.pose_mutex);
        px      = state.pose.x;
        py      = state.pose.y;
        heading = state.pose.heading;
    }

    SDL_RenderClear(ren_);
    drawBackground();
    drawTileLines();
    drawRobot(px, py, heading);
    drawHUD(state, px, py, heading);
    SDL_RenderPresent(ren_);
}

} // namespace sim
