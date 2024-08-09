#ifndef __DEVICE_MOTOR_UART_H__
#define __DEVICE_MOTOR_UART_H__

/* ---------------------------- user header file ---------------------------- */
#include "./device.h"
#include "motor_uart_prot.h"

/* ---------------------------- macro definition ---------------------------- */
#define MOTOR_DEVICE_UART_BUFF_TX_LEN (AGT_UART_BUFF_LEN * 1)
#define MOTOR_DEVICE_UART_BUFF_RX_LEN (AGT_UART_BUFF_LEN * 1)

/* --------------------------- struct and enum definitions --------------------------- */
typedef struct
{
    void (*reinit)(void *p_self);
    void (*parse_data)(void *p_self);
    void (*receive_buff)(void *p_self);

    void (*send_buff)(void *p_self);
    void (*send_drive)(void *p_self);
    void (*send_get_feedback)(void *p_self);
    void (*send_set_mode)(void *p_self);
    void (*send_set_id)(void *p_self);
    void (*send_get_check)(void *p_self);

    void (*get_index)(void *p_self);
    void (*get_means)(void *p_self);
    void (*get_target)(void *p_self);
    void (*get_baudrate)(void *p_self);
    void (*set_huartx)(void *p_self);
} motor_uart_ops_t;

typedef struct
{
    UART_HandleTypeDef *huartx;
    uint8_t buff_tx[MOTOR_DEVICE_UART_BUFF_TX_LEN];
    uint8_t buff_rx[MOTOR_DEVICE_UART_BUFF_RX_LEN];
    uint8_t buff_temp;
    uint32_t baudrate;
    bool flag_initialized;
    int16_t motor_target;
    uint8_t motor_mode;
    uint8_t motor_id;
    motor_models_index_t motor_index;
    motor_connect_means_t motor_means;
    motor_rec_t motor_rec;
    uint8_t count_rx;
    motor_uart_ops_t ops;
    p_callback_func_dev call_func;
} motor_device_uart_t;

/* -------------------------- function declaration -------------------------- */
motor_device_uart_t *motor_device_uart_get_pointer(void);

void motor_device_uart_register(p_callback_func_dev p_func);

#endif