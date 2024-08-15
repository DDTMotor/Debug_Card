#ifndef __TASK_MOTOR_UART_H__
#define __TASK_MOTOR_UART_H__

/* ---------------------------- user header file ---------------------------- */
#include "device_motor_uart.h"

/* -------------------------- function declaration -------------------------- */

void mytask_uart_entry(void *p_self);

void motor_drive_callback(uint8_t index, uint16_t speed);

#endif
