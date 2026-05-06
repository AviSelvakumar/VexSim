// Stub implementations for VEX Optical Sensor
#include "pros/optical.h"
#include "pros/optical.hpp"
#include "pros/error.h"

// ── C API ─────────────────────────────────────────────────────────────────────

#ifdef __cplusplus
extern "C" {
namespace pros { namespace c {
#endif

double optical_get_hue(uint8_t port)                          { return PROS_ERR_F; }
double optical_get_saturation(uint8_t port)                   { return PROS_ERR_F; }
double optical_get_brightness(uint8_t port)                   { return PROS_ERR_F; }
int32_t optical_get_proximity(uint8_t port)                   { return PROS_ERR; }
int32_t optical_set_led_pwm(uint8_t port, uint8_t value)      { return PROS_ERR; }
int32_t optical_get_led_pwm(uint8_t port)                     { return PROS_ERR; }
optical_rgb_s_t optical_get_rgb(uint8_t port)                 { optical_rgb_s_t r{}; r.red = PROS_ERR_F; r.green = PROS_ERR_F; r.blue = PROS_ERR_F; r.brightness = PROS_ERR_F; return r; }
optical_raw_s_t optical_get_raw(uint8_t port)                 { optical_raw_s_t r{}; return r; }
optical_direction_e_t optical_get_gesture(uint8_t port)       { return optical_direction_e_t::ERROR; }
optical_gesture_s_t optical_get_gesture_raw(uint8_t port)     { optical_gesture_s_t r{}; return r; }
int32_t optical_enable_gesture(uint8_t port)                  { return PROS_ERR; }
int32_t optical_disable_gesture(uint8_t port)                 { return PROS_ERR; }
double optical_get_integration_time(uint8_t port)             { return PROS_ERR_F; }
int32_t optical_set_integration_time(uint8_t port, double t)  { return PROS_ERR; }

#ifdef __cplusplus
}} // namespace pros::c
} // extern "C"
#endif

// ── C++ Optical class ─────────────────────────────────────────────────────────

namespace pros {
inline namespace v5 {

Optical::Optical(const std::uint8_t port) : Device(port) {}

double  Optical::get_hue()                         { return PROS_ERR_F; }
double  Optical::get_saturation()                  { return PROS_ERR_F; }
double  Optical::get_brightness()                  { return PROS_ERR_F; }
std::int32_t Optical::get_proximity()              { return PROS_ERR; }
std::int32_t Optical::set_led_pwm(uint8_t value)   { return PROS_ERR; }
std::int32_t Optical::get_led_pwm()                { return PROS_ERR; }

pros::c::optical_rgb_s_t Optical::get_rgb() {
    pros::c::optical_rgb_s_t r{};
    r.red = PROS_ERR_F; r.green = PROS_ERR_F; r.blue = PROS_ERR_F; r.brightness = PROS_ERR_F;
    return r;
}

pros::c::optical_raw_s_t Optical::get_raw() {
    pros::c::optical_raw_s_t r{};
    return r;
}

pros::c::optical_direction_e_t Optical::get_gesture() {
    return pros::c::optical_direction_e_t::ERROR;
}

pros::c::optical_gesture_s_t Optical::get_gesture_raw() {
    pros::c::optical_gesture_s_t r{};
    return r;
}

std::int32_t Optical::enable_gesture()                 { return PROS_ERR; }
std::int32_t Optical::disable_gesture()                { return PROS_ERR; }
double       Optical::get_integration_time()           { return PROS_ERR_F; }
std::int32_t Optical::set_integration_time(double t)   { return PROS_ERR; }

std::vector<Optical> Optical::get_all_devices()        { return {}; }

std::ostream& operator<<(std::ostream& os, pros::Optical& o) {
    return os << "Optical [port: " << (int)o.get_port() << "]";
}

namespace literals {
const pros::Optical operator"" _opt(const unsigned long long int o) {
    return pros::Optical(static_cast<std::uint8_t>(o));
}
}

} // inline namespace v5
} // namespace pros
