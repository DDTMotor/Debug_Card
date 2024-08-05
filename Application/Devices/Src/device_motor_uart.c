/**
 * ******************************************************************************
 * @Copyright:  Copyright  (C)  2021-2024 东莞市本末科技有限公司
 * @file 	: device_motor_uart.c
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
#include "device_motor_uart.h"
#include "device_menu.h"

/* ----------------------- global variable definition ----------------------- */
motor_device_uart_t g_motor_device_uart = {0};

/* ----------------------- public function definition ----------------------- */

/**
 * ******************************************************************************
 * @brief 	: get a pointer to an device.
 * @retval 	: This device index corresponds to a device pointer.
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
motor_device_uart_t *motor_device_uart_get_pointer(void)
{
    return &g_motor_device_uart;
}

/* ------------------ private operation function definition ----------------- */

/**
 * ******************************************************************************
 * @brief 	: uart motor analysis feedback data
 * @param 	  p_self  	: a pointer to get the device object
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
static void motor_device_uart_ops_parse_data(void *p_self)
{
    motor_device_uart_t *dev = (motor_device_uart_t *)p_self;
    switch (dev->motor_index)
    {
    case MOTOR_ID_M0601C:
    case MOTOR_ID_M0602C:
        for (uint8_t i = 0; i < MOTOR_DEVICE_UART_BUFF_RX_LEN; i++)
        {
            if (dev->buff_rx[(i + 0) % MOTOR_DEVICE_UART_BUFF_RX_LEN] == dev->motor_id && dev->buff_rx[(i + 1) % MOTOR_DEVICE_UART_BUFF_RX_LEN] == dev->motor_mode)
            {
                dev->motor_rec.current = (dev->buff_rx[(i + 2) % MOTOR_DEVICE_UART_BUFF_RX_LEN] << 8) + dev->buff_rx[(i + 3) % MOTOR_DEVICE_UART_BUFF_RX_LEN];
                dev->motor_rec.speed = ((dev->buff_rx[(i + 4) % MOTOR_DEVICE_UART_BUFF_RX_LEN] << 8) + dev->buff_rx[(i + 5) % MOTOR_DEVICE_UART_BUFF_RX_LEN]);
                dev->motor_rec.position = (dev->buff_rx[(i + 6) % MOTOR_DEVICE_UART_BUFF_RX_LEN] << 8) + dev->buff_rx[(i + 7) % MOTOR_DEVICE_UART_BUFF_RX_LEN];
                dev->motor_rec.errcode = dev->buff_rx[(i + 8) % MOTOR_DEVICE_UART_BUFF_RX_LEN];
                break;
            }
        }
        break;
    case MOTOR_ID_M0603C:
        for (uint8_t i = 0; i < MOTOR_DEVICE_UART_BUFF_RX_LEN; i++)
        {
            if (dev->buff_rx[(i + 0) % MOTOR_DEVICE_UART_BUFF_RX_LEN] == dev->motor_id && dev->buff_rx[(i + 1) % MOTOR_DEVICE_UART_BUFF_RX_LEN] == AGT_UART_CMD_DRIVE)
            {
                dev->motor_rec.speed = ((int16_t)((dev->buff_rx[(i + 2) % MOTOR_DEVICE_UART_BUFF_RX_LEN] << 8) + dev->buff_rx[(i + 3) % MOTOR_DEVICE_UART_BUFF_RX_LEN])) / 10.f;
                dev->motor_rec.current = (dev->buff_rx[(i + 4) % MOTOR_DEVICE_UART_BUFF_RX_LEN] << 8) + dev->buff_rx[(i + 5) % MOTOR_DEVICE_UART_BUFF_RX_LEN];
                dev->motor_rec.temperature = dev->buff_rx[(i + 7) % MOTOR_DEVICE_UART_BUFF_RX_LEN];
                dev->motor_rec.errcode = dev->buff_rx[(i + 8) % MOTOR_DEVICE_UART_BUFF_RX_LEN];
                break;
            }
        }
        break;
    case MOTOR_ID_M0603A:
        for (uint8_t i = 0; i < MOTOR_DEVICE_UART_BUFF_RX_LEN; i++)
        {
            if (dev->buff_rx[(i + 0) % MOTOR_DEVICE_UART_BUFF_RX_LEN] == dev->motor_id && dev->buff_rx[(i + 1) % MOTOR_DEVICE_UART_BUFF_RX_LEN] == AGT_UART_CMD_DRIVE_M0603A)
            {
                dev->motor_rec.speed = ((int16_t)((dev->buff_rx[(i + 2) % MOTOR_DEVICE_UART_BUFF_RX_LEN] << 8) + dev->buff_rx[(i + 3) % MOTOR_DEVICE_UART_BUFF_RX_LEN])) / 10.f;
                dev->motor_rec.current = (dev->buff_rx[(i + 4) % MOTOR_DEVICE_UART_BUFF_RX_LEN] << 8) + dev->buff_rx[(i + 5) % MOTOR_DEVICE_UART_BUFF_RX_LEN];
                dev->motor_rec.temperature = dev->buff_rx[(i + 7) % MOTOR_DEVICE_UART_BUFF_RX_LEN];
                dev->motor_rec.errcode = dev->buff_rx[(i + 8) % MOTOR_DEVICE_UART_BUFF_RX_LEN];
                break;
            }
        }
        break;
    default:
        break;
    }
}

/**
 * ******************************************************************************
 * @brief 	: the uart motor reinitializes the baud rate etc according to the model
 * @param 	  p_self  	: a pointer to get the device object
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
static void motor_device_uart_ops_reinit(void *p_self)
{
    motor_device_uart_t *dev = (motor_device_uart_t *)p_self;

    huart3.Init.BaudRate = dev->baudrate;
    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        LOG_ERROR("Failed to reinit motor UART device");
    }
}

/**
 * ******************************************************************************
 * @brief 	: uart motor obtains the motor model index operation function
 * @param 	  p_self  	: a pointer to get the device object
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
static void motor_device_uart_ops_get_index(void *p_self)
{
    motor_models_index_t index_temp;
    motor_device_uart_t *dev = (motor_device_uart_t *)p_self;

    index_temp = menu_device_return_motor_index();
    dev->motor_index = index_temp;
}

/**
 * ******************************************************************************
 * @brief 	: The UART motor obtains the communication mode operation function
 * @param 	  p_self  	: a pointer to get the device object
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
static void motor_device_uart_ops_get_means(void *p_self)
{
    motor_connect_means_t means_temp;
    motor_device_uart_t *dev = (motor_device_uart_t *)p_self;

    means_temp = motor_info_get_means(dev->motor_index);
    dev->motor_means = means_temp;
}

/**
 * ******************************************************************************
 * @brief 	: the uart motor obtains the desired speed operation function
 * @param 	  p_self  	: a pointer to get the device object
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
static void motor_device_uart_ops_get_target(void *p_self)
{
    int16_t speed_temp = 0;
    motor_device_uart_t *dev = (motor_device_uart_t *)p_self;

    // updata for speed
    speed_temp = menu_device_return_motor_speed_target();

    dev->motor_target = speed_temp;
}

/**
 * ******************************************************************************
 * @brief 	: the uart motor obtains the baud rate
 * @param 	  p_self  	: a pointer to get the device object
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
static void motor_device_uart_ops_get_baudrate(void *p_self)
{
    motor_device_uart_t *dev = (motor_device_uart_t *)p_self;
    uint32_t baudrate_temp = 115200;

    baudrate_temp = motor_info_get_baudrate(dev->motor_index);
    dev->baudrate = baudrate_temp;
}

/**
 * ******************************************************************************
 * @brief 	: the uart motor resets the uart handle
 * @param 	  p_self  	: a pointer to get the device object
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
static void motor_device_uart_ops_set_huartx(void *p_self)
{
    motor_device_uart_t *dev = (motor_device_uart_t *)p_self;

    switch (dev->motor_means)
    {
    case MEANS_RS485:
        dev->huartx = &huart1;
        break;
    case MEANS_UART:
        dev->huartx = &huart3;
        break;
    default:
        break;
    }
}

/**
 * ******************************************************************************
 * @brief 	: the uart motor receives the buff function
 * @param 	  p_self  	: a pointer to get the device object
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
static void motor_device_uart_ops_receive_buff(void *p_self)
{
    motor_device_uart_t *dev = (motor_device_uart_t *)p_self;
    // if it s not full
    if (dev->count_rx < MOTOR_DEVICE_UART_BUFF_RX_LEN)
    {
        dev->buff_rx[dev->count_rx++] = dev->buff_temp;
    }
    // if full
    if (dev->count_rx >= MOTOR_DEVICE_UART_BUFF_RX_LEN)
    {
        dev->count_rx = 0;
    }
}

static void motor_device_uart_ops_send_buff(void *p_self)
{
    motor_device_uart_t *dev = (motor_device_uart_t *)p_self;
    motor_uart_send_buff(dev->huartx,
                         dev->buff_tx);
}

static void motor_device_uart_ops_send_drive(void *p_self)
{
    motor_device_uart_t *dev = (motor_device_uart_t *)p_self;
    motor_uart_send_drive(dev->buff_tx,
                          dev->motor_id,
                          dev->motor_target);
    motor_uart_send_buff(dev->huartx,
                         dev->buff_tx);
}

static void motor_device_uart_ops_send_get_feedback(void *p_self)
{
    motor_device_uart_t *dev = (motor_device_uart_t *)p_self;

    motor_uart_send_get_feedback(dev->buff_tx,
                                 dev->motor_id);
    motor_uart_send_buff(dev->huartx,
                         dev->buff_tx);
}

static void motor_device_uart_ops_send_set_mode(void *p_self)
{
    motor_device_uart_t *dev = (motor_device_uart_t *)p_self;

    switch (dev->motor_index)
    {
    case MOTOR_ID_M0603A:
        motor_uart_send_set_mode_M0603A(dev->buff_tx,
                                        dev->motor_id,
                                        dev->motor_mode);
        break;
    default:
        motor_uart_send_set_mode(dev->buff_tx,
                                 dev->motor_id,
                                 dev->motor_mode);
        break;
    }
    motor_uart_send_buff(dev->huartx,
                         dev->buff_tx);
}

static void motor_device_uart_ops_send_get_check(void *p_self)
{
    motor_device_uart_t *dev = (motor_device_uart_t *)p_self;

    motor_uart_send_get_check(dev->buff_tx);
    motor_uart_send_buff(dev->huartx,
                         dev->buff_tx);
}

/**
 * ******************************************************************************
 * @brief 	: the uart motor sets the id operation function
 * @param 	  p_self  	: a pointer to get the device object
 * @author 	: chenningzhan
 * @note	: the set id command needs to be sent 5 times in a row
 * ******************************************************************************
 */
