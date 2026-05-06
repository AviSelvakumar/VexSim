#pragma once
#include "imu.h"
#include "device.hpp"

namespace pros {
inline namespace v5 {

class Imu : public Device {
public:
    explicit Imu(uint8_t port);

    int32_t reset() const;
    int32_t set_data_rate(uint32_t rate) const;
    double  get_rotation() const;
    double  get_heading() const;
    double  get_roll() const;
    double  get_pitch() const;
    double  get_yaw() const;
    imu_gyro_s_t  get_gyro_rate() const;
    imu_accel_s_t get_accel() const;
    imu_status_e_t get_status() const;
    bool    is_calibrating() const;
    int32_t tare_heading() const;
    int32_t tare_rotation() const;
    int32_t tare_pitch() const;
    int32_t tare_roll() const;
    int32_t tare_yaw() const;
    int32_t tare() const;
    int32_t set_heading(double target) const;
    int32_t set_rotation(double target) const;

    // Convenience alias for calibrate in some codebases
    int32_t calibrate() const { return reset(); }

private:
    double heading_offset_{0.0};
    double rotation_offset_{0.0};
};

} // namespace v5
} // namespace pros
