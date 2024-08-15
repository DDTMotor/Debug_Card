/**
 * ******************************************************************************
 * @Copyright:  Copyright  (C)  2021-2024 东莞市本末科技有限公司
 * @file 	: motor_uart_prot.h
 * ******************************************************************************
 * @brief 	: 电机UART协议层定义，协议详情请查看电机规格说明书
 * @version	: 1.0
 * @date 	: 2024-07-05
 * @note 	:
 * ******************************************************************************
 */
#ifndef __MOTOR_UART_PROT_H__
#define __MOTOR_UART_PROT_H__

/* ---------------------------- user header file ---------------------------- */
#include "motor_info.h"
#include "usart.h"

/* ---------------------------- macro definition ---------------------------- */
#define AGT_UART_BUFF_LEN 10
#define AGT_UART_CMD_DRIVE 0X64
#define AGT_UART_CMD_DRIVE_M0603A 0X65
#define AGT_UART_CMD_SET_MODE 0XA0
#define AGT_UART_CMD_GET_FEEDBACK 0X74

/* --------------------------- struct and enum definitions --------------------------- */
typedef enum
{
    RS485_DIR_RECV = 0u,
    RS485_DIR_SEND = 1,
} motor_uart_rs485_dir_t;

/* -------------------------- function declaration -------------------------- */
void motor_uart_send_buff(UART_HandleTypeDef *huart, uint8_t *buff);
void motor_uart_receive_buff(UART_HandleTypeDef *huart, uint8_t *buff);

void motor_uart_send_drive(uint8_t *buff, const uint8_t id, int16_t const speed);
void motor_uart_send_get_feedback(uint8_t *buff, uint8_t id);
void motor_uart_send_set_mode(uint8_t *buff, uint8_t id, motor_mode_t mode);
void motor_uart_send_set_mode_M0603A(uint8_t *buff, uint8_t id, motor_mode_t mode);
void motor_uart_send_set_id(uint8_t *buff, uint8_t id);
void motor_uart_send_get_check(uint8_t *buff);

#endif