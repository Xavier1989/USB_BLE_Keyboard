/**
 ****************************************************************************************
 *
 * @file app_kbd_key_matrix.h
 *
 * @brief HID Keyboard key scan matrix definitions.
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

#ifndef APP_KBD_KEY_MATRIX_H_
#define APP_KBD_KEY_MATRIX_H_

#include "app_kbd_config.h"
#include "gpio.h"


#define INIT_LED_PINS       0
#define INIT_EEPROM_PINS    0


#if MATRIX_SETUP == 0
/*****************************************************************************************************************************************
 *
 * FREE - Left for customer applications
 *
 *****************************************************************************************************************************************/
 
#include "app_kbd_key_matrix_setup_0.h"





#elif MATRIX_SETUP == 1
/*****************************************************************************************************************************************
 *
 * Rev.3 DK with QFN48 (keyboard #1)
 *
 *****************************************************************************************************************************************/
 
#include "app_kbd_key_matrix_setup_1.h"





#elif MATRIX_SETUP == 2
/*****************************************************************************************************************************************
 *
 * Rev.3 DK with QFN48 (keyboard #2)
 *
 *****************************************************************************************************************************************/

#include "app_kbd_key_matrix_setup_2.h"





#elif MATRIX_SETUP == 3
/*****************************************************************************************************************************************
 *
 * Rev.3 DK with QFN48 (keyboard #3)
 *
 *****************************************************************************************************************************************/

#include "app_kbd_key_matrix_setup_3.h"




    
#elif MATRIX_SETUP == 4
/*****************************************************************************************************************************************
 *
 * Rev.3 DK with QFN48 (keyboard #1 with EEPROM)
 *
 *****************************************************************************************************************************************/
 
#include "app_kbd_key_matrix_setup_4.h"

        




#elif MATRIX_SETUP == 5
/*****************************************************************************************************************************************
 *
 * Rev.3 DK with QFN48 (keyboard #1 for Apple products)
 *
 *****************************************************************************************************************************************/

#include "app_kbd_key_matrix_setup_5.h"


    


#elif MATRIX_SETUP == 6
/*****************************************************************************************************************************************
 *
 * Rev.3 DK with QFN48 (keyboard #1 for Apple products)
 *
 *****************************************************************************************************************************************/
 
#include "app_kbd_key_matrix_setup_6.h"





#elif MATRIX_SETUP == 7
/*****************************************************************************************************************************************
 *
 * Rev.3 DK with QFN48 (keyboard #2 with EEPROM)
 *
 *****************************************************************************************************************************************/

#include "app_kbd_key_matrix_setup_7.h"





#elif MATRIX_SETUP == 8
/*****************************************************************************************************************************************
 *
 * Reference Design (#1)
 *
 *****************************************************************************************************************************************/

#include "app_kbd_key_matrix_setup_8.h"





#elif MATRIX_SETUP == 9
/*****************************************************************************************************************************************
 *
 * RC 12-keys with QFN48 (based on keyboard #3)
 *
 *****************************************************************************************************************************************/

#include "app_kbd_key_matrix_setup_9.h"
    
        
        
        

#elif MATRIX_SETUP == 10
/*****************************************************************************************************************************************
 *
 * DK 2-keys with QFN48
 *
 *****************************************************************************************************************************************/

#include "app_kbd_key_matrix_setup_10.h"


    
    
#elif MATRIX_SETUP == 11
/*****************************************************************************************************************************************
 *
 * 5-keys with QFN48
 *
 *****************************************************************************************************************************************/

#include "app_kbd_key_matrix_setup_11.h"

#endif // MATRIX_SETUP




// Dummy declarations to satisfy the compiler...
#if !(HAS_EEPROM)
#define EEPROM_BASE_ADDR    (0)             // Address offset of the bond info storage area in EEPROM
#define MAX_BOND_PEER       (1)             // Maximum number of bonds    
#define EEPROM_BOND_SIZE    (0)             // EEPROM_BOND_SIZE = (1 + no_of_bonds + no_of_bonds * 40) rounded up to the next multiple of 32
                                            // i.e. for no_of_bonds = 6, 1+6+6*40 = 247 ==> EEPROM_BOND_SIZE = 256
#endif
    
#if !(INIT_EEPROM_PINS)
#define I2C_SDA_PORT        GPIO_PORT_1
#define I2C_SDA_PIN         GPIO_PIN_8
#define I2C_SCL_PORT        GPIO_PORT_1
#define I2C_SCL_PIN         GPIO_PIN_9
#endif    

#if !(INIT_LED_PINS)
#define KBD_GREEN_LED_PORT  GPIO_PORT_1
#define KBD_GREEN_LED_PIN   GPIO_PIN_8

#define KBD_RED_LED_PORT    GPIO_PORT_1
#define KBD_RED_LED_PIN     GPIO_PIN_9
    
#define LED_STATE_ON        false            // LEDs are active low
#define LED_STATE_OFF       true    
#endif

#endif // APP_KBD_KEY_MATRIX_H_
