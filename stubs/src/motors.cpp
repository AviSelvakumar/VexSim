#include "pros/motors.h"
#include "pros/motors.hpp"
#include "sim/SimState.hpp"
#include <cmath>
#include <algorithm>

// ── C API ─────────────────────────────────────────────────────────────────────

static int abs_port(int8_t port) { return port < 0 ? -port : port; }
static bool is_valid(int8_t port) {
    int p = abs_port(port);
    return p >= 1 && p <= 21;
}

namespace pros { namespace c {

int32_t motor_move(int8_t port, int32_t voltage) {
    if (!is_valid(port)) return PROS_ERR;
    int idx = abs_port(port);
    auto& m = sim::SimState::get().motors[idx];
    int clamped = std::clamp(voltage, -127, 127);
    // Apply reversal: port sign XOR SimState reversed flag
    bool rev = (port < 0) ^ m.reversed.load();
    m.voltage.store(rev ? -clamped : clamped);
    m.mode = sim::MotorState::Mode::VOLTAGE;
    return 1;
}

int32_t motor_move_velocity(int8_t port, int32_t velocity) {
    if (!is_valid(port)) return PROS_ERR;
    int idx = abs_port(port);
    auto& m = sim::SimState::get().motors[idx];
    m.target_velocity.store(port < 0 ? -velocity : velocity);
    m.mode = sim::MotorState::Mode::VELOCITY;
    // Map velocity to voltage proportionally (200 RPM gearset baseline)
    int v = static_cast<int>((velocity / 200.0) * 127.0);
    v = std::clamp(v, -127, 127);
    m.voltage.store(port < 0 ? -v : v);
    return 1;
}

int32_t motor_move_voltage(int8_t port, int32_t voltage_mv) {
    if (!is_valid(port)) return PROS_ERR;
    int idx = abs_port(port);
    auto& m = sim::SimState::get().motors[idx];
    int clamped = std::clamp(voltage_mv, -12000, 12000);
    m.millivolts.store(port < 0 ? -clamped : clamped);
    m.mode = sim::MotorState::Mode::MILLIVOLTS;
    // Store equivalent -127..127 voltage for physics
    int v = static_cast<int>((clamped / 12000.0) * 127.0);
    m.voltage.store(v);
    return 1;
}

int32_t motor_move_absolute(int8_t port, double position, int32_t velocity) {
    return motor_move_velocity(port, velocity); // simplified
}

int32_t motor_move_relative(int8_t port, double position, int32_t velocity) {
    return motor_move_velocity(port, velocity); // simplified
}

int32_t motor_brake(int8_t port) {
    if (!is_valid(port)) return PROS_ERR;
    int idx = abs_port(port);
    auto& m = sim::SimState::get().motors[idx];
    m.voltage.store(0);
    m.target_velocity.store(0);
    m.millivolts.store(0);
    return 1;
}

int32_t motor_modify_profiled_velocity(int8_t port, int32_t velocity) {
    return motor_move_velocity(port, velocity);
}

double  motor_get_target_position(int8_t port) { return 0.0; }
int32_t motor_get_target_velocity(int8_t port) {
    if (!is_valid(port)) return PROS_ERR;
    return sim::SimState::get().motors[abs_port(port)].target_velocity.load();
}

double motor_get_actual_velocity(int8_t port) {
    if (!is_valid(port)) return PROS_ERR_F;
    // Approximate from current voltage (simplified)
    int v = sim::SimState::get().motors[abs_port(port)].voltage.load();
    return (v / 127.0) * 200.0;
}

int32_t motor_get_current_draw(int8_t port)  { return 0; }
int32_t motor_get_direction(int8_t port)     { return 1; }
double  motor_get_efficiency(int8_t port)    { return 100.0; }
int32_t motor_is_over_current(int8_t port)   { return 0; }
int32_t motor_is_over_temp(int8_t port)      { return 0; }
int32_t motor_is_stopped(int8_t port)        { return 0; }
int32_t motor_get_zero_velocity_flag(int8_t port) { return 0; }
int32_t motor_get_faults(int8_t port)        { return 0; }
int32_t motor_get_flags(int8_t port)         { return 0; }

double motor_get_position(int8_t port) {
    if (!is_valid(port)) return PROS_ERR_F;
    return sim::SimState::get().motors[abs_port(port)].get_position_deg();
}

double  motor_get_power(int8_t port)       { return 0.0; }
double  motor_get_temperature(int8_t port) { return 25.0; }
double  motor_get_torque(int8_t port)      { return 0.0; }

int32_t motor_get_voltage(int8_t port) {
    if (!is_valid(port)) return PROS_ERR;
    return sim::SimState::get().motors[abs_port(port)].voltage.load();
}

int32_t motor_get_raw_position(int8_t port, uint32_t* timestamp) {
    if (timestamp) *timestamp = sim::SimState::get().sim_millis.load();
    return (int32_t)motor_get_position(port);
}

int32_t motor_tare_position(int8_t port) {
    if (!is_valid(port)) return PROS_ERR;
    sim::SimState::get().motors[abs_port(port)].set_position_deg(0.0);
    return 1;
}

int32_t motor_set_brake_mode(int8_t port, motor_brake_mode_e_t mode) { return 1; }
int32_t motor_set_current_limit(int8_t port, int32_t limit)          { return 1; }
int32_t motor_set_encoder_units(int8_t port, motor_encoder_units_e_t u) { return 1; }
int32_t motor_set_gearing(int8_t port, motor_gearset_e_t g)          { return 1; }
int32_t motor_set_reversed(int8_t port, bool rev) {
    if (!is_valid(port)) return PROS_ERR;
    sim::SimState::get().motors[abs_port(port)].reversed.store(rev);
    return 1;
}
int32_t motor_set_voltage_limit(int8_t port, int32_t limit) { return 1; }

motor_brake_mode_e_t    motor_get_brake_mode(int8_t port)    { return E_MOTOR_BRAKE_COAST; }
motor_encoder_units_e_t motor_get_encoder_units(int8_t port) { return E_MOTOR_ENCODER_DEGREES; }
motor_gearset_e_t       motor_get_gearing(int8_t port)       { return E_MOTOR_GEARSET_18; }
int32_t motor_is_reversed(int8_t port) {
    if (!is_valid(port)) return PROS_ERR;
    return sim::SimState::get().motors[abs_port(port)].reversed.load() ? 1 : 0;
}

}} // namespace pros::c

