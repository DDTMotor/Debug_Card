/**
 * ******************************************************************************
 * @Copyright:  Copyright  (C)  2021-2024 东莞市本末科技有限公司
 * @file 	: task_key.c
 * ******************************************************************************
 * @brief 	: This file implements the key handling task.
 *            It provides an interface for managing key presses and associated actions.
 * @version	: 1.0
 * @date 	: 2024-07-27
 * @note 	: Perform OLED, buzzer, and various motor callbacks in the key callback function
 * *
 * *
 * ******* 1. Improvement *********
 * * 1. Improved the functionality of the key handling task.
 * *
 * * 2. improvement the function.
 * *
 * *
 * ******* 2. Fix the bug *********
 * * 1. fix the bug of :
 * *
 * *
 * ******* 3. HOW TO USE *********
 * * 0. Include this file into your current project.
 * * 1. include the header file where needed.
 * * 2. Modify the macro definitions and enums in 'header file.h'.
 * * 3. change the resources in 'files name.c':
 * * 4. Registers the device by calling the provided function using a callback.
 * * 5. Use the pointer to get the operation handle and use the device.
 * *
 * ******************************************************************************
 */

/* ---------------------------- user header file ---------------------------- */
#include "task_key.h"
#include "device_buzzer.h"
#include "device_key.h"
#include "device_menu.h"
#include "device_motor_can.h"
#include "device_motor_uart.h"

/* --------------------------- function definition -------------------------- */

/**
 * ******************************************************************************
 * @brief 	: Callback function for key device operations.
 * @param 	: p_self [in/out], a pointer to get the device object
 * @author 	: chenningzhan
 * @note	: The button is pressed and called to handle OLED UI page updates
 * drive motor and other callback functions.
 * ******************************************************************************
 */
static void key_device_ops_callback(void *p_self)
{
    // get the device operate pointer
    key_device_t *p_dev_key = (key_device_t *)p_self;
    motor_device_can_t *p_dev_can = motor_device_can_get_pointer();
    motor_device_uart_t *p_dev_uart = motor_device_uart_get_pointer();
    buzzer_device_t *p_dev_buzzer = buzzer_device_get_pointer();
    menu_device_t *p_dev_menu = menu_device_get_pointer();

    // put key log
    switch (p_dev_key->res->index)
    {
    case KEY_ID_ENTER:
        LOG_DEBUG("KEY_ENTER!");
        break;
    case KEY_ID_BACK:
        LOG_DEBUG("KEY_BACK!");
        break;
    case KEY_ID_UP:
        LOG_DEBUG("KEY_UP!");
        break;
    case KEY_ID_DOWN:
        LOG_DEBUG("KEY_DOWN!");
        break;
    default:
        break;    
    }
    // 1.1 buzzer switch to on
    p_dev_buzzer->call_func((void *)p_dev_buzzer);
    // 1.2 oled ui switch
    p_dev_menu->call_func((void *)p_dev_menu);
    // 1.3 driver the motor
    p_dev_can->call_func((void *)p_dev_can);
    p_dev_uart->call_func((void *)p_dev_uart);
}

/**
 * ******************************************************************************
 * @brief 	: Entry function for the key handling task.
 * @param 	: p_self [in/out], a pointer to get the task object
 * @author 	: chenningzhan
 * @note	: enters a loop to continuously check the status of the key device.
 * ******************************************************************************
 */
void mytask_key_entry(void *p_self)
{
    schedule_t *task = (schedule_t *)p_self;

    static key_device_t *p_key_dev_enter = NULL,
                        *p_key_dev_back = NULL,
                        *p_key_dev_up = NULL,
                        *p_key_dev_down = NULL;

    // the task initialization
    if (0 == task->params.count)
    {
        // 1.1 init the device and callback function
        key_device_register(key_device_ops_callback);

        // 1.2 gets pointer by index
        p_key_dev_enter = key_device_get_pointer(KEY_ID_ENTER);
        p_key_dev_back = key_device_get_pointer(KEY_ID_BACK);
        p_key_dev_up = key_device_get_pointer(KEY_ID_UP);
        p_key_dev_down = key_device_get_pointer(KEY_ID_DOWN);

        // 1.3 pointer check
        if (NULL == p_key_dev_enter || NULL == p_key_dev_back ||
            NULL == p_key_dev_up || NULL == p_key_dev_down)
        {
            LOG_ERROR("get device pointer failed !");
            return;
        }
        else
        {
            LOG_INFO("find device success");
        }
    }
    // the task starts a loop
    else
    {
        // check the status of each keystroke device
        p_key_dev_enter->ops.update_status((void *)p_key_dev_enter);
        p_key_dev_back->ops.update_status((void *)p_key_dev_back);
        p_key_dev_up->ops.update_status((void *)p_key_dev_up);
        p_key_dev_down->ops.update_status((void *)p_key_dev_down);
    }
}
