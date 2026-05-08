#pragma once
#include <array>

namespace sim {

struct RobotConfig {
    double track_width_px  = 60.0;   // pixels between left/right wheel centers
    double wheel_radius_px = 15.0;   // pixel radius of each drive wheel
    double max_rpm         = 200.0;  // baseline gearset RPM
    double accel_time_constant = 0.15; // seconds to reach ~63% of max speed (powered / braking)
    double coast_time_constant = 1.5;  // seconds to coast to rest (motor disconnected, friction only)
    double field_w         = 720.0;  // field width in pixels
    double field_h         = 720.0;  // field height in pixels
    double robot_half_w    = 20.0;   // half robot width for boundary clamping
    double robot_half_h    = 20.0;   // half robot height for boundary clamping

    // Left drive motor ports (0 = end of list, up to 4 motors per side)
    std::array<int, 4> left_ports  = {1, 0, 0, 0};
    std::array<int, 4> right_ports = {2, 0, 0, 0};

    // Optional: dedicated tracking wheel ports (Rotation sensors on separate ports).
    // Set to 0 to disable. Physics will update these ports' encoder positions so
    // pros::Rotation::get_position() returns correct centidegrees for odometry.
    // tracking_wheel_radius_px must be set to match the physical tracking wheel size.
    int    tracking_left_port      = 0;   // port of left  tracking wheel rotation sensor
    int    tracking_right_port     = 0;   // port of right tracking wheel rotation sensor
    int    tracking_mid_port       = 0;   // port of middle (strafe) tracking wheel (always 0 for diff drive)
    double tracking_wheel_radius_px = 15.0; // pixel radius of tracking wheels
};

class Physics {
public:
    explicit Physics(const RobotConfig& cfg);
    void step(double dt_sec);

private:
    RobotConfig cfg_;
    double vL_{0.0};  // current left  wheel velocity (px/s)
    double vR_{0.0};  // current right wheel velocity (px/s)

    // Returns average effective voltage across ports, with each motor applying its own
    // back-EMF when in brake mode. speed_frac = current side velocity / max_speed (-1..1).
    double averageEffectiveVoltage(const std::array<int, 4>& ports, double speed_frac) const;

    // Returns true if every active port on this side is in COAST mode.
    bool allCoast(const std::array<int, 4>& ports) const;
};

} // namespace sim
