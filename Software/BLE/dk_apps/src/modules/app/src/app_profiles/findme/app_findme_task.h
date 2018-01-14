/**
****************************************************************************************
*
* @file app_findme_task.h
*
* @brief Findme Target task header.
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

#ifndef APP_FINDME_TASK_H_
#define APP_FINDME_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Findme Application task handlers.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"

#if BLE_FINDME_TARGET 
#include "findt_task.h"
#endif

#if BLE_FINDME_LOCATOR
#include "findl_task.h"
#endif


/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
#if BLE_FINDME_LOCATOR
int findl_enable_cfm_handler(ke_msg_id_t const msgid,
                                      struct findl_enable_cfm const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);
#endif                                      
                                      
#if BLE_FINDME_TARGET
int findt_alert_ind_handler(ke_msg_id_t const msgid,
                                      struct findt_alert_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);
#endif

/// @} APP

#endif // APP_FINDT_TASK_H_
