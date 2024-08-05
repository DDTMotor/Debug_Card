/**
 * ******************************************************************************
 * @Copyright:  Copyright  (C)  2021-2024 东莞市本末科技有限公司
 * @file 	: task_menu.c
 * ******************************************************************************
 * @brief 	: menu ui display
 * @version	: 1.0
 * @date 	: 2024-08-02
 * @note 	: use arrays to manage information such as the number of pages function pointers and more
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
#include "task_menu.h"
#include "device_key.h"
#include "device_menu.h"
#include "device_motor_can.h"
#include "device_motor_uart.h"

/* ---------------------------- macro definition ---------------------------- */
#define ENUM_LINE_COUNT_ZERO 0
#define ENUM_LINE_COUNT_FIRST (MOTOR_ID_COUNT)
#define ENUM_LINE_COUNT_SECOND 1

/* -------------------------- Function Declaration -------------------------- */
void menu_page_first(void);
void menu_page_second(void);

/* ----------------------- global variable definition ----------------------- */
extern motor_info_table_t g_motor_info_table[];
const uint8_t g_id_1st = 0,
              g_id_2nd = g_id_1st + ENUM_LINE_COUNT_FIRST,
              g_id_3nd = g_id_2nd + ENUM_LINE_COUNT_SECOND;
const menu_device_table_t g_menu_table[10] =
    {
        // current     last           next          enter         back
        {g_id_1st + 0, g_id_2nd - 1, g_id_1st + 1, g_id_2nd + 0, g_id_1st + 0, (menu_page_first)}, // 一级界面选择第一项
        {g_id_1st + 1, g_id_1st + 0, g_id_1st + 2, g_id_2nd + 0, g_id_1st + 1, (menu_page_first)}, // 一级界面选择第二项
        {g_id_1st + 2, g_id_1st + 1, g_id_1st + 3, g_id_2nd + 0, g_id_1st + 2, (menu_page_first)}, // 一级界面选择第三项
        {g_id_1st + 3, g_id_1st + 2, g_id_1st + 4, g_id_2nd + 0, g_id_1st + 3, (menu_page_first)}, // 一级界面选择第四项
        {g_id_1st + 4, g_id_1st + 3, g_id_1st + 5, g_id_2nd + 0, g_id_1st + 4, (menu_page_first)}, // 一级界面选择第五项
        {g_id_1st + 5, g_id_1st + 4, g_id_1st + 6, g_id_2nd + 0, g_id_1st + 5, (menu_page_first)}, // 一级界面选择第五项
        {g_id_1st + 6, g_id_1st + 5, g_id_1st + 7, g_id_2nd + 0, g_id_1st + 6, (menu_page_first)}, // 一级界面选择第五项
        {g_id_1st + 7, g_id_1st + 6, g_id_1st + 8, g_id_2nd + 0, g_id_1st + 7, (menu_page_first)}, // 一级界面选择第五项
        {g_id_2nd - 1, g_id_2nd - 2, g_id_1st + 0, g_id_2nd + 0, g_id_2nd - 1, (menu_page_first)}, // 一级界面选择最后一项

        {g_id_2nd + 0, g_id_2nd + 0, g_id_2nd + 0, g_id_2nd + 0, g_id_1st + 0, (*menu_page_second)}, // 二级界面
};

/* ------------------ private operation function definition ----------------- */

/**
 * ******************************************************************************
 * @brief 	: menu page callback function
 * @param 	  p_self  	: a pointer to get the task object
 * @author 	: chenningzhan
 * @note 	: this function is called in the keystroke callback function which updates the global function index
 *            and the current operation index based on the user keystrokes
 * ******************************************************************************
 */
static void menu_device_callback(void *p_self)
{
    // gets the device pointer
    menu_device_t *p_dev_menu = (menu_device_t *)p_self;
    key_device_index_t key_index = key_device_return_index();

    // change the index based on the key
    switch (key_index)
    {
    case KEY_ID_ENTER:
        p_dev_menu->params.table_index = p_dev_menu->table[p_dev_menu->params.table_index].enter;
        break;
    case KEY_ID_BACK:
        p_dev_menu->params.table_index = p_dev_menu->table[p_dev_menu->params.table_index].back;
        break;
    case KEY_ID_UP:
        p_dev_menu->params.table_index = p_dev_menu->table[p_dev_menu->params.table_index].last;
        break;
    case KEY_ID_DOWN:
        p_dev_menu->params.table_index = p_dev_menu->table[p_dev_menu->params.table_index].next;
        break;
    default:
        break;
    }

    // the oled operation after the index is changed
    p_dev_menu->ops.refresh((void *)p_dev_menu);
}

/* here are the display functions for each page in the menu system */

/**
 * ******************************************************************************
 * @brief 	: menu level page
 * @author 	: chenningzhan
 * @note 	: select the motor model
 * ******************************************************************************
 */
void menu_page_first(void)
{
    static menu_device_t *p_dev_menu = (void *)NULL;
    p_dev_menu = menu_device_get_pointer();

    // set the motor models index
    p_dev_menu->ops.update_motor_index((void *)p_dev_menu);
    p_dev_menu->params.motor_speed_percent_index = 10;
    p_dev_menu->params.motor_speed_percent = 0;

    // show the string
    OLED_ShowString(0, 0, "select the motor type", OLED_6X8);
    if ((p_dev_menu->params.motor_index - 1) >= 0)
    {
        OLED_ShowString(10, 15, g_motor_info_table[(p_dev_menu->params.motor_index - 1) % MOTOR_ID_COUNT].name, OLED_6X8);
    }
    OLED_ShowString(10, 25, g_motor_info_table[p_dev_menu->params.motor_index % MOTOR_ID_COUNT].name, OLED_8X16);
    OLED_ShowString(100, 25, "<<<", OLED_8X16);
    OLED_ShowString(10, 45, g_motor_info_table[(p_dev_menu->params.motor_index + 1) % MOTOR_ID_COUNT].name, OLED_6X8);
}

