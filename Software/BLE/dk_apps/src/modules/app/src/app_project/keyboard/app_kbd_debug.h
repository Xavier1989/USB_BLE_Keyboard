/**
 ****************************************************************************************
 *
 * @file app_kbd_debug.h
 *
 * @brief Keyboard (HID) Debug header file.
 *
 * Copyright (C) 2014. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

#ifndef APP_KBD_DEBUG_H_
#define APP_KBD_DEBUG_H_

#include "arch.h"
#include "arch_sleep.h"

#ifndef HAS_WDOG
#ifdef CFG_WDOG
# define HAS_WDOG   1
#else
# define HAS_WDOG   0
#endif
#endif

#ifndef ASSERT_ERROR
#if (DEVELOPMENT_DEBUG == 0)
	#warning "KBD: ASSERT_ERROR() will cause a Soft-Reset!"
#endif

#define ASSERT_ERROR(x)                                         \
    {                                                           \
        do {                                                    \
            if (DEVELOPMENT_DEBUG)                              \
            {                                                   \
                if (!(x))                                       \
                {                                               \
                    __asm("BKPT #0\n");                         \
                }                                               \
            }                                                   \
            else                                                \
            {                                                   \
                if (!(x))                                       \
                {                                               \
                    SetWord16(WATCHDOG_REG, 0x1);               \
                    SetWord16(RESET_FREEZE_REG, FRZ_WDOG);      \
                    while(1);                                   \
                }                                               \
            }                                                   \
        } while (0);                                            \
    }

#define ASSERT_WARNING(x)                                       \
    {                                                           \
        do {                                                    \
            if (DEVELOPMENT_DEBUG)                              \
            {                                                   \
                if (!(x))                                       \
                {                                               \
                    __asm("BKPT #0\n");                         \
                }                                               \
            }                                                   \
        } while (0);                                            \
    }
#endif

#define DBG_DIS_LVL         0        
#define DBG_APP_LVL         1
#define DBG_CONN_LVL        2
#define DBG_FSM_LVL         4
#define DBG_SCAN_LVL        8
#define DBG_ALL             0xF        

#define KBD_DEBUG_LEVEL    (DBG_ALL)// (DBG_FSM_LVL)

#if defined (CFG_PRINTF)
    #if (KBD_DEBUG_LEVEL > DBG_DIS_LVL)
        #define dbg_printf(lvl, fmt, args...)                                   \
            {                                                                   \
                if ((lvl & KBD_DEBUG_LEVEL) == lvl)                             \
                    arch_printf(fmt, args);                                     \
            }
            
        #define dbg_puts(lvl, s)                                                \
            {                                                                   \
                if ((lvl & KBD_DEBUG_LEVEL) == lvl)                             \
                    arch_puts(s);                                               \
            }
    #else
        #define dbg_printf(lvl, fmt, args...) {}
        #define dbg_puts(lvl, s) {}
    #endif
#else
    #define dbg_printf(lvl, fmt, args...) {}
    #define dbg_puts(lvl, s) {}
#endif    


#endif // APP_KBD_DEBUG_H_
