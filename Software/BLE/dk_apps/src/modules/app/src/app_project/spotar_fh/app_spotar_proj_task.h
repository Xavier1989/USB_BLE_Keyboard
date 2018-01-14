/**
****************************************************************************************
*
* @file app_spotar_proj_task.h
*
* @brief SPOTAR application task header file.
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

#ifndef APP_SPORAR_PROJ_TASK_H_
#define APP_SPOTAR_PROJ_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief SPOTAR Application Task entry point.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"

#if BLE_SPOTA_RECEIVER

#include "spotar.h"
#include "spotar_task.h"
#include "app_spotar.h"

int spotar_patch_mem_dev_ind_handler(ke_msg_id_t const msgid,
                                      struct spotar_patch_mem_dev_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);
int app_spotar_timer_handler(ke_msg_id_t const msgid,
                                   void const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id);
int spotar_create_db_cfm_handler(ke_msg_id_t const msgid,
                                      struct spotar_create_db_cfm const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);
int spotar_disable_ind_handler(ke_msg_id_t const msgid,
                                      struct spotar_disable_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);
int spotar_gpio_map_ind_handler(ke_msg_id_t const msgid,
                                      struct spotar_gpio_map_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);
int spotar_patch_len_ind_handler(ke_msg_id_t const msgid,
                                      struct spotar_patch_len_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);                                      
                                      
int spotar_patch_data_ind_handler(ke_msg_id_t const msgid,
                                      struct spotar_patch_data_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);

#endif

#if BLE_BATT_SERVER
#include "bass.h"
#include "bass_task.h"
#include "app_batt.h"

int batt_create_db_cfm_handler(ke_msg_id_t const msgid,
                                      struct bass_create_db_cfm const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);
                                      
int batt_level_upd_cfm_handler(ke_msg_id_t const msgid,
                                      struct bass_batt_level_upd_cfm const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);
                                      
int batt_level_ntf_cfg_ind_handler(ke_msg_id_t const msgid,
                                      struct bass_batt_level_ntf_cfg_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);                                   

int app_batt_timer_handler(ke_msg_id_t const msgid,
                                   void const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id);
                                   
int app_batt_alert_timer_handler(ke_msg_id_t const msgid,
                                        void const *param,
										ke_task_id_t const dest_id,
										ke_task_id_t const src_id);
    
#endif 


/// @} APP

#endif // APP_SPOTAR_PROJ_TASK_H_
