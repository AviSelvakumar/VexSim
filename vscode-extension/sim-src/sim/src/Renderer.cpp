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
    setColor(ren_, 20, 20, 20);
    SDL_Rect bg = {0, 0, fw_, fh_ + 80};
    SDL_RenderFillRect(ren_, &bg);
}

// Filled circle via horizontal scanlines
static void fillCircle(SDL_Renderer* ren, int cx, int cy, int r) {
    for (int dy = -r; dy <= r; ++dy) {
        int dx = (int)std::sqrt((double)(r * r - dy * dy));
        SDL_RenderDrawLine(ren, cx - dx, cy + dy, cx + dx, cy + dy);
    }
}

// Filled regular octagon (stop-sign orientation) with circumradius r.
// Derived from the flat-edge inradius = r * cos(π/8) ≈ 0.924r
// and the diagonal clipping: half_width = 1.307r − |dy| for corner rows.
static void fillOctagon(SDL_Renderer* ren, int cx, int cy, int r) {
    const int ir  = (int)(0.9239 * r + 0.5);   // inradius (flat-edge distance)
    const int cr  = (int)(1.3066 * r + 0.5);   // inradius + r*sin(π/8) for diagonal clip
    for (int dy = -ir; dy <= ir; ++dy) {
        int hw = (std::abs(dy) <= (int)(0.3827 * r + 0.5)) ? ir : cr - std::abs(dy);
        if (hw > 0)
            SDL_RenderDrawLine(ren, cx - hw, cy + dy, cx + hw, cy + dy);
    }
}

// Octagon outline only
static void drawOctagon(SDL_Renderer* ren, int cx, int cy, int r) {
    SDL_Point pts[9];
    for (int i = 0; i < 8; ++i) {
        double a = (i * 45.0 + 22.5) * M_PI / 180.0;
        pts[i] = { cx + (int)(r * std::cos(a) + 0.5),
                   cy + (int)(r * std::sin(a) + 0.5) };
    }
    pts[8] = pts[0];
    SDL_RenderDrawLines(ren, pts, 9);
}

void Renderer::drawField() {
    // All positions from game diagram are in inches (field = 140.41" × 140.41")
    const double S = (double)fw_ / 140.41;  // px per inch

    // Convert: field has (0,0) at bottom-left, Y increases up
    // SDL has (0,0) at top-left, Y increases down
    auto fx = [&](double in) -> int { return (int)(in * S + 0.5); };
    auto fy = [&](double in) -> int { return fh_ - (int)(in * S + 0.5); };

    // ── Field tiles (6×6 grid, ~23.4" each) ──────────────────────────────────
    setColor(ren_, 50, 50, 50);
    SDL_Rect fieldBg = {0, 0, fw_, fh_};
    SDL_RenderFillRect(ren_, &fieldBg);

    setColor(ren_, 70, 70, 70);
    for (int i = 0; i <= 6; ++i) {
        int pos = fx(i * 140.41 / 6.0);
        SDL_RenderDrawLine(ren_, pos, 0, pos, fh_);
        SDL_RenderDrawLine(ren_, 0, pos, fw_, pos);
    }

    // ── Diagonal tape pattern (both field diagonals) ─────────────────────────
    setColor(ren_, 100, 95, 55);
    for (int t = -2; t <= 2; ++t) {
        SDL_RenderDrawLine(ren_, t, 0,      fw_ + t, fh_);   // top-left → bottom-right
        SDL_RenderDrawLine(ren_, t, fh_,    fw_ + t, 0);     // bottom-left → top-right
    }

    // ── Alliance border strips ────────────────────────────────────────────────
    // Red on left (x = 0..1.58"), Blue on right (x = 138.83..140.41")
    setColor(ren_, 160, 35, 35, 220);
    SDL_Rect rb = {0, 0, fx(1.58), fh_};
    SDL_RenderFillRect(ren_, &rb);

    setColor(ren_, 35, 35, 160, 220);
    int bx = fx(138.83);
    SDL_Rect bb = {bx, 0, fw_ - bx, fh_};
    SDL_RenderFillRect(ren_, &bb);

    // ── Toggles — yellow vertical bars on left/right walls (mid-field zone) ──
    // Positioned at y = 43.50"..96.91" on each side wall (span = 53.41")
    setColor(ren_, 220, 200, 50);
    int tgy0 = fy(96.91), tgy1 = fy(43.50);   // note: fy flips so tgy0 < tgy1
    SDL_Rect tLeft  = {0,       tgy0, 6,         tgy1 - tgy0};
    SDL_Rect tRight = {fw_ - 6, tgy0, 6,         tgy1 - tgy0};
    SDL_RenderFillRect(ren_, &tLeft);
    SDL_RenderFillRect(ren_, &tRight);

    // ── Goals ─────────────────────────────────────────────────────────────────
    // Spec (Appendix A): short goal outer circumradius R=3.22", center hole Ø2.37".
    // All goals use the same octagonal profile; center tall is gold and slightly larger.
    const int GOAL_R   = (int)(3.22 * S + 0.5);   // outer circumradius  (px)
    const int HOLE_R   = (int)(1.19 * S + 0.5);   // center opening radius Ø2.37"/2 (px)
    const int CENTER_R = (int)(3.50 * S + 0.5);   // center tall goal is a bit larger

    struct GoalDef { double xi, yi; uint8_t r, g, b; bool isTall; };
    static const GoalDef goals[] = {
        // Red alliance goals
        {23.11,  23.11,  200,  45,  45, false},
        {23.11, 117.30,  200,  45,  45, false},
        // Blue alliance goals
        {117.30,  23.11,  45,  45, 200, false},
        {117.30, 117.30,  45,  45, 200, false},
        // Neutral short goals
        {48.24, 48.24,  160, 160, 160, false},
        {48.24, 92.17,  160, 160, 160, false},
        {92.17, 48.24,  160, 160, 160, false},
        {92.17, 92.17,  160, 160, 160, false},
        // Center tall goal
        {70.20, 70.20,  210, 195,  55, true},
    };

    for (const auto& g : goals) {
        int gx = fx(g.xi), gy = fy(g.yi);
        int R  = g.isTall ? CENTER_R : GOAL_R;
        int Hr = g.isTall ? (int)(1.40 * S + 0.5) : HOLE_R;

        // Body fill
        setColor(ren_, g.r, g.g, g.b);
        fillOctagon(ren_, gx, gy, R);

        // White octagon outline
        setColor(ren_, 220, 220, 220);
        drawOctagon(ren_, gx, gy, R);

        // Dark center hole (the opening / scoring cavity)
        setColor(ren_, 25, 25, 25);
        fillCircle(ren_, gx, gy, Hr);
        setColor(ren_, 90, 90, 90);
        drawOctagon(ren_, gx, gy, Hr);
    }

    // ── Field border ──────────────────────────────────────────────────────────
    setColor(ren_, 180, 180, 180);
    SDL_Rect border = {0, 0, fw_, fh_};
    SDL_RenderDrawRect(ren_, &border);
}

