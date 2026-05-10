#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "sim/SimState.hpp"
#include <chrono>
#include <mutex>
#include <thread>

// Forward declaration — Scheduler is defined in sim/
namespace sim {
    void scheduler_register_task(std::thread t);
}

// ── C API ─────────────────────────────────────────────────────────────────────

// Thrown by interruptible_sleep when a mode switch is requested.
struct TaskAbortException {};

// Sleep in short chunks so the thread wakes up promptly when running→false
// or when abort_task is set (mode switched mid-run).
static void interruptible_sleep(uint32_t ms) {
    using namespace std::chrono;
    auto& state = sim::SimState::get();
    auto deadline = steady_clock::now() + milliseconds(ms);
    while (state.running.load()) {
        if (state.abort_task.load()) throw TaskAbortException{};
        auto left = deadline - steady_clock::now();
        if (left <= milliseconds(0)) return;
        auto chunk = left < milliseconds(5) ? left : milliseconds(5);
        std::this_thread::sleep_for(chunk);
    }
}

void pros_delay(uint32_t milliseconds) {
    interruptible_sleep(milliseconds);
}

uint32_t pros_millis(void) {
    return sim::SimState::get().sim_millis.load();
}

uint32_t pros_micros(void) {
    return sim::SimState::get().sim_millis.load() * 1000;
}

// ── C++ API ───────────────────────────────────────────────────────────────────

namespace pros { inline namespace rtos {

void delay(uint32_t ms) {
    interruptible_sleep(ms);
}

uint32_t millis() { return pros_millis(); }
uint32_t micros() { return pros_micros(); }

// ── Mutex ─────────────────────────────────────────────────────────────────────

Mutex::Mutex() : handle_(new std::mutex()) {}
Mutex::~Mutex() { delete static_cast<std::mutex*>(handle_); }

bool Mutex::take() {
    static_cast<std::mutex*>(handle_)->lock();
    return true;
}
bool Mutex::take(uint32_t timeout_ms) {
    static_cast<std::mutex*>(handle_)->lock();
    return true;
}
bool Mutex::give() {
    static_cast<std::mutex*>(handle_)->unlock();
    return true;
}
void Mutex::lock()        { static_cast<std::mutex*>(handle_)->lock(); }
void Mutex::unlock()      { static_cast<std::mutex*>(handle_)->unlock(); }
void Mutex::lazy_init()   { /* mutex is already initialized in constructor */ }

// ── Task ──────────────────────────────────────────────────────────────────────

Task::Task(task_fn_t fn, void* params, uint32_t, uint16_t, const char*) {
    std::thread t([fn, params]() { fn(params); });
    sim::scheduler_register_task(std::move(t));
    handle_ = nullptr;
}

Task::Task(std::function<void()> fn, const char*) {
    std::thread t(fn);
    sim::scheduler_register_task(std::move(t));
    handle_ = nullptr;
}

void Task::suspend() {}
void Task::resume()  {}
void Task::remove()  {}

uint32_t Task::notify() { return 0; }

uint32_t Task::notify_take(bool clear_on_exit, uint32_t timeout) {
    // In the simulator every task notification is immediately available
    (void)clear_on_exit; (void)timeout;
    return 0;
}

task_state_e_t Task::get_state() { return E_TASK_STATE_RUNNING; }

char* Task::get_name() { return const_cast<char*>(""); }

task_t Task::get_current() { return nullptr; }

uint32_t Task::get_priority() { return TASK_PRIORITY_DEFAULT; }

void Task::set_priority(uint32_t priority) { (void)priority; }

void Task::join() {
    if (handle_) {
        static_cast<std::thread*>(handle_)->join();
    }
}

void Task::delay(uint32_t ms) {
    interruptible_sleep(ms);
}

uint32_t Task::delay_until(uint32_t* prev_time, uint32_t delta) {
    if (prev_time) {
        uint32_t now = pros_millis();
        uint32_t next = *prev_time + delta;
        if (now < next)
            interruptible_sleep(next - now);
        *prev_time = next;
        return next;
    }
    return 0;
}

}} // namespace pros::rtos
