#define _USE_MATH_DEFINES
#include "pros/imu.h"
#include "pros/imu.hpp"
#include "sim/SimState.hpp"
#include <cmath>

static constexpr double PI = 3.14159265358979323846;

// Convert simulation heading (radians, math convention) to VEX compass heading
// VEX: 0-360 degrees, clockwise, 0 = North (up on field)
static double sim_to_vex_heading(double heading_rad) {
    // Math angle: 0=right, CCW positive
    // Compass: 0=up, CW positive
    double deg = heading_rad * (180.0 / PI);
    double compass = fmod(90.0 - deg, 360.0);
    if (compass < 0) compass += 360.0;
    return compass;
}

namespace pros { namespace c {

int32_t imu_reset(uint8_t port) {
    // No-op: calibration is instant in simulation
    return 1;
}

int32_t imu_set_data_rate(uint8_t port, uint32_t rate) { return 1; }

double imu_get_rotation(uint8_t port) {
    auto& state = sim::SimState::get();
    std::lock_guard<std::mutex> lock(state.pose_mutex);
    return -state.pose.heading_accumulated * (180.0 / PI);
}

double imu_get_heading(uint8_t port) {
    auto& state = sim::SimState::get();
    std::lock_guard<std::mutex> lock(state.pose_mutex);
    return sim_to_vex_heading(state.pose.heading);
}

double imu_get_roll(uint8_t port)  { return 0.0; }
double imu_get_pitch(uint8_t port) { return 0.0; }
double imu_get_yaw(uint8_t port)   { return imu_get_heading(port); }

imu_gyro_s_t imu_get_gyro_rate(uint8_t port) {
    return {0.0, 0.0, 0.0};
}

imu_accel_s_t imu_get_accel(uint8_t port) {
    return {0.0, 0.0, 1.0}; // 1g down
}

imu_status_e_t imu_get_status(uint8_t port) {
    return static_cast<imu_status_e_t>(0); // ready
}

int32_t imu_tare_heading(uint8_t port) {
    auto& state = sim::SimState::get();
    std::lock_guard<std::mutex> lock(state.pose_mutex);
    state.pose.heading = 0.0;
    return 1;
}

int32_t imu_tare_rotation(uint8_t port) {
    auto& state = sim::SimState::get();
    std::lock_guard<std::mutex> lock(state.pose_mutex);
    state.pose.heading_accumulated = 0.0;
    return 1;
}

int32_t imu_tare_pitch(uint8_t port) { return 1; }
int32_t imu_tare_roll(uint8_t port)  { return 1; }
int32_t imu_tare_yaw(uint8_t port)   { return imu_tare_heading(port); }

int32_t imu_tare(uint8_t port) {
    imu_tare_heading(port);
    imu_tare_rotation(port);
    return 1;
}

int32_t imu_set_heading(uint8_t port, double target) {
    auto& state = sim::SimState::get();
    std::lock_guard<std::mutex> lock(state.pose_mutex);
    // Convert target compass heading back to math angle
    double rad = (90.0 - target) * (PI / 180.0);
    state.pose.heading = rad;
    return 1;
}

int32_t imu_set_rotation(uint8_t port, double target) {
    auto& state = sim::SimState::get();
    std::lock_guard<std::mutex> lock(state.pose_mutex);
    state.pose.heading_accumulated = -target * (PI / 180.0);
    return 1;
}

}} // namespace pros::c

namespace pros { inline namespace v5 {

Imu::Imu(uint8_t port) : Device(port) {}

int32_t Imu::reset() const             { return pros::c::imu_reset(_port); }
int32_t Imu::set_data_rate(uint32_t r) const { return pros::c::imu_set_data_rate(_port, r); }
double  Imu::get_rotation() const      { return pros::c::imu_get_rotation(_port) - rotation_offset_; }
double  Imu::get_heading() const       { return pros::c::imu_get_heading(_port); }
double  Imu::get_roll() const          { return 0.0; }
double  Imu::get_pitch() const         { return 0.0; }
double  Imu::get_yaw() const           { return pros::c::imu_get_yaw(_port); }
imu_gyro_s_t  Imu::get_gyro_rate() const { return pros::c::imu_get_gyro_rate(_port); }
imu_accel_s_t Imu::get_accel() const  { return pros::c::imu_get_accel(_port); }
ImuStatus Imu::get_status() const { return ImuStatus::ready; }
bool    Imu::is_calibrating() const    { return false; }
int32_t Imu::tare_heading() const      { return pros::c::imu_tare_heading(_port); }
int32_t Imu::tare_rotation() const     { return pros::c::imu_tare_rotation(_port); }
int32_t Imu::tare_pitch() const        { return 1; }
int32_t Imu::tare_roll() const         { return 1; }
int32_t Imu::tare_yaw() const          { return pros::c::imu_tare_yaw(_port); }
int32_t Imu::tare() const              { return pros::c::imu_tare(_port); }
int32_t Imu::set_heading(double t) const   { return pros::c::imu_set_heading(_port, t); }
int32_t Imu::set_rotation(double t) const  { return pros::c::imu_set_rotation(_port, t); }

}} // namespace pros::v5
