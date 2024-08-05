/* ---------------------------- user header file ---------------------------- */
#include "task_motor_can.h"
#include "device_menu.h"
#include "device_motor_can.h"

/* ----------------------- public function definition ----------------------- */

/**
 * ******************************************************************************
 * @brief 	: can receive the completion callback function
 * @param 	  hcan  	: can handle
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    static motor_device_can_t *p_dev = NULL;
    p_dev = motor_device_can_get_pointer();

    if (hcan->Instance == CAN1)
    {
        if (NULL != p_dev)
        {
            HAL_CAN_GetRxMessage(hcan, p_dev->rxfifo, &p_dev->hrx, p_dev->buff_rx);
        }
    }
}

/* ------------------ private operation function definition ----------------- */

/**
 * ******************************************************************************
 * @brief 	: can motor device callback function
 * @param 	  p_self  	: a pointer to get the device object
 * @author 	: chenningzhan
 * @note 	: the key callback function is called and after the key is pressed the information corresponding to the motor model is updated first
 *            and the reporting time id etc is set and then the drive command is sent
 * ******************************************************************************
 */
static void motor_device_can_callback(void *p_self)
{
    static motor_models_index_t index_last = MOTOR_ID_M0601C;

    // 1.1 get the device pointer
    motor_device_can_t *p_dev = (motor_device_can_t *)p_self;

    // 1.2 update the motor device information
    p_dev->ops.get_index((void *)p_dev);
    p_dev->ops.get_means((void *)p_dev);
    p_dev->ops.get_target((void *)p_dev);
    p_dev->ops.get_baudrate((void *)p_dev);
    if (index_last != p_dev->motor_index)
    {
        p_dev->flag_initialized = true;
    }

    // 1.4 reset the buad rate when the motor is m0603a
    if (p_dev->flag_initialized && p_dev->motor_means == MEANS_CAN)
    {
        // reinitialize the baud rate
        p_dev->ops.reinit((void *)p_dev);
        switch (p_dev->motor_index)
        {
        case MOTOR_ID_M1502E:
        case MOTOR_ID_M1502D:
        case MOTOR_ID_M1505A:
            p_dev->ops.send_set_id((void *)p_dev);
            HAL_Delay(1);
            p_dev->ops.send_set_feedback_time((void *)p_dev);
            HAL_Delay(1);
            p_dev->ops.send_set_mode((void *)p_dev);
            HAL_Delay(1);
            p_dev->ops.send_set_res((void *)p_dev); // set the terminal resistance is connected to 120R
            break;
        case MOTOR_ID_P1010A:
        case MOTOR_ID_P1010B:
            p_dev->ops.send_set_mode((void *)p_dev);
            HAL_Delay(1);
            p_dev->ops.send_set_feedback_time((void *)p_dev);
            HAL_Delay(1);
            p_dev->ops.enable((void *)p_dev);
            break;
        default:
            break;
        }

        // set the initialization flag bit to no to prevent repeated initialization
        p_dev->flag_initialized = false;
    }

    // update the motor index last time
    index_last = p_dev->motor_index;

    // drive the motor device
    if (MEANS_CAN == p_dev->motor_means)
    {
        p_dev->ops.send_drive((void *)p_dev);
    }
}

/**
 * ******************************************************************************
 * @brief 	: Entry function for the can handling task.
 * @param 	  p_self  	: a pointer to get the task object
 * @author 	: chenningzhan
 * @note 	: None
 * ******************************************************************************
 */
void mytask_can_entry(void *p_self)
{
    schedule_t *task = (schedule_t *)p_self;
    static motor_device_can_t *p_dev = NULL;

    // the task initialization
    if (0 == task->params.count)
    {
        // register the device operation callback function
        motor_device_can_register(motor_device_can_callback);
        p_dev = motor_device_can_get_pointer();

        // check the pointer
        if (NULL == p_dev)
        {
            LOG_ERROR("get device pointer failed !");
            return;
        }
        else
        {
            LOG_INFO("find device success");
            p_dev->ops.enable((void *)p_dev);
        }
    }
    // the task starts a loop
    else
    {
        if (MEANS_CAN == p_dev->motor_means)
        {
            //            p_dev->ops.send_drive((void *)p_dev);
            //            p_dev->ops.parse_data((void *)p_dev);
        }
    }
}
