/**
 * ******************************************************************************
 * @Copyright:  Copyright  (C)  2021-2024 东莞市本末科技有限公司
 * @file 	: device_motor_can.c
 * ******************************************************************************
 * @brief 	: the motor CAN device layer defines the motor CAN device ops interface
 * @version	: 1.0
 * @date 	: 2024-08-01
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
 * * 2. Modify the macro definitions and enums in 'header file.h'.
 * * 3. change the resources in 'files name.c':
 * * 4. Registers the device by calling the provided function using a callback.
 * * 5. Use the pointer to get the operation handle and use the device.
 * *
 * ******************************************************************************
 */
/* ---------------------------- user header file ---------------------------- */
#include "device_motor_can.h"
#include "device_menu.h"

/* ----------------------- global variable definition ----------------------- */
motor_device_can_t g_motor_device_can = {0};

/* ----------------------- public function definition ----------------------- */

/**
 * ******************************************************************************
 * @brief 	: get a pointer to an device.
 * @retval 	: This device index corresponds to a device pointer.
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
motor_device_can_t *motor_device_can_get_pointer(void)
{
    return &g_motor_device_can;
}

/* ------------------ private operation function definition ----------------- */

static void motor_device_can_ops_enable(void *p_self)
{
    motor_device_can_t *p_dev = (motor_device_can_t *)p_self;

    CAN_FilterTypeDef can_filter_st;
    can_filter_st.FilterActivation = ENABLE;
    can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter_st.FilterIdHigh = 0x0000;
    can_filter_st.FilterIdLow = 0x0000;
    can_filter_st.FilterMaskIdHigh = 0x0000;
    can_filter_st.FilterMaskIdLow = 0x0000;
    can_filter_st.FilterBank = 0;
    can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;

    HAL_CAN_ConfigFilter(p_dev->hcanx, &can_filter_st);
    HAL_CAN_ActivateNotification(p_dev->hcanx, CAN_IT_RX_FIFO0_MSG_PENDING);
    HAL_CAN_Start(p_dev->hcanx);
}

/**
 * ******************************************************************************
 * @brief 	: the can motor reinitializes the baud rate etc according to the model
 * @param 	  p_self  	: Pointer to the device structure.
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
static void motor_device_can_ops_reinit(void *p_self)
{
    motor_device_can_t *dev = (motor_device_can_t *)p_self;

    switch (dev->baudrate)
    {
    case 500000:
        hcan.Init.Prescaler = dev->Prescaler = 8;
        hcan.Init.TimeSeg1 = dev->TimeSeg1 = CAN_BS1_4TQ;
        hcan.Init.TimeSeg2 = dev->TimeSeg2 = CAN_BS2_4TQ;
        break;
    case 1000000:
        hcan.Init.Prescaler = dev->Prescaler = 4;
        hcan.Init.TimeSeg1 = dev->TimeSeg1 = CAN_BS1_4TQ;
        hcan.Init.TimeSeg2 = dev->TimeSeg2 = CAN_BS2_4TQ;
        break;
    default:
        LOG_ERROR("motor can device reinit baudrate error");
        break;
    }
    if (HAL_CAN_Init(&hcan) != HAL_OK)
    {
        LOG_ERROR("Failed to reinit motor can device");
    }
    else
    {
        HAL_CAN_Start(&hcan);
        LOG_DEBUG("reinit motor can device success");
    }
}

/**
 * ******************************************************************************
 * @brief 	: the can motor analyzes the feedback data
 * @param 	  p_self  	: Pointer to the device structure.
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
static void motor_device_can_ops_parse_data(void *p_self)
{
    motor_device_can_t *dev = (motor_device_can_t *)p_self;

    switch (dev->motor_index)
    {
    case MOTOR_ID_M1502E:
    case MOTOR_ID_M1502D:
    case MOTOR_ID_M1505A:
        if ((AGT_CAN_STD_DRIVE_RX + dev->motor_id) == dev->hrx.StdId)
        {
            dev->motor_rec.speed = ((int16_t)((dev->buff_rx[0] << 8) + dev->buff_rx[1])) / 100.f;
            dev->motor_rec.current = ((dev->buff_rx[2] << 8) + dev->buff_rx[3]);
            dev->motor_rec.position = ((dev->buff_rx[4] << 8) + dev->buff_rx[5]);
            dev->motor_rec.errcode = (dev->buff_rx[6]);
            dev->motor_rec.mode = (dev->buff_rx[7]);
        }
        break;
    case MOTOR_ID_P1010A:
    case MOTOR_ID_P1010B:
        if (AGT_CAN_STD_RX_FEEDBACK == dev->hrx.StdId)
        {
            dev->motor_rec.speed = ((int16_t)((dev->buff_rx[0] << 8) + dev->buff_rx[1])) / 10.f;
            dev->motor_rec.current = ((dev->buff_rx[2] << 8) + dev->buff_rx[3]) / 100.f;
            dev->motor_rec.position = ((dev->buff_rx[4] << 8) + dev->buff_rx[5]);
            dev->motor_rec.temperature = ((dev->buff_rx[6] << 8) + dev->buff_rx[7]);
        }
        break;
    default:
        break;
    }
}

static void motor_device_can_ops_get_index(void *p_self)
{
    motor_models_index_t index_temp;
    motor_device_can_t *dev = (motor_device_can_t *)p_self;

    index_temp = menu_device_return_motor_index();
    dev->motor_index = index_temp;
}

static void motor_device_can_ops_get_means(void *p_self)
{
    motor_connect_means_t means_temp;
    motor_device_can_t *dev = (motor_device_can_t *)p_self;

    means_temp = motor_info_get_means(dev->motor_index);
    dev->motor_means = means_temp;
}

static void motor_device_can_ops_get_target(void *p_self)
{
    int16_t speed_temp = 0;
    motor_device_can_t *dev = (motor_device_can_t *)p_self;

    // updata for speed
    speed_temp = menu_device_return_motor_speed_target();

    /* initially I used the memset function to assign a non-zero value to an array of type int, which can cause problems */
    // memset(dev->motor_target, speed_temp, sizeof(dev->motor_target));、
    /* Instead, assign a value to each element using a for loop */
    for (int i = 0; i < sizeof(dev->motor_target) / sizeof(int16_t); i++)
    {
        dev->motor_target[i] = speed_temp;
    }
}

