#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool    lcd_initialize(void);
bool    lcd_is_initialized(void);
bool    lcd_shutdown(void);
bool    lcd_print(int16_t line, const char* fmt, ...);
bool    lcd_clear(void);
bool    lcd_clear_line(int16_t line);
bool    lcd_set_text(int16_t line, const char* text);

#ifdef __cplusplus
}
#endif
