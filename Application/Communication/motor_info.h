/**
 * ******************************************************************************
 * @Copyright:  Copyright  (C)  2021-2024 东莞市本末科技有限公司
 * @file 	: motor_info.h
 * ******************************************************************************
 * @brief 	: Motor Protocol layer definitions for supporting motor model information, motor mode
 *            specific information to receive feedback, please refer to motor specification guide for details
 * @version	: 1.0
 * @date 	: 2024-08-02
 * @note 	: about UART, RS485, CAN, Lin communication protocols unified in the motor CAN again do protocol layer definition
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
 * ******* 3. HOW TO USE *********
 * * 0. Include this file into your current project.
 * * 1. include the header file where needed.
 * * 2. Modify the macro definitions and enums in 'motor_info.h'.
 * * 3. change the resources in 'files_name.c':
 * * 4. Registers the device by calling the provided function using a callback.
 * * 5. Use the pointer to get the operation handle and use the device.
 * *
 * ******************************************************************************
 */
#ifndef __MOTOR_INFO_H__
#define __MOTOR_INFO_H__

/* ---------------------------- user header file ---------------------------- */
#include "lib.h"

/* ---------------------------- macro definition ---------------------------- */

/* --------------------------- struct and enum definitions --------------------------- */
typedef enum
{
    MOTOR_ID_M0601C = 0,
    MOTOR_ID_M0602C = 1,
    MOTOR_ID_M0603C = 2,
    MOTOR_ID_M0603A = 3,
    MOTOR_ID_M1502E = 4,
    MOTOR_ID_M1502D = 5,
    MOTOR_ID_M1505A = 6,
    MOTOR_ID_P1010A = 7,
    MOTOR_ID_P1010B = 8,
    MOTOR_ID_COUNT,
} motor_models_index_t;

// the motor communication means enum definition
typedef enum
{
    MEANS_NONE = 0,
    MEANS_CAN = 1,
    MEANS_RS485 = 2,
    MEANS_UART = 3,
} motor_connect_means_t;

// the motor mode enum definition
typedef enum
{
    OPEN_VOLTAGE = 0x00,  // Voltage Open Loop
    LOOP_CURRENT = 0x01,  //
    LOOP_SPEED = 0x02,    //
    LOOP_POSITION = 0x03, // the driving mode is to reach the target position by the shortest route
    MOTOR_START = 0x08,   // M0603A enable
    MOTOR_DISABLE = 0x09, // disabled motor
    MOTOR_ENANLE = 0x0A,  // enable motor (default enable)
} motor_mode_t;

// the motor recive data structure definition
typedef struct
{
    uint8_t id;          // ID of motor
    uint8_t temperature; // temperature of motor
    uint16_t current;    // electric current of motor
    int16_t speed;       // FeedBack Speed of motor
    uint8_t errcode;     // Fault code of motor
    motor_mode_t mode;   // Feedback Mode of motor
    uint16_t position;   // Feedback Position of motor
} motor_rec_t;

/**
 * @brief 	: Note that in the motor speed ring drive instruction
 * @brief 	: the desired speed of some models needs to be magnified (10 or 100 times)
 * @brief 	: so the maximum speed also needs to be multiplied by this coefficient
 */
typedef struct
{
    uint8_t index;               // motor models index
    char *name;                  // motor name
    motor_connect_means_t means; // motor communication means
    uint32_t baudrate;           // baud rate of motor communication means
    int16_t unloaded_speed;      // the max speed of the motor(speed loop)
} motor_info_table_t;

/* -------------------------- function declaration -------------------------- */

int16_t motor_info_get_target_speed(motor_models_index_t index, int8_t percent);
motor_connect_means_t motor_info_get_means(motor_models_index_t index);
uint32_t motor_info_get_baudrate(motor_models_index_t index);

#endif
