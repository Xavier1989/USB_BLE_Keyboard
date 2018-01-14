/**
****************************************************************************************
*
* @file app_streamdatad.h
*
* @brief Streamdata Host application.
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

#ifndef APP_STREAMDATAD_H_
#define APP_STREAMDATAD_H_

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

#include <stdint.h>          // standard integer definition
#include <co_bt.h>

#include "streamdatad.h"
#include "streamdatad_task.h"


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
 * @brief Create Database of Streamdata Device profile.
 *
 * @return void.
 ****************************************************************************************
 */ 

void app_stream_create_db(void);

/**
 ****************************************************************************************
 * @brief Enable the Streamdata Device profile
 *
 ****************************************************************************************
 */

void app_streamdatad_enable(void);

// @} APP

#endif // APP_H_
