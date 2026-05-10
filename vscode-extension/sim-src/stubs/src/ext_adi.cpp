// Stub implementations for the 3-Wire Expander ADI
#include "pros/ext_adi.h"
#include "pros/error.h"

#ifdef __cplusplus
extern "C" {
namespace pros {
namespace c {
#endif

// ── General ADI ───────────────────────────────────────────────────────────────
adi_port_config_e_t ext_adi_port_get_config(uint8_t smart_port, uint8_t adi_port)             { return E_ADI_ERR; }
int32_t ext_adi_port_get_value(uint8_t smart_port, uint8_t adi_port)                          { return PROS_ERR; }
int32_t ext_adi_port_set_config(uint8_t smart_port, uint8_t adi_port, adi_port_config_e_t t)  { return PROS_ERR; }
int32_t ext_adi_port_set_value(uint8_t smart_port, uint8_t adi_port, int32_t value)           { return PROS_ERR; }

// ── Analog ────────────────────────────────────────────────────────────────────
int32_t ext_adi_analog_calibrate(uint8_t smart_port, uint8_t adi_port)                        { return PROS_ERR; }
int32_t ext_adi_analog_read(uint8_t smart_port, uint8_t adi_port)                             { return PROS_ERR; }
int32_t ext_adi_analog_read_calibrated(uint8_t smart_port, uint8_t adi_port)                  { return PROS_ERR; }
int32_t ext_adi_analog_read_calibrated_HR(uint8_t smart_port, uint8_t adi_port)               { return PROS_ERR; }

// ── Digital ───────────────────────────────────────────────────────────────────
int32_t ext_adi_digital_read(uint8_t smart_port, uint8_t adi_port)                            { return PROS_ERR; }
int32_t ext_adi_digital_get_new_press(uint8_t smart_port, uint8_t adi_port)                   { return PROS_ERR; }
int32_t ext_adi_digital_write(uint8_t smart_port, uint8_t adi_port, bool value)               { return PROS_ERR; }
int32_t ext_adi_pin_mode(uint8_t smart_port, uint8_t adi_port, uint8_t mode)                  { return PROS_ERR; }

// ── Motor ─────────────────────────────────────────────────────────────────────
int32_t ext_adi_motor_set(uint8_t smart_port, uint8_t adi_port, int8_t speed)                 { return PROS_ERR; }
int32_t ext_adi_motor_get(uint8_t smart_port, uint8_t adi_port)                               { return PROS_ERR; }
int32_t ext_adi_motor_stop(uint8_t smart_port, uint8_t adi_port)                              { return PROS_ERR; }

// ── Encoder ───────────────────────────────────────────────────────────────────
ext_adi_encoder_t ext_adi_encoder_init(uint8_t smart_port, uint8_t adi_port_top,
                                       uint8_t adi_port_bottom, bool reverse)                 { return PROS_ERR; }
int32_t ext_adi_encoder_get(ext_adi_encoder_t enc)                                            { return PROS_ERR; }
int32_t ext_adi_encoder_reset(ext_adi_encoder_t enc)                                          { return PROS_ERR; }
int32_t ext_adi_encoder_shutdown(ext_adi_encoder_t enc)                                       { return PROS_ERR; }

// ── Ultrasonic ────────────────────────────────────────────────────────────────
ext_adi_ultrasonic_t ext_adi_ultrasonic_init(uint8_t smart_port, uint8_t adi_port_ping,
                                             uint8_t adi_port_echo)                           { return PROS_ERR; }
int32_t ext_adi_ultrasonic_get(ext_adi_ultrasonic_t ult)                                      { return PROS_ERR; }
int32_t ext_adi_ultrasonic_shutdown(ext_adi_ultrasonic_t ult)                                 { return PROS_ERR; }

// ── Gyro ──────────────────────────────────────────────────────────────────────
ext_adi_gyro_t ext_adi_gyro_init(uint8_t smart_port, uint8_t adi_port, double multiplier)     { return PROS_ERR; }
double  ext_adi_gyro_get(ext_adi_gyro_t gyro)                                                 { return PROS_ERR_F; }
int32_t ext_adi_gyro_reset(ext_adi_gyro_t gyro)                                               { return PROS_ERR; }
int32_t ext_adi_gyro_shutdown(ext_adi_gyro_t gyro)                                            { return PROS_ERR; }

// ── Potentiometer ─────────────────────────────────────────────────────────────
ext_adi_potentiometer_t ext_adi_potentiometer_init(uint8_t smart_port, uint8_t adi_port,
                                                   adi_potentiometer_type_e_t t)              { return PROS_ERR; }
double ext_adi_potentiometer_get_angle(ext_adi_potentiometer_t p)                             { return PROS_ERR_F; }

// ── LED ───────────────────────────────────────────────────────────────────────
ext_adi_led_t ext_adi_led_init(uint8_t smart_port, uint8_t adi_port)                          { return PROS_ERR; }
int32_t ext_adi_led_clear_all(ext_adi_led_t led, uint32_t* buf, uint32_t len)                 { return PROS_ERR; }
int32_t ext_adi_led_set(ext_adi_led_t led, uint32_t* buf, uint32_t len)                       { return PROS_ERR; }
int32_t ext_adi_led_set_all(ext_adi_led_t led, uint32_t* buf, uint32_t len, uint32_t color)   { return PROS_ERR; }
int32_t ext_adi_led_set_pixel(ext_adi_led_t led, uint32_t* buf, uint32_t len, uint32_t color,
                              uint32_t pixel_position)                                         { return PROS_ERR; }
int32_t ext_adi_led_clear_pixel(ext_adi_led_t led, uint32_t* buf, uint32_t len,
                                uint32_t pixel_position)                                       { return PROS_ERR; }

#ifdef __cplusplus
} // namespace c
} // namespace pros
} // extern "C"
#endif
