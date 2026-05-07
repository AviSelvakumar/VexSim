#define _USE_MATH_DEFINES
#include "sim/Physics.hpp"
#include "sim/SimState.hpp"
#include <cmath>
#include <algorithm>

namespace sim {

Physics::Physics(const RobotConfig& cfg) : cfg_(cfg) {}

double Physics::averageVoltage(const std::array<int, 4>& ports) const {
    double sum = 0.0;
    int count  = 0;
    for (int port : ports) {
        if (port <= 0) break;
        sum += SimState::get().motors[port].voltage.load();
        ++count;
    }
    return count > 0 ? sum / count : 0.0;
}

void Physics::step(double dt_sec) {
    auto& state = SimState::get();

    double left_v  = averageVoltage(cfg_.left_ports);   // -127..127
    double right_v = averageVoltage(cfg_.right_ports);

    // Max linear wheel speed in pixels/sec
    double wheel_circ = 2.0 * M_PI * cfg_.wheel_radius_px;
    double max_speed  = (cfg_.max_rpm / 60.0) * wheel_circ;

    double vL = (left_v  / 127.0) * max_speed;
    double vR = (right_v / 127.0) * max_speed;

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
