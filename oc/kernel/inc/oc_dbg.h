/*
 * Copyright (c) 2019-2029, ocean Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-03-23     longmain       first version
 */

#ifndef __OC_DEBUG_H__
#define __OC_DEBUG_H__

#include "oc_config.h"
#ifdef OC_BASE_OS_USING_RT
#include "rtdebug.h"
#define OC_ASSERT(EX)   RT_ASSERT(EX)
#else
#include "FreeRTOSConfig.h"
#define OC_ASSERT( x )   configASSERT( x )
#endif // OC_OS_USING_RT


/* the debug log will force enable when OC_DEBUG macro is defined */
#if defined(OC_DEBUG) && !defined(DBG_ENABLE)
#define DBG_ENABLE
#endif

/* it will force output color log when OC_DEBUG_COLOR macro is defined */
#if defined(OC_DEBUG_COLOR) && !defined(DBG_COLOR)
#define DBG_COLOR
#endif

#if defined(OC_USING_ULOG)
/* using ulog compatible with rtdbg  */
#include <ulog.h>
#else

/* DEBUG level */
#define DBG_ERROR           0
#define DBG_WARNING         1
#define DBG_INFO            2
#define DBG_LOG             3

#ifdef DBG_TAG
#ifndef DBG_SECTION_NAME
#define DBG_SECTION_NAME    DBG_TAG
#endif
#else
/* compatible with old version */
#ifndef DBG_SECTION_NAME
#define DBG_SECTION_NAME    "DBG"
#endif
#endif /* DBG_TAG */

#ifdef DBG_ENABLE

#ifdef DBG_LVL
#ifndef DBG_LEVEL
#define DBG_LEVEL         DBG_LVL
#endif
#else
/* compatible with old version */
#ifndef DBG_LEVEL
#define DBG_LEVEL         DBG_WARNING
#endif
#endif /* DBG_LVL */

/*
 * The color for terminal (foreground)
 * BLACK    30
 * RED      31
 * GREEN    32
 * YELLOW   33
 * BLUE     34
 * PURPLE   35
 * CYAN     36
 * WHITE    37
 */
#ifdef DBG_COLOR
#define _DBG_COLOR(n)        oc_printf("\033["#n"m")
#define _DBG_LOG_HDR(lvl_name, color_n)                    \
    oc_printf("\033["#color_n"m["lvl_name"/"DBG_SECTION_NAME"] ")
#define _DBG_LOG_X_END                                     \
    oc_printf("\033[0m\r\n")
#else
#define _DBG_COLOR(n)
#define _DBG_LOG_HDR(lvl_name, color_n)                    \
    oc_printf("["lvl_name"/"DBG_SECTION_NAME"] ")
#define _DBG_LOG_X_END                                     \
    oc_printf("\n")
#endif /* DBG_COLOR */

/*
 * static debug routine
 * NOTE: This is a NOT RECOMMENDED API. Please using LOG_X API.
 *       It will be DISCARDED later. Because it will take up more resources.
 */
#define dbg_log(level, fmt, ...)                            \
    if ((level) <= DBG_LEVEL)                               \
    {                                                       \
        switch(level)                                       \
        {                                                   \
            case DBG_ERROR:   _DBG_LOG_HDR("E", 31); break; \
            case DBG_WARNING: _DBG_LOG_HDR("W", 33); break; \
            case DBG_INFO:    _DBG_LOG_HDR("I", 32); break; \
            case DBG_LOG:     _DBG_LOG_HDR("D", 0); break;  \
            default: break;                                 \
        }                                                   \
        oc_printf(fmt, ##__VA_ARGS__);                     \
        _DBG_COLOR(0);                                      \
    }

#define dbg_here                                            \
    if ((DBG_LEVEL) <= DBG_LOG){                            \
        oc_printf(DBG_SECTION_NAME " Here %s:%d\n",        \
            __FUNCTION__, __LINE__);                        \
    }

#define dbg_log_line(lvl, color_n, fmt, ...)                \
    do                                                      \
    {                                                       \
        _DBG_LOG_HDR(lvl, color_n);                         \
        oc_printf(fmt, ##__VA_ARGS__);                     \
        _DBG_LOG_X_END;                                     \
    }                                                       \
    while (0)

#define dbg_raw(...)         oc_printf(__VA_ARGS__);

#else
#define dbg_log(level, fmt, ...)
#define dbg_here
#define dbg_enter
#define dbg_exit
#define dbg_log_line(lvl, color_n, fmt, ...)
#define dbg_raw(...)
#endif /* DBG_ENABLE */

#if (DBG_LEVEL >= DBG_LOG)
#define LOG_D(fmt, ...)      dbg_log_line("D", 0, fmt, ##__VA_ARGS__)
#else
#define LOG_D(...)
#endif

#if (DBG_LEVEL >= DBG_INFO)
#define LOG_I(fmt, ...)      dbg_log_line("I", 32, fmt, ##__VA_ARGS__)
#else
#define LOG_I(...)
#endif

#if (DBG_LEVEL >= DBG_WARNING)
#define LOG_W(fmt, ...)      dbg_log_line("W", 33, fmt, ##__VA_ARGS__)
#else
#define LOG_W(...)
#endif

#if (DBG_LEVEL >= DBG_ERROR)
#define LOG_E(fmt, ...)      dbg_log_line("E", 31, fmt, ##__VA_ARGS__)
#else
#define LOG_E(...)
#endif

#define LOG_RAW(...)         dbg_raw(__VA_ARGS__)

#endif /* defined(RT_USING_ULOG) && define(DBG_ENABLE) */


#endif
