#ifndef __DEVICE_LED_H__
#define __DEVICE_LED_H__

/* ---------------------------- user header file ---------------------------- */
#include "./device.h"


/* --------------------------- struct and enum definitions --------------------------- */
/**
 * @brief Enumeration for the led device status.
 */
typedef enum
{
    LED_STATUS_OFF = 0,
    LED_STATUS_ON = 1,
    LED_STATUS_UNKNOWN,
} led_device_status_t;

/**
 * @brief Enumeration for the led device index.
 */
typedef enum
{
    LED_ID_R = 0,
    LED_ID_G,
    LED_ID_B,
    LED_ID_COUNT,
} led_device_index_t;

/**
 * @brief Structure to hold the hardware resources of a leds device.
 */
typedef struct
{
    led_device_index_t index;
    GPIO_PinState level_valid;
    GPIO_TypeDef *gpio_port;
    uint16_t pin;
} led_device_resources_t;

/**
 * @brief Structure to hold the device parameters and operations.
 */
typedef struct
{
    led_device_status_t status;
} led_device_params_t;

/**
 * @brief Structure to hold the device operations.
 */
typedef struct
{
    void (*control)(void *p_self, led_device_status_t arg_status);
} led_device_ops_t;

/**
 * @brief Structure to represent a led device.
 */
typedef struct
{
    const led_device_resources_t *res;
    led_device_params_t params;
    led_device_ops_t ops;
    p_callback_func_dev call_func;
} led_device_t;

/* -------------------------- function declaration -------------------------- */

led_device_t *led_device_get_pointer(led_device_index_t arg_index);

void led_device_register(p_callback_func_dev p_func);

#endif
