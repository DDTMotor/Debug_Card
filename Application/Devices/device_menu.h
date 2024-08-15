#ifndef __DEVICE_MENU_H__
#define __DEVICE_MENU_H__

/* ---------------------------- user header file ---------------------------- */
#include "./device.h"
#include "OLED.h"
#include "motor_info.h"

/* ---------------------------- macro definition ---------------------------- */
typedef void (*p_func_page)(void);

/* --------------------------- struct and enum definitions --------------------------- */

typedef struct
{
    uint8_t current;
    uint8_t last;
    uint8_t next;
    uint8_t enter;
    uint8_t back;
    p_func_page p_func_current;
} menu_device_table_t;

typedef struct
{
    motor_models_index_t motor_index;      // 电机型号索引
    uint8_t motor_speed_percent_index;     // 电机速度百分比数组下标
    int8_t motor_speed_percent;            // 电机速度百分比
    int16_t motor_speed_target;            // 电机期望速度
    const int8_t *motor_speed_percent_arr; // 电机速度百分比
    uint8_t table_index;                   // 菜单下标索引
} menu_device_params_t;

typedef struct
{
    p_func_page first;
    p_func_page second;
} menu_device_page_t;

typedef struct
{
    void (*refresh)(void *p_self);
    void (*init)(void *p_self);
    void (*update_motor_index)(void *p_self);
    void (*update_motor_speed_target)(void *p_self);
} menu_device_ops_t;

typedef struct
{
    menu_device_params_t params;      //
    const menu_device_table_t *table; //
    p_func_page p_func_page;          // 菜单当前页面函数指针
    menu_device_ops_t ops;            //
    menu_device_page_t page;          //
    p_callback_func_dev call_func;    //
} menu_device_t;

/* -------------------------- function declaration -------------------------- */

menu_device_t *menu_device_get_pointer(void);

motor_models_index_t menu_device_return_motor_index(void);

int16_t menu_device_return_motor_speed_target(void);

void menu_device_register(p_callback_func_dev p_func, const menu_device_table_t *arg_table);

#endif
