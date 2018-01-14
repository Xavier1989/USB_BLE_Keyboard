/**
****************************************************************************************
*
* @file app_device_config_task.h
*
* @brief Beacon Config task header. Sample128 Handlers declaration.
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

#ifndef APP_DEVICE_CONFIG_TASK_H_
#define APP_DEVICE_CONFIG_TASK_H_

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


#if BLE_DEVICE_CONFIG

#include "device_config_task.h"
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
 * @brief Handles device_config profile database creation confirmation.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
                                   
int app_device_config_create_db_cfm_handler(ke_msg_id_t const msgid,
                                   struct device_config_create_db_cfm const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id);
                                   
/**
 ****************************************************************************************
 * @brief Handles disable indication from the device_config profile.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
                                   
int app_device_config_disable_ind_handler(ke_msg_id_t const msgid,
                                   struct device_config_disable_ind const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id);
                                   
/**
 ****************************************************************************************
 * @brief Handles write configuration characteristic event indication from device_config profile
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

int app_device_config_val_ind_handler(ke_msg_id_t const msgid,
                                      struct device_config_val_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);
                                      
/**
 ****************************************************************************************
 * @brief Handles read command characteristic event indication from device_config profile
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_device_read_request_ind_handler(ke_msg_id_t const msgid,
                                      struct device_config_read_request_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);
                                                                         

#endif //BLE_DEVICE_CONFIG

/// @} APP

#endif // APP_DEVICE_CONFIG_TASK_H_
