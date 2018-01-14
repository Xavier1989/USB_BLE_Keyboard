/**
****************************************************************************************
*
* @file app_spotar_proj.h
*
* @brief SPOTAR Project application header file.
*
* Copyright (C) 2013. Dialog Semiconductor Ltd, unpublished work. This computer 
* program includes Confidential, Proprietary Information and is a Trade Secret of 
* Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
* unless authorized in writing. All Rights Reserved.
*
* <bluetooth.support@diasemi.com> and contributors.
*
****************************************************************************************
*/

#ifndef APP_SPOTAR_PROJ_H_
#define APP_SPOTAR_PROJ_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief SPOTAR Application project header file
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"
#include "app_task.h"                  // application task
#include "gapc_task.h"                  // gap functions and messages
#include "gapm_task.h"                  // gap functions and messages
#include "app.h"                       // application definitions
#include "co_error.h"                  // error code definitions
#include "smpc_task.h"                  // error code definitions
                  

#if (BLE_SPOTA_RECEIVER)
#include "app_spotar.h"
#include "app_spotar_task.h"
#endif 

#if (BLE_BATT_SERVER)
#include "app_batt.h"
#include "app_batt_task.h"
#endif

#if (BLE_DIS_SERVER)
#include "app_dis.h"
#include "app_dis_task.h"
#endif


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/*
 * DEFINES
 ****************************************************************************************
 */

/**
 * Default Advertising data
 * --------------------------------------------------------------------------------------
 * x02 - Length
 * x01 - Flags
 * x06 - LE General Discoverable Mode + BR/EDR Not Supported
 * --------------------------------------------------------------------------------------
 * x07 - Length
 * x03 - Complete list of 16-bit UUIDs available
 * --------------------------------------------------------------------------------------
 */
#if (!BLE_APP_KEYBOARD)
#if BLE_APP_SPOTAR
#define APP_DEVICE_NAME "DA14580 SPOTAR"
#else
#define APP_DEVICE_NAME "DA14580"
#endif

// SPOTAR service 16bit UUID 0xFEF5
#define APP_ADV_DATA        "\x03\x03\xF5\xFE"
#define APP_ADV_DATA_LEN    (4)


/**
 * Default Scan response data
 * --------------------------------------------------------------------------------------
 * x09                             - Length
 * xFF                             - Vendor specific advertising type
 * x00\x60\x52\x57\x2D\x42\x4C\x45 - "RW-BLE"
 * --------------------------------------------------------------------------------------
 */
#define APP_SCNRSP_DATA         "\x09\xFF\x00\x60\x52\x57\x2D\x42\x4C\x45"
#define APP_SCNRSP_DATA_LENGTH  (10)

#endif
/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/// @} APP

#endif //APP_SPOTAR_PROJ_H_
