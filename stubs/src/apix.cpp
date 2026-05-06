// Stub implementations for PROS Extended API (apix.h)
// Semaphores, queues, and device registry — all no-op/minimal stubs for the simulator.

// Must include apix.h FIRST to get the declarations (it pulls in everything)
#include "pros/apix.h"
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>
#include <cstring>
#include <thread>
#include <chrono>

// ─── Types ────────────────────────────────────────────────────────────────────
// sem_t and queue_t are defined as void* in apix.h (inside namespace pros::c).
// Here we create concrete structs and cast them.

struct SimSem {
    std::mutex mtx;
    std::condition_variable cv;
    uint32_t count;
    uint32_t max_count;
    SimSem(uint32_t max_c, uint32_t init_c) : count(init_c), max_count(max_c) {}
};

struct SimQueue {
    std::mutex mtx;
    std::condition_variable not_empty;
    std::condition_variable not_full;
    std::deque<std::vector<uint8_t>> items;
    uint32_t max_length;
    uint32_t item_size;
    SimQueue(uint32_t len, uint32_t sz) : max_length(len), item_size(sz) {}
};

// ─── All functions defined in pros::c with extern "C" linkage ─────────────────
// Wrapping in the same namespace/extern structure as apix.h:
#ifdef __cplusplus
namespace pros { namespace c {
extern "C" {
#endif

// ── Semaphore ─────────────────────────────────────────────────────────────────

sem_t sem_create(uint32_t max_count, uint32_t init_count) {
    return new SimSem(max_count, init_count);
}

void sem_delete(sem_t sem) {
    delete static_cast<SimSem*>(sem);
}

sem_t sem_binary_create(void) {
    return sem_create(1, 0);
}

bool sem_wait(sem_t sem, uint32_t timeout) {
    if (!sem) return false;
    auto* s = static_cast<SimSem*>(sem);
    std::unique_lock<std::mutex> lock(s->mtx);
    auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout);
    while (s->count == 0) {
        if (timeout == UINT32_MAX) {
            s->cv.wait(lock);
        } else {
            if (s->cv.wait_until(lock, deadline) == std::cv_status::timeout)
                return false;
        }
    }
    --s->count;
    return true;
}

bool sem_post(sem_t sem) {
    if (!sem) return false;
    auto* s = static_cast<SimSem*>(sem);
    std::lock_guard<std::mutex> lock(s->mtx);
    if (s->count < s->max_count) {
        ++s->count;
        s->cv.notify_one();
        return true;
    }
    return false;
}

uint32_t sem_get_count(sem_t sem) {
    if (!sem) return 0;
    auto* s = static_cast<SimSem*>(sem);
    std::lock_guard<std::mutex> lock(s->mtx);
    return s->count;
}

// ── Recursive mutex ───────────────────────────────────────────────────────────

mutex_t mutex_recursive_create(void) {
    return new std::recursive_mutex();
}

bool mutex_recursive_take(mutex_t mutex, uint32_t timeout) {
    if (!mutex) return false;
    auto* m = static_cast<std::recursive_mutex*>(mutex);
    if (timeout == 0) return m->try_lock();
    auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout);
    while (std::chrono::steady_clock::now() < deadline) {
        if (m->try_lock()) return true;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return false;
}

bool mutex_recursive_give(mutex_t mutex) {
    if (!mutex) return false;
    static_cast<std::recursive_mutex*>(mutex)->unlock();
    return true;
}

task_t mutex_get_owner(mutex_t mutex) {
    return nullptr;
}

// ── Queue ─────────────────────────────────────────────────────────────────────

queue_t queue_create(uint32_t length, uint32_t item_size) {
    return new SimQueue(length, item_size);
}

bool queue_prepend(queue_t queue, const void* item, uint32_t timeout) {
    if (!queue || !item) return false;
    auto* q = static_cast<SimQueue*>(queue);
    std::unique_lock<std::mutex> lock(q->mtx);
    auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout);
    while (q->items.size() >= q->max_length) {
        if (q->not_full.wait_until(lock, deadline) == std::cv_status::timeout) return false;
    }
    std::vector<uint8_t> buf(q->item_size);
    std::memcpy(buf.data(), item, q->item_size);
    q->items.push_front(std::move(buf));
    q->not_empty.notify_one();
    return true;
}

bool queue_append(queue_t queue, const void* item, uint32_t timeout) {
    if (!queue || !item) return false;
    auto* q = static_cast<SimQueue*>(queue);
    std::unique_lock<std::mutex> lock(q->mtx);
    auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout);
    while (q->items.size() >= q->max_length) {
        if (q->not_full.wait_until(lock, deadline) == std::cv_status::timeout) return false;
    }
    std::vector<uint8_t> buf(q->item_size);
    std::memcpy(buf.data(), item, q->item_size);
    q->items.push_back(std::move(buf));
    q->not_empty.notify_one();
    return true;
}

