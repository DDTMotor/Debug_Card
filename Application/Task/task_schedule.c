/**
 * ******************************************************************************
 * @Copyright:  Copyright  (C)  2021-2024 东莞市本末科技有限公司
 * @file 	: task_schedule.c
 * ******************************************************************************
 * @brief 	: use systick for easy task scheduling
 * @version	: 3.0
 * @date 	: 2024-07-27
 * @note 	: improved access to system time base for ease of migration and maintenance
 * *
 * ******* Improvement *********
 * * 1. the system timebase acquisition method is encapsulated into a function
 * *    and the interface is called in other places which is more convenient for portability:
 * *
 * * 2. added the task structure pointer to obtain the task structure pointer through the task entry function pointer
 * *    and you can dynamically modify the task parameters
 * *
 * * 3. Encapsulating functions, changing code frames, variable naming styles
 * *
 * *
 * ******* Fix the bug *********
 * * 1. fix the bug of :
 * *
 * *
 * ******* HOW TO USE *********
 * * 0. add the files in this folder to current project
 * * 1. #include 'task_schedule.h'
 * * 2. change the macro in TASK_MAX_COUNT:
 * * 3. use the function 'schedule_register' to register tasks to the schedule list
 * * 4. call the function 'schedule_start' in the loop
 * *
 * ******************************************************************************
 */
/* ---------------------------- user header file ---------------------------- */
#include "task_schedule.h"

/* ---------------------------- macro definition ---------------------------- */
#define TASK_MAX_COUNT 6

/* ----------------------- global variable definition ----------------------- */
schedule_t g_task_list[TASK_MAX_COUNT] = {0};

/* ------------------ private operation function definition ----------------- */

/**
 * ******************************************************************************
 * @brief 	: Retrieve the current system tick count and store it in the provided pointer.
 * @param 	: p_tick [in/out], Pointer to a uint32_t where the current tick count will be stored.
 * @author 	: chenningzhan
 * @note	: This function is used to update the tick count in the task's schedule operation.
 * ******************************************************************************
 */
static void schedule_ops_gettick(volatile uint32_t *p_tick)
{
    *p_tick = HAL_GetTick();
}

/* ----------------------- public function definition ----------------------- */

/**
 * ******************************************************************************
 * @brief 	: Retrieve a pointer to a scheduled task using its callback function pointer.
 * @param 	: func [in/out], func The task function pointer.
 * @retval 	: Pointer to the task structure or NULL if not found.
 * @author 	: chenningzhan
 * @note	: None
 * ******************************************************************************
 */
schedule_t *schedule_get_pointer(p_callback_func_schedule p_func)
{
    for (int i = 0; i < TASK_MAX_COUNT; ++i)
    {
        if (p_func == g_task_list[i].call_func)
        {
            return &g_task_list[i];
        }
    }
    return (void *)NULL;
}

/**
 * ******************************************************************************
 * @brief 	: Register a new task to the scheduler with a given callback function and interval.
 * @param 	: p_func [in/out],The callback function pointer of the task to register.
 * @param 	: arg_interval [in/out], The desired execution interval for the task in milliseconds.
 * @retval 	: The index of the registered task, or -1 if registration failed.
 * @author 	: chenningzhan
 * @note	: None
 * ******************************************************************************
 */
int schedule_register(p_callback_func_schedule p_func, uint32_t arg_interval)
{
    for (int i = 0; i < TASK_MAX_COUNT; i++)
    {
        // The initialization is performed only if the task has not been initialized
        if (TASK_INIT_UNCOMPLETE == g_task_list[i].params.init)
        {
            // bind the callback function
            g_task_list[i].ops.update_tick = schedule_ops_gettick;
            // bind the callback function
            g_task_list[i].call_func = p_func;
            // set interval time
            g_task_list[i].params.tick_interval = arg_interval;
            // set the initial status
            g_task_list[i].params.init = TASK_INIT_COMPLETE;
            return i;
        }
    }
    return -1;
}

/**
 * ******************************************************************************
 * @brief 	: Start the execution of all registered tasks according to their intervals.
 * @author 	: chenningzhan
 * @note	: This function checks each task's initialization status and runs them based on
 * their tick intervals. It updates the current tick and the last tick, and calls the task's
 * callback function if the interval condition is met.
 * ******************************************************************************
 */
void schedule_start(void)
{
    for (int i = 0; i < TASK_MAX_COUNT; i++)
    {
        if (TASK_INIT_COMPLETE == g_task_list[i].params.init)
        {
            // update the current tick
            g_task_list[i].ops.update_tick(&g_task_list[i].params.tick_current);

            // task running condition check
            if ((g_task_list[i].params.tick_current - g_task_list[i].params.tick_last) >= g_task_list[i].params.tick_interval ||
                0 == g_task_list[i].params.count)
            {
                g_task_list[i].ops.update_tick(&g_task_list[i].params.tick_last);
                g_task_list[i].call_func((void *)&g_task_list[i]);
                g_task_list[i].params.count += 1;
            }
        }
    }
}
