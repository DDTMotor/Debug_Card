#ifndef __TASK_SCHDULE_H__
#define __TASK_SCHDULE_H__

/* ---------------------------- user header file ---------------------------- */
#include "log.h"
#include "stm32f1xx_hal.h"

/* --------------------------- struct definitions --------------------------- */
typedef void (*p_callback_func_schedule)(void *p_self);

typedef struct
{
    void (*update_tick)(volatile uint32_t *p_tick);
} schedule_ops_t;

/**
 * @brief Enumeration to represent the initialization status of a task.
 */
typedef enum
{
    TASK_INIT_UNCOMPLETE = 0,
    TASK_INIT_COMPLETE,
} schedule_init_t;

typedef struct
{
    volatile uint32_t tick_last;    // The tick count at the time of the last task execution.
    volatile uint32_t tick_current; // The current tick count.
    uint32_t tick_interval;         // The desired interval between task executions.
    uint32_t count;                 // The number of times the task has been executed.
    schedule_init_t init;           // The initialization status of the task.
} schedule_params_t;

typedef struct
{
    schedule_params_t params;           // Parameters for the scheduled task.
    schedule_ops_t ops;                 // Operations for the task, such as updating the tick.
    p_callback_func_schedule call_func; // Pointer to the callback function to be executed.
} schedule_t;

/* -------------------------- function declaration -------------------------- */

int schedule_register(p_callback_func_schedule p_func, uint32_t arg_interval);
schedule_t *schedule_get_pointer(p_callback_func_schedule p_func);
void schedule_start(void);

#endif