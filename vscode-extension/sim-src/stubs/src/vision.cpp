// Stub implementations for VEX Vision Sensor
#include "pros/vision.h"
#include "pros/vision.hpp"
#include "pros/error.h"
#include <cstring>

// ── C API ─────────────────────────────────────────────────────────────────────

#ifdef __cplusplus
extern "C" {
namespace pros {
#endif

static vision_object_s_t err_obj() {
    vision_object_s_t o{};
    o.signature = VISION_OBJECT_ERR_SIG;
    return o;
}

static vision_signature_s_t err_sig() {
    vision_signature_s_t s{};
    return s;
}

int32_t vision_get_object_count(uint8_t port)                                          { return PROS_ERR; }
vision_object_s_t vision_get_by_size(uint8_t port, uint32_t size_id)                   { return err_obj(); }
vision_object_s_t vision_get_by_sig(uint8_t port, uint32_t size_id, uint32_t sig_id)   { return err_obj(); }
vision_object_s_t vision_get_by_code(uint8_t port, uint32_t size_id, vision_color_code_t color_code) { return err_obj(); }
int32_t vision_read_by_size(uint8_t port, uint32_t size_id, uint32_t object_count, vision_object_s_t* object_arr) { return PROS_ERR; }
int32_t vision_read_by_sig(uint8_t port, uint32_t size_id, uint32_t sig_id, uint32_t object_count, vision_object_s_t* object_arr) { return PROS_ERR; }
int32_t vision_read_by_code(uint8_t port, uint32_t size_id, vision_color_code_t color_code, uint32_t object_count, vision_object_s_t* object_arr) { return PROS_ERR; }
vision_signature_s_t vision_get_signature(uint8_t port, uint8_t sig_id)                { return err_sig(); }
int32_t vision_set_signature(uint8_t port, uint8_t sig_id, vision_signature_s_t* ptr)  { return PROS_ERR; }
int32_t vision_print_signature(const vision_signature_s_t sig)                         { return PROS_ERR; }
vision_signature_s_t vision_signature_from_utility(int32_t id, int32_t u_min, int32_t u_max, int32_t u_mean, int32_t v_min, int32_t v_max, int32_t v_mean, float range, int32_t type) { return err_sig(); }
vision_color_code_t vision_create_color_code(uint8_t port, uint32_t sig_id1, uint32_t sig_id2, uint32_t sig_id3, uint32_t sig_id4, uint32_t sig_id5) { return 0; }
int32_t vision_get_exposure(uint8_t port)                                              { return PROS_ERR; }
int32_t vision_set_exposure(uint8_t port, uint8_t exposure)                           { return PROS_ERR; }
int32_t vision_get_white_balance(uint8_t port)                                        { return PROS_ERR; }
int32_t vision_set_white_balance(uint8_t port, int32_t rgb)                           { return PROS_ERR; }
int32_t vision_set_auto_white_balance(uint8_t port, uint8_t enable)                   { return PROS_ERR; }
int32_t vision_clear_led(uint8_t port)                                                { return PROS_ERR; }
int32_t vision_set_led(uint8_t port, int32_t rgb)                                     { return PROS_ERR; }
int32_t vision_set_zero_point(uint8_t port, vision_zero_e_t zero_point)               { return PROS_ERR; }
int32_t vision_set_wifi_mode(uint8_t port, uint8_t enable)                            { return PROS_ERR; }

#ifdef __cplusplus
} // namespace pros
} // extern "C"
#endif

// ── C++ Vision class ──────────────────────────────────────────────────────────

namespace pros {
inline namespace v5 {

Vision::Vision(std::uint8_t port, vision_zero_e_t zero_point) : Device(port) {}

std::int32_t Vision::clear_led(void) const                                            { return PROS_ERR; }
vision_signature_s_t Vision::signature_from_utility(int32_t id, int32_t u_min, int32_t u_max, int32_t u_mean, int32_t v_min, int32_t v_max, int32_t v_mean, float range, int32_t type) { return {}; }
vision_color_code_t Vision::create_color_code(uint32_t sig_id1, uint32_t sig_id2, uint32_t sig_id3, uint32_t sig_id4, uint32_t sig_id5) const { return 0; }
std::vector<Vision> Vision::get_all_devices()                                         { return {}; }
vision_object_s_t Vision::get_by_size(uint32_t size_id) const                        { vision_object_s_t o{}; o.signature = VISION_OBJECT_ERR_SIG; return o; }
vision_object_s_t Vision::get_by_sig(uint32_t size_id, uint32_t sig_id) const        { vision_object_s_t o{}; o.signature = VISION_OBJECT_ERR_SIG; return o; }
vision_object_s_t Vision::get_by_code(uint32_t size_id, vision_color_code_t cc) const{ vision_object_s_t o{}; o.signature = VISION_OBJECT_ERR_SIG; return o; }
std::int32_t Vision::get_exposure(void) const                                         { return PROS_ERR; }
std::int32_t Vision::get_object_count(void) const                                     { return PROS_ERR; }
vision_signature_s_t Vision::get_signature(uint8_t sig_id) const                     { return {}; }
std::int32_t Vision::get_white_balance(void) const                                    { return PROS_ERR; }
std::int32_t Vision::read_by_size(uint32_t size_id, uint32_t object_count, vision_object_s_t* object_arr) const { return PROS_ERR; }
std::int32_t Vision::read_by_sig(uint32_t size_id, uint32_t sig_id, uint32_t object_count, vision_object_s_t* object_arr) const { return PROS_ERR; }
std::int32_t Vision::read_by_code(uint32_t size_id, vision_color_code_t cc, uint32_t object_count, vision_object_s_t* object_arr) const { return PROS_ERR; }
std::int32_t Vision::print_signature(const vision_signature_s_t sig)                  { return PROS_ERR; }
std::int32_t Vision::set_auto_white_balance(uint8_t enable) const                    { return PROS_ERR; }
std::int32_t Vision::set_exposure(uint8_t exposure) const                            { return PROS_ERR; }
std::int32_t Vision::set_led(int32_t rgb) const                                      { return PROS_ERR; }
std::int32_t Vision::set_signature(uint8_t sig_id, vision_signature_s_t* ptr) const  { return PROS_ERR; }
std::int32_t Vision::set_white_balance(int32_t rgb) const                            { return PROS_ERR; }
std::int32_t Vision::set_zero_point(vision_zero_e_t zero_point) const               { return PROS_ERR; }
std::int32_t Vision::set_wifi_mode(const std::uint8_t enable) const                 { return PROS_ERR; }

} // inline namespace v5
} // namespace pros
