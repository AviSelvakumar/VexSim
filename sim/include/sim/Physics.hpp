#pragma once
#include <array>

namespace sim {

struct RobotConfig {
    double track_width_px  = 60.0;   // pixels between left/right wheel centers
    double wheel_radius_px = 15.0;   // pixel radius of each wheel
    double max_rpm         = 200.0;  // baseline gearset RPM
    double field_w         = 720.0;  // field width in pixels
    double field_h         = 720.0;  // field height in pixels
    double robot_half_w    = 20.0;   // half robot width for boundary clamping
    double robot_half_h    = 20.0;   // half robot height for boundary clamping

    // Left drive motor ports (0 = end of list, up to 4 motors per side)
    std::array<int, 4> left_ports  = {1, 0, 0, 0};
    std::array<int, 4> right_ports = {2, 0, 0, 0};
};

class Physics {
public:
    explicit Physics(const RobotConfig& cfg);
    void step(double dt_sec);

private:
    RobotConfig cfg_;

    double averageVoltage(const std::array<int, 4>& ports) const;
};

} // namespace sim
