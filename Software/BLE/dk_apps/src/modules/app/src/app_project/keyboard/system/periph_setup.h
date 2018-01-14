/**
 ****************************************************************************************
 *
 * @file periph_setup.h
 *
 * @brief Peripherals setup header file. 
 *
 * Copyright (C) 2012. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */
 
 #ifndef PERIPH_SETUP_H_
 #define PERIPH_SETUP_H_
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include "global_io.h"
#include "arch.h"
#include "app_kbd.h"
#include "app_kbd_key_matrix.h"
#include "i2c_eeprom.h"
/*
 * DEFINES
 ****************************************************************************************
 */

/****************************************************************************************/ 
/* i2c eeprom configuration                                                             */
/****************************************************************************************/ 
#if (HAS_EEPROM)
#if (EEPROM_IS_8K)
#define I2C_SLAVE_ADDRESS       0x50            // Set slave device address
#define I2C_ADDRESS_MODE        I2C_7BIT_ADDR   // 7-bit addressing
#define I2C_EEPROM_SIZE         8192            // EEPROM size in bytes
#define I2C_EEPROM_PAGE         32              // EEPROM's page size in bytes
#define I2C_SPEED_MODE          I2C_FAST        // fast mode (400 kbits/s)
#define I2C_ADRESS_BYTES_CNT    I2C_2BYTES_ADDR
#else
#define I2C_SLAVE_ADDRESS       0x50            // Set slave device address
#define I2C_ADDRESS_MODE        I2C_7BIT_ADDR   // 7-bit addressing
#define I2C_EEPROM_SIZE         256             // EEPROM size in bytes
#define I2C_EEPROM_PAGE         8               // EEPROM's page size in bytes
#define I2C_SPEED_MODE          I2C_FAST        // fast mode (400 kbits/s)
#define I2C_ADRESS_BYTES_CNT    I2C_1BYTE_ADDR
#endif // EEPROM_IS_8K

#else // dummy settings so that the compiler does not complain
#define I2C_SLAVE_ADDRESS       0x50            // Set slave device address
#define I2C_ADDRESS_MODE        I2C_7BIT_ADDR   // 7-bit addressing
#define I2C_EEPROM_SIZE         256             // EEPROM size in bytes
#define I2C_EEPROM_PAGE         8               // EEPROM's page size in bytes
#define I2C_SPEED_MODE          I2C_FAST        // fast mode (400 kbits/s)
#define I2C_ADRESS_BYTES_CNT    I2C_1BYTE_ADDR
#endif // HAS_EEPROM


#if BLE_SPOTA_RECEIVER
/****************************************************************************************/ 
/* SPI FLASH configuration                                                             */
/****************************************************************************************/
#define SPI_FLASH_SIZE      0x20000
#define SPI_FLASH_PAGE_SIZE 0x100

#endif // BLE_SPOTA_RECEIVER

/****************************************************************************************/ 
/* Wkupct configuration                                                                 */
/****************************************************************************************/ 
#define WKUP_ENABLED



/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
 
void periph_init(void);

void GPIO_reservations(void);

#endif // PERIPH_SETUP_H_
