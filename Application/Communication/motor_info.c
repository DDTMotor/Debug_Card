/**
 * ******************************************************************************
 * @Copyright:  Copyright  (C)  2021-2024 东莞市本末科技有限公司
 * @file 	: motor_info.c
 * ******************************************************************************
 * @brief 	: motor info list
 * @version	: 1.0
 * @date 	: 2024-08-02
 * @note 	: Contains information about the motor support list
 *            as well as functions to get the desired speed of the motor and control the mode of communication
 * *
 * *
 * ******* 1. Improvement *********
 * * 1. improvement the function :
 * *
 * * 2. improvement the function :
 * *
 * *
 * ******* 2. Fix the bug *********
 * * 1. fix the bug of :
 * *
 * *
 * *
 * ******************************************************************************
 */
/* ---------------------------- macro definition ---------------------------- */
#include "motor_info.h"

/* ----------------------- global variable definition ----------------------- */
motor_info_table_t g_motor_info_table[MOTOR_ID_COUNT] = {
    {MOTOR_ID_M0601C, "M0601C", MEANS_RS485, 115200, 330},
    {MOTOR_ID_M0602C, "M0602C", MEANS_RS485, 115200, 315},
    {MOTOR_ID_M0603C, "M0603C", MEANS_UART, 115200, 2100},
    {MOTOR_ID_M0603A, "M0603A", MEANS_UART, 38400, 3800},
    {MOTOR_ID_M1502E, "M1502E", MEANS_CAN, 500000, 21000},
    {MOTOR_ID_M1502D, "M1502D", MEANS_CAN, 1000000, 21000},
    {MOTOR_ID_M1505A, "M1505A", MEANS_CAN, 1000000, 2500},
    {MOTOR_ID_P1010A, "P1010A", MEANS_CAN, 1000000, 1500},
    {MOTOR_ID_P1010B, "P1010B", MEANS_CAN, 1000000, 1500},
};

/**
 * ******************************************************************************
 * @brief 	: obtain the expected speed corresponding to the motor model
 * @param 	  index  	: motor model index
 * @param 	  percent  	: expected speed percentage
 * @retval 	: expect speed
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
int16_t motor_info_get_target_speed(motor_models_index_t index, int8_t percent)
{
    int16_t speed = 0;

    if (index >= MOTOR_ID_COUNT)
    {
        return 0;
    }
    speed = g_motor_info_table[index].unloaded_speed * percent / 100.f;
    return speed;
}

/**
 * ******************************************************************************
 * @brief 	: obtain the control communication mode corresponding to the motor model
 * @param 	  index  	: motor model index
 * @retval 	: communication mode
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
motor_connect_means_t motor_info_get_means(motor_models_index_t index)
{
    motor_connect_means_t means;
    means = g_motor_info_table[index].means;
    return means;
}

/**
 * ******************************************************************************
 * @brief 	: obtain the baud rate corresponding to the motor model
 * @param 	  index  	: motor model index
 * @retval 	: baud rate
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
uint32_t motor_info_get_baudrate(motor_models_index_t index)
{
    uint32_t baudrate;
    baudrate = g_motor_info_table[index].baudrate;
    return baudrate;
}