bool queue_peek(queue_t queue, void* const buffer, uint32_t timeout) {
    if (!queue || !buffer) return false;
    auto* q = static_cast<SimQueue*>(queue);
    std::unique_lock<std::mutex> lock(q->mtx);
    auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout);
    while (q->items.empty()) {
        if (q->not_empty.wait_until(lock, deadline) == std::cv_status::timeout) return false;
    }
    std::memcpy(buffer, q->items.front().data(), q->item_size);
    return true;
}

bool queue_recv(queue_t queue, void* const buffer, uint32_t timeout) {
    if (!queue || !buffer) return false;
    auto* q = static_cast<SimQueue*>(queue);
    std::unique_lock<std::mutex> lock(q->mtx);
    auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout);
    while (q->items.empty()) {
        if (q->not_empty.wait_until(lock, deadline) == std::cv_status::timeout) return false;
    }
    std::memcpy(buffer, q->items.front().data(), q->item_size);
    q->items.pop_front();
    q->not_full.notify_one();
    return true;
}

uint32_t queue_get_waiting(const queue_t queue) {
    if (!queue) return 0;
    auto* q = static_cast<SimQueue*>(queue);
    std::lock_guard<std::mutex> lock(q->mtx);
    return static_cast<uint32_t>(q->items.size());
}

uint32_t queue_get_available(const queue_t queue) {
    if (!queue) return 0;
    auto* q = static_cast<SimQueue*>(queue);
    std::lock_guard<std::mutex> lock(q->mtx);
    return q->max_length - static_cast<uint32_t>(q->items.size());
}

void queue_delete(queue_t queue) {
    delete static_cast<SimQueue*>(queue);
}

void queue_reset(queue_t queue) {
    if (!queue) return;
    auto* q = static_cast<SimQueue*>(queue);
    std::lock_guard<std::mutex> lock(q->mtx);
    q->items.clear();
    q->not_full.notify_all();
}

// ── Task abort / notify ───────────────────────────────────────────────────────

bool task_abort_delay(task_t task) { return false; }

void task_notify_when_deleting(task_t target_task, task_t task_to_notify,
                               uint32_t value, notify_action_e_t notify_action) {}

// ── Device registry ───────────────────────────────────────────────────────────

int registry_bind_port(uint8_t port, v5_device_e_t device_type) { return 1; }
int registry_unbind_port(uint8_t port)                           { return 1; }
v5_device_e_t registry_get_bound_type(uint8_t port)              { return E_DEVICE_UNDEFINED; }
v5_device_e_t registry_get_plugged_type(uint8_t port)            { return E_DEVICE_UNDEFINED; }

// ── Serial/fd control ─────────────────────────────────────────────────────────

int32_t serctl(const uint32_t action, void* const extra_arg)                   { return 0; }
int32_t fdctl(int file, const uint32_t action, void* const extra_arg)          { return 0; }

#ifdef __cplusplus
} // extern "C"
} // namespace c
} // namespace pros
#endif
