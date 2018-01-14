/**
****************************************************************************************
*
* @file app_adc_notify_task.h
*
* @brief ADC notify task header. ADC Notify Handlers declaration.
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

#ifndef APP_ADC_NOTIFY_TASK_H_
#define APP_ADC_NOTIFY_TASK_H_

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


#if BLE_ADC_NOTIFY

#include "adc_notify_task.h"
#include "llc_task.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/*
 * DEFINES
 ****************************************************************************************
 */
//sampling timer timeout. This corrsponds to 5 sec. 
#define APP_ADC_SAMPLING_TIMEOUT    500 

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Handles adc_notify profile database creation confirmation.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
                                   
int app_adc_notify_create_db_cfm_handler(ke_msg_id_t const msgid,
                                   struct adc_notify_create_db_cfm const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id);
                                   
/**
 ****************************************************************************************
 * @brief Handles disable indication from the adc_notify profile.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
                                   
int app_adc_notify_disable_ind_handler(ke_msg_id_t const msgid,
                                   struct adc_notify_disable_ind const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id);
                                   
/**
 ****************************************************************************************
 * @brief Handles write of 1st characteristic event indication from adc_notify profile
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

int app_adc_notify_cfg_ind_handler(ke_msg_id_t const msgid,
                                      struct adc_notify_cfg_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);
                                      
                                 /**
 ****************************************************************************************
 * @brief Handles APP_SAMPLING_TIMER's expiration message. Samples ADC and writes value to characteristic.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_adc_sampling_timer_handler(ke_msg_id_t const msgid,
                                      void *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);
                                                                         

#endif //BLE_ADC_NOTIFY

/// @} APP

#endif // APP_ADC_NOTIFY_TASK_H_
