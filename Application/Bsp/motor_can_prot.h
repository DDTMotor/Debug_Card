#ifndef __MOTOR_CAN_PROT_H__
#define __MOTOR_CAN_PROT_H__

/* ---------------------------- user header file ---------------------------- */
#include "can.h"
#include "motor_info.h"

/* ---------------------------- macro definition ---------------------------- */
#define AGT_CAN_BUFF_LEN 8    // 8 bytes
#define AGT_CAN_MULTI_COUNT 4 // one stdid can multi 4 motors
// Motor Drive Command
#define AGT_CAN_STD_DRIVE_TX 0X32
#define AGT_CAN_STD_DRIVE_RX 0X96
// Motor set mode Command
#define AGT_CAN_STD_SET_MODE_TX 0X105
#define AGT_CAN_STD_SET_MODE_RX 0X200
// Motor set feedback time Command
#define AGT_CAN_STD_SET_BACK_TIME_TX 0x106
#define AGT_CAN_STD_SET_BACK_TIME_RX 0x264
// Motor set id Command
#define AGT_CAN_STD_SET_ID_TX 0X108
#define AGT_CAN_STD_SET_ID_RX 0X96
// Motor CAN terminal resistance gating setting
#define AGT_CAN_STD_SET_RES_TX 0X109
#define AGT_CAN_STD_SET_RES_RX 0X390
// P1010A & P1010B
#define AGT_CAN_STD_DRIVE_TX 0X32
#define AGT_CAN_STD_SET_FEEDBACK_TX 0X34
#define AGT_CAN_STD_SET_ARG_TX 0X36
#define AGT_CAN_STD_SET_STATE_TX 0X38

#define AGT_CAN_ARG_SET_MODE_TX 0X1C
#define AGT_CAN_STD_RX_FEEDBACK 0XC1

/* --------------------------- struct and enum definitions --------------------------- */

/* -------------------------- function declaration -------------------------- */
void motor_can_send_enable(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *header, uint8_t *buff);
void motor_can_send_drive(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *header, uint8_t *buff, int16_t *target);
void motor_can_send_set_mode(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *header, uint8_t *buff, uint8_t *mode);
void motor_can_send_set_mode_P1010AB(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *header, uint8_t *buff);
void motor_can_send_set_feedback_time(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *header, uint8_t *buff, uint8_t *mode);
void motor_can_send_set_feedback_time_P1010AB(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *header, uint8_t *buff);
void motor_can_send_set_id(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *header, uint8_t *buff, uint8_t id);
void motor_can_send_set_res(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *header, uint8_t *buff, uint8_t *res);

#endif
