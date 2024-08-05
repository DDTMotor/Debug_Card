/**
 * ******************************************************************************
 * @Copyright:  Copyright  (C)  2021-2024 ��ݸ�б�ĩ�Ƽ����޹�˾
 * @file 	: motor_uart_prot.c
 * ******************************************************************************
 * @brief 	: ��introduction��
 * @version	: 1.0
 * @date 	: 2024-08-02
 * @note 	: ��note��
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
 * @brief 	: �����ṩ�������Ԫ���������㲢����CRC8У��ֵ
 * @param 	: p [in/out], ָ��Ҫ����CRC�����������ָ��
 * @param 	: counter [in/out], �����е�Ԫ������
 * @retval 	: ����õ���CRC8ֵ
 * @author 	: chenningzhan
 * @note	: �ú���ʹ��Ԥ�����CRC-8/MAXIM���ұ�������У��ֵ
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
 * @brief 	: ͨ�����ڽ�buff���͸����
 * @param 	: huart [in/out], UART����ṹ��ָ��
 * @param 	: buff [in/out], Ҫ���͵����ݻ�������ָ��
 * @param 	: rx_count [in/out], ���ռ�����ָ�룬���ͺ��ֵ��������
 * @author 	: chenningzhan
 * @note	: ��������ǰ���ú���������RS485ͨ�ŷ��򣻷�����ɺ󣬻ָ�RS485ͨ�ŷ���
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
 * @brief 	: UART��������ָ��
 * @param 	: buff [in/out], ���ͻ�����
 * @param 	: id [in/out], ���id
 * @param 	: speed [in/out], �����ٶ�
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
 * @brief 	: UART���ͻ�ȡ�������ָ��
 * @param 	: buff [in/out], ���ͻ�����
 * @param 	: id [in/out], ���id
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
 * @brief 	: UART��������ģʽָ��
 * @param 	: buff [in/out], ���ͻ�����
 * @param 	: id [in/out], ���id
 * @param 	: mode [in/out], �������ģʽ
 * @author 	: chenningzhan
 * @note	: ��ָ�����ڸı���������ģʽ�����ѹ�����������ջ���
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
 * @brief 	: UART��������ģʽָ��(M0603A)
 * @param 	: buff [in/out], ���ͻ�����
 * @param 	: id [in/out], ���id
 * @param 	: mode [in/out], �������ģʽ
 * @author 	: chenningzhan
 * @note	: ��ָ�����ڸı���������ģʽ�����ѹ�����������ջ���
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
 * @brief 	: UART�������õ��idָ��
 * @param 	: buff [in/out], ���ͻ�����
 * @param 	: id [in/out], ���id
 * @author 	: chenningzhan
 * @note	: ���� ID ʱ�뱣֤������ֻ��һ�������ÿ���ϵ�ֻ��������һ��
 *            ������յ� 5 �� ID����ָ����������
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
 * @brief 	: UART���Ͳ�ѯָ��
 * @param 	: buff [in/out], ���ͻ�����
 * @author 	: chenningzhan
 * @note	: ��ָ�����������������䵱ǰ״̬�Ͳ�����Ϣ
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
