/**
****************************************************************************************
*
* @file app_findme.c
*
* @brief Findme locator and target application.
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

#include "app_findme.h"
#include "app.h"

#if BLE_FINDME_LOCATOR
#include "findl_task.h"              // findme target functions
#endif

#if BLE_FINDME_TARGET
#include "findt_task.h"              // findme target functions
#endif

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
*/
 
 /**
 ****************************************************************************************
 * Findme Target Application Functions
 ****************************************************************************************
 */
 
/**
 ****************************************************************************************
 * @brief Inialize application and enable Findme Target profile.
 *
 * @return void.
 ****************************************************************************************
 */

#if BLE_FINDME_TARGET
void app_findt_enable(void)
{
		
    // Allocate the message
    struct findt_enable_req * req = KE_MSG_ALLOC(FINDT_ENABLE_REQ, TASK_FINDT, TASK_APP,
                                                 findt_enable_req);
	
    // Fill in the parameter structure
    req->conhdl = app_env.conhdl;
	req->sec_lvl = PERM(SVC, ENABLE); 
	
    // Send the message
    ke_msg_send(req);

}
#endif //BLE_FINDME_TARGET

#if BLE_FINDME_LOCATOR
/**
 ****************************************************************************************
 * Findme Locator Application Functions
 ****************************************************************************************
 */

void app_findl_init(void)
{
    return;
}


/**
 ****************************************************************************************
 * @brief Inialize application and enable Findme Locator profile.
 *
 * @return void.
 ****************************************************************************************
 */

void app_findl_enable(void)
{
		
    // Allocate the message
    struct findl_enable_req *req = KE_MSG_ALLOC(FINDL_ENABLE_REQ, TASK_FINDL, TASK_APP,
                                                 findl_enable_req);
	
    // Fill in the parameter structure
    req->conhdl = app_env.conhdl;
	req->con_type = PRF_CON_DISCOVERY;
	
    // Send the message
    ke_msg_send(req);

}

/**
 ****************************************************************************************
 * @brief Send Immediate alert.
 *
 * @return void.
 ****************************************************************************************
 */

void app_findl_set_alert(void)
{
	struct findl_set_alert_req *req = KE_MSG_ALLOC(FINDL_SET_ALERT_REQ, TASK_FINDL, TASK_APP,
                                                 findl_set_alert_req);
	
	req->conhdl = app_env.conhdl;
	req->alert_lvl = FINDL_ALERT_HIGH;
	
	// Send the message
    ke_msg_send(req);

}
#endif //BLE_FINDME_LOCATOR


/// @} APP
