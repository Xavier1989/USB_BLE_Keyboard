/**
****************************************************************************************
*
* @file app_proxr_task.h
*
* @brief Proximity Reporter task header. Proximity Reporter Handlers declaration.
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

#include "rwip_config.h"

#ifndef APP_PROXR_TASK_H_
#define APP_PROXR_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief 
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */


#if BLE_PROX_REPORTER

#include "proxr_task.h"
#include "llc_task.h"

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
 * @brief Handles Proximity reporter's profile database creation confirmation.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
                                   
int proxr_create_db_cfm_handler(ke_msg_id_t const msgid,
                                   struct proxr_create_db_cfm const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id);
                                   
/**
 ****************************************************************************************
 * @brief Handles disable indication from the Proximity Reporter profile.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
                                   
int proxr_disable_ind_handler(ke_msg_id_t const msgid,
                                   struct proxr_disable_ind const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id);
                                   
/**
 ****************************************************************************************
 * @brief Handles Read Tx Power Level response from llc task.
 * @return If the message was consumed or not.
 ****************************************************************************************
*/
                                   
int llc_rd_tx_pw_lvl_cmp_evt_handler(ke_msg_id_t const msgid,
                                   struct llc_rd_tx_pw_lvl_cmd_complete const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief Handles Alert indication from proximity reporter profile
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
 
int proxr_alert_ind_handler(ke_msg_id_t const msgid,
                                   struct proxr_alert_ind const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id);
                                   
/**
 ****************************************************************************************
 * @brief Handles proximitty alert timer
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
                                   
int app_proxr_timer_handler(ke_msg_id_t const msgid,
                                   void const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id);
                                   
/**
 ****************************************************************************************
 * @brief Handles proximitty alert timer
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
                                   
int app_proxr_adv_handler(ke_msg_id_t const msgid,
                                   void const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id);
                                   

#endif //BLE_PROX_REPORTER

/// @} APP

#endif // APP_PROXR_TASK_H_
