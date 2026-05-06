#pragma once
#include <stdint.h>
#include "error.h"

#ifdef __cplusplus
extern "C" {
namespace pros {
#endif

typedef void* task_t;
typedef void* mutex_t;
typedef void (*task_fn_t)(void*);

#define TASK_PRIORITY_DEFAULT    8
#define TASK_PRIORITY_MAX        16
#define TASK_PRIORITY_MIN        1
#define TASK_STACK_DEPTH_DEFAULT 0x2000
#define TASK_STACK_DEPTH_MIN     0x200
#define TASK_NAME_MAX_LEN        32
#define TIMEOUT_MAX              UINT32_MAX
#define MAX_DELAY                TIMEOUT_MAX

typedef enum {
    E_TASK_STATE_RUNNING = 0,
    E_TASK_STATE_READY,
    E_TASK_STATE_BLOCKED,
    E_TASK_STATE_SUSPENDED,
    E_TASK_STATE_DELETED,
    E_TASK_STATE_INVALID
} task_state_e_t;

typedef enum {
    E_NOTIFY_ACTION_NONE,
    E_NOTIFY_ACTION_BITS,
    E_NOTIFY_ACTION_INCR,
    E_NOTIFY_ACTION_OWRITE,
    E_NOTIFY_ACTION_NO_OWRITE
} notify_action_e_t;

#ifdef __cplusplus
namespace c {
#endif

void     task_delay(uint32_t milliseconds);
void     delay(uint32_t milliseconds);
uint32_t millis(void);
uint32_t micros(void);

task_t   task_create(task_fn_t fn, void* params, uint32_t prio, uint16_t stack_depth, const char* name);
void     task_delete(task_t task);
void     task_delay_until(uint32_t* prev_time, uint32_t delta);
uint32_t task_get_priority(task_t task);
void     task_set_priority(task_t task, uint32_t prio);
task_state_e_t task_get_state(task_t task);
void     task_suspend(task_t task);
void     task_resume(task_t task);
uint32_t task_get_count(void);
char*    task_get_name(task_t task);
task_t   task_get_by_name(const char* name);
task_t   task_get_current(void);
uint32_t task_notify(task_t task);
void     task_join(task_t task);
uint32_t task_notify_ext(task_t task, uint32_t value, notify_action_e_t action, uint32_t* prev_value);
uint32_t task_notify_take(bool clear_on_exit, uint32_t timeout);
bool     task_notify_clear(task_t task);

mutex_t  mutex_create(void);
bool     mutex_take(mutex_t mutex, uint32_t timeout);
bool     mutex_give(mutex_t mutex);
void     mutex_delete(mutex_t mutex);

#ifdef __cplusplus
} // namespace c
} // namespace pros
} // extern "C"
#endif
