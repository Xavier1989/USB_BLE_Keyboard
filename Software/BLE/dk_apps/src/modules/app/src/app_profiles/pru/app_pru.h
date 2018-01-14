/**
****************************************************************************************
*
* @file app_pru.h
*
* @brief Header file Main A4WP Wireless Power Transfer PRU application.
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

#ifndef APP_PRU_H_
#define APP_PRU_H_

#include <stdint.h>

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
 
/**
 ****************************************************************************************
 * @brief Send enable request to WPTS profile task.
 *
 * @return void.
 ****************************************************************************************
 */
void app_pru_enable(uint16_t conhdl);
/**
 ****************************************************************************************
 * @brief Send create data base request to WPTS profile task.
 *
 * @return void.
 ****************************************************************************************
 */
void app_pru_create_db_send(void);

#endif // APP_PRU_H_
