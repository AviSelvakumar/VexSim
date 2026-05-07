#pragma once
#include "device.hpp"
#include "error.h"
#include <cstdint>

namespace pros {
inline namespace v5 {

// Stub — all ADI functions return PROS_ERR
class ADIDigitalIn : public Device {
public:
    ADIDigitalIn(uint8_t adi_port) : Device(0), adi_port_(adi_port) {}
    int32_t get_value() const { return PROS_ERR; }
private:
    uint8_t adi_port_;
};

class ADIDigitalOut : public Device {
public:
    ADIDigitalOut(uint8_t adi_port, bool init = false) : Device(0), adi_port_(adi_port) {}
    int32_t set_value(int32_t value) { return PROS_ERR; }
private:
    uint8_t adi_port_;
};

class ADIAnalogIn : public Device {
public:
    ADIAnalogIn(uint8_t adi_port) : Device(0), adi_port_(adi_port) {}
    int32_t get_value() const { return PROS_ERR; }
    int32_t get_value_calibrated() const { return PROS_ERR; }
    int32_t calibrate() const { return PROS_ERR; }
private:
    uint8_t adi_port_;
};

class ADIEncoder : public Device {
public:
    ADIEncoder(uint8_t top_port, uint8_t bottom_port, bool reversed = false)
        : Device(0), top_(top_port), bottom_(bottom_port) {}
    int32_t get_value() const { return PROS_ERR; }
    int32_t reset() { return PROS_ERR; }
private:
    uint8_t top_, bottom_;
};

} // namespace v5

// ── pros::adi namespace — PROS 4 shortened class names ───────────────────────
namespace adi {

class Port {
public:
    explicit Port(uint8_t adi_port) : adi_port_(adi_port) {}
    virtual ~Port() = default;
    virtual int32_t set_value(int value) const { return PROS_ERR; }
    virtual int32_t get_value() const { return PROS_ERR; }
protected:
    uint8_t adi_port_;
};

class DigitalOut : public Port {
public:
    explicit DigitalOut(uint8_t adi_port, bool init_state = false)
        : Port(adi_port), state_(init_state) {}
    int32_t set_value(int value) const override { return PROS_ERR; }
private:
    mutable bool state_;
};

class DigitalIn : public Port {
public:
    explicit DigitalIn(uint8_t adi_port) : Port(adi_port) {}
    int32_t get_value() const override { return PROS_ERR; }
};

class AnalogIn : public Port {
public:
    explicit AnalogIn(uint8_t adi_port) : Port(adi_port) {}
    int32_t get_value() const override { return PROS_ERR; }
    int32_t get_value_calibrated() const { return PROS_ERR; }
    int32_t calibrate() const { return PROS_ERR; }
};

class Encoder : public Port {
public:
    Encoder(uint8_t top_port, uint8_t bottom_port, bool reversed = false)
        : Port(top_port), bottom_(bottom_port) {}
    int32_t get_value() const override { return PROS_ERR; }
    int32_t reset() const { return PROS_ERR; }
private:
    uint8_t bottom_;
};

} // namespace adi

} // namespace pros
