/**
 ****************************************************************************************
 *
 * @file app_batt_task.h
 *
 * @brief Battery Service Task header. 
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

#ifndef APP_BATT_TASK_H_
#define APP_BATT_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Battery Service Application.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"

#if (BLE_BATT_SERVER)

#include "bass_task.h"

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

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

#ifndef CUSTOM_BATTERY_LEVEL_ALERT_LED_HANDLING                                          
int app_batt_alert_timer_handler(ke_msg_id_t const msgid,
                                        void const *param,
										ke_task_id_t const dest_id,
										ke_task_id_t const src_id);
#endif //CUSTOM_BATTERY_LEVEL_ALERT_LED_HANDLING   
                                        

#endif //BLE_BATT_SERVER

/// @} APP

#endif // APP_BATT_TASK_H_
