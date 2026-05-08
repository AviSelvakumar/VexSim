#define _USE_MATH_DEFINES
#include "sim/Physics.hpp"
#include "sim/SimState.hpp"
#include <cmath>
#include <algorithm>

namespace sim {

Physics::Physics(const RobotConfig& cfg) : cfg_(cfg) {
    // Goal positions in field inches (origin = bottom-left, Y up).
    // Derived from 2026-27 Appendix A diagram (field = 140.41" × 140.41").
    // Collision radius: half of short-goal base width (5.61") = 2.81"
    const double S  = cfg_.field_w / 140.41; // px per inch
    const double FH = cfg_.field_h;
    const double R  = 3.22 * S;              // collision radius = outer octagonal circumradius (px)

    struct { double xi, yi; } pos[9] = {
        // Red alliance goals (left wall side)
        {23.11,  23.11}, {23.11, 117.30},
        // Blue alliance goals (right wall side)
        {117.30, 23.11}, {117.30, 117.30},
        // Neutral short goals (inner quadrant corners)
        {48.24, 48.24}, {48.24, 92.17},
        {92.17, 48.24}, {92.17, 92.17},
        // Center tall goal
        {70.20, 70.20},
    };
    for (int i = 0; i < 9; ++i)
        goals_[i] = { pos[i].xi * S, FH - pos[i].yi * S, R };
}

double Physics::averageEffectiveVoltage(const std::array<int, 4>& ports, double speed_frac) const {
    double sum = 0.0;
    int count  = 0;
    for (int port : ports) {
        if (port <= 0) break;
        sum += SimState::get().motors[port].get_effective_voltage(speed_frac, cfg_.gear_friction_coeff);
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

        // Goal collision: circle-circle push-out.
        // Robot is approximated as a circle (conservative circumscribed radius).
        const double rr = std::sqrt(cfg_.robot_half_w * cfg_.robot_half_w +
                                    cfg_.robot_half_h * cfg_.robot_half_h);
        for (const auto& g : goals_) {
            double dx = state.pose.x - g.x;
            double dy = state.pose.y - g.y;
            double minDist = rr + g.r;
            double dist2   = dx * dx + dy * dy;
            if (dist2 >= minDist * minDist) continue;

            double dist = std::sqrt(dist2);
            double nx = dist > 1e-9 ? dx / dist : 1.0;
            double ny = dist > 1e-9 ? dy / dist : 0.0;

            // Positional correction: push robot out of overlap
            double push = minDist - dist;
            state.pose.x += nx * push;
            state.pose.y += ny * push;

            // Cancel velocity directed into the goal.
            double v_lin = (vL_ + vR_) * 0.5;
            double vx    = v_lin * std::cos(state.pose.heading);
            double vy    = v_lin * std::sin(state.pose.heading);
            double vn    = vx * nx + vy * ny;   // + = moving away from goal
            if (vn < 0.0) {
                vL_ = 0.0;
                vR_ = 0.0;
            }
        }

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
