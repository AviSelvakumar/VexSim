#pragma once
#include <stdint.h>
#include "error.h"

// ── Enums: declared inside pros:: so that pros::E_CONTROLLER_MASTER works ──
#ifdef __cplusplus
extern "C" {
namespace pros {
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

#ifdef __cplusplus
} // namespace pros
} // extern "C"
#endif

// ── Short-form macros (activated by PROS_USE_SIMPLE_NAMES in main.h) ─────────
#ifdef PROS_USE_SIMPLE_NAMES
#ifdef __cplusplus
#define ANALOG_LEFT_X  pros::E_CONTROLLER_ANALOG_LEFT_X
#define ANALOG_LEFT_Y  pros::E_CONTROLLER_ANALOG_LEFT_Y
#define ANALOG_RIGHT_X pros::E_CONTROLLER_ANALOG_RIGHT_X
#define ANALOG_RIGHT_Y pros::E_CONTROLLER_ANALOG_RIGHT_Y
#define DIGITAL_L1     pros::E_CONTROLLER_DIGITAL_L1
#define DIGITAL_L2     pros::E_CONTROLLER_DIGITAL_L2
#define DIGITAL_R1     pros::E_CONTROLLER_DIGITAL_R1
#define DIGITAL_R2     pros::E_CONTROLLER_DIGITAL_R2
#define DIGITAL_UP     pros::E_CONTROLLER_DIGITAL_UP
#define DIGITAL_DOWN   pros::E_CONTROLLER_DIGITAL_DOWN
#define DIGITAL_LEFT   pros::E_CONTROLLER_DIGITAL_LEFT
#define DIGITAL_RIGHT  pros::E_CONTROLLER_DIGITAL_RIGHT
#define DIGITAL_X      pros::E_CONTROLLER_DIGITAL_X
#define DIGITAL_B      pros::E_CONTROLLER_DIGITAL_B
#define DIGITAL_Y      pros::E_CONTROLLER_DIGITAL_Y
#define DIGITAL_A      pros::E_CONTROLLER_DIGITAL_A
#else
#define ANALOG_LEFT_X  E_CONTROLLER_ANALOG_LEFT_X
#define ANALOG_LEFT_Y  E_CONTROLLER_ANALOG_LEFT_Y
#define ANALOG_RIGHT_X E_CONTROLLER_ANALOG_RIGHT_X
#define ANALOG_RIGHT_Y E_CONTROLLER_ANALOG_RIGHT_Y
#define DIGITAL_L1     E_CONTROLLER_DIGITAL_L1
#define DIGITAL_L2     E_CONTROLLER_DIGITAL_L2
#define DIGITAL_R1     E_CONTROLLER_DIGITAL_R1
#define DIGITAL_R2     E_CONTROLLER_DIGITAL_R2
#define DIGITAL_UP     E_CONTROLLER_DIGITAL_UP
#define DIGITAL_DOWN   E_CONTROLLER_DIGITAL_DOWN
#define DIGITAL_LEFT   E_CONTROLLER_DIGITAL_LEFT
#define DIGITAL_RIGHT  E_CONTROLLER_DIGITAL_RIGHT
#define DIGITAL_X      E_CONTROLLER_DIGITAL_X
#define DIGITAL_B      E_CONTROLLER_DIGITAL_B
#define DIGITAL_Y      E_CONTROLLER_DIGITAL_Y
#define DIGITAL_A      E_CONTROLLER_DIGITAL_A
#endif
#endif

namespace pros { namespace c {
    int32_t controller_get_analog(pros::controller_id_e_t id, pros::controller_analog_e_t channel);
    int32_t controller_get_digital(pros::controller_id_e_t id, pros::controller_digital_e_t button);
    int32_t controller_get_digital_new_press(pros::controller_id_e_t id, pros::controller_digital_e_t button);
    int32_t controller_is_connected(pros::controller_id_e_t id);
    int32_t controller_rumble(pros::controller_id_e_t id, const char* rumble_pattern);
    int32_t controller_print(pros::controller_id_e_t id, uint8_t line, uint8_t col, const char* fmt, ...);
    int32_t controller_clear(pros::controller_id_e_t id);
    int32_t controller_clear_line(pros::controller_id_e_t id, uint8_t line);
    uint32_t competition_get_status(void);
    uint8_t  competition_is_autonomous(void);
    uint8_t  competition_is_connected(void);
    uint8_t  competition_is_disabled(void);
}} // namespace pros::c

#ifdef __cplusplus
// ── Global aliases so unqualified E_CONTROLLER_* still works in sim internals
using pros::controller_id_e_t;
using pros::controller_analog_e_t;
using pros::controller_digital_e_t;
using pros::E_CONTROLLER_MASTER;
using pros::E_CONTROLLER_PARTNER;
using pros::E_CONTROLLER_ANALOG_LEFT_X;
using pros::E_CONTROLLER_ANALOG_LEFT_Y;
using pros::E_CONTROLLER_ANALOG_RIGHT_X;
using pros::E_CONTROLLER_ANALOG_RIGHT_Y;
using pros::E_CONTROLLER_DIGITAL_L1;
using pros::E_CONTROLLER_DIGITAL_L2;
using pros::E_CONTROLLER_DIGITAL_R1;
using pros::E_CONTROLLER_DIGITAL_R2;
using pros::E_CONTROLLER_DIGITAL_UP;
using pros::E_CONTROLLER_DIGITAL_DOWN;
using pros::E_CONTROLLER_DIGITAL_LEFT;
using pros::E_CONTROLLER_DIGITAL_RIGHT;
using pros::E_CONTROLLER_DIGITAL_X;
using pros::E_CONTROLLER_DIGITAL_B;
using pros::E_CONTROLLER_DIGITAL_Y;
using pros::E_CONTROLLER_DIGITAL_A;
#endif
