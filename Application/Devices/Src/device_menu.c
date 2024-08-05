/**
 * ******************************************************************************
 * @Copyright:  Copyright  (C)  2021-2024 东莞市本末科技有限公司
 * @file 	: device_menu.c
 * ******************************************************************************
 * @brief 	: 【introduction】
 * @version	: 2.0
 * @date 	: 2024-07-30
 * @note 	: 【note】
 * *
 * *
 * ******* 1. Improvement *********
 * * 1. improvement the function :
 * *
 * * 2. improvement the function :
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
#include "device_menu.h"
#include "device_key.h"
#include "device_motor_can.h"
#include "device_motor_uart.h"

/* ----------------------- global variable definition ----------------------- */
menu_device_t g_menu_device = {0};
const int8_t g_menu_motor_speed_percent[21] =
    {
        -99, -90, -80, -70, -60, -50, -40, -30, -20, -10,
        0,
        10, 20, 30, 40, 50, 60, 70, 80, 90, 99};

/* ----------------------- public function definition ----------------------- */

menu_device_t *menu_device_get_pointer(void)
{
    return &g_menu_device;
}

/**
 * ******************************************************************************
 * @brief 	: return to the motor model index
 * @retval 	: motor model index
 * @author 	: chenningzhan
 * @note 	: open interfaces other files can be directly called
 * ******************************************************************************
 */
motor_models_index_t menu_device_return_motor_index(void)
{
    return g_menu_device.params.motor_index;
}

int16_t menu_device_return_motor_speed_target(void)
{
    int16_t speed_temp;
    speed_temp = motor_info_get_target_speed(g_menu_device.params.motor_index, g_menu_device.params.motor_speed_percent);
    return speed_temp;
}

/* ------------------ private operation function definition ----------------- */

static void menu_device_ops_refresh(void *p_self)
{
    menu_device_t *p_dev = (menu_device_t *)p_self;

    OLED_Clear();
    p_dev->p_func_page = p_dev->table[p_dev->params.table_index].p_func_current;
    p_dev->p_func_page();
    OLED_Update();
}

static void menu_device_ops_init(void *p_self)
{
    menu_device_t *p_dev = (menu_device_t *)p_self;

    OLED_Clear();
    OLED_Update();
    OLED_ShowString(10, 0, "Direct Drive Tech", OLED_6X8);
    OLED_ShowString(5, 20, "system initializing", OLED_6X8);
    OLED_ShowString(5, 40, "intoing system...", OLED_6X8);
    OLED_Update();
    HAL_Delay(1000);
    p_dev->ops.refresh((void *)p_dev);
}

static void menu_device_ops_update_motor_index(void *p_self)
{
    menu_device_t *p_dev = (menu_device_t *)p_self;

    // check the motor index for not being
    if (p_dev->params.table_index >= 0 && p_dev->params.table_index < MOTOR_ID_COUNT)
    {
        p_dev->params.motor_index = (motor_models_index_t)p_dev->params.table_index;
    }
}

static void menu_device_ops_update_motor_speed_target(void *p_self)
{
    menu_device_t *p_dev = (menu_device_t *)p_self;
    key_device_index_t key_index = key_device_return_index();

    // change the index based on the key
    switch (key_index)
    {
    case KEY_ID_UP:
        p_dev->params.motor_speed_percent_index -= 1;
        break;
    case KEY_ID_DOWN:
        p_dev->params.motor_speed_percent_index += 1;
        break;
    default:
        break;
    }
    // check the motor index for not being
    if (p_dev->params.motor_speed_percent_index < 0)
    {
        p_dev->params.motor_speed_percent_index = 10;
    }
    else if (p_dev->params.motor_speed_percent_index >= 21)
    {
        p_dev->params.motor_speed_percent_index = 10;
    }

    p_dev->params.motor_speed_percent = p_dev->params.motor_speed_percent_arr[p_dev->params.motor_speed_percent_index];
    p_dev->params.motor_speed_target = motor_info_get_target_speed(p_dev->params.motor_index, p_dev->params.motor_speed_percent);
}

void menu_device_register(p_callback_func_dev p_func, const menu_device_table_t *arg_table)
{
    // gets a pointer to the global variable device
    menu_device_t *p_dev = &g_menu_device;

    // bind the hardware resource
    p_dev->table = arg_table;
    p_dev->p_func_page = (void *)NULL;

    // set the default parameter
    p_dev->params.table_index = 0;
    p_dev->params.motor_index = MOTOR_ID_M0601C;
    p_dev->params.motor_speed_percent_index = 10;
    p_dev->params.motor_speed_percent = 0;
    p_dev->params.motor_speed_target = 0;
    p_dev->params.motor_speed_percent_arr = g_menu_motor_speed_percent;

    // set operation function
    p_dev->ops.init = menu_device_ops_init;
    p_dev->ops.refresh = menu_device_ops_refresh;
    p_dev->ops.update_motor_index = menu_device_ops_update_motor_index;
    p_dev->ops.update_motor_speed_target = menu_device_ops_update_motor_speed_target;

    // bind the callback function
    if ((void *)NULL != p_func)
    {
        p_dev->call_func = p_func;
    }
    else
    {
        p_dev->call_func = (void *)NULL;
    }
}