static void motor_device_can_ops_get_baudrate(void *p_self)
{
    motor_device_can_t *dev = (motor_device_can_t *)p_self;
    uint32_t baudrate_temp = 500000;

    baudrate_temp = motor_info_get_baudrate(dev->motor_index);
    dev->baudrate = baudrate_temp;
}

static void motor_device_can_ops_send_enable(void *p_self)
{
    motor_device_can_t *dev = (motor_device_can_t *)p_self;
    motor_can_send_enable(dev->hcanx,
                          &dev->htx,
                          dev->buff_tx);
}

static void motor_can_ops_send_drive(void *p_self)
{
    motor_device_can_t *dev = (motor_device_can_t *)p_self;
    motor_can_send_drive(dev->hcanx,
                         &dev->htx,
                         dev->buff_tx,
                         dev->motor_target);
}

static void motor_can_ops_send_set_mode(void *p_self)
{
    motor_device_can_t *dev = (motor_device_can_t *)p_self;

    switch (dev->motor_index)
    {
    case MOTOR_ID_P1010A:
    case MOTOR_ID_P1010B:
        motor_can_send_set_mode_P1010AB(dev->hcanx,
                                        &dev->htx,
                                        dev->buff_tx);
        break;
    default:
        motor_can_send_set_mode(dev->hcanx,
                                &dev->htx,
                                dev->buff_tx,
                                dev->motor_mode);
        break;
    }
}

static void motor_can_ops_send_set_feedback_time(void *p_self)
{
    motor_device_can_t *dev = (motor_device_can_t *)p_self;

    switch (dev->motor_index)
    {
    case MOTOR_ID_P1010A:
    case MOTOR_ID_P1010B:
        motor_can_send_set_feedback_time_P1010AB(dev->hcanx,
                                                 &dev->htx,
                                                 dev->buff_tx);
        break;
    default:
        motor_can_send_set_feedback_time(dev->hcanx,
                                         &dev->htx,
                                         dev->buff_tx,
                                         dev->motor_feedback_time);
        break;
    }
}

static void motor_can_ops_send_set_id(void *p_self)
{
    motor_device_can_t *dev = (motor_device_can_t *)p_self;
    motor_can_send_set_id(dev->hcanx,
                          &dev->htx,
                          dev->buff_tx,
                          dev->motor_id);
}

static void motor_can_ops_send_set_res(void *p_self)
{
    motor_device_can_t *dev = (motor_device_can_t *)p_self;
    motor_can_send_set_res(dev->hcanx,
                           &dev->htx,
                           dev->buff_tx,
                           dev->motor_res);
}

/* ----------------------- public function definition ----------------------- */

/**
 * ******************************************************************************
 * @brief 	: register an can motor device with its associated callback function.
 * @param 	  p_func  	: The callback function pointer for the device operations.
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
void motor_device_can_register(p_callback_func_dev p_func)
{
    // gets a pointer to the global variable device
    motor_device_can_t *p_dev = &g_motor_device_can;

    // bind the hardware resource
    p_dev->hcanx = &hcan;

    // set the default parameter
    p_dev->rxfifo = CAN_FILTER_FIFO0;
    p_dev->baudrate = 500000;
    p_dev->Prescaler = 8;
    p_dev->TimeSeg1 = 4;
    p_dev->TimeSeg2 = 4;
    p_dev->flag_initialized = true;
    p_dev->motor_id = 1;
    p_dev->motor_index = 0;
    p_dev->motor_means = MEANS_RS485;

    // clean the buffer
    memset(p_dev->buff_tx, 0, sizeof(p_dev->buff_tx));
    memset(p_dev->buff_rx, 0, sizeof(p_dev->buff_rx));
    memset(p_dev->motor_target, 0, sizeof(p_dev->motor_target));
    memset(p_dev->motor_res, 0x01, sizeof(p_dev->motor_res));
    memset(p_dev->motor_mode, LOOP_SPEED, sizeof(p_dev->motor_mode));
    memset(p_dev->motor_feedback_time, 0x7F, sizeof(p_dev->motor_feedback_time));

    // set operation function
    p_dev->ops.reinit = motor_device_can_ops_reinit;
    p_dev->ops.enable = motor_device_can_ops_enable;

    p_dev->ops.parse_data = motor_device_can_ops_parse_data;
    p_dev->ops.send_enable = motor_device_can_ops_send_enable;
    p_dev->ops.send_drive = motor_can_ops_send_drive;
    p_dev->ops.send_set_mode = motor_can_ops_send_set_mode;
    p_dev->ops.send_set_feedback_time = motor_can_ops_send_set_feedback_time;
    p_dev->ops.send_set_id = motor_can_ops_send_set_id;
    p_dev->ops.send_set_res = motor_can_ops_send_set_res;
    p_dev->ops.get_index = motor_device_can_ops_get_index;
    p_dev->ops.get_means = motor_device_can_ops_get_means;
    p_dev->ops.get_target = motor_device_can_ops_get_target;
    p_dev->ops.get_baudrate = motor_device_can_ops_get_baudrate;

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
