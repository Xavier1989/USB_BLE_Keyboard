/**
****************************************************************************************
*
* @file app_findt.h
*
* @brief Findme Target application.
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

#ifndef APP_FINDT_H_
#define APP_FINDT_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Accelerometer Application entry point.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"

#if BLE_FINDME_TARGET || BLE_FINDME_LOCATOR

#include <stdint.h>          // standard integer definition
#include <co_bt.h>

#include "findt.h"
#include "findl.h"
#include "findl_task.h"
#include "findt_task.h"


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
 * Proximity Application Functions
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Enable the proximity profile
 *
 ****************************************************************************************
 */
 
void app_findt_enable(void);

void app_findl_enable(void);

void app_findl_set_alert(void);

void app_findl_init(void);

#endif //BLE_PROX_REPORTER

/// @} APP

#endif // APP_H_
