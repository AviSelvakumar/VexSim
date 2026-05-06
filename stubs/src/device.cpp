// Stub implementations for pros::Device and C device API
#include "pros/device.h"
#include "pros/device.hpp"
#include <vector>

// ── C API ─────────────────────────────────────────────────────────────────────

namespace pros::c {
extern "C" {

v5_device_e_t get_plugged_type(uint8_t port) {
    return E_DEVICE_UNDEFINED;
}

} // extern "C"
} // namespace pros::c

// ── C++ Device class ──────────────────────────────────────────────────────────

namespace pros {
inline namespace v5 {

Device::Device(const std::uint8_t port)
    : _port(port), _deviceType(DeviceType::undefined) {}

std::uint8_t Device::get_port(void) const {
    return _port;
}

bool Device::is_installed() {
    return true; // simulator: assume all configured devices are present
}

pros::DeviceType Device::get_plugged_type() const {
    return _deviceType;
}

pros::DeviceType Device::get_plugged_type(std::uint8_t port) {
    return pros::DeviceType::undefined;
}

std::vector<Device> Device::get_all_devices(pros::DeviceType device_type) {
    return {};
}

} // inline namespace v5
} // namespace pros
