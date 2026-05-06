#include "pros/llemu.h"
#include "pros/llemu.hpp"
#include <cstdarg>
#include <cstdio>

// ── C API ─────────────────────────────────────────────────────────────────────

bool lcd_initialize(void) { return true; }
bool lcd_is_initialized(void) { return true; }
bool lcd_shutdown(void) { return true; }

bool lcd_print(int16_t line, const char* fmt, ...) {
    printf("[LCD %d] ", line);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
    return true;
}

bool lcd_clear(void) { return true; }
bool lcd_clear_line(int16_t line) { return true; }

bool lcd_set_text(int16_t line, const char* text) {
    printf("[LCD %d] %s\n", line, text);
    return true;
}

// ── C++ API ───────────────────────────────────────────────────────────────────

namespace pros { namespace lcd {

bool initialize()      { return lcd_initialize(); }
bool is_initialized()  { return true; }
bool shutdown()        { return true; }

bool print(int16_t line, const char* fmt, ...) {
    printf("[LCD %d] ", line);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
    return true;
}

bool clear()                    { return true; }
bool clear_line(int16_t line)   { return true; }

bool set_text(int16_t line, const std::string& text) {
    printf("[LCD %d] %s\n", line, text.c_str());
    return true;
}

}} // namespace pros::lcd
