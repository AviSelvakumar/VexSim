#pragma once
#include "llemu.h"
#include <cstdint>
#include <string>

namespace pros {
namespace lcd {

bool initialize();
bool is_initialized();
bool shutdown();
bool print(int16_t line, const char* fmt, ...);
bool clear();
bool clear_line(int16_t line);
bool set_text(int16_t line, const std::string& text);

// Button callbacks (LLEMU center/left/right buttons)
typedef void (*lcd_btn_cb_fn_t)(void);
bool register_btn0_cb(lcd_btn_cb_fn_t cb);
bool register_btn1_cb(lcd_btn_cb_fn_t cb);
bool register_btn2_cb(lcd_btn_cb_fn_t cb);

} // namespace lcd
} // namespace pros
