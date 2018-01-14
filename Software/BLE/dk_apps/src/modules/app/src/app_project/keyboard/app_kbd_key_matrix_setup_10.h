/**
 ****************************************************************************************
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


#ifndef _APP_KBD_KEY_MATRIX_SETUP_10_H_
#define _APP_KBD_KEY_MATRIX_SETUP_10_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup HID
 *
 * @brief HID (Keyboard) Application GPIO reservations for the matrix of setup #10.
 *
 * @{
 ****************************************************************************************
 */


#define HAS_EEPROM			(0)

#if (HAS_MULTI_BOND)
#error "This keyboard setup does not support EEPROM!"
#endif

// Switch required by the compiler
#undef INIT_EEPROM_PINS
#define INIT_EEPROM_PINS	(0)

// Pin definition (irrelevant to whether LED pins are used or not since the HW setup is fixed!)
#define KBD_GREEN_LED_PORT	GPIO_PORT_0
#define KBD_GREEN_LED_PIN	GPIO_PIN_7

#define KBD_RED_LED_PORT	GPIO_PORT_1
#define KBD_RED_LED_PIN		GPIO_PIN_0

#undef INIT_LED_PINS
#define INIT_LED_PINS		(1)
#define LED_STATE_ON		true			// LEDs are active high
#define LED_STATE_OFF		false

#define DECLARE_UART_USED_PINS

#define DECLARE_EEPROM_PINS

#define DECLARE_LED_PINS                                                             	\
    {                                                                                	\
        RESERVE_GPIO(       GREEN_LED,    GPIO_PORT_0,     GPIO_PIN_7,    PID_GPIO ) 	\
        RESERVE_GPIO(         RED_LED,    GPIO_PORT_1,     GPIO_PIN_0,    PID_GPIO ) 	\
    }

#define DECLARE_KEYBOARD_GPIOS                                                       	\
    {                                                                                	\
        RESERVE_GPIO(     INPUT_COL_0,    GPIO_PORT_1,     GPIO_PIN_1,    PID_GPIO ) 	\
        RESERVE_GPIO(     INPUT_COL_1,    GPIO_PORT_0,     GPIO_PIN_6,    PID_GPIO ) 	\
        RESERVE_GPIO(    OUTPUT_ROW_0,    GPIO_PORT_1,     GPIO_PIN_3,    PID_GPIO ) 	\
        DECLARE_EEPROM_PINS                                                          	\
        DECLARE_LED_PINS                                                             	\
        DECLARE_UART_USED_PINS                                                       	\
    }


/// @} APP

#endif //_APP_KBD_KEY_MATRIX_SETUP_10_H_
