/* ---------------------------- user header file ---------------------------- */
#include "task_motor_uart.h"

/* ----------------------- public function definition ----------------------- */

/**
 * ******************************************************************************
 * @brief 	: the serial port receives the callback function for completion
 * @param 	  huart  	: ¡¾parameter¡¿
 * @author 	: chenningzhan
 * @note 	: here the received bytes are stored in the buff and the receive interrupt is enabled
 * ******************************************************************************
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    static motor_device_uart_t *dev = NULL;
    dev = motor_device_uart_get_pointer();

    if (NULL == dev)
    {
        LOG_ERROR("get device pointer failed !");
        return;
    }
    else if (NULL != dev)
    {
        HAL_UART_Receive_IT(dev->huartx, (uint8_t *)&dev->buff_temp, 1);
        dev->ops.receive_buff((void *)dev);
    }
}

/**
 * ******************************************************************************
 * @brief 	: uart motor device callback function
 * @param 	  p_self  	: a pointer to get the device object
 * @author 	: chenningzhan
 * @note 	: the key callback function is called and the motor information is updated after the key is pressed
 *            the id is set etc and the drive command is sent
 * ******************************************************************************
 */
static void motor_device_uart_callback(void *p_self)
{
    static motor_models_index_t index_last = MOTOR_ID_M0601C;

    // 1.1 get the device pointer
    motor_device_uart_t *dev = (motor_device_uart_t *)p_self;

    // 1.2 update the motor device information
    dev->ops.get_index((void *)dev);
    dev->ops.get_means((void *)dev);
    dev->ops.get_baudrate((void *)dev);
    dev->ops.get_target((void *)dev);
    if (dev->motor_means != MEANS_CAN)
    {
        dev->ops.set_huartx((void *)dev);
    }

    // 1.3 reset the initialization flag bit when the motor model changes
    if (index_last != dev->motor_index)
    {
        dev->flag_initialized = true;
    }

    // 1.4 reset the buad rate when the motor is m0603a
    if (dev->flag_initialized && dev->motor_means != MEANS_CAN)
    {

        // reinitialize the baud rate
        dev->ops.reinit((void *)dev);
        HAL_UART_Receive_IT(dev->huartx, (uint8_t *)&dev->buff_temp, 1);

        // the motor model M0603A needs to be reset
        if (MOTOR_ID_M0603A == dev->motor_index)
        {
            dev->motor_mode = MOTOR_START;
            dev->ops.send_set_mode((void *)dev);
            HAL_Delay(10);
            dev->motor_mode = LOOP_SPEED;
            dev->ops.send_set_mode((void *)dev);
        }
        else
        {
            dev->ops.send_set_id((void *)dev);
        }

        // set the initialization flag bit to no to prevent repeated initialization
        dev->flag_initialized = false;
    }

    // update the motor index last time
    index_last = dev->motor_index;

    // drive the motor device
    if (MEANS_CAN != dev->motor_means)
    {
        dev->ops.send_drive((void *)dev);
    }
}

/**
 * ******************************************************************************
 * @brief 	: Entry function for the uart handling task.
 * @param 	  p_self  	: a pointer to get the task object
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
void mytask_uart_entry(void *p_self)
{
    schedule_t *task = (schedule_t *)p_self;
    static motor_device_uart_t *dev = NULL;

    // the task initialization
    if (0 == task->params.count)
    {
        // register the device operation callback function
        motor_device_uart_register(motor_device_uart_callback);
        dev = motor_device_uart_get_pointer();

        // check the pointer
        if (NULL == dev)
        {
            LOG_ERROR("get device pointer failed !");
            return;
        }
        else
        {
            LOG_INFO("find device success");
        }
    }
    // the task starts a loop
    else
    {
        if (MEANS_CAN != dev->motor_means)
        {
            switch (dev->motor_index)
            {
            case MOTOR_ID_M0601C:
            case MOTOR_ID_M0602C:
            case MOTOR_ID_M0603C:
            case MOTOR_ID_M0603A:
                //                dev->ops.send_drive((void *)dev);
                //                dev->ops.parse_data((void *)dev);
                break;
            default:
                break;
            }
        }
    }
}