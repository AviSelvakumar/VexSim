#include "sim/Scheduler.hpp"
#include "sim/SimState.hpp"
#include <chrono>
#include <thread>

// Forward declarations of user robot entry points
extern "C" {
    void initialize(void);
    void disabled(void);
    void competition_initialize(void);
    void autonomous(void);
    void opcontrol(void);
}

namespace sim {

Scheduler::Scheduler() = default;

Scheduler::~Scheduler() {
    shutdown();
}

Scheduler& Scheduler::get() {
    static Scheduler instance;
    return instance;
}

void scheduler_register_task(std::thread t) {
    Scheduler::get().registerTask(std::move(t));
}

void Scheduler::registerTask(std::thread t) {
    std::lock_guard<std::mutex> lock(tasks_mutex_);
    user_tasks_.push_back(std::move(t));
}

void Scheduler::start() {
    robot_thread_ = std::thread(&Scheduler::robotMain);
}

void Scheduler::shutdown() {
    SimState::get().running.store(false);

    if (robot_thread_.joinable())
        robot_thread_.detach();

    std::lock_guard<std::mutex> lock(tasks_mutex_);
    for (auto& t : user_tasks_) {
        if (t.joinable()) t.detach(); // can't cleanly interrupt; detach
    }
    user_tasks_.clear();
}

void Scheduler::robotMain() {
    // Initialize user code
    ::initialize();
    ::competition_initialize();

    auto& state = SimState::get();

    while (state.running.load()) {
        // Clear any pending abort before entering a new mode
        state.abort_task.store(false);

        CompMode mode = state.comp_mode.load();

        if (mode == CompMode::AUTONOMOUS) {
            try { ::autonomous(); } catch (...) {}
            // Only auto-transition to DISABLED if the user hasn't switched modes
            CompMode expected = CompMode::AUTONOMOUS;
            state.comp_mode.compare_exchange_strong(expected, CompMode::DISABLED);

        } else if (mode == CompMode::OPCONTROL) {
            try { ::opcontrol(); } catch (...) {}
            CompMode expected = CompMode::OPCONTROL;
            state.comp_mode.compare_exchange_strong(expected, CompMode::DISABLED);

        } else {
            // DISABLED — call disabled() once, then poll until mode changes
            try { ::disabled(); } catch (...) {}
            while (state.running.load() &&
                   state.comp_mode.load() == CompMode::DISABLED) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }
}

} // namespace sim
