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

} // namespace lcd
} // namespace pros