static void motor_device_uart_ops_send_set_id(void *p_self)
{
    motor_device_uart_t *dev = (motor_device_uart_t *)p_self;

    for (int i = 0; i < 5; i++)
    {
        motor_uart_send_set_id(dev->buff_tx,
                               dev->motor_id);
        motor_uart_send_buff(dev->huartx,
                             dev->buff_tx);
    }
}

/* ----------------------- public function definition ----------------------- */

/**
 * ******************************************************************************
 * @brief 	: register an uart motor device with its associated callback function.
 * @param 	  p_func  	: The callback function pointer for the device operations.
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
void motor_device_uart_register(p_callback_func_dev p_func)
{
    // gets a pointer to the global variable device
    motor_device_uart_t *p_dev = &g_motor_device_uart;

    // bind the hardware resource
    p_dev->huartx = &huart1;

    // set the default parameter
    p_dev->baudrate = 115200;
    p_dev->flag_initialized = true;
    p_dev->motor_target = 0;
    p_dev->motor_mode = LOOP_SPEED;
    p_dev->motor_id = 1;
    p_dev->motor_index = 0;
    p_dev->motor_means = MEANS_RS485;
    p_dev->count_rx = 0;

    // clean the buffer
    memset(p_dev->buff_tx, 0, sizeof(p_dev->buff_tx));
    memset(p_dev->buff_rx, 0, sizeof(p_dev->buff_rx));

    // set operation function
    p_dev->ops.reinit = motor_device_uart_ops_reinit;
    p_dev->ops.parse_data = motor_device_uart_ops_parse_data;
    p_dev->ops.receive_buff = motor_device_uart_ops_receive_buff;

    p_dev->ops.send_buff = motor_device_uart_ops_send_buff;
    p_dev->ops.send_drive = motor_device_uart_ops_send_drive;
    p_dev->ops.send_set_mode = motor_device_uart_ops_send_set_mode;
    p_dev->ops.send_get_feedback = motor_device_uart_ops_send_get_feedback;
    p_dev->ops.send_set_id = motor_device_uart_ops_send_set_id;
    p_dev->ops.send_get_check = motor_device_uart_ops_send_get_check;

    p_dev->ops.get_index = motor_device_uart_ops_get_index;
    p_dev->ops.get_means = motor_device_uart_ops_get_means;
    p_dev->ops.get_target = motor_device_uart_ops_get_target;
    p_dev->ops.get_baudrate = motor_device_uart_ops_get_baudrate;
    p_dev->ops.set_huartx = motor_device_uart_ops_set_huartx;

    // bind the callback function
    if ((void *)NULL != p_func)
    {
        p_dev->call_func = p_func;
    }
    else
    {
        p_dev->call_func = (void *)NULL;
    }
}
