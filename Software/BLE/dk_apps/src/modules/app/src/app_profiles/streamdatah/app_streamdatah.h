/**
****************************************************************************************
*
* @file app_streamdatah.h
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

#ifndef APP_STREAMDATAH_H_
#define APP_STREAMDATAH_H_

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

#include "streamdatah.h"
#include "streamdatah.h"
#include "streamdatah_task.h"
#include "streamdatah_task.h"


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
 * @brief Enable the Streamdata Host profile
 *
 ****************************************************************************************
 */
 
void app_streamdatah_enable(void);

// @} APP

#endif // APP_H_
