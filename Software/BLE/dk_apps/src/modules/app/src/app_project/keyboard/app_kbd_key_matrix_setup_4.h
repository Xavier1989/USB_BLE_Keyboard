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


#ifndef _APP_KBD_KEY_MATRIX_SETUP_4_H_
#define _APP_KBD_KEY_MATRIX_SETUP_4_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup HID
 *
 * @brief HID (Keyboard) Application GPIO reservations for the matrix of setup #4.
 *
 * @{
 ****************************************************************************************
 */


#ifdef EEPROM_ON

#define HAS_EEPROM			(1)

#define EEPROM_BASE_ADDR	(0)				// Address offset of the bond info storage area in EEPROM
#if (HAS_MULTI_BOND)
#define MAX_BOND_PEER		(0x04)			// Maximum number of bonds
#else
#define MAX_BOND_PEER		(0x01)			// Maximum number of bonds
#endif
#define EEPROM_BOND_SIZE	(256)			// EEPROM_BOND_SIZE = (1 + no_of_bonds + no_of_bonds * 40) rounded up to the next multiple of 32
											// i.e. for no_of_bonds = 6, 1+6+6*40 = 247 ==> EEPROM_BOND_SIZE = 256

#else
#define HAS_EEPROM			(0)
#endif // EEPROM_ON

#if (HAS_MULTI_BOND)
 #if ( !(HAS_EEPROM) )
  #error "Multiple bonding support requires EEPROM!"
 #endif
#endif // (HAS_MULTI_BOND)

#undef INIT_EEPROM_PINS
#define INIT_EEPROM_PINS	(1)

// Pin definition (irrelevant to whether EEPROM pins are used or not since the HW setup is fixed!)
#define I2C_SDA_PORT		GPIO_PORT_0
#define I2C_SDA_PIN			GPIO_PIN_6
#define I2C_SCL_PORT		GPIO_PORT_0
#define I2C_SCL_PIN			GPIO_PIN_7


#undef INIT_LED_PINS
#define INIT_LED_PINS		(0)

#if !defined(CFG_PRINTF)
#define DECLARE_UART_USED_PINS                                                       	\
    {                                                                                	\
        RESERVE_GPIO(    OUTPUT_ROW_4,    GPIO_PORT_0,     GPIO_PIN_4,    PID_GPIO ) 	\
        RESERVE_GPIO(    OUTPUT_ROW_5,    GPIO_PORT_0,     GPIO_PIN_5,    PID_GPIO ) 	\
    }
#else
#define DECLARE_UART_USED_PINS
#endif

#define DECLARE_EEPROM_PINS                                                          	\
    {                                                                                	\
        RESERVE_GPIO(      EEPROM_SDA,    GPIO_PORT_0,     GPIO_PIN_6, PID_I2C_SDA ) 	\
        RESERVE_GPIO(      EEPROM_SCL,    GPIO_PORT_0,     GPIO_PIN_7, PID_I2C_SCL ) 	\
    }

#define DECLARE_LED_PINS

#define DECLARE_KEYBOARD_GPIOS                                                       	\
    {                                                                                	\
        RESERVE_GPIO(     INPUT_COL_0,    GPIO_PORT_2,     GPIO_PIN_0,    PID_GPIO ) 	\
        RESERVE_GPIO(     INPUT_COL_1,    GPIO_PORT_2,     GPIO_PIN_1,    PID_GPIO ) 	\
        RESERVE_GPIO(     INPUT_COL_2,    GPIO_PORT_2,     GPIO_PIN_2,    PID_GPIO ) 	\
        RESERVE_GPIO(     INPUT_COL_3,    GPIO_PORT_3,     GPIO_PIN_6,    PID_GPIO ) 	\
        RESERVE_GPIO(     INPUT_COL_4,    GPIO_PORT_2,     GPIO_PIN_3,    PID_GPIO ) 	\
        RESERVE_GPIO(     INPUT_COL_5,    GPIO_PORT_3,     GPIO_PIN_7,    PID_GPIO ) 	\
        RESERVE_GPIO(     INPUT_COL_6,    GPIO_PORT_2,     GPIO_PIN_4,    PID_GPIO ) 	\
        RESERVE_GPIO(     INPUT_COL_7,    GPIO_PORT_2,     GPIO_PIN_5,    PID_GPIO ) 	\
        RESERVE_GPIO(     INPUT_COL_8,    GPIO_PORT_2,     GPIO_PIN_6,    PID_GPIO ) 	\
        RESERVE_GPIO(     INPUT_COL_9,    GPIO_PORT_2,     GPIO_PIN_7,    PID_GPIO ) 	\
        RESERVE_GPIO(    INPUT_COL_10,    GPIO_PORT_1,     GPIO_PIN_0,    PID_GPIO ) 	\
        RESERVE_GPIO(    INPUT_COL_11,    GPIO_PORT_1,     GPIO_PIN_1,    PID_GPIO ) 	\
        RESERVE_GPIO(    INPUT_COL_12,    GPIO_PORT_1,     GPIO_PIN_2,    PID_GPIO ) 	\
        RESERVE_GPIO(    INPUT_COL_13,    GPIO_PORT_1,     GPIO_PIN_3,    PID_GPIO ) 	\
        RESERVE_GPIO(    OUTPUT_ROW_0,    GPIO_PORT_0,     GPIO_PIN_0,    PID_GPIO ) 	\
        RESERVE_GPIO(    OUTPUT_ROW_1,    GPIO_PORT_2,     GPIO_PIN_8,    PID_GPIO ) 	\
        RESERVE_GPIO(    OUTPUT_ROW_2,    GPIO_PORT_2,     GPIO_PIN_9,    PID_GPIO ) 	\
        RESERVE_GPIO(    OUTPUT_ROW_3,    GPIO_PORT_0,     GPIO_PIN_3,    PID_GPIO ) 	\
        RESERVE_GPIO(    OUTPUT_ROW_6,    GPIO_PORT_3,     GPIO_PIN_1,    PID_GPIO ) 	\
        RESERVE_GPIO(    OUTPUT_ROW_7,    GPIO_PORT_3,     GPIO_PIN_2,    PID_GPIO ) 	\
        DECLARE_EEPROM_PINS                                                          	\
        DECLARE_LED_PINS                                                             	\
        DECLARE_UART_USED_PINS                                                       	\
    }


/// @} APP

#endif //_APP_KBD_KEY_MATRIX_SETUP_4_H_
