#define _USE_MATH_DEFINES
#include "sim/Physics.hpp"
#include "sim/SimState.hpp"
#include <cmath>
#include <algorithm>

namespace sim {

Physics::Physics(const RobotConfig& cfg) : cfg_(cfg) {}

double Physics::averageEffectiveVoltage(const std::array<int, 4>& ports, double speed_frac) const {
    double sum = 0.0;
    int count  = 0;
    for (int port : ports) {
        if (port <= 0) break;
        sum += SimState::get().motors[port].get_effective_voltage(speed_frac);
        ++count;
    }
    return count > 0 ? sum / count : 0.0;
}

bool Physics::allCoast(const std::array<int, 4>& ports) const {
    for (int port : ports) {
        if (port <= 0) break;
        if (SimState::get().motors[port].brake_mode_int.load() >= 1) return false;
    }
    return true;
}

void Physics::step(double dt_sec) {
    auto& state = SimState::get();

    // Max linear wheel speed in pixels/sec
    double wheel_circ = 2.0 * M_PI * cfg_.wheel_radius_px;
    double max_speed  = (cfg_.max_rpm / 60.0) * wheel_circ;

    // Motors report their own effective voltage:
    //   - Powered:     commanded voltage (normal drive)
    //   - BRAKE/HOLD + unpowered: back-EMF opposing current motion (motor generates this)
    //   - COAST + unpowered:      0 (motor disconnected)
    double eff_vL = averageEffectiveVoltage(cfg_.left_ports,  vL_ / max_speed);
    double eff_vR = averageEffectiveVoltage(cfg_.right_ports, vR_ / max_speed);

    double target_vL = (eff_vL / 127.0) * max_speed;
    double target_vR = (eff_vR / 127.0) * max_speed;

    // COAST uses a long friction-only tau; powered/braking uses the motor tau.
    double tauL = (std::abs(eff_vL) < 1e-3 && allCoast(cfg_.left_ports))
                  ? cfg_.coast_time_constant : cfg_.accel_time_constant;
    double tauR = (std::abs(eff_vR) < 1e-3 && allCoast(cfg_.right_ports))
                  ? cfg_.coast_time_constant : cfg_.accel_time_constant;

    vL_ += (target_vL - vL_) * std::min(1.0, dt_sec / tauL);
    vR_ += (target_vR - vR_) * std::min(1.0, dt_sec / tauR);

    double vL = vL_;
    double vR = vR_;

    // Differential drive kinematics
    double v     = (vL + vR) / 2.0;                       // linear velocity px/s
    double omega = (vR - vL) / cfg_.track_width_px;       // angular velocity rad/s

    {
        std::lock_guard<std::mutex> lock(state.pose_mutex);
        state.pose.x       += v * std::cos(state.pose.heading) * dt_sec;
        state.pose.y       += v * std::sin(state.pose.heading) * dt_sec;
        state.pose.heading += omega * dt_sec;
        state.pose.heading_accumulated += omega * dt_sec;

        // Clamp to field bounds
        state.pose.x = std::clamp(state.pose.x,
                                  cfg_.robot_half_w,
                                  cfg_.field_w - cfg_.robot_half_w);
        state.pose.y = std::clamp(state.pose.y,
                                  cfg_.robot_half_h,
                                  cfg_.field_h - cfg_.robot_half_h);
    }

    // Update motor encoder positions
    for (int port : cfg_.left_ports) {
        if (port <= 0) break;
        double deg_per_sec = (vL / wheel_circ) * 360.0;
        state.motors[port].add_position_deg(deg_per_sec * dt_sec);
    }
    for (int port : cfg_.right_ports) {
        if (port <= 0) break;
        double deg_per_sec = (vR / wheel_circ) * 360.0;
        state.motors[port].add_position_deg(deg_per_sec * dt_sec);
    }

    // Update dedicated tracking wheel rotation sensors (non-motor ports).
    // These use their own wheel radius, which may differ from the drive wheels.
    double tw_circ = 2.0 * M_PI * cfg_.tracking_wheel_radius_px;
    if (cfg_.tracking_left_port > 0) {
        double deg_per_sec = (vL / tw_circ) * 360.0;
        state.motors[cfg_.tracking_left_port].add_position_deg(deg_per_sec * dt_sec);
    }
    if (cfg_.tracking_right_port > 0) {
        double deg_per_sec = (vR / tw_circ) * 360.0;
        state.motors[cfg_.tracking_right_port].add_position_deg(deg_per_sec * dt_sec);
    }
    // Middle (strafe) tracking wheel: in a diff-drive model, lateral velocity = 0.
    // The port is updated but will always accumulate 0, which is correct —
    // LemLib uses it to detect sideways slip, and an ideal sim has none.
    // (Leave tracking_mid_port at 0 for diff-drive robots.)

    // Advance simulation clock
    state.sim_millis.fetch_add(static_cast<uint32_t>(dt_sec * 1000.0));
}

} // namespace sim
