/**
****************************************************************************************
*
* @file systick.h
*
* @brief SysTick driver header.
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
#ifndef _SYSTICK_H_
    #define _SYSTICK_H_

    #include <stdbool.h>
    #include <stdint.h>
    #include "ARMCM0.h"
    
typedef void (*systick_callback_function_t)(void);

extern bool systick_core_clock;
extern systick_callback_function_t systick_callback_function;

/**
 ****************************************************************************************
 * @brief Register Callback function for SysTick exception.
 *
 * @param[in] callback  Callback function's reference.
 *
 * @return void
 ****************************************************************************************
 */
__INLINE void systick_register_callback(systick_callback_function_t callback)
{
    systick_callback_function = callback;
}

/**
 ****************************************************************************************
 * @brief Function to define the SysTick timer units of time
 *
 * @param[in] usec  set to TRUE to use a reference clock frequency of 1 MHz, FALSE to
 *                  use the core clock
 *
 * @return void
 ****************************************************************************************
 */
__INLINE void systick_usec_units(bool usec)
{
    systick_core_clock = !usec;
}

void systick_start(uint32_t ticks, bool exception);
void systick_wait(uint32_t ticks);
void systick_stop(void);
uint32_t systick_value(void);

#endif //_SYSTICK_H_
