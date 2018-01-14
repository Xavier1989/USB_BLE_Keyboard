/**
****************************************************************************************
*
* @file app_streamdatah.c
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

#include "app_streamdatah.h"
#include "app.h"

#if BLE_STREAMDATA_HOST
#include "streamdatah_task.h"              // findme target functions


/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
*/
 
/**
 ****************************************************************************************
 * @brief Inialize application and enable Streamdata Host profile.
 *
 * @return void.
 ****************************************************************************************
 */

void app_streamdatah_enable(void)
{
		
    // Allocate the message
    struct streamdatah_enable_req *req = KE_MSG_ALLOC(STREAMDATAH_ENABLE_REQ, TASK_STREAMDATAH, TASK_APP,
                                                 streamdatah_enable_req);
	
    // Fill in the parameter structure
    req->conhdl = app_env.conhdl;
	req->con_type = PRF_CON_DISCOVERY;
	
    // Send the message
    ke_msg_send(req);

}

#endif //

/// @} APP
