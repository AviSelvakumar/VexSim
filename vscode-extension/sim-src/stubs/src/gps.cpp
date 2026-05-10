// Stub implementations for VEX GPS Sensor
#include "pros/gps.h"
#include "pros/gps.hpp"
#include "pros/error.h"

// ── C API ─────────────────────────────────────────────────────────────────────

#ifdef __cplusplus
extern "C" {
namespace pros {
#endif

int32_t gps_initialize_full(uint8_t port, double xI, double yI,
                            double hI, double xO, double yO)        { return PROS_ERR; }
int32_t gps_set_offset(uint8_t port, double xO, double yO)          { return PROS_ERR; }
gps_position_s_t gps_get_offset(uint8_t port)                       { gps_position_s_t r{}; return r; }
int32_t gps_set_position(uint8_t port, double xI, double yI, double hI) { return PROS_ERR; }
int32_t gps_set_data_rate(uint8_t port, uint32_t rate)              { return PROS_ERR; }
double  gps_get_error(uint8_t port)                                  { return PROS_ERR_F; }
gps_status_s_t gps_get_position_and_orientation(uint8_t port)       { gps_status_s_t r{}; return r; }
gps_position_s_t gps_get_position(uint8_t port)                     { gps_position_s_t r{}; return r; }
double  gps_get_position_x(uint8_t port)                            { return PROS_ERR_F; }
double  gps_get_position_y(uint8_t port)                            { return PROS_ERR_F; }
gps_orientation_s_t gps_get_orientation(uint8_t port)               { gps_orientation_s_t r{}; return r; }
double  gps_get_pitch(uint8_t port)                                  { return PROS_ERR_F; }
double  gps_get_roll(uint8_t port)                                   { return PROS_ERR_F; }
double  gps_get_yaw(uint8_t port)                                    { return PROS_ERR_F; }
double  gps_get_heading(uint8_t port)                                { return PROS_ERR_F; }
double  gps_get_heading_raw(uint8_t port)                            { return PROS_ERR_F; }
gps_gyro_s_t gps_get_gyro_rate(uint8_t port)                        { gps_gyro_s_t r{}; return r; }
double  gps_get_gyro_rate_x(uint8_t port)                           { return PROS_ERR_F; }
double  gps_get_gyro_rate_y(uint8_t port)                           { return PROS_ERR_F; }
double  gps_get_gyro_rate_z(uint8_t port)                           { return PROS_ERR_F; }
gps_accel_s_t gps_get_accel(uint8_t port)                           { gps_accel_s_t r{}; return r; }
double  gps_get_accel_x(uint8_t port)                               { return PROS_ERR_F; }
double  gps_get_accel_y(uint8_t port)                               { return PROS_ERR_F; }
double  gps_get_accel_z(uint8_t port)                               { return PROS_ERR_F; }

#ifdef __cplusplus
} // namespace pros
} // extern "C"
#endif

// ── C++ Gps class ─────────────────────────────────────────────────────────────

namespace pros {
inline namespace v5 {

std::int32_t Gps::initialize_full(double xI, double yI, double hI,
                                   double xO, double yO) const       { return PROS_ERR; }
std::int32_t Gps::set_offset(double xO, double yO) const            { return PROS_ERR; }
pros::gps_position_s_t Gps::get_offset() const                      { pros::gps_position_s_t r{}; return r; }
std::int32_t Gps::set_position(double xI, double yI, double hI) const { return PROS_ERR; }
std::int32_t Gps::set_data_rate(std::uint32_t rate) const           { return PROS_ERR; }
double Gps::get_error() const                                        { return PROS_ERR_F; }
pros::gps_status_s_t Gps::get_position_and_orientation() const      { pros::gps_status_s_t r{}; return r; }
pros::gps_position_s_t Gps::get_position() const                    { pros::gps_position_s_t r{}; return r; }
double Gps::get_position_x() const                                   { return PROS_ERR_F; }
double Gps::get_position_y() const                                   { return PROS_ERR_F; }
pros::gps_orientation_s_t Gps::get_orientation() const              { pros::gps_orientation_s_t r{}; return r; }
double Gps::get_pitch() const                                        { return PROS_ERR_F; }
double Gps::get_roll() const                                         { return PROS_ERR_F; }
double Gps::get_yaw() const                                          { return PROS_ERR_F; }
double Gps::get_heading() const                                      { return PROS_ERR_F; }
double Gps::get_heading_raw() const                                  { return PROS_ERR_F; }
pros::gps_gyro_s_t Gps::get_gyro_rate() const                       { pros::gps_gyro_s_t r{}; return r; }
double Gps::get_gyro_rate_x() const                                  { return PROS_ERR_F; }
double Gps::get_gyro_rate_y() const                                  { return PROS_ERR_F; }
double Gps::get_gyro_rate_z() const                                  { return PROS_ERR_F; }
pros::gps_accel_s_t Gps::get_accel() const                          { pros::gps_accel_s_t r{}; return r; }
double Gps::get_accel_x() const                                      { return PROS_ERR_F; }
double Gps::get_accel_y() const                                      { return PROS_ERR_F; }
double Gps::get_accel_z() const                                      { return PROS_ERR_F; }

std::vector<Gps> Gps::get_all_devices()                             { return {}; }

Gps Gps::get_gps() { return Gps(21); }

std::ostream& operator<<(std::ostream& os, const pros::Gps& gps) {
    return os << "Gps [port: " << (int)gps.get_port() << "]";
}

namespace literals {
const pros::Gps operator"" _gps(const unsigned long long int g) {
    return pros::Gps(static_cast<std::uint8_t>(g));
}
}

} // inline namespace v5
} // namespace pros
