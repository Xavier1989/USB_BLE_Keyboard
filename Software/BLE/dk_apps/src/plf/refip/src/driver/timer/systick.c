/**
****************************************************************************************
*
* @file systick.c
*
* @brief SysTick driver.
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

#include <stdio.h>
#include "systick.h"

systick_callback_function_t systick_callback_function = NULL;
bool systick_core_clock = true;

void SysTick_Handler(void)
{
    if (systick_callback_function != NULL)
        systick_callback_function();
}

/**
 ****************************************************************************************
 * @brief Function to start the SysTick timer
 *
 * @param[in] ticks  	the duration of the countdown
 * @param[in] exception set to TRUE to generate an exception when the timer counts down
 *                      to 0, FALSE not to
 *
 * @return void
 ****************************************************************************************
 */
void systick_start(uint32_t ticks, bool exception)
{   
    SetBits32(&SysTick->CTRL, SysTick_CTRL_ENABLE_Msk, 0);			    // disable systick
    SetBits32(&SysTick->LOAD, SysTick_LOAD_RELOAD_Msk, ticks-1);        // set systick timeout
    SetBits32(&SysTick->VAL,  SysTick_VAL_CURRENT_Msk, 0);			    // clear the Current Value Register and the COUNTFLAG to 0
    SetBits32(&SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk, systick_core_clock);  // select the clock
    SetBits32(&SysTick->CTRL, SysTick_CTRL_TICKINT_Msk, exception);     // generate or not the SysTick exception
    SetBits32(&SysTick->CTRL, SysTick_CTRL_ENABLE_Msk, 1);			    // enable systick
}

/**
 ****************************************************************************************
 * @brief Function to create a delay
 *
 * @param[in] ticks the duration of the delay
 *
 * @return void
 ****************************************************************************************
 */
void systick_wait(uint32_t ticks)
{
    systick_start(ticks, false);

    // wait for the counter flag to become 1
    // Because the flag is cleared automatically when the register is read, there is no need to clear it
    while (!GetBits32(&SysTick->CTRL, SysTick_CTRL_COUNTFLAG_Msk));

    systick_stop();
}

/**
 ****************************************************************************************
 * @brief Function to stop the SysTick timer
 *
 * @param[in] void
 *
 * @return void
 ****************************************************************************************
 */
void systick_stop(void)
{
    SetBits32(&SysTick->VAL,  SysTick_VAL_CURRENT_Msk, 0);			// clear the Current Value Register and the COUNTFLAG to 0
    SetBits32(&SysTick->CTRL, SysTick_CTRL_ENABLE_Msk, 0);			// disable systick
}

/**
 ****************************************************************************************
 * @brief Function to read the current value of the timer
 *
 * @param[in] void
 *
 * @return the current value of the timer
 ****************************************************************************************
 */
uint32_t systick_value(void)
{
    return GetBits32(&SysTick->VAL, SysTick_VAL_CURRENT_Msk);
}
