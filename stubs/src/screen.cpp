// Stub implementations for VEX Brain Screen
// All drawing operations are no-ops in the simulator (we use SDL2 for our own display)
#include "pros/screen.h"
#include "pros/screen.hpp"
#include "pros/error.h"
#include <cstdio>
#include <cstdarg>

// ── C API ─────────────────────────────────────────────────────────────────────

#ifdef __cplusplus
extern "C" {
namespace pros {
namespace c {
#endif

uint32_t screen_set_pen(uint32_t color)                                      { return 1; }
uint32_t screen_set_eraser(uint32_t color)                                   { return 1; }
uint32_t screen_get_pen(void)                                                { return 0; }
uint32_t screen_get_eraser(void)                                             { return 0; }
uint32_t screen_erase(void)                                                  { return 1; }
uint32_t screen_scroll(int16_t start_line, int16_t lines)                    { return 1; }
uint32_t screen_scroll_area(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                            int16_t lines)                                   { return 1; }
uint32_t screen_copy_area(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                          uint32_t* buf, int32_t stride)                     { return 1; }
uint32_t screen_draw_pixel(int16_t x, int16_t y)                             { return 1; }
uint32_t screen_erase_pixel(int16_t x, int16_t y)                            { return 1; }
uint32_t screen_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1)   { return 1; }
uint32_t screen_erase_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1)  { return 1; }
uint32_t screen_draw_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1)   { return 1; }
uint32_t screen_erase_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1)  { return 1; }
uint32_t screen_fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1)   { return 1; }
uint32_t screen_draw_circle(int16_t x, int16_t y, int16_t radius)            { return 1; }
uint32_t screen_erase_circle(int16_t x, int16_t y, int16_t radius)           { return 1; }
uint32_t screen_fill_circle(int16_t x, int16_t y, int16_t radius)            { return 1; }

uint32_t screen_print(text_format_e_t txt_fmt, const int16_t line,
                      const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
    return 1;
}

uint32_t screen_print_at(text_format_e_t txt_fmt, int16_t x, int16_t y,
                         const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
    return 1;
}

uint32_t screen_vprint(text_format_e_t txt_fmt, const int16_t line,
                       const char* fmt, va_list args) {
    vprintf(fmt, args);
    printf("\n");
    return 1;
}

uint32_t screen_vprint_at(text_format_e_t txt_fmt, int16_t x, int16_t y,
                          const char* fmt, va_list args) {
    vprintf(fmt, args);
    printf("\n");
    return 1;
}

screen_touch_status_s_t screen_touch_status(void) {
    return { E_TOUCH_RELEASED, 0, 0, 0, 0 };
}

uint32_t screen_touch_callback(touch_event_cb_fn_t cb, last_touch_e_t event_type) {
    return 1;
}

#ifdef __cplusplus
} // namespace c
} // namespace pros
} // extern "C"
#endif

// ── C++ Screen namespace ──────────────────────────────────────────────────────

namespace pros {
namespace screen {

std::uint32_t set_pen(pros::Color color)                          { return 1; }
std::uint32_t set_pen(std::uint32_t color)                        { return 1; }
std::uint32_t set_eraser(pros::Color color)                       { return 1; }
std::uint32_t set_eraser(std::uint32_t color)                     { return 1; }
std::uint32_t get_pen()                                           { return 0; }
std::uint32_t get_eraser()                                        { return 0; }
std::uint32_t erase()                                             { return 1; }
std::uint32_t scroll(const std::int16_t start_line, const std::int16_t lines) { return 1; }
std::uint32_t scroll_area(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1, std::int16_t lines) { return 1; }
std::uint32_t copy_area(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1, std::uint32_t* buf, const std::int32_t stride) { return 1; }
std::uint32_t draw_pixel(const std::int16_t x, const std::int16_t y) { return 1; }
std::uint32_t erase_pixel(const std::int16_t x, const std::int16_t y) { return 1; }
std::uint32_t draw_line(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1) { return 1; }
std::uint32_t erase_line(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1) { return 1; }
std::uint32_t draw_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1) { return 1; }
std::uint32_t erase_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1) { return 1; }
std::uint32_t fill_rect(const std::int16_t x0, const std::int16_t y0, const std::int16_t x1, const std::int16_t y1) { return 1; }
std::uint32_t draw_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius) { return 1; }
std::uint32_t erase_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius) { return 1; }
std::uint32_t fill_circle(const std::int16_t x, const std::int16_t y, const std::int16_t radius) { return 1; }

screen_touch_status_s_t touch_status() {
    screen_touch_status_s_t s{};
    s.touch_status = E_TOUCH_RELEASED;
    return s;
}

std::uint32_t touch_callback(touch_event_cb_fn_t cb, last_touch_e_t event_type) { return 1; }

} // namespace screen
} // namespace pros
