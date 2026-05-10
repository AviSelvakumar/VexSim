#pragma once
#include "rtos.h"
#include <cstdint>
#include <functional>
#include <string>

// Remove FreeRTOS/Windows macro that may conflict with pros::delay
#undef delay

namespace pros {
inline namespace rtos {

void delay(uint32_t ms);
uint32_t millis();
uint32_t micros();

class Mutex {
public:
    Mutex();
    ~Mutex();
    bool take();
    bool take(uint32_t timeout_ms);
    bool give();
    void lock();
    void unlock();
    void lazy_init();

private:
    void* handle_;
};

class MutexGuard {
public:
    explicit MutexGuard(Mutex& m) : m_(m) { m_.take(); }
    ~MutexGuard() { m_.give(); }
    MutexGuard(const MutexGuard&) = delete;
    MutexGuard& operator=(const MutexGuard&) = delete;
private:
    Mutex& m_;
};

class Task {
public:
    Task(task_fn_t fn, void* params = nullptr,
         uint32_t prio = TASK_PRIORITY_DEFAULT,
         uint16_t stack_depth = TASK_STACK_DEPTH_DEFAULT,
         const char* name = "");
    explicit Task(std::function<void()> fn, const char* name = "");

    void suspend();
    void resume();
    void remove();

    uint32_t notify();
    static uint32_t notify_take(bool clear_on_exit, uint32_t timeout);

    task_state_e_t get_state();
    char*          get_name();
    uint32_t       get_priority();
    void           set_priority(uint32_t priority);
    void           join();

    static task_t    get_current();
    static void      delay(uint32_t ms);
    static uint32_t  delay_until(uint32_t* prev_time, uint32_t delta);

private:
    void* handle_;
};

} // namespace rtos
} // namespace pros
