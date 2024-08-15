/* ---------------------------- user header file ---------------------------- */
#include "task_led.h"
#include "device_led.h"

/* ---------------------------- macro definition ---------------------------- */

/* --------------------------- function definition -------------------------- */

/**
 * ******************************************************************************
 * @brief 	: Entry function for the led handling task.
 * @param 	  p_self  	: a pointer to get the task object.
 * @author 	: chenningzhan
 * @note 	:  enters a loop to Blink a LED
 * ******************************************************************************
 */
void mytask_led_entry(void *p_self)
{
    schedule_t *task = (schedule_t *)p_self;
    static led_device_t *p_dev_g = (void *)NULL;

    /* the task initialization */
    if (0 == task->params.count)
    {
        // register the device operation callback function
        led_device_register((void *)NULL);
        p_dev_g = led_device_get_pointer(LED_ID_G);

        // check the pointer
        if ((void *)NULL == p_dev_g)
        {
            LOG_ERROR("get device pointer failed !");
            return;
        }
        else
        {
            LOG_INFO("find device success");
        }
    }
    /* the task starts a loop */
    else
    {
        // let the led flicker
        p_dev_g->ops.control((void *)p_dev_g, !p_dev_g->params.status);
    }
}
