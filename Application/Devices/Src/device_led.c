/**
 * ******************************************************************************
 * @Copyright:  Copyright  (C)  2021-2024 东莞市本末科技有限公司
 * @file 	: device_led.c
 * ******************************************************************************
 * @brief 	: This file implements the led device layer
 *            providing an operation interface for led devices.
 * @date 	: 2024-07-26
 * @note 	: It encapsulates the functionalities needed to control the LEDs and can be extended for
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
 * * 2. Modify the macro definitions and enums in 'header file.h'.
 * * 3. change the resources in 'files name.c':
 * * 4. Registers the device by calling the provided function using a callback.
 * * 5. Use the pointer to get the operation handle and use the device.
 * *
 * ******************************************************************************
 */
/* ---------------------------- user header file ---------------------------- */
#include "device_led.h"

/* ----------------------- global variable definition ----------------------- */
led_device_t g_led_devices[LED_ID_COUNT] = {0};
static const led_device_resources_t g_led_device_resources[LED_ID_COUNT] =
    {
        {LED_ID_R, GPIO_PIN_RESET, LED_RED_GPIO_Port, LED_RED_Pin},
        {LED_ID_G, GPIO_PIN_RESET, LED_GREEN_GPIO_Port, LED_GREEN_Pin},
        {LED_ID_B, GPIO_PIN_RESET, LED_BLUE_GPIO_Port, LED_BLUE_Pin},
};

/* ----------------------- public function definition ----------------------- */

/**
 * ******************************************************************************
 * @brief 	: get a pointer to an device based on its index.
 * @param 	: arg_index [in/out], The index of the led device.
 * @retval 	: This device index corresponds to a device pointer.
 * @author 	: chenningzhan
 * @note	: If the index is out of range, it logs an error and returns NULL.
 * @note	: An open interface for other modules to call to get a device pointer
 * ******************************************************************************
 */
led_device_t *led_device_get_pointer(led_device_index_t arg_index)
{
    if (arg_index < 0 || arg_index >= LED_ID_COUNT)
    {
        LOG_ERROR("get pointer index out of range");
        return (void *)NULL;
    }
    else
    {
        return &g_led_devices[arg_index];
    }
}

/* ------------------ private operation function definition ----------------- */

/**
 * ******************************************************************************
 * @brief 	: control the status of an led device.
 * @param 	: p_self [in/out], a pointer to get the device object
 * @param 	: arg_status [in/out], The status to set for the led device.
 * @author 	: chenningzhan
 * @note	: sets the led device to on or off based on the provided status, writing to the
 *            appropriate GPIO pin
 * ******************************************************************************
 */

static void led_device_ops_control(void *p_self, led_device_status_t arg_status)
{
    led_device_t *p_dev = (led_device_t *)p_self;

    switch (arg_status)
    {
    case LED_STATUS_ON:
        HAL_GPIO_WritePin(p_dev->res->gpio_port, p_dev->res->pin, p_dev->res->level_valid);
        break;
    case LED_STATUS_OFF:
        HAL_GPIO_WritePin(p_dev->res->gpio_port, p_dev->res->pin, !p_dev->res->level_valid);
        break;
    default:
        break;
    }

    // update the current status
    p_dev->params.status = arg_status;
}

/* ----------------------- public function definition ----------------------- */

/**
 * ******************************************************************************
 * @brief 	: register an led device with its associated callback function.
 * @param 	: p_func [in/out], The callback function pointer for the device operations.
 * @author 	: chenningzhan
 * @note	: None
 * ******************************************************************************
 */
void led_device_register(p_callback_func_dev p_func)
{
    for (uint8_t i = 0; i < LED_ID_COUNT; i++)
    {
        // gets a pointer to the global variable device
        led_device_t *p_dev = &g_led_devices[i];

        // bind the hardware resource
        p_dev->res = &g_led_device_resources[i];

        // set the default parameter
        p_dev->params.status = LED_STATUS_OFF;

        // set operation function
        p_dev->ops.control = led_device_ops_control;

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
}
