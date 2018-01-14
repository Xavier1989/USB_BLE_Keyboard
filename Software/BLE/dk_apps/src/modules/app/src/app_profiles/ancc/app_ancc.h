/**
****************************************************************************************
*
* @file app_ancc.h
*
* @brief ANCC application.
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

#ifndef APP_ANCC_H_
#define APP_ANCC_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 *
 * @brief ANCC Application entry point.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"
#include "gpio.h"

#if BLE_ANC_CLIENT
#include "ancc.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

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
 *
 * ANCC Application Functions
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Inialize application and enable ANCC profile.
 *
 ****************************************************************************************
 */
void app_ancc_enable(void);

/**
 ****************************************************************************************
 * @brief Subscribe to the Notification Source notifications
 *
 ****************************************************************************************
 */
void app_ancc_ntf_src_subscribe(void);

/**
 ****************************************************************************************
 * @brief Subscribe to the Data Source notifications
 *
 ****************************************************************************************
 */
void app_ancc_data_src_subscribe(void);


#endif //BLE_ANC_CLIENT

/// @} APP

#endif // APP_ANCC_H_
