/**
****************************************************************************************
*
* @file app_findme_task.c
*
* @brief Findme locator and target application task.
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
 * @addtogroup APPTASK
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"               // SW configuration

#if (BLE_APP_PRESENT)
#include "findl_task.h"                  // Application Task API
#include "app_task.h"                  // Application Task API

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

#if BLE_FINDME_TARGET
/**
 ****************************************************************************************
 * @brief Handles Handles Alert indication from proximity reporter profile
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

int findt_alert_ind_handler(ke_msg_id_t const msgid,
                                      struct findt_alert_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{	
	  
		if (param->alert_lvl)
			app_proxr_alert_start(param->alert_lvl); 
		else
			app_proxr_alert_stop();

    return (KE_MSG_CONSUMED);
}

#endif 

#if BLE_FINDME_LOCATOR
/**
 ****************************************************************************************
 * @brief Handles enable profile confirmation for findme locator.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

int findl_enable_cfm_handler(ke_msg_id_t const msgid,
                                      struct findl_enable_cfm const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{	
		
    return (KE_MSG_CONSUMED);
}

#endif

#endif //(BLE_APP_PRESENT)

/// @} APPTASK
