/**
 * ******************************************************************************
 * @Copyright:  Copyright  (C)  2021-2024 东莞市本末科技有限公司
 * @file 	: device_buzzer.c
 * ******************************************************************************
 * @brief 	: This file implements the buzzer device layer
 * @version	: 1.0
 * @date 	: 2024-08-01
 * @note 	: Use hardware timer output PWM control buzzer
 * *
 * *
 * ******* 1. Improvement *********
 * * 1. Added members to the device resource structure.
 * *
 * * 2. Optimize the code framework.
 * *
 * *
 * ******* 2. Fix the bug *********
 * * 1. fix the bug of :
 * *
 * *
 * ******* 3. HOW TO USE *********
 * * 0. Include this file into your current project.
 * * 1. include the header file where needed.
 * * 2. Modify the macro definitions and enums in 'device_buzzer.h'.
 * * 3. change the resources in 'device_buzzer.c':
 * * 4. Registers the device by calling the provided function using a callback.
 * * 5. Use the pointer to get the operation handle and use the device.
 * *
 * ******************************************************************************
 */
/* ---------------------------- user header file ---------------------------- */
#include "device_buzzer.h"

/* ----------------------- global variable definition ----------------------- */
buzzer_device_t g_buzzer_devices = {0};
const buzzer_device_resources_t g_buzzer_device_resources =
    {
        &htim3,
        TIM_CHANNEL_2,
};

/* ----------------------- public function definition ----------------------- */

/**
 * ******************************************************************************
 * @brief 	: get a pointer to an device
 * @retval 	: Pointer to the buzzer_device_t structure.
 * @author 	: chenningzhan
 * @note	: An open interface for other modules to call to get a device pointer
 * ******************************************************************************
 */
buzzer_device_t *buzzer_device_get_pointer(void)
{
    return &g_buzzer_devices;
}

/* ------------------ private operation function definition ----------------- */

/**
 * ******************************************************************************
 * @brief 	: enable the buzzer by starting the PWM signal.
 * @param 	  p_self  	: a pointer to get the device object.
 * @author 	: chenningzhan
 * @note	: None
 * ******************************************************************************
 */
static void buzzer_device_ops_enable(void *p_self)
{
    buzzer_device_t *p_dev = (buzzer_device_t *)p_self;

    HAL_TIM_PWM_Start(p_dev->res->timer, p_dev->res->channel);
}

/**
 * ******************************************************************************
 * @brief 	: control the status of an buzzer device.
 * @param 	  p_self  	: a pointer to get the device object.
 * @param 	  status    : The status to set for the buzzer device.
 * @author 	: chenningzhan
 * @note	: sets the buzzer to on or off by adjusting the compare value
 *            in the timer's capture/compare register.
 * ******************************************************************************
 */
static void buzzer_device_ops_control(void *p_self, buzzer_status_t status)
{
    buzzer_device_t *p_dev = (buzzer_device_t *)p_self;

    switch (status)
    {
    case BUZZER_STATUS_ON:
        __HAL_TIM_SET_COMPARE(p_dev->res->timer, p_dev->res->channel, p_dev->params.ccr);
        break;
    case BUZZER_STATUS_OFF:
        __HAL_TIM_SET_COMPARE(p_dev->res->timer, p_dev->res->channel, 0);
        break;
    default:
        break;
    }

    // update the current status
    p_dev->params.status = status;
}

/**
 * ******************************************************************************
 * @brief 	: register the buzzer device with its associated callback function.
 * @param 	  p_func  	: The callback function pointer for the device operations.
 * @author 	: chenningzhan
 * @note	: None
 * ******************************************************************************
 */
void buzzer_device_register(p_callback_func_dev p_func)
{
    // gets a pointer to the global variable device
    buzzer_device_t *p_dev = &g_buzzer_devices;

    // bind the hardware resource
    p_dev->res = &g_buzzer_device_resources;

    // set the default parameter
    p_dev->params.ccr = 300;

    // set operation function
    p_dev->ops.enable = buzzer_device_ops_enable;
    p_dev->ops.control = buzzer_device_ops_control;

    // bind the callback function
    if ((void *)NULL != p_func)
    {
        p_dev->call_func = p_func;
    }
    else
    {
        p_dev->call_func = (void *)NULL;
    }

    // .....
}
