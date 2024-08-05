#ifndef __DEVICE_H__
#define __DEVICE_H__

/* --------------------------- library header file -------------------------- */
#include "stdlib.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* ---------------------------- user header file ---------------------------- */
#include "log.h"
#include "main.h"
#include "stm32f1xx_hal.h"

/* ---------------------------- macro definition ---------------------------- */

/**
 * @brief callback function pointer type definition.
 */
typedef void (*p_callback_func_dev)(void *p_self);

/**
 * @brief NULL definition safeguard.
 */
#ifndef NULL
#define NULL ((void *)(0))
#endif

/* -------------------------- function declaration -------------------------- */

#endif