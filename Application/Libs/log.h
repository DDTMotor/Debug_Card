/**
 * ******************************************************************************
 * @Copyright:  Copyright  (C)  2021-2024 东莞市本末科技有限公司
 * @file 	: log.h
 * ******************************************************************************
 * @brief 	: This is a simple log function，use JLINK RTT tool
 * @version	: 3.0
 * @date 	: 2024-08-01
 * @note 	: add file,func,line infomation
 * *
 * *
 * ******* 1. Improvement *********
 * * 1. add file,func,line infomation.
 * *
 * * 2. Change to will not show the full path,Only the file name is displayed,the log more concise.
 * *
 * *
 * ******* 2. Fix the bug *********
 * * 1. fix the bug of :
 * *
 * *
 * ******* 3. HOW TO USE *********
 * * 0. Include this file into your current project.
 * * 1. include the header file where needed.
 * * 2. Modify the macro definitions and enums in 'log.h'.
 * * 3. change the resources in 'files name.c':
 * * 4. Registers the device by calling the provided function using a callback.
 * * 5. Use the pointer to get the operation handle and use the device.
 * *
 * ******************************************************************************
 */
#ifndef __LOG_H__
#define __LOG_H__

/* --------------------------- library header file -------------------------- */
#include "SEGGER_RTT.h"

/* ---------------------------- macro definition ---------------------------- */
/**
 * @brief the macro is used to debug log functions
 */
#define DEBUG_LOG_ENABLE 1

/* ---------------------------- user header file ---------------------------- */
#include <string.h>

#if (DEBUG_LOG_ENABLE == 1)

/**
 * @brief 两次执行strrchr,对绝对路径字符串裁剪，只显示文件名
 */
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : __FILE__)

#define LOG_SEGGER(type, color, format, ...)                           \
    SEGGER_RTT_printf(0, "%s%s[%4d][%20s][%20s][%4d]" format "\r\n%s", \
                      color,                                           \
                      type,                                            \
                      (int)HAL_GetTick() / 1000,                       \
                      __FILENAME__,                                    \
                      __FUNCTION__,                                    \
                      __LINE__,                                        \
                      ##__VA_ARGS__,                                   \
                      RTT_CTRL_RESET)

#define LOG_ASSERT(format, ...) LOG_SEGGER("[AST] ", RTT_CTRL_TEXT_WHITE, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) LOG_SEGGER("[DBG] ", RTT_CTRL_TEXT_WHITE, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) LOG_SEGGER("[INF] ", RTT_CTRL_TEXT_BRIGHT_GREEN, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) LOG_SEGGER("[ERR] ", RTT_CTRL_TEXT_BRIGHT_RED, format, ##__VA_ARGS__)

#elif (DEBUG_LOG_ENABLE == 0)
#define LOG_DEBUG(format, ...)
#define LOG_INFO(format, ...)
#define LOG_ERROR(format, ...)
#else
#error "DEBUG_LOG_ENABLE:The macro is not defined correctly"
#endif

#endif