// ── C++ Motor class ───────────────────────────────────────────────────────────

namespace pros { inline namespace v5 {

Motor::Motor(int8_t port, motor_gearset_e_t gearset, bool reversed,
             motor_encoder_units_e_t encoder_units)
    : Device(static_cast<std::uint8_t>(port < 0 ? -port : port))
{
    // Negative port number means reversed direction
    if (port < 0 || reversed) {
        sim::SimState::get().motors[_port].reversed.store(true);
    }
}

int32_t Motor::move(int32_t voltage)                         { return pros::c::motor_move(_port, voltage); }
int32_t Motor::move_absolute(double pos, int32_t vel)        { return pros::c::motor_move_absolute(_port, pos, vel); }
int32_t Motor::move_relative(double pos, int32_t vel)        { return pros::c::motor_move_relative(_port, pos, vel); }
int32_t Motor::move_velocity(int32_t velocity)               { return pros::c::motor_move_velocity(_port, velocity); }
int32_t Motor::move_voltage(int32_t voltage)                 { return pros::c::motor_move_voltage(_port, voltage); }
int32_t Motor::brake()                                       { return pros::c::motor_brake(_port); }
int32_t Motor::modify_profiled_velocity(int32_t velocity)    { return pros::c::motor_modify_profiled_velocity(_port, velocity); }
double  Motor::get_target_position() const                   { return pros::c::motor_get_target_position(_port); }
int32_t Motor::get_target_velocity() const                   { return pros::c::motor_get_target_velocity(_port); }
double  Motor::get_actual_velocity() const                   { return pros::c::motor_get_actual_velocity(_port); }
int32_t Motor::get_current_draw() const                      { return pros::c::motor_get_current_draw(_port); }
int32_t Motor::get_direction() const                         { return pros::c::motor_get_direction(_port); }
double  Motor::get_efficiency() const                        { return pros::c::motor_get_efficiency(_port); }
int32_t Motor::is_over_current() const                       { return pros::c::motor_is_over_current(_port); }
int32_t Motor::is_over_temp() const                          { return pros::c::motor_is_over_temp(_port); }
int32_t Motor::is_stopped() const                            { return pros::c::motor_is_stopped(_port); }
int32_t Motor::get_zero_velocity_flag() const                { return pros::c::motor_get_zero_velocity_flag(_port); }
int32_t Motor::get_faults() const                            { return pros::c::motor_get_faults(_port); }
int32_t Motor::get_flags() const                             { return pros::c::motor_get_flags(_port); }
double  Motor::get_position() const                          { return pros::c::motor_get_position(_port); }
double  Motor::get_power() const                             { return pros::c::motor_get_power(_port); }
int32_t Motor::get_raw_position(uint32_t* ts) const          { return pros::c::motor_get_raw_position(_port, ts); }
double  Motor::get_temperature() const                       { return pros::c::motor_get_temperature(_port); }
double  Motor::get_torque() const                            { return pros::c::motor_get_torque(_port); }
int32_t Motor::get_voltage() const                           { return pros::c::motor_get_voltage(_port); }
int32_t Motor::tare_position()                               { return pros::c::motor_tare_position(_port); }
int32_t Motor::set_brake_mode(motor_brake_mode_e_t mode)     { return pros::c::motor_set_brake_mode(_port, mode); }
int32_t Motor::set_current_limit(int32_t limit)              { return pros::c::motor_set_current_limit(_port, limit); }
int32_t Motor::set_encoder_units(motor_encoder_units_e_t u)  { return pros::c::motor_set_encoder_units(_port, u); }
int32_t Motor::set_gearing(motor_gearset_e_t g)              { return pros::c::motor_set_gearing(_port, g); }
int32_t Motor::set_reversed(bool rev)                        { return pros::c::motor_set_reversed(_port, rev); }
int32_t Motor::set_voltage_limit(int32_t limit)              { return pros::c::motor_set_voltage_limit(_port, limit); }
motor_brake_mode_e_t    Motor::get_brake_mode() const        { return pros::c::motor_get_brake_mode(_port); }
motor_encoder_units_e_t Motor::get_encoder_units() const     { return pros::c::motor_get_encoder_units(_port); }
motor_gearset_e_t       Motor::get_gearing() const           { return pros::c::motor_get_gearing(_port); }
int32_t Motor::is_reversed() const                           { return pros::c::motor_is_reversed(_port); }

// ── MotorGroup ────────────────────────────────────────────────────────────────

MotorGroup::MotorGroup(std::initializer_list<int8_t> ports)
    : ports_(ports) {}

MotorGroup::MotorGroup(std::vector<int8_t> ports)
    : ports_(std::move(ports)) {}

int32_t MotorGroup::move(int32_t v) {
    for (auto p : ports_) pros::c::motor_move(p, v);
    return 1;
}
int32_t MotorGroup::move_velocity(int32_t v) {
    for (auto p : ports_) pros::c::motor_move_velocity(p, v);
    return 1;
}
int32_t MotorGroup::move_voltage(int32_t v) {
    for (auto p : ports_) pros::c::motor_move_voltage(p, v);
    return 1;
}
int32_t MotorGroup::move_absolute(double pos, int32_t vel) {
    for (auto p : ports_) pros::c::motor_move_absolute(p, pos, vel);
    return 1;
}
int32_t MotorGroup::move_relative(double pos, int32_t vel) {
    for (auto p : ports_) pros::c::motor_move_relative(p, pos, vel);
    return 1;
}
int32_t MotorGroup::brake() {
    for (auto p : ports_) pros::c::motor_brake(p);
    return 1;
}
int32_t MotorGroup::modify_profiled_velocity(int32_t v) {
    for (auto p : ports_) pros::c::motor_modify_profiled_velocity(p, v);
    return 1;
}
int32_t MotorGroup::tare_position() {
    for (auto p : ports_) pros::c::motor_tare_position(p);
    return 1;
}
int32_t MotorGroup::set_brake_mode(motor_brake_mode_e_t m) {
    for (auto p : ports_) pros::c::motor_set_brake_mode(p, m);
    return 1;
}
int32_t MotorGroup::set_encoder_units(motor_encoder_units_e_t u) {
    for (auto p : ports_) pros::c::motor_set_encoder_units(p, u);
    return 1;
}
int32_t MotorGroup::set_gearing(motor_gearset_e_t g) {
    for (auto p : ports_) pros::c::motor_set_gearing(p, g);
    return 1;
}
int32_t MotorGroup::set_reversed(bool rev) {
    for (auto p : ports_) pros::c::motor_set_reversed(p, rev);
    return 1;
}
int32_t MotorGroup::set_voltage_limit(int32_t l) {
    for (auto p : ports_) pros::c::motor_set_voltage_limit(p, l);
    return 1;
}
int32_t MotorGroup::set_current_limit(int32_t l) {
    for (auto p : ports_) pros::c::motor_set_current_limit(p, l);
    return 1;
}

// Read operations use first port
double  MotorGroup::get_target_position() const { return ports_.empty() ? 0 : pros::c::motor_get_target_position(ports_[0]); }
int32_t MotorGroup::get_target_velocity() const { return ports_.empty() ? 0 : pros::c::motor_get_target_velocity(ports_[0]); }
double  MotorGroup::get_actual_velocity() const { return ports_.empty() ? 0 : pros::c::motor_get_actual_velocity(ports_[0]); }
int32_t MotorGroup::get_current_draw() const    { return 0; }
int32_t MotorGroup::get_direction() const        { return 1; }
double  MotorGroup::get_efficiency() const       { return 100.0; }
int32_t MotorGroup::is_over_current() const      { return 0; }
int32_t MotorGroup::is_over_temp() const         { return 0; }
int32_t MotorGroup::is_stopped() const           { return 0; }
int32_t MotorGroup::get_zero_velocity_flag() const { return 0; }
int32_t MotorGroup::get_faults() const           { return 0; }
int32_t MotorGroup::get_flags() const            { return 0; }
double  MotorGroup::get_position() const         { return ports_.empty() ? 0 : pros::c::motor_get_position(ports_[0]); }
double  MotorGroup::get_power() const            { return 0.0; }
int32_t MotorGroup::get_raw_position(uint32_t* ts) const { return ports_.empty() ? 0 : pros::c::motor_get_raw_position(ports_[0], ts); }
double  MotorGroup::get_temperature() const      { return 25.0; }
double  MotorGroup::get_torque() const           { return 0.0; }
int32_t MotorGroup::get_voltage() const          { return ports_.empty() ? 0 : pros::c::motor_get_voltage(ports_[0]); }
motor_brake_mode_e_t    MotorGroup::get_brake_mode() const    { return E_MOTOR_BRAKE_COAST; }
motor_encoder_units_e_t MotorGroup::get_encoder_units() const { return E_MOTOR_ENCODER_DEGREES; }
motor_gearset_e_t       MotorGroup::get_gearing() const       { return E_MOTOR_GEARSET_18; }
int32_t MotorGroup::is_reversed() const { return 0; }

}} // namespace pros::v5