/**
 * ******************************************************************************
 * @brief 	: menu level 2 page
 * @author 	: chenningzhan
 * @note 	: control the motor speed and display the motor information
 * ******************************************************************************
 */
void menu_page_second(void)
{
    static uint8_t pixels_x_1 = 5, pixels_x_2 = 60, pixels_x_3 = 100;
    static uint8_t pixels_y_1 = 0, pixels_y_2 = 20, pixels_y_3 = 40;
    static menu_device_t *p_dev_menu = (void *)NULL;
    static motor_device_can_t *motor_dev_can = NULL;
    static motor_device_uart_t *motor_dev_uart = NULL;
    int16_t speed_temp;
    uint8_t errcode, temp;

    p_dev_menu = menu_device_get_pointer();
    motor_dev_uart = motor_device_uart_get_pointer();
    motor_dev_can = motor_device_can_get_pointer();

    // 1. show tips
    OLED_ShowString(pixels_x_1, pixels_y_1, "target:", OLED_8X16);
    OLED_ShowString(pixels_x_1, pixels_y_2, "speed :", OLED_8X16);

    p_dev_menu->ops.update_motor_speed_target((void *)p_dev_menu);
    motor_connect_means_t means_temp = motor_info_get_means(p_dev_menu->params.motor_index);

    // show speed
    switch (means_temp)
    {
    case MEANS_RS485:
    case MEANS_UART:
        motor_dev_uart->ops.send_drive((void *)motor_dev_uart);
        motor_dev_uart->ops.parse_data((void *)motor_dev_uart);
        speed_temp = motor_dev_uart->motor_rec.speed;
        temp = motor_dev_uart->motor_rec.temperature;
        errcode = motor_dev_uart->motor_rec.errcode;
        break;
    case MEANS_CAN:
        motor_dev_can->ops.send_drive((void *)motor_dev_can);
        motor_dev_can->ops.parse_data((void *)motor_dev_can);
        speed_temp = motor_dev_can->motor_rec.speed;
        temp = motor_dev_can->motor_rec.temperature;
        errcode = motor_dev_can->motor_rec.errcode;
        /* code */
        break;
    default:
        break;
    }
    // 2.1 show motor speed percent value
    OLED_ShowSignedNum(pixels_x_2, pixels_y_1, (int32_t)p_dev_menu->params.motor_speed_percent, 2, OLED_8X16);
    // 2.2 show motor speed rpm value
    if (speed_temp >= 100 || speed_temp <= -100)
    {
        OLED_ShowSignedNum(pixels_x_2, pixels_y_2, (int32_t)speed_temp, 3, OLED_8X16);
    }
    else
    {
        OLED_ShowSignedNum(pixels_x_2, pixels_y_2, (int32_t)speed_temp, 2, OLED_8X16);
    }

    switch (motor_dev_uart->motor_index)
    {
    case MOTOR_ID_M0601C:
    case MOTOR_ID_M0602C:
    case MOTOR_ID_M1502E:
    case MOTOR_ID_M1502D:
        OLED_ShowString(pixels_x_1, pixels_y_3, "error :", OLED_8X16);
        OLED_ShowNum(pixels_x_2, pixels_y_3, (uint32_t)errcode, 2, OLED_8X16);
        break;
    case MOTOR_ID_M0603C:
    case MOTOR_ID_M0603A:
    case MOTOR_ID_P1010A:
    case MOTOR_ID_P1010B:
        OLED_ShowString(pixels_x_1, pixels_y_3, "temp  :", OLED_8X16);
        OLED_ShowNum(pixels_x_2, pixels_y_3, (uint32_t)temp, 2, OLED_8X16);
        OLED_ShowChar(pixels_x_3, pixels_y_3, 'C', OLED_8X16);
        break;
    default:
        break;
    }
    // 3.show unit symbol
    OLED_ShowChar(pixels_x_3, pixels_y_1, '%', OLED_8X16);
    OLED_ShowString(pixels_x_3, pixels_y_2, "RPM", OLED_8X16);
}

/**
 * ******************************************************************************
 * @brief 	: menu task entry function
 * @param 	  p_self  	: a pointer to get the device object
 * @author 	: chenningzhan
 * @note 	: it only needs to be displayed once at the beginning
 *            and subsequent refreshes are executed in the callback function of the key
 * ******************************************************************************
 */
void mytask_menu_entry(void *p_self)
{
    schedule_t *task = (schedule_t *)p_self;
    static menu_device_t *p_dev = NULL;

    // the task initialization
    if (0 == task->params.count)
    {
        OLED_Init();

        // 1.1 init the device and callback function
        menu_device_register(menu_device_callback, g_menu_table);

        // 1.2 gets pointer
        p_dev = menu_device_get_pointer();

        // 1.3 pointer check
        if (NULL == p_dev)
        {
            LOG_ERROR("get device pointer failed !");
            return;
        }
        else
        {
            LOG_INFO("find device success");
            p_dev->ops.init((void *)p_dev);
        }
    }
    // the task starts a loop
    else
    {
        p_dev->ops.refresh((void *)p_dev);
        //        p_dev->ops.update_motor_index((void *)p_dev);
        //        p_dev->ops.update_motor_speed_target((void *)p_dev);
    }
}
