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
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include "global_io.h"
#include "arch.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/****************************************************************************************/ 
/* i2c eeprom configuration                                                             */
/****************************************************************************************/ 
#define DEVICE_ADDR   0x50  // Set slave device address
#define I2C_10BITADDR 0     // 0: 7-bit addressing, 1: 10-bit addressing
#define EEPROM_SIZE   256   // EEPROM size in bytes
#define PAGE_SIZE     8     // EEPROM's page size in bytes
#define SPEED         2     // 1: standard mode (100 kbits/s), 2: fast mode (400 kbits/s)

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
 
void periph_init(void);

void GPIO_reservations(void);