// ── Pixel-font text rendering ──────────────────────────────────────────────────
// 3×5 glyph bitmaps. Each uint8_t = one row, 3 LSBs: bit2=left, bit1=center, bit0=right.
// Rendered at 2× scale → each glyph is 6×10 px; advance = 7 px (6 + 1 gap).

static const struct { char ch; uint8_t rows[5]; } FONT[] = {
    {'0', {0b111, 0b101, 0b101, 0b101, 0b111}},
    {'1', {0b010, 0b110, 0b010, 0b010, 0b111}},
    {'2', {0b111, 0b001, 0b111, 0b100, 0b111}},
    {'3', {0b111, 0b001, 0b111, 0b001, 0b111}},
    {'4', {0b101, 0b101, 0b111, 0b001, 0b001}},
    {'5', {0b111, 0b100, 0b111, 0b001, 0b111}},
    {'6', {0b111, 0b100, 0b111, 0b101, 0b111}},
    {'7', {0b111, 0b001, 0b001, 0b001, 0b001}},
    {'8', {0b111, 0b101, 0b111, 0b101, 0b111}},
    {'9', {0b111, 0b101, 0b111, 0b001, 0b111}},
    {'.', {0b000, 0b000, 0b000, 0b000, 0b010}},
    {'-', {0b000, 0b000, 0b111, 0b000, 0b000}},
    {':', {0b000, 0b010, 0b000, 0b010, 0b000}},
    {'X', {0b101, 0b101, 0b010, 0b101, 0b101}},
    {'Y', {0b101, 0b101, 0b010, 0b010, 0b010}},
    {' ', {0b000, 0b000, 0b000, 0b000, 0b000}},
};

static void drawText(SDL_Renderer* ren, int x, int y, const char* text) {
    constexpr int SCALE = 2;
    constexpr int ADV   = 3 * SCALE + 1;   // 7 px advance per character
    int cx = x;
    for (const char* p = text; *p; ++p) {
        if (*p == '\n') { y += 5 * SCALE + 3;  cx = x;  continue; }
        // find glyph
        const uint8_t* rows = nullptr;
        for (const auto& g : FONT)
            if (g.ch == *p) { rows = g.rows; break; }
        if (rows) {
            for (int row = 0; row < 5; ++row) {
                uint8_t bits = rows[row];
                for (int col = 0; col < 3; ++col) {
                    if (bits & (1u << (2 - col))) {
                        SDL_Rect px = { cx + col * SCALE, y + row * SCALE, SCALE, SCALE };
                        SDL_RenderFillRect(ren, &px);
                    }
                }
            }
        }
        cx += ADV;
    }
}

void Renderer::drawCursorOverlay(const SimState& state) {
    int mx = state.mouse_x.load();
    int my = state.mouse_y.load();
    if (mx < 0 || my < 0 || mx >= fw_ || my >= fh_) return;

    // Convert SDL pixel → field inches, origin at field center
    const double S  = fw_ / 140.41;
    double field_x  =  (mx / S) - 70.205;
    double field_y  = -(my / S) + 70.205;  // flip Y

    char buf[32];
    std::snprintf(buf, sizeof(buf), "X:%.1f\nY:%.1f", field_x, field_y);

    // Background box (top-left corner of field)
    constexpr int PAD = 3;
    constexpr int BOX_W = 54, BOX_H = 27;
    setColor(ren_, 0, 0, 0, 180);
    SDL_Rect bg = { PAD, PAD, BOX_W, BOX_H };
    SDL_RenderFillRect(ren_, &bg);

    setColor(ren_, 200, 200, 200);
    drawText(ren_, PAD + 3, PAD + 3, buf);
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
    drawField();
    drawRobot(px, py, heading);
    drawCursorOverlay(state);
    drawHUD(state, px, py, heading);
    SDL_RenderPresent(ren_);
}

} // namespace sim
