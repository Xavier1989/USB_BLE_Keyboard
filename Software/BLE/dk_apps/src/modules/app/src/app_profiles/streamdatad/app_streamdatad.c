/**
****************************************************************************************
*
* @file app_streamdatad.c
*
* @brief Streamdata Device application.
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

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app_streamdatad.h"
#include "app.h"

#if BLE_STREAMDATA_DEVICE
#include "streamdatad_task.h"              // findme target functions


/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
*/
 
/**
 ****************************************************************************************
 * @brief Create Database of Streamdata Device profile.
 *
 * @return void.
 ****************************************************************************************
 */ 
 
void app_stream_create_db(void)
{
    // Add STREAMDATA in the database
    struct streamdatad_create_db_req * req = KE_MSG_ALLOC(STREAMDATAD_CREATE_DB_REQ, TASK_STREAMDATAD,
                                                   TASK_APP, streamdatad_create_db_req);

    req->start_hdl = 0;

    // Send the message
    ke_msg_send(req);
}


/**
 ****************************************************************************************
 * @brief Inialize application and enable Streamdata Device profile.
 *
 * @return void.
 ****************************************************************************************
 */

void app_streamdatad_enable(void)
{
		
    // Allocate the message
    struct streamdatad_enable_req *req = KE_MSG_ALLOC(STREAMDATAD_ENABLE_REQ, TASK_STREAMDATAD, TASK_APP,
                                                 streamdatad_enable_req);
	
    // Fill in the parameter structure
    req->appid = TASK_APP;
    req->conhdl = app_env.conhdl;
		
    // Send the message
    ke_msg_send(req);

}

#endif //

/// @} APP
