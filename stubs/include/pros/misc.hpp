#pragma once
#include "misc.h"
#include <cstdint>
#include <string>

namespace pros {
inline namespace v5 {

class Controller {
public:
    explicit Controller(controller_id_e_t id);

    int32_t get_analog(controller_analog_e_t channel) const;
    int32_t get_digital(controller_digital_e_t button) const;
    int32_t get_digital_new_press(controller_digital_e_t button) const;
    int32_t is_connected() const;
    int32_t rumble(const char* rumble_pattern);
    int32_t print(uint8_t line, uint8_t col, const char* fmt, ...) const;
    int32_t clear();
    int32_t clear_line(uint8_t line);

private:
    controller_id_e_t id_;
};

class Competition {
public:
    static bool is_autonomous();
    static bool is_connected();
    static bool is_disabled();
};

} // namespace v5
} // namespace pros
