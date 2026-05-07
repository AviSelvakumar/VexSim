#pragma once
#include "motors.h"
#include <cstdint>

namespace pros {
inline namespace v5 {

class AbstractMotor {
public:
    virtual ~AbstractMotor() = default;

    virtual int32_t move(int32_t voltage) const = 0;
    virtual int32_t move_absolute(double position, int32_t velocity) = 0;
    virtual int32_t move_relative(double position, int32_t velocity) = 0;
    virtual int32_t move_velocity(int32_t velocity) = 0;
    virtual int32_t move_voltage(int32_t voltage) = 0;
    virtual int32_t brake() = 0;
    virtual int32_t modify_profiled_velocity(int32_t velocity) = 0;
    virtual double  get_target_position() const = 0;
    virtual int32_t get_target_velocity() const = 0;
    virtual double  get_actual_velocity() const = 0;
    virtual int32_t get_current_draw() const = 0;
    virtual int32_t get_direction() const = 0;
    virtual double  get_efficiency() const = 0;
    virtual int32_t is_over_current() const = 0;
    virtual int32_t is_over_temp() const = 0;
    virtual int32_t is_stopped() const = 0;
    virtual int32_t get_zero_velocity_flag() const = 0;
    virtual int32_t get_faults() const = 0;
    virtual int32_t get_flags() const = 0;
    virtual double  get_position() const = 0;
    virtual double  get_power() const = 0;
    virtual int32_t get_raw_position(uint32_t* timestamp) const = 0;
    virtual double  get_temperature() const = 0;
    virtual double  get_torque() const = 0;
    virtual int32_t get_voltage() const = 0;
    virtual int32_t tare_position() = 0;
    virtual int32_t set_brake_mode(motor_brake_mode_e_t mode) = 0;
    virtual int32_t set_brake_mode_all(motor_brake_mode_e_t mode) = 0;
    virtual int32_t set_current_limit(int32_t limit) = 0;
    virtual int32_t set_encoder_units(motor_encoder_units_e_t units) = 0;
    virtual int32_t set_gearing(motor_gearset_e_t gearset) = 0;
    virtual int32_t set_reversed(bool reversed) = 0;
    virtual int32_t set_voltage_limit(int32_t limit) = 0;
    virtual motor_brake_mode_e_t    get_brake_mode() const = 0;
    virtual motor_encoder_units_e_t get_encoder_units() const = 0;
    virtual motor_gearset_e_t       get_gearing() const = 0;
    virtual int32_t is_reversed() const = 0;
};

} // namespace v5
} // namespace pros
