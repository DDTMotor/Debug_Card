#ifndef __LIB_H__
#define __LIB_H__

/* --------------------------- library header file -------------------------- */
#include "stdlib.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* ---------------------------- user header file ---------------------------- */
#include "stm32f1xx_hal.h"
#define APP_VERSION "V1.1"

/* ---------------------------- macro definition ---------------------------- */

/**
 * data type redefinitions for volatile unsigned integers.
 */
typedef volatile unsigned char vuint8_t;
typedef volatile unsigned short vuint16_t;
typedef volatile unsigned int vuint32_t;

/**
 * NULL definition safeguard.
 */
#ifndef NULL
#define NULL ((void *)(0))
#endif

/**
 * calculates the number of elements in an array.
 */
#define ARRAY_NUM(arr) (sizeof(arr) / sizeof(arr[0]))

/**
 * https://www.zhaixue.cc/c-arm/c-arm-express.html
 */
#define MAX_TWO(type, x, y) ({ \
    type _x = x;               \
    type _y = y;               \
    _x > _y ? _x : _y;         \
})

typedef enum
{
    ERR_OK = 0,
    ERR_ARG_,
} error_t;

/* -------------------------- function declaration -------------------------- */

#endif

