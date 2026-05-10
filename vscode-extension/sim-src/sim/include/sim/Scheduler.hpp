#pragma once
#include <thread>
#include <vector>
#include <mutex>

namespace sim {

class Scheduler {
public:
    Scheduler();
    ~Scheduler();

    void start();
    void shutdown();

    // Called by pros::Task stub to register a spawned thread
    void registerTask(std::thread t);

    static Scheduler& get();

private:
    std::thread robot_thread_;
    std::vector<std::thread> user_tasks_;
    std::mutex tasks_mutex_;

    static void robotMain();
};

// Free function called from rtos.cpp stub (avoids circular header dependency)
void scheduler_register_task(std::thread t);

} // namespace sim
