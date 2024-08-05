#ifndef __DEVICE_BUZZER_H__
#define __DEVICE_BUZZER_H__

/* ---------------------------- user header file ---------------------------- */
#include "./device.h"
#include "tim.h"

/* ---------------------------- macro definition ---------------------------- */

/* --------------------------- struct definitions --------------------------- */

/**
 * @brief Enumeration for the buzzer device status.
 */
typedef enum
{
    BUZZER_STATUS_OFF = 0,
    BUZZER_STATUS_ON = 1,
    BUZZER_STATUS_UNKNOWN,
} buzzer_status_t;

/**
 * @brief Structure to hold the hardware resources of a buzzer device.
 */
typedef struct
{
    TIM_HandleTypeDef *timer;
    uint32_t channel;
} buzzer_device_resources_t;

typedef struct
{
    buzzer_status_t status; //
    uint32_t ccr;           // compare register value for PWM control
} buzzer_device_params_t;

typedef struct
{
    void (*enable)(void *p_self);
    void (*control)(void *p_self, buzzer_status_t arg_status);
} buzzer_device_ops_t;

typedef struct
{
    const buzzer_device_resources_t *res;
    buzzer_device_params_t params;
    buzzer_device_ops_t ops;
    p_callback_func_dev call_func;
} buzzer_device_t;

/* -------------------------- function declaration -------------------------- */

buzzer_device_t *buzzer_device_get_pointer(void);

void buzzer_device_register(p_callback_func_dev p_func);

#endif
