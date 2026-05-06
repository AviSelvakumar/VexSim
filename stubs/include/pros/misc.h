#pragma once
#include <stdint.h>
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum controller_id_e {
    E_CONTROLLER_MASTER = 0,
    E_CONTROLLER_PARTNER = 1,
} controller_id_e_t;

typedef enum controller_analog_e {
    E_CONTROLLER_ANALOG_LEFT_X  = 0,
    E_CONTROLLER_ANALOG_LEFT_Y  = 1,
    E_CONTROLLER_ANALOG_RIGHT_X = 2,
    E_CONTROLLER_ANALOG_RIGHT_Y = 3,
} controller_analog_e_t;

typedef enum controller_digital_e {
    E_CONTROLLER_DIGITAL_L1    = 6,
    E_CONTROLLER_DIGITAL_L2    = 7,
    E_CONTROLLER_DIGITAL_R1    = 8,
    E_CONTROLLER_DIGITAL_R2    = 9,
    E_CONTROLLER_DIGITAL_UP    = 10,
    E_CONTROLLER_DIGITAL_DOWN  = 11,
    E_CONTROLLER_DIGITAL_LEFT  = 12,
    E_CONTROLLER_DIGITAL_RIGHT = 13,
    E_CONTROLLER_DIGITAL_X     = 14,
    E_CONTROLLER_DIGITAL_B     = 15,
    E_CONTROLLER_DIGITAL_Y     = 16,
    E_CONTROLLER_DIGITAL_A     = 17,
} controller_digital_e_t;

namespace pros { namespace c {
    int32_t controller_get_analog(controller_id_e_t id, controller_analog_e_t channel);
    int32_t controller_get_digital(controller_id_e_t id, controller_digital_e_t button);
    int32_t controller_get_digital_new_press(controller_id_e_t id, controller_digital_e_t button);
    int32_t controller_is_connected(controller_id_e_t id);
    int32_t controller_rumble(controller_id_e_t id, const char* rumble_pattern);
    int32_t controller_print(controller_id_e_t id, uint8_t line, uint8_t col, const char* fmt, ...);
    int32_t controller_clear(controller_id_e_t id);
    int32_t controller_clear_line(controller_id_e_t id, uint8_t line);
    uint32_t competition_get_status(void);
    uint8_t  competition_is_autonomous(void);
    uint8_t  competition_is_connected(void);
    uint8_t  competition_is_disabled(void);
}} // namespace pros::c

#ifdef __cplusplus
}
#endif
