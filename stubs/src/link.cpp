// Stub implementations for VEX Link (robot-to-robot radio)
#include "pros/link.h"
#include "pros/link.hpp"
#include "pros/error.h"

// ── C API ─────────────────────────────────────────────────────────────────────

#ifdef __cplusplus
extern "C" {
namespace pros {
namespace c {
#endif

uint32_t link_init(uint8_t port, const char* link_id, link_type_e_t type) { return PROS_ERR; }
uint32_t link_init_override(uint8_t port, const char* link_id, link_type_e_t type) { return PROS_ERR; }
bool     link_connected(uint8_t port)                             { return false; }
uint32_t link_raw_receivable_size(uint8_t port)                   { return PROS_ERR; }
uint32_t link_raw_transmittable_size(uint8_t port)                { return PROS_ERR; }
uint32_t link_transmit_raw(uint8_t port, void* data, uint16_t data_size) { return PROS_ERR; }
uint32_t link_receive_raw(uint8_t port, void* dest, uint16_t data_size)  { return PROS_ERR; }
uint32_t link_transmit(uint8_t port, void* data, uint16_t data_size)     { return PROS_ERR; }
uint32_t link_receive(uint8_t port, void* dest, uint16_t data_size)      { return PROS_ERR; }
uint32_t link_clear_receive_buf(uint8_t port)                    { return PROS_ERR; }

#ifdef __cplusplus
} // namespace c
} // namespace pros
} // extern "C"
#endif

// ── C++ Link class ────────────────────────────────────────────────────────────

namespace pros {

Link::Link(const std::uint8_t port, const std::string link_id, link_type_e_t type, bool ov)
    : Device(port) {}

bool          Link::connected()                                  { return false; }
std::uint32_t Link::raw_receivable_size()                        { return PROS_ERR; }
std::uint32_t Link::raw_transmittable_size()                     { return PROS_ERR; }
std::uint32_t Link::transmit_raw(void* data, std::uint16_t data_size) { return PROS_ERR; }
std::uint32_t Link::receive_raw(void* dest, std::uint16_t data_size)  { return PROS_ERR; }
std::uint32_t Link::transmit(void* data, std::uint16_t data_size)     { return PROS_ERR; }
std::uint32_t Link::receive(void* dest, std::uint16_t data_size)      { return PROS_ERR; }
std::uint32_t Link::clear_receive_buf()                          { return PROS_ERR; }

} // namespace pros
