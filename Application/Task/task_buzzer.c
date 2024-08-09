/* ---------------------------- user header file ---------------------------- */
#include "task_buzzer.h"

/***** here it is function Definition ********************/

/**
 * ******************************************************************************
 * @brief 	: Callback function for key device operations.
 * @param 	  p_self  	: a pointer to get the device object.
 * @author 	: chenningzhan
 * @note 	: Is invoked when the button is pressed, giving a Didi sound prompt
 * ******************************************************************************
 */
static void buzzer_device_ops_callback(void *p_self)
{
    buzzer_device_t *p_dev = (buzzer_device_t *)p_self;

    p_dev->ops.control((void *)p_dev, BUZZER_STATUS_ON);
    HAL_Delay(10);
    p_dev->ops.control((void *)p_dev, BUZZER_STATUS_OFF);
}

/**
 * ******************************************************************************
 * @brief 	: Entry function for the buzzer handling task.
 * @param 	  p_self  	: a pointer to get the task object.
 * @author 	: chenningzhan
 * @note 	:
 * ******************************************************************************
 */
void mytask_buzzer_entry(void *p_self)
{
    schedule_t *task = (schedule_t *)p_self;
    static buzzer_device_t *p_dev = (void *)NULL;

    /* the task initialization */
    if (0 == task->params.count)
    {
        // register the device operation callback function
        buzzer_device_register(buzzer_device_ops_callback);
        p_dev = (buzzer_device_t *)buzzer_device_get_pointer();

        // check the pointer
        if ((void *)NULL == p_dev)
        {
            LOG_ERROR("get device pointer failed !");
            return;
        }
        else
        {
            LOG_INFO("find device success");

            // enable the device
            p_dev->ops.enable((void *)p_dev);

            // The sound of a successful device initialization
            p_dev->call_func((void *)p_dev);
            HAL_Delay(150);
            p_dev->call_func((void *)p_dev);
        }
    }
    /* the task starts a loop */
    else
    {
        p_dev->ops.control((void *)p_dev, BUZZER_STATUS_OFF);
    }
}
