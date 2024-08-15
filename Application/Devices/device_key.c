/**
 * ******************************************************************************
 * @Copyright:  Copyright  (C)  2021-2024 东莞市本末科技有限公司
 * @file 	: device_key.c
 * ******************************************************************************
 * @brief 	: This file implements the keys device layer
 *            providing an operation interface for keys devices.
 * @version	: 2.0
 * @date 	: 2024-07-27
 * @note 	: The button has a click-to-trigger interval mechanism
 *            pending the implementation of a long press processing mechanism
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
 * * 2. Modify the macro definitions and enums in 'device_key.h'.
 * * 3. change the resources in 'device_key.c':
 * * 4. Registers the device by calling the provided function using a callback.
 * * 5. Use the pointer to get the operation handle and use the device.
 * *
 * ******************************************************************************
 */
/* ---------------------------- user header file ---------------------------- */
#include "device_key.h"

/* ----------------------- global variable definition ----------------------- */
key_device_t g_key_devices[KEY_ID_COUNT] = {0};
const key_device_resources_t g_key_device_resources[KEY_ID_COUNT] =
    {
        {KEY_ID_ENTER, KEY_ENTER_GPIO_Port, KEY_ENTER_Pin},
        {KEY_ID_BACK, KEY_BACK_GPIO_Port, KEY_BACK_Pin},
        {KEY_ID_UP, KEY_UP_GPIO_Port, KEY_UP_Pin},
        {KEY_ID_DOWN, KEY_DOWN_GPIO_Port, KEY_DOWN_Pin},
};

/* ----------------------- public function definition ----------------------- */

/**
 * ******************************************************************************
 * @brief 	: get a pointer to an device based on its index.
 * @param 	  index  	: The index of the key device.
 * @retval 	: This device index corresponds to a device pointer.
 * @author 	: chenningzhan
 * @note	: If the index is out of range, it logs an error and returns NULL.
 * @note	: An open interface for other modules to call to get a device pointer
 * ******************************************************************************
 */
key_device_t *key_device_get_pointer(key_device_index_t index)
{
    if (index < 0 || index >= KEY_ID_COUNT)
    {
        LOG_ERROR("get pointer index out of range");
        return NULL;
    }
    else
    {
        return &g_key_devices[index];
    }
}

/**
 * ******************************************************************************
 * @brief 	: return the index of a key device that has been pressed.
 * @retval 	: The index of the pressed key device or -1 if no key is pressed.
 * @author 	: chenningzhan
 * @note	: An open interface, called by other modules, for querying the ID of a pressed key
 * ******************************************************************************
 */
key_device_index_t key_device_return_index(void)
{
    key_device_t *p_key_dev_enter = key_device_get_pointer(KEY_ID_ENTER),
                 *p_key_dev_back = key_device_get_pointer(KEY_ID_BACK),
                 *p_key_dev_up = key_device_get_pointer(KEY_ID_UP),
                 *p_key_dev_down = key_device_get_pointer(KEY_ID_DOWN);

    // pointer check
    if (p_key_dev_enter == NULL || NULL == p_key_dev_back || p_key_dev_up == NULL || NULL == p_key_dev_down)
    {
        return -1;
    }
    else
    {
        // if key is pressed,return the key index
        if (KEY_STATUS_CLICK == p_key_dev_enter->params.status)
            return p_key_dev_enter->res->index;
        else if (KEY_STATUS_CLICK == p_key_dev_back->params.status)
            return p_key_dev_back->res->index;
        else if (KEY_STATUS_CLICK == p_key_dev_up->params.status)
            return p_key_dev_up->res->index;
        else if (KEY_STATUS_CLICK == p_key_dev_down->params.status)
            return p_key_dev_down->res->index;
        // if no key is pressed,return -1
        else
            return -1;
    }
}

/* ------------------ private operation function definition ----------------- */

/**
 * ******************************************************************************
 * @brief 	: Retrieve the current system tick count and store it in the provided pointer.
 * @param 	  p_tick  	: a pointer to where the current tick count will be stored.
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
static void key_device_ops_update_time(uint32_t *p_tick)
{
    *p_tick = HAL_GetTick();
}

/**
 * ******************************************************************************
 * @brief 	: update the current level of a key device.
 * @param 	  p_self  	: a pointer to get the device object.
 * @author 	: chenningzhan
 * @note	: reads the current level of the key pin and updates the key device's parameters.
 * ******************************************************************************
 */
static void key_device_ops_update_level(void *p_self)
{
    key_device_t *p_dev = (key_device_t *)p_self;

    p_dev->params.level_current = HAL_GPIO_ReadPin(p_dev->res->gpio_port, p_dev->res->pin);
}

/**
 * ******************************************************************************
 * @brief 	: update the status of a key device.
 * @param 	  p_self  	: a pointer to get the device object
 * @author 	: chenningzhan
 * @note	: checks the key's level and time to determine if the key has been pressed or held.
 *            It updates the key device's status accordingly and performs a callback function if the key is pressed.
 * ******************************************************************************
 */
static void key_device_ops_update_status(void *p_self)
{
    key_device_t *p_dev = (key_device_t *)p_self;

    // get current time
    uint32_t time_current;
    p_dev->ops.update_time(&time_current);

    // check the key level
    p_dev->ops.update_level((void *)p_dev);

    // check if the key is pressed and the time is valid
    if (p_dev->params.level_valid == p_dev->params.level_current && time_current > p_dev->params.time_next_press)
    {
        p_dev->params.status = KEY_STATUS_CLICK;

        // performs a callback function
        p_dev->call_func((void *)p_dev);

        // update the next time press the button
        p_dev->ops.update_time(&p_dev->params.time_next_press);
        p_dev->params.time_next_press += p_dev->params.time_interval;
    }
    else
    {
        p_dev->params.status = KEY_STATUS_IDLE;
    }
}

/* ----------------------- public function definition ----------------------- */

/**
 * ******************************************************************************
 * @brief 	: register keys device with its associated callback function.
 * @param 	  p_func  	: The callback function pointer for the device operations.
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
void key_device_register(p_callback_func_dev p_func)
{
    for (uint8_t i = 0; i < KEY_ID_COUNT; i++)
    {
        // gets a pointer to the global variable device
        key_device_t *p_dev = &g_key_devices[i];

        // bind the hardware resource
        p_dev->res = &g_key_device_resources[i];

        // set the default parameter
        p_dev->params.status = KEY_STATUS_IDLE;
        p_dev->params.time_interval = KEY_PRESS_INTERVAL_TIME;
        p_dev->params.level_valid = KEY_VALID_LEVEL;
        p_dev->params.level_current = !p_dev->params.level_valid;

        // set operation function
        p_dev->ops.update_time = key_device_ops_update_time;
        p_dev->ops.update_level = key_device_ops_update_level;
        p_dev->ops.update_status = key_device_ops_update_status;

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