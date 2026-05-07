#pragma once
#include "abstract_motor.hpp"
#include "device.hpp"
#include "motors.h"
#include <vector>
#include <initializer_list>

namespace pros {

// Bring global C enum types and constants into pros:: namespace
using motor_brake_mode_e    = ::motor_brake_mode_e_t;
using motor_gearset_e       = ::motor_gearset_e_t;
using motor_encoder_units_e = ::motor_encoder_units_e_t;

// Enum constants in pros:: (LemLib uses pros::E_MOTOR_BRAKE_HOLD etc.)
using ::E_MOTOR_BRAKE_COAST;
using ::E_MOTOR_BRAKE_BRAKE;
using ::E_MOTOR_BRAKE_HOLD;
using ::E_MOTOR_BRAKE_INVALID;
using ::E_MOTOR_GEARSET_36;
using ::E_MOTOR_GEAR_RED;
using ::E_MOTOR_GEARSET_18;
using ::E_MOTOR_GEAR_GREEN;
using ::E_MOTOR_GEARSET_06;
using ::E_MOTOR_GEAR_BLUE;
using ::E_MOTOR_GEAR_INVALID;
using ::E_MOTOR_ENCODER_DEGREES;
using ::E_MOTOR_ENCODER_ROTATIONS;
using ::E_MOTOR_ENCODER_COUNTS;
using ::E_MOTOR_ENCODER_INVALID;

inline namespace v5 {

// ── New-style enum classes (LemLib / PROS 4 API) ──────────────────────────────
enum class MotorGears {
    rpm_100 = 0,  red     = 0,   // 100 RPM (red cartridge)
    rpm_200 = 1,  green   = 1,   // 200 RPM (green cartridge)
    rpm_600 = 2,  blue    = 2,   // 600 RPM (blue cartridge)
    invalid = -1
};
enum class MotorUnits {
    degrees   = 0,
    rotations = 1,
    counts    = 2,
    invalid   = -1
};
// Alias used by some LemLib versions
using MotorEncoderUnits = MotorUnits;
// Alias matching PROS 4 naming (MotorGearset)
using MotorGearset = MotorGears;

// ── Brake mode enum class ─────────────────────────────────────────────────────
enum class MotorBrake {
    coast   = 0,
    brake   = 1,
    hold    = 2,
    invalid = -1
};

class Motor : public AbstractMotor, public Device {
public:
    // Old-style constructor (motor_gearset_e_t)
    explicit Motor(int8_t port,
                   motor_gearset_e_t gearset = E_MOTOR_GEARSET_18,
                   bool reversed = false,
                   motor_encoder_units_e_t encoder_units = E_MOTOR_ENCODER_DEGREES);

    // New-style constructor (MotorGears / MotorUnits) — used by LemLib
    Motor(const std::int8_t port,
          const MotorGears gearset,
          const MotorUnits encoder_units = MotorUnits::degrees);
    Motor(const std::int8_t port,
          const MotorUnits encoder_units,
          const MotorGears gearset = MotorGears::green);

    int32_t move(int32_t voltage) const override;
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
    int32_t set_brake_mode_all(motor_brake_mode_e_t mode) override;
    int32_t set_brake_mode_all(MotorBrake mode) { return set_brake_mode_all(static_cast<motor_brake_mode_e_t>(mode)); }
    std::vector<MotorBrake> get_brake_mode_all() const { return { static_cast<MotorBrake>(get_brake_mode()) }; }
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
    // LemLib constructor: MotorGroup({1,-2,3}, pros::MotorGears::blue)
    MotorGroup(std::initializer_list<int8_t> ports, MotorGears gearset,
               MotorUnits units = MotorUnits::degrees);
    MotorGroup(std::vector<int8_t> ports, MotorGears gearset,
               MotorUnits units = MotorUnits::degrees);

    int32_t move(int32_t voltage) const override;
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
    int32_t tare_position_all() { return tare_position(); }
    int32_t set_brake_mode(motor_brake_mode_e_t mode) override;
    int32_t set_brake_mode_all(motor_brake_mode_e_t mode) override;
    int32_t set_brake_mode_all(MotorBrake mode) { return set_brake_mode_all(static_cast<motor_brake_mode_e_t>(mode)); }
    std::vector<MotorBrake> get_brake_mode_all() const { return { static_cast<MotorBrake>(get_brake_mode()) }; }
    int32_t set_current_limit(int32_t limit) override;
    int32_t set_encoder_units(motor_encoder_units_e_t units) override;
    int32_t set_encoder_units_all(motor_encoder_units_e_t units) { return set_encoder_units(units); }
    int32_t set_gearing(motor_gearset_e_t gearset) override;
    int32_t set_reversed(bool reversed) override;
    int32_t set_voltage_limit(int32_t limit) override;
    motor_brake_mode_e_t    get_brake_mode() const override;
    motor_encoder_units_e_t get_encoder_units() const override;
    motor_gearset_e_t       get_gearing() const override;
    std::vector<MotorGears> get_gearing_all() const { return { static_cast<MotorGears>(get_gearing()) }; }
    std::vector<double>     get_position_all() const;
    int32_t is_reversed() const override;
    int size() const { return static_cast<int>(ports_.size()); }

private:
    std::vector<int8_t> ports_;
};

} // namespace v5
} // namespace pros
