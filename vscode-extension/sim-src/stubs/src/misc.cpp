#include "pros/misc.h"
#include "pros/misc.hpp"
#include "sim/SimState.hpp"
#include <cstdarg>
#include <cstdio>
#include <chrono>
#include <thread>

namespace pros { namespace c {

int32_t controller_get_analog(controller_id_e_t id, controller_analog_e_t channel) {
    // Only master controller is simulated
    auto& c = sim::SimState::get().controller;
    switch (channel) {
        case E_CONTROLLER_ANALOG_LEFT_X:  return c.leftX.load();
        case E_CONTROLLER_ANALOG_LEFT_Y:  return c.leftY.load();
        case E_CONTROLLER_ANALOG_RIGHT_X: return c.rightX.load();
        case E_CONTROLLER_ANALOG_RIGHT_Y: return c.rightY.load();
        default: return 0;
    }
}

int32_t controller_get_digital(controller_id_e_t id, controller_digital_e_t button) {
    uint32_t mask = 1u << static_cast<int>(button);
    return (sim::SimState::get().controller.buttons.load() & mask) ? 1 : 0;
}

int32_t controller_get_digital_new_press(controller_id_e_t id, controller_digital_e_t button) {
    uint32_t mask = 1u << static_cast<int>(button);
    auto& c = sim::SimState::get().controller;
    bool now  = (c.buttons.load()      & mask) != 0;
    bool prev = (c.prev_buttons.load() & mask) != 0;
    return (now && !prev) ? 1 : 0;
}

int32_t controller_is_connected(controller_id_e_t id) {
    return id == E_CONTROLLER_MASTER ? 1 : 0;
}

int32_t controller_rumble(controller_id_e_t id, const char* pattern) {
    return 1; // no-op
}

int32_t controller_print(controller_id_e_t id, uint8_t line, uint8_t col, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
    return 1;
}

int32_t controller_clear(controller_id_e_t id)             { return 1; }
int32_t controller_clear_line(controller_id_e_t id, uint8_t line) { return 1; }

uint32_t competition_get_status(void) {
    auto mode = sim::SimState::get().comp_mode.load();
    switch (mode) {
        case sim::CompMode::AUTONOMOUS: return 0x02; // COMPETITION_AUTONOMOUS
        case sim::CompMode::OPCONTROL:  return 0x00; // normal (enabled, driver)
        case sim::CompMode::DISABLED:   return 0x08; // COMPETITION_DISABLED
    }
    return 0x08;
}

uint8_t competition_is_autonomous(void) {
    return sim::SimState::get().comp_mode.load() == sim::CompMode::AUTONOMOUS ? 1 : 0;
}

uint8_t competition_is_connected(void) { return 1; }

uint8_t competition_is_disabled(void) {
    return sim::SimState::get().comp_mode.load() == sim::CompMode::DISABLED ? 1 : 0;
}

}} // namespace pros::c

// ── C++ Controller ────────────────────────────────────────────────────────────

namespace pros { inline namespace v5 {

Controller::Controller(controller_id_e_t id) : id_(id) {}

int32_t Controller::get_analog(controller_analog_e_t ch) const {
    return pros::c::controller_get_analog(id_, ch);
}
int32_t Controller::get_digital(controller_digital_e_t btn) const {
    return pros::c::controller_get_digital(id_, btn);
}
int32_t Controller::get_digital_new_press(controller_digital_e_t btn) const {
    return pros::c::controller_get_digital_new_press(id_, btn);
}
int32_t Controller::is_connected() const {
    return pros::c::controller_is_connected(id_);
}
int32_t Controller::rumble(const char* pattern) {
    return pros::c::controller_rumble(id_, pattern);
}
int32_t Controller::print(uint8_t line, uint8_t col, const char* fmt, ...) const {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
    return 1;
}
int32_t Controller::clear()              { return pros::c::controller_clear(id_); }
int32_t Controller::clear_line(uint8_t l){ return pros::c::controller_clear_line(id_, l); }

bool Competition::is_autonomous() { return pros::c::competition_is_autonomous() != 0; }
bool Competition::is_connected()  { return pros::c::competition_is_connected()  != 0; }
bool Competition::is_disabled()   { return pros::c::competition_is_disabled()   != 0; }

}} // namespace pros::v5

// ── pros::competition namespace (PROS 4 free-function API) ────────────────────
namespace pros { namespace competition {
    uint32_t get_status() { return pros::c::competition_get_status(); }
    bool is_autonomous()  { return pros::c::competition_is_autonomous() != 0; }
    bool is_connected()   { return pros::c::competition_is_connected() != 0; }
    bool is_disabled()    { return pros::c::competition_is_disabled() != 0; }
}} // namespace pros::competition

// ── Bare C-compatible shims (called without namespace by some PROS code) ──────
extern "C" {
    void delay(uint32_t ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
    uint32_t millis(void) {
        return sim::SimState::get().sim_millis.load();
    }
    // controller_print / controller_rumble already in pros::c; provide bare shims
    int32_t controller_print(pros::controller_id_e_t id, uint8_t line, uint8_t col,
                             const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\n");
        return 1;
    }
    int32_t controller_rumble(pros::controller_id_e_t id, const char* pattern) {
        return 1;
    }
}
