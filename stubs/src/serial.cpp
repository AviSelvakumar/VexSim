// Stub implementations for VEX Generic Serial
#include "pros/serial.h"
#include "pros/serial.hpp"
#include "pros/error.h"

// ── C API ─────────────────────────────────────────────────────────────────────

#ifdef __cplusplus
extern "C" {
namespace pros {
namespace c {
#endif

int32_t serial_enable(uint8_t port)                                        { return PROS_ERR; }
int32_t serial_set_baudrate(uint8_t port, int32_t baudrate)                { return PROS_ERR; }
int32_t serial_flush(uint8_t port)                                         { return PROS_ERR; }
int32_t serial_get_read_avail(uint8_t port)                                { return PROS_ERR; }
int32_t serial_get_write_free(uint8_t port)                                { return PROS_ERR; }
int32_t serial_peek_byte(uint8_t port)                                     { return PROS_ERR; }
int32_t serial_read_byte(uint8_t port)                                     { return PROS_ERR; }
int32_t serial_read(uint8_t port, uint8_t* buffer, int32_t length)         { return PROS_ERR; }
int32_t serial_write_byte(uint8_t port, uint8_t buffer)                    { return PROS_ERR; }
int32_t serial_write(uint8_t port, uint8_t* buffer, int32_t length)        { return PROS_ERR; }

#ifdef __cplusplus
} // namespace c
} // namespace pros
} // extern "C"
#endif

// ── C++ Serial class ──────────────────────────────────────────────────────────

namespace pros {

Serial::Serial(std::uint8_t port, std::int32_t baudrate) : Device(port) {}
Serial::Serial(std::uint8_t port) : Device(port) {}

std::int32_t Serial::set_baudrate(std::int32_t baudrate) const    { return PROS_ERR; }
std::int32_t Serial::flush() const                                { return PROS_ERR; }
std::int32_t Serial::get_read_avail() const                       { return PROS_ERR; }
std::int32_t Serial::get_write_free() const                       { return PROS_ERR; }
std::int32_t Serial::peek_byte() const                            { return PROS_ERR; }
std::int32_t Serial::read_byte() const                            { return PROS_ERR; }
std::int32_t Serial::read(std::uint8_t* buffer, std::int32_t length) const { return PROS_ERR; }
std::int32_t Serial::write_byte(std::uint8_t buffer) const        { return PROS_ERR; }
std::int32_t Serial::write(std::uint8_t* buffer, std::int32_t length) const { return PROS_ERR; }

namespace literals {
const pros::Serial operator"" _ser(const unsigned long long int m) {
    return pros::Serial(static_cast<std::uint8_t>(m));
}
}

} // namespace pros
