#pragma once
#include <stdint.h>
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct imu_gyro_s {
    double x, y, z;
} imu_gyro_s_t;

typedef struct imu_accel_s {
    double x, y, z;
} imu_accel_s_t;

typedef enum imu_status_e {
    E_IMU_STATUS_CALIBRATING = 0x01,
    E_IMU_STATUS_ERROR       = 0xFF,
    E_IMU_STATUS_NUM         = 0x02,
} imu_status_e_t;

namespace pros { namespace c {
    int32_t imu_reset(uint8_t port);
    int32_t imu_set_data_rate(uint8_t port, uint32_t rate);
    double  imu_get_rotation(uint8_t port);
    double  imu_get_heading(uint8_t port);
    double  imu_get_roll(uint8_t port);
    double  imu_get_pitch(uint8_t port);
    double  imu_get_yaw(uint8_t port);
    imu_gyro_s_t  imu_get_gyro_rate(uint8_t port);
    imu_accel_s_t imu_get_accel(uint8_t port);
    imu_status_e_t imu_get_status(uint8_t port);
    int32_t imu_tare_heading(uint8_t port);
    int32_t imu_tare_rotation(uint8_t port);
    int32_t imu_tare_pitch(uint8_t port);
    int32_t imu_tare_roll(uint8_t port);
    int32_t imu_tare_yaw(uint8_t port);
    int32_t imu_tare(uint8_t port);
    int32_t imu_set_heading(uint8_t port, double target);
    int32_t imu_set_rotation(uint8_t port, double target);
}} // namespace pros::c

#ifdef __cplusplus
}
#endif
