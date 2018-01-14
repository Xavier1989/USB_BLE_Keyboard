/**
 ****************************************************************************************
 *
 * @file timer.h
 *
 * @brief TIMER Driver.
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

#ifndef _TIMER_H_
#define _TIMER_H_

/**
 ****************************************************************************************
 * @defgroup TIMER
 * @ingroup DRIVERS
 *
 * @brief Timer driver
 *
 * @{
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>      // standard integer definition
/*
 * ENUMERATION DEFINITIONS
 *****************************************************************************************
 */
///Prescaler
enum
{
    TIM_DIV1,
    TIM_DIV16,
    TIM_DIV256,
    TIM_DIV_END
};

///Modes
enum
{
    TIM_FREERUN,
    TIM_PERIO,
    TIM_MODE_END
};
///State
enum
{
    TIM_DIS,
    TIM_EN,
    TIM_STATE_END
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize the timer1 as a count-down counter at 13MHz
 *
 ****************************************************************************************
 */
void timer_init(void);

/**
 ****************************************************************************************
 * @brief Waits for the passed number of microseconds.
 *
 * @param us  Number of the us to wait for. Maximum supported value is 1000us
 *
 ****************************************************************************************
 */
void timer_wait(int16_t us);

/// @} TIMER

#endif /* _TIMER_H_ */
