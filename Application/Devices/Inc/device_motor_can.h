#ifndef __DEVICE_MOTOR_CAN_H__
#define __DEVICE_MOTOR_CAN_H__

/* ---------------------------- user header file ---------------------------- */
#include "./device.h"
#include "motor_can_prot.h"

/* ---------------------------- macro definition ---------------------------- */
#define MOTOR_DEVICE_CAN_BUFF_TX_LEN (AGT_CAN_BUFF_LEN * 1)
#define MOTOR_DEVICE_CAN_BUFF_RX_LEN (AGT_CAN_BUFF_LEN * 1)

/* --------------------------- struct and enum definitions --------------------------- */
typedef struct
{
    void (*reinit)(void *p_self);
    void (*enable)(void *p_self);
    void (*parse_data)(void *p_self);
    void (*receive_buff)(void *p_self);
    void (*send_drive)(void *p_self);
    void (*send_set_mode)(void *p_self);
    void (*send_set_feedback_time)(void *p_self);
    void (*send_set_id)(void *p_self);
    void (*send_set_res)(void *p_self);
    void (*get_index)(void *p_self);
    void (*get_means)(void *p_self);
    void (*get_target)(void *p_self);
    void (*get_baudrate)(void *p_self);
} motor_device_can_ops_t;

typedef struct
{
    CAN_HandleTypeDef *hcanx;
    uint32_t rxfifo;
    CAN_TxHeaderTypeDef htx;
    CAN_RxHeaderTypeDef hrx;
    uint8_t buff_tx[MOTOR_DEVICE_CAN_BUFF_TX_LEN];
    uint8_t buff_rx[MOTOR_DEVICE_CAN_BUFF_RX_LEN];
    uint32_t baudrate;
    uint32_t Prescaler;
    uint32_t TimeSeg1;
    uint32_t TimeSeg2;
    bool flag_initialized;
    int16_t motor_target[AGT_CAN_MULTI_COUNT];
    uint8_t motor_mode[AGT_CAN_BUFF_LEN];
    uint8_t motor_feedback_time[AGT_CAN_BUFF_LEN];
    uint8_t motor_id;
    motor_rec_t motor_rec;
    motor_models_index_t motor_index;
    motor_connect_means_t motor_means;
    uint8_t motor_res[AGT_CAN_BUFF_LEN];
    motor_device_can_ops_t ops;
    p_callback_func_dev call_func;
} motor_device_can_t;

/* -------------------------- function declaration -------------------------- */
motor_device_can_t *motor_device_can_get_pointer(void);

void motor_device_can_register(p_callback_func_dev p_func);

#endif