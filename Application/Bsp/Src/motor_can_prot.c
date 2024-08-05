/**
 * ******************************************************************************
 * @Copyright:  Copyright  (C)  2021-2024 东莞市本末科技有限公司
 * @file 	: motor_can_prot.c
 * ******************************************************************************
 * @brief 	: the motor can communication drives the interface layer and provides an operation interface to the device layer
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
#include "motor_can_prot.h"

/* ----------------------- global variable definition ----------------------- */

/**
 * ******************************************************************************
 * @brief 	: send a can enable command
 * @param 	  hcan  	: 【parameter】
 * @param 	  header  	: 【parameter】
 * @param 	  buff  	: 【parameter】
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
void motor_can_send_enable(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *header, uint8_t *buff)
{
    uint32_t send_mail_box;
    header->StdId = AGT_CAN_STD_SET_STATE_TX;
    header->IDE = CAN_ID_STD;
    header->RTR = CAN_RTR_DATA;
    header->DLC = AGT_CAN_BUFF_LEN;
    buff[0] = 0x02;
    buff[1] = 0x02;
    buff[2] = 0x02;
    buff[3] = 0x02;
    buff[4] = 0x02;
    buff[5] = 0x02;
    buff[6] = 0x02;
    buff[7] = 0x02;

    HAL_CAN_AddTxMessage(hcan, header, buff, &send_mail_box);
}

/**
 * ******************************************************************************
 * @brief 	: send a can driver command
 * @param 	  hcan  	: 【parameter】
 * @param 	  header  	: 【parameter】
 * @param 	  buff  	: 【parameter】
 * @param 	  target  	: 【parameter】
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
void motor_can_send_drive(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *header, uint8_t *buff, int16_t *target)
{
    uint32_t send_mail_box;
    header->StdId = AGT_CAN_STD_DRIVE_TX;
    header->IDE = CAN_ID_STD;
    header->RTR = CAN_RTR_DATA;
    header->DLC = AGT_CAN_BUFF_LEN;
    buff[0] = target[0] >> 8;
    buff[1] = target[0];
    buff[2] = target[1] >> 8;
    buff[3] = target[1];
    buff[4] = target[2] >> 8;
    buff[5] = target[2];
    buff[6] = target[3] >> 8;
    buff[7] = target[3];

    HAL_CAN_AddTxMessage(hcan, header, buff, &send_mail_box);
}

/**
 * ******************************************************************************
 * @brief 	: the can setting mode command is transmitted
 * @param 	  hcan  	: 【parameter】
 * @param 	  header  	: 【parameter】
 * @param 	  buff  	: 【parameter】
 * @param 	  mode  	: 【parameter】
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
void motor_can_send_set_mode(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *header, uint8_t *buff, uint8_t *mode)
{
    uint32_t send_mail_box;
    header->StdId = AGT_CAN_STD_SET_MODE_TX;
    header->IDE = CAN_ID_STD;
    header->RTR = CAN_RTR_DATA;
    header->DLC = AGT_CAN_BUFF_LEN;
    buff[0] = mode[0];
    buff[1] = mode[1];
    buff[2] = mode[2];
    buff[3] = mode[3];
    buff[4] = mode[4];
    buff[5] = mode[5];
    buff[6] = mode[6];
    buff[7] = mode[7];

    HAL_CAN_AddTxMessage(hcan, header, buff, &send_mail_box);
}

/**
 * ******************************************************************************
 * @brief 	: the can setting mode command is transmitted(P1010A/P1010B)
 * @param 	  hcan  	: 【parameter】
 * @param 	  header  	: 【parameter】
 * @param 	  buff  	: 【parameter】
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
void motor_can_send_set_mode_P1010AB(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *header, uint8_t *buff)
{
    uint32_t send_mail_box;
    header->StdId = AGT_CAN_STD_SET_ARG_TX;
    header->IDE = CAN_ID_STD;
    header->RTR = CAN_RTR_DATA;
    header->DLC = AGT_CAN_BUFF_LEN;
    buff[0] = 0X01;
    buff[1] = AGT_CAN_ARG_SET_MODE_TX;
    buff[2] = 0X03;
    buff[3] = 0;
    buff[4] = 0;
    buff[5] = 0;
    buff[6] = 0;
    buff[7] = 0;

    HAL_CAN_AddTxMessage(hcan, header, buff, &send_mail_box);
}

/**
 * ******************************************************************************
 * @brief 	: sends a can setting feedback time command
 * @param 	  hcan  	: 【parameter】
 * @param 	  header  	: 【parameter】
 * @param 	  buff  	: 【parameter】
 * @param 	  time  	: 【parameter】
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
void motor_can_send_set_feedback_time(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *header, uint8_t *buff, uint8_t *time)
{
    uint32_t send_mail_box;
    header->StdId = AGT_CAN_STD_SET_BACK_TIME_TX;
    header->IDE = CAN_ID_STD;
    header->RTR = CAN_RTR_DATA;
    header->DLC = AGT_CAN_BUFF_LEN;
    buff[0] = time[0];
    buff[1] = time[1];
    buff[2] = time[2];
    buff[3] = time[3];
    buff[4] = time[4];
    buff[5] = time[5];
    buff[6] = time[6];
    buff[7] = time[7];

    HAL_CAN_AddTxMessage(hcan, header, buff, &send_mail_box);
}

/**
 * ******************************************************************************
 * @brief 	: send can to set feedback time(P1010A/B)
 * @param 	  hcan  	: 【parameter】
 * @param 	  header  	: 【parameter】
 * @param 	  buff  	: 【parameter】
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
void motor_can_send_set_feedback_time_P1010AB(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *header, uint8_t *buff)
{
    uint32_t send_mail_box;
    header->StdId = AGT_CAN_STD_SET_FEEDBACK_TX;
    header->IDE = CAN_ID_STD;
    header->RTR = CAN_RTR_DATA;
    header->DLC = AGT_CAN_BUFF_LEN;
    buff[0] = 0X01;
    buff[1] = 0X01;
    buff[2] = 0X7f;
    buff[3] = 0X01;
    buff[4] = 0X02;
    buff[5] = 0X04;
    buff[6] = 0X08; // 温度
    buff[7] = 0X00;

    HAL_CAN_AddTxMessage(hcan, header, buff, &send_mail_box);
}

/**
 * ******************************************************************************
 * @brief 	: send the can setup id command
 * @param 	  hcan  	: 【parameter】
 * @param 	  header  	: 【parameter】
 * @param 	  buff  	: 【parameter】
 * @param 	  id  	: 【parameter】
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
void motor_can_send_set_id(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *header, uint8_t *buff, uint8_t id)
{
    uint32_t send_mail_box;
    header->StdId = AGT_CAN_STD_SET_ID_TX;
    header->IDE = CAN_ID_STD;
    header->RTR = CAN_RTR_DATA;
    header->DLC = AGT_CAN_BUFF_LEN;
    buff[0] = id;
    buff[1] = 0;
    buff[2] = 0;
    buff[3] = 0;
    buff[4] = 0;
    buff[5] = 0;
    buff[6] = 0;
    buff[7] = 0;

    HAL_CAN_AddTxMessage(hcan, header, buff, &send_mail_box);
}

/**
 * ******************************************************************************
 * @brief 	: transmit can setting termination resistor strobe setting
 * @param 	  hcan  	: 【parameter】
 * @param 	  header  	: 【parameter】
 * @param 	  buff  	: 【parameter】
 * @param 	  res  	: 【parameter】
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
void motor_can_send_set_res(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *header, uint8_t *buff, uint8_t *res)
{
    uint32_t send_mail_box;
    header->StdId = AGT_CAN_STD_SET_RES_TX;
    header->IDE = CAN_ID_STD;
    header->RTR = CAN_RTR_DATA;
    header->DLC = AGT_CAN_BUFF_LEN;
    buff[0] = res[0];
    buff[1] = res[1];
    buff[2] = res[2];
    buff[3] = res[3];
    buff[4] = res[4];
    buff[5] = res[5];
    buff[6] = res[6];
    buff[7] = res[7];

    HAL_CAN_AddTxMessage(hcan, header, buff, &send_mail_box);
}
