/**
 * ******************************************************************************
 * @Copyright:  Copyright  (C)  2021-2024 东莞市本末科技有限公司
 * @file 	: motor_uart_prot.c
 * ******************************************************************************
 * @brief 	: 【introduction】
 * @version	: 1.0
 * @date 	: 2024-08-02
 * @note 	: 【note】
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
 * * 2. Modify the macro definitions and enums in 'header_file.h'.
 * * 3. change the resources in 'files_name.c':
 * * 4. Registers the device by calling the provided function using a callback.
 * * 5. Use the pointer to get the operation handle and use the device.
 * *
 * ******************************************************************************
 */

/* ---------------------------- user header file ---------------------------- */
#include "motor_uart_prot.h"

/* ----------------------- global variable definition ----------------------- */

// crc inspection table
// Model:CRC-8/MAXIM  polynomial x8 + x5 + x4 + 1
const char g_CRC8Table[] =
    {
        0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
        157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
        35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
        190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
        70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
        219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
        101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
        248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
        140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
        17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
        175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
        50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
        202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
        87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
        233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
        116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53};

/***** here it is function Definition ********************/

/**
 * ******************************************************************************
 * @brief 	: 根据提供的数组和元素数量计算并返回CRC8校验值
 * @param 	: p [in/out], 指向要计算CRC的数据数组的指针
 * @param 	: counter [in/out], 数组中的元素数量
 * @retval 	: 计算得到的CRC8值
 * @author 	: chenningzhan
 * @note	: 该函数使用预定义的CRC-8/MAXIM查找表来计算校验值
 * ******************************************************************************
 */
unsigned char CRC8_calc(unsigned char *p, int counter)
{
    unsigned char crc8 = 0;

    for (; counter > 0; counter--)
    {
        crc8 = g_CRC8Table[crc8 ^ *p];
        p++;
    }
    return (crc8);
}

/**
 * ******************************************************************************
 * @brief 	: 通过串口将buff发送给电机
 * @param 	: huart [in/out], UART句柄结构的指针
 * @param 	: buff [in/out], 要发送的数据缓冲区的指针
 * @param 	: rx_count [in/out], 接收计数的指针，发送后该值将被清零
 * @author 	: chenningzhan
 * @note	: 发送数据前，该函数会设置RS485通信方向；发送完成后，恢复RS485通信方向
 * ******************************************************************************
 */
void motor_uart_send_buff(UART_HandleTypeDef *huart, uint8_t *buff)
{
    if (huart->Instance == USART1)
    {
        // set the rs485 communication direction to send
        HAL_GPIO_WritePin(RS485_DIR_GPIO_Port, RS485_DIR_Pin, GPIO_PIN_SET);
        // send the data to the motor
        HAL_UART_Transmit(huart, buff, (uint16_t)AGT_UART_BUFF_LEN, 0xffff);
        // set the rs485 communication direction to receive
        HAL_GPIO_WritePin(RS485_DIR_GPIO_Port, RS485_DIR_Pin, GPIO_PIN_RESET);
    }
    else if (huart->Instance == USART3)
    {
        // send the data to the motor
        HAL_UART_Transmit(huart, buff, (uint16_t)AGT_UART_BUFF_LEN, 0xffff);
    }
}

/**
 * ******************************************************************************
 * @brief 	: UART发送驱动指令
 * @param 	: buff [in/out], 发送缓冲区
 * @param 	: id [in/out], 电机id
 * @param 	: speed [in/out], 期望速度
 * @author 	: chenningzhan
 * @note	: None
 * ******************************************************************************
 */
void motor_uart_send_drive(uint8_t *buff, const uint8_t id, int16_t const speed)
{
    buff[0] = id;
    buff[1] = (uint8_t)AGT_UART_CMD_DRIVE;
    buff[2] = speed >> 8;
    buff[3] = speed & 0x00ff;
    buff[4] = 0;
    buff[5] = 0;
    buff[6] = 0;
    buff[7] = 0;
    buff[8] = 0;
    buff[9] = CRC8_calc(buff, 9);
}

