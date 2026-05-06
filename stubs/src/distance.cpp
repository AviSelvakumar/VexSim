// Stub implementations for VEX Distance Sensor
#include "pros/distance.h"
#include "pros/distance.hpp"
#include "pros/error.h"

// ── C API ─────────────────────────────────────────────────────────────────────

#ifdef __cplusplus
extern "C" {
namespace pros {
namespace c {
#endif

int32_t distance_get(uint8_t port)              { return PROS_ERR; }
int32_t distance_get_confidence(uint8_t port)   { return PROS_ERR; }
int32_t distance_get_object_size(uint8_t port)  { return PROS_ERR; }
double  distance_get_object_velocity(uint8_t port) { return PROS_ERR_F; }

#ifdef __cplusplus
} // namespace c
} // namespace pros
} // extern "C"
#endif

// ── C++ Distance class ────────────────────────────────────────────────────────

namespace pros {
inline namespace v5 {

Distance::Distance(const std::uint8_t port) : Device(port) {}

std::int32_t Distance::get()                   { return PROS_ERR; }
std::int32_t Distance::get_distance()          { return PROS_ERR; }
std::int32_t Distance::get_confidence()        { return PROS_ERR; }
std::int32_t Distance::get_object_size()       { return PROS_ERR; }
double       Distance::get_object_velocity()   { return PROS_ERR_F; }
std::vector<Distance> Distance::get_all_devices() { return {}; }

std::ostream& operator<<(std::ostream& os, pros::Distance& d) {
    return os << "Distance [port: " << (int)d.get_port() << "]";
}

namespace literals {
const pros::Distance operator"" _dist(const unsigned long long int d) {
    return pros::Distance(static_cast<std::uint8_t>(d));
}
}

} // inline namespace v5
} // namespace pros
