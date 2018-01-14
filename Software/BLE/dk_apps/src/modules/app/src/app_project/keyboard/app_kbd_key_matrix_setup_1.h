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


#ifndef _APP_KBD_KEY_MATRIX_SETUP_1_H_
#define _APP_KBD_KEY_MATRIX_SETUP_1_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup HID
 *
 * @brief HID (Keyboard) Application GPIO reservations for the matrix of setup #1.
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


#undef INIT_LED_PINS
#define INIT_LED_PINS		(0)

#if !defined(CFG_PRINTF)
#define DECLARE_UART_USED_PINS                                                       	\
    {                                                                                	\
        RESERVE_GPIO(  OUTPUT_ROW_4,  GPIO_PORT_0,   GPIO_PIN_4, PID_GPIO );         	\
        RESERVE_GPIO(  OUTPUT_ROW_5,  GPIO_PORT_0,   GPIO_PIN_5, PID_GPIO );         	\
    }
#else
#define DECLARE_UART_USED_PINS
#endif

#define DECLARE_EEPROM_PINS

#define DECLARE_LED_PINS

#define DECLARE_KEYBOARD_GPIOS                                                       	\
    {                                                                                	\
        RESERVE_GPIO(   INPUT_COL_0,  GPIO_PORT_2,   GPIO_PIN_0, PID_GPIO );         	\
        RESERVE_GPIO(   INPUT_COL_1,  GPIO_PORT_2,   GPIO_PIN_1, PID_GPIO );         	\
        RESERVE_GPIO(   INPUT_COL_2,  GPIO_PORT_2,   GPIO_PIN_2, PID_GPIO );         	\
        RESERVE_GPIO(   INPUT_COL_3,  GPIO_PORT_3,   GPIO_PIN_6, PID_GPIO );         	\
        RESERVE_GPIO(   INPUT_COL_4,  GPIO_PORT_2,   GPIO_PIN_3, PID_GPIO );         	\
        RESERVE_GPIO(   INPUT_COL_5,  GPIO_PORT_3,   GPIO_PIN_7, PID_GPIO );         	\
        RESERVE_GPIO(   INPUT_COL_6,  GPIO_PORT_2,   GPIO_PIN_4, PID_GPIO );         	\
        RESERVE_GPIO(   INPUT_COL_7,  GPIO_PORT_2,   GPIO_PIN_5, PID_GPIO );         	\
        RESERVE_GPIO(   INPUT_COL_8,  GPIO_PORT_2,   GPIO_PIN_6, PID_GPIO );         	\
        RESERVE_GPIO(   INPUT_COL_9,  GPIO_PORT_2,   GPIO_PIN_7, PID_GPIO );         	\
        RESERVE_GPIO(   INPUT_COL_10,  GPIO_PORT_1,   GPIO_PIN_0, PID_GPIO );        	\
        RESERVE_GPIO(   INPUT_COL_11,  GPIO_PORT_1,   GPIO_PIN_1, PID_GPIO );        	\
        RESERVE_GPIO(   INPUT_COL_12,  GPIO_PORT_1,   GPIO_PIN_2, PID_GPIO );        	\
        RESERVE_GPIO(   INPUT_COL_13,  GPIO_PORT_1,   GPIO_PIN_3, PID_GPIO );        	\
        RESERVE_GPIO(  OUTPUT_ROW_0,  GPIO_PORT_0,   GPIO_PIN_0, PID_GPIO );         	\
        RESERVE_GPIO(  OUTPUT_ROW_1,  GPIO_PORT_2,   GPIO_PIN_8, PID_GPIO );         	\
        RESERVE_GPIO(  OUTPUT_ROW_2,  GPIO_PORT_2,   GPIO_PIN_9, PID_GPIO );         	\
        RESERVE_GPIO(  OUTPUT_ROW_3,  GPIO_PORT_0,   GPIO_PIN_3, PID_GPIO );         	\
        RESERVE_GPIO(  OUTPUT_ROW_6,  GPIO_PORT_0,   GPIO_PIN_6, PID_GPIO );         	\
        RESERVE_GPIO(  OUTPUT_ROW_7,  GPIO_PORT_0,   GPIO_PIN_7, PID_GPIO );         	\
        DECLARE_EEPROM_PINS                                                          	\
        DECLARE_LED_PINS                                                             	\
        DECLARE_UART_USED_PINS                                                       	\
    }


/// @} APP

#endif //_APP_KBD_KEY_MATRIX_SETUP_1_H_
