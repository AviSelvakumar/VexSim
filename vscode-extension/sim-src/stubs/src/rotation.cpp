#include "pros/rotation.hpp"
#include "pros/rotation.h"
#include "pros/error.h"
#include "sim/SimState.hpp"

namespace pros { inline namespace v5 {

Rotation::Rotation(const std::int8_t port)
    : Device(static_cast<std::uint8_t>(port < 0 ? -port : port)) {}

// non-const reset (matches real PROS header)
std::int32_t Rotation::reset() {
    int idx = static_cast<int>(_port);
    if (idx < 1 || idx > 21) return PROS_ERR;
    sim::SimState::get().motors[idx].set_position_deg(0.0);
    return 1;
}

std::int32_t Rotation::set_data_rate(std::uint32_t r) const { return 1; }

std::int32_t Rotation::set_position(std::uint32_t pos) const {
    int idx = static_cast<int>(_port);
    if (idx < 1 || idx > 21) return PROS_ERR;
    sim::SimState::get().motors[idx].set_position_deg(pos / 100.0);
    return 1;
}

std::int32_t Rotation::reset_position(void) const {
    int idx = static_cast<int>(_port);
    if (idx < 1 || idx > 21) return PROS_ERR;
    sim::SimState::get().motors[idx].set_position_deg(0.0);
    return 1;
}

std::vector<Rotation> Rotation::get_all_devices() { return {}; }

std::int32_t Rotation::get_position() const {
    int idx = static_cast<int>(_port);
    if (idx < 1 || idx > 21) return PROS_ERR;
    // centidegrees
    return static_cast<int32_t>(sim::SimState::get().motors[idx].get_position_deg() * 100.0);
}

std::int32_t Rotation::get_velocity() const { return 0; }

std::int32_t Rotation::get_angle() const {
    std::int32_t pos = get_position();
    if (pos == PROS_ERR) return PROS_ERR;
    int32_t angle = pos % 36000;
    return angle < 0 ? angle + 36000 : angle;
}

std::int32_t Rotation::set_reversed(bool v) const { return 1; }
std::int32_t Rotation::reverse() const            { return 1; }
std::int32_t Rotation::get_reversed() const       { return 0; }

std::ostream& operator<<(std::ostream& os, pros::Rotation& r) {
    return os << "Rotation [port: " << (int)r.get_port()
              << ", position: " << r.get_position() << " centideg]";
}

namespace literals {
const pros::Rotation operator"" _rot(const unsigned long long int r) {
    return pros::Rotation(static_cast<std::int8_t>(r));
}
}

}} // namespace pros::v5
