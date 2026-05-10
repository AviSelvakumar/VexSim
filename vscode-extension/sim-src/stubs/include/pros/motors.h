#pragma once
#include <stdint.h>
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum motor_gearset_e {
    E_MOTOR_GEARSET_36 = 0,   // 100 RPM
    E_MOTOR_GEAR_RED   = E_MOTOR_GEARSET_36,
    E_MOTOR_GEARSET_18 = 1,   // 200 RPM (default)
    E_MOTOR_GEAR_GREEN = E_MOTOR_GEARSET_18,
    E_MOTOR_GEARSET_06 = 2,   // 600 RPM
    E_MOTOR_GEAR_BLUE  = E_MOTOR_GEARSET_06,
    E_MOTOR_GEAR_INVALID = INT32_MAX,
} motor_gearset_e_t;

typedef enum motor_encoder_units_e {
    E_MOTOR_ENCODER_DEGREES    = 0,
    E_MOTOR_ENCODER_ROTATIONS  = 1,
    E_MOTOR_ENCODER_COUNTS     = 2,
    E_MOTOR_ENCODER_INVALID    = INT32_MAX,
} motor_encoder_units_e_t;

typedef enum motor_brake_mode_e {
    E_MOTOR_BRAKE_COAST   = 0,
    E_MOTOR_BRAKE_BRAKE   = 1,
    E_MOTOR_BRAKE_HOLD    = 2,
    E_MOTOR_BRAKE_INVALID = INT32_MAX,
} motor_brake_mode_e_t;

namespace pros { namespace c {
    int32_t motor_move(int8_t port, int32_t voltage);
    int32_t motor_move_absolute(int8_t port, double position, int32_t velocity);
    int32_t motor_move_relative(int8_t port, double position, int32_t velocity);
    int32_t motor_move_velocity(int8_t port, int32_t velocity);
    int32_t motor_move_voltage(int8_t port, int32_t voltage);
    int32_t motor_brake(int8_t port);
    int32_t motor_modify_profiled_velocity(int8_t port, int32_t velocity);
    double  motor_get_target_position(int8_t port);
    int32_t motor_get_target_velocity(int8_t port);
    double  motor_get_actual_velocity(int8_t port);
    int32_t motor_get_current_draw(int8_t port);
    int32_t motor_get_direction(int8_t port);
    double  motor_get_efficiency(int8_t port);
    int32_t motor_is_over_current(int8_t port);
    int32_t motor_is_over_temp(int8_t port);
    int32_t motor_is_stopped(int8_t port);
    int32_t motor_get_zero_velocity_flag(int8_t port);
    int32_t motor_get_faults(int8_t port);
    int32_t motor_get_flags(int8_t port);
    double  motor_get_position(int8_t port);
    double  motor_get_power(int8_t port);
    int32_t motor_get_raw_position(int8_t port, uint32_t* timestamp);
    double  motor_get_temperature(int8_t port);
    double  motor_get_torque(int8_t port);
    int32_t motor_get_voltage(int8_t port);
    int32_t motor_tare_position(int8_t port);
    int32_t motor_set_brake_mode(int8_t port, motor_brake_mode_e_t mode);
    int32_t motor_set_current_limit(int8_t port, int32_t limit);
    int32_t motor_set_encoder_units(int8_t port, motor_encoder_units_e_t units);
    int32_t motor_set_gearing(int8_t port, motor_gearset_e_t gearset);
    int32_t motor_set_reversed(int8_t port, bool reversed);
    int32_t motor_set_voltage_limit(int8_t port, int32_t limit);
    motor_brake_mode_e_t    motor_get_brake_mode(int8_t port);
    motor_encoder_units_e_t motor_get_encoder_units(int8_t port);
    motor_gearset_e_t       motor_get_gearing(int8_t port);
    int32_t motor_is_reversed(int8_t port);
}} // namespace pros::c

#ifdef __cplusplus
}
#endif
