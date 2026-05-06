#pragma once
#include "abstract_motor.hpp"
#include "device.hpp"
#include "motors.h"
#include <vector>
#include <initializer_list>

namespace pros {
inline namespace v5 {

class Motor : public AbstractMotor, public Device {
public:
    explicit Motor(int8_t port,
                   motor_gearset_e_t gearset = E_MOTOR_GEARSET_18,
                   bool reversed = false,
                   motor_encoder_units_e_t encoder_units = E_MOTOR_ENCODER_DEGREES);

    int32_t move(int32_t voltage) override;
    int32_t move_absolute(double position, int32_t velocity) override;
    int32_t move_relative(double position, int32_t velocity) override;
    int32_t move_velocity(int32_t velocity) override;
    int32_t move_voltage(int32_t voltage) override;
    int32_t brake() override;
    int32_t modify_profiled_velocity(int32_t velocity) override;
    double  get_target_position() const override;
    int32_t get_target_velocity() const override;
    double  get_actual_velocity() const override;
    int32_t get_current_draw() const override;
    int32_t get_direction() const override;
    double  get_efficiency() const override;
    int32_t is_over_current() const override;
    int32_t is_over_temp() const override;
    int32_t is_stopped() const override;
    int32_t get_zero_velocity_flag() const override;
    int32_t get_faults() const override;
    int32_t get_flags() const override;
    double  get_position() const override;
    double  get_power() const override;
    int32_t get_raw_position(uint32_t* timestamp) const override;
    double  get_temperature() const override;
    double  get_torque() const override;
    int32_t get_voltage() const override;
    int32_t tare_position() override;
    int32_t set_brake_mode(motor_brake_mode_e_t mode) override;
    int32_t set_current_limit(int32_t limit) override;
    int32_t set_encoder_units(motor_encoder_units_e_t units) override;
    int32_t set_gearing(motor_gearset_e_t gearset) override;
    int32_t set_reversed(bool reversed) override;
    int32_t set_voltage_limit(int32_t limit) override;
    motor_brake_mode_e_t    get_brake_mode() const override;
    motor_encoder_units_e_t get_encoder_units() const override;
    motor_gearset_e_t       get_gearing() const override;
    int32_t is_reversed() const override;
};

class MotorGroup : public AbstractMotor {
public:
    explicit MotorGroup(std::initializer_list<int8_t> ports);
    explicit MotorGroup(std::vector<int8_t> ports);

    int32_t move(int32_t voltage) override;
    int32_t move_absolute(double position, int32_t velocity) override;
    int32_t move_relative(double position, int32_t velocity) override;
    int32_t move_velocity(int32_t velocity) override;
    int32_t move_voltage(int32_t voltage) override;
    int32_t brake() override;
    int32_t modify_profiled_velocity(int32_t velocity) override;
    double  get_target_position() const override;
    int32_t get_target_velocity() const override;
    double  get_actual_velocity() const override;
    int32_t get_current_draw() const override;
    int32_t get_direction() const override;
    double  get_efficiency() const override;
    int32_t is_over_current() const override;
    int32_t is_over_temp() const override;
    int32_t is_stopped() const override;
    int32_t get_zero_velocity_flag() const override;
    int32_t get_faults() const override;
    int32_t get_flags() const override;
    double  get_position() const override;
    double  get_power() const override;
    int32_t get_raw_position(uint32_t* timestamp) const override;
    double  get_temperature() const override;
    double  get_torque() const override;
    int32_t get_voltage() const override;
    int32_t tare_position() override;
    int32_t set_brake_mode(motor_brake_mode_e_t mode) override;
    int32_t set_current_limit(int32_t limit) override;
    int32_t set_encoder_units(motor_encoder_units_e_t units) override;
    int32_t set_gearing(motor_gearset_e_t gearset) override;
    int32_t set_reversed(bool reversed) override;
    int32_t set_voltage_limit(int32_t limit) override;
    motor_brake_mode_e_t    get_brake_mode() const override;
    motor_encoder_units_e_t get_encoder_units() const override;
    motor_gearset_e_t       get_gearing() const override;
    int32_t is_reversed() const override;

private:
    std::vector<int8_t> ports_;
};

} // namespace v5
} // namespace pros
