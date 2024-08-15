#ifndef __DEVICE_KEY_H__
#define __DEVICE_KEY_H__

/* ---------------------------- user header file ---------------------------- */
#include "./device.h"

/* ---------------------------- macro definition ---------------------------- */
#define KEY_VALID_LEVEL GPIO_PIN_RESET
#define KEY_PRESS_INTERVAL_TIME 200

/* --------------------------- struct and enum definitions --------------------------- */

/**
 * @brief Enumeration for the key device status.
 */
typedef enum
{
    KEY_STATUS_IDLE = 0, // key is idle
    KEY_STATUS_CLICK,    // key is pressed click
    KEY_STATUS_HOLD,     // key is press long time
    KEY_STATUS_UNKNOWN,  //
} key_device_status_t;

/**
 * @brief Enumeration for the key device index.
 */
typedef enum
{
    KEY_ID_ENTER = 0x00,
    KEY_ID_BACK,
    KEY_ID_UP,
    KEY_ID_DOWN,
    KEY_ID_COUNT,
} key_device_index_t;

/**
 * @brief The parameter structure of the device.
 */
typedef struct
{
    key_device_status_t status;  //
    GPIO_PinState level_valid;   //
    GPIO_PinState level_current; // current state
    uint16_t time_hold;          // the press time
    uint32_t time_interval;      // the key is pressed at an interval (ms)
    uint32_t time_next_press;    // the next time to check the status
} key_device_params_t;

/**
 * @brief Structure to hold the hardware resources of a keys device.
 */
typedef struct
{
    key_device_index_t index;
    GPIO_TypeDef *gpio_port;
    uint16_t pin;
} key_device_resources_t;


/**
 * @brief Structure to hold the operations of a keys device.
 */
typedef struct
{
    void (*update_time)(uint32_t *p_tick);
    void (*update_level)(void *p_self);
    void (*update_status)(void *p_self);
} key_device_ops_t;

/**
 * @brief Structure to hold the keys device.
 */
typedef struct
{
    const key_device_resources_t *res;
    key_device_params_t params;
    key_device_ops_t ops;
    p_callback_func_dev call_func;
} key_device_t;

/* -------------------------- function declaration -------------------------- */
key_device_index_t key_device_return_index(void);

key_device_t *key_device_get_pointer(key_device_index_t arg_index);

void key_device_register(p_callback_func_dev p_func);

#endif