/**
 * ******************************************************************************
 * @brief 	: UART发送获取电机反馈指令
 * @param 	: buff [in/out], 发送缓冲区
 * @param 	: id [in/out], 电机id
 * @author 	: chenningzhan
 * @note	: None
 * ******************************************************************************
 */
void motor_uart_send_get_feedback(uint8_t *buff, uint8_t id)
{
    buff[0] = id;
    buff[1] = (uint8_t)AGT_UART_CMD_GET_FEEDBACK;
    buff[2] = 0;
    buff[3] = 0;
    buff[4] = 0;
    buff[5] = 0;
    buff[6] = 0;
    buff[7] = 0;
    buff[8] = 0;
    buff[9] = CRC8_calc(buff, 9);
}

/**
 * ******************************************************************************
 * @brief 	: UART发送设置模式指令
 * @param 	: buff [in/out], 发送缓冲区
 * @param 	: id [in/out], 电机id
 * @param 	: mode [in/out], 电机驱动模式
 * @author 	: chenningzhan
 * @note	: 该指令用于改变电机的运行模式，如电压开环、电流闭环等
 * ******************************************************************************
 */
void motor_uart_send_set_mode(uint8_t *buff, uint8_t id, motor_mode_t mode)
{
    buff[0] = id;
    buff[1] = (uint8_t)AGT_UART_CMD_SET_MODE;
    buff[2] = 0;
    buff[3] = 0;
    buff[4] = 0;
    buff[5] = 0;
    buff[6] = 0;
    buff[7] = 0;
    buff[8] = 0;
    buff[9] = (uint8_t)mode;
}

/**
 * ******************************************************************************
 * @brief 	: UART发送设置模式指令(M0603A)
 * @param 	: buff [in/out], 发送缓冲区
 * @param 	: id [in/out], 电机id
 * @param 	: mode [in/out], 电机驱动模式
 * @author 	: chenningzhan
 * @note	: 该指令用于改变电机的运行模式，如电压开环、电流闭环等
 * ******************************************************************************
 */
void motor_uart_send_set_mode_M0603A(uint8_t *buff, uint8_t id, motor_mode_t mode)
{
    buff[0] = id;
    buff[1] = (uint8_t)AGT_UART_CMD_SET_MODE;
    buff[2] = mode;
    buff[3] = 0;
    buff[4] = 0;
    buff[5] = 0;
    buff[6] = 0;
    buff[7] = 0;
    buff[8] = 0;
    buff[9] = CRC8_calc(buff, 9);
}

/**
 * ******************************************************************************
 * @brief 	: UART发送设置电机id指令
 * @param 	: buff [in/out], 发送缓冲区
 * @param 	: id [in/out], 电机id
 * @author 	: chenningzhan
 * @note	: 设置 ID 时请保证总线上只有一个电机，每次上电只允许设置一次
 *            电机接收到 5 次 ID设置指令后进行设置
 * ******************************************************************************
 */
void motor_uart_send_set_id(uint8_t *buff, uint8_t id)
{
    buff[0] = 0xAA;
    buff[1] = 0x55;
    buff[2] = 0x53;
    buff[3] = id;
    buff[4] = 0;
    buff[5] = 0;
    buff[6] = 0;
    buff[7] = 0;
    buff[8] = 0;
    buff[9] = 0;
}

/**
 * ******************************************************************************
 * @brief 	: UART发送查询指令
 * @param 	: buff [in/out], 发送缓冲区
 * @author 	: chenningzhan
 * @note	: 该指令用于请求电机返回其当前状态和参数信息
 * ******************************************************************************
 */
void motor_uart_send_get_check(uint8_t *buff)
{
    buff[0] = 0xc8;
    buff[1] = 0x64;
    buff[2] = 0;
    buff[3] = 0;
    buff[4] = 0;
    buff[5] = 0;
    buff[6] = 0;
    buff[7] = 0;
    buff[8] = 0;
    buff[9] = CRC8_calc(buff, 9);
}
