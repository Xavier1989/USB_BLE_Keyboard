/**
****************************************************************************************
*
* @file app_api.h
*
* @brief app - project api header file.
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

#ifndef APP_API_H_
#define APP_API_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"
#include "app_task.h"                  // application task
#include "gapc_task.h"                  // gap functions and messages
#include "gapm_task.h"                  // gap functions and messages
#include "app.h"                       // application definitions
#include "smpc_task.h"                  // error code definitions
                  

#if (BLE_APP_PROXR)
#include "app_proxr_proj.h"
#endif 

#if (BLE_ACCEL)
#include "app_accel_proj.h"
#endif

#if (BLE_APP_KEYBOARD)
#include "app_kbd_proj.h"         // Application Keyboard (HID) Definition
#endif // (BLE_APP_KEYBOARD)

#if (BLE_APP_KEYBOARD_TESTER)
#include "app_kbdtest_proj.h"
#endif // (BLE_APP_KEYBOARD_TESTER)

#if (BLE_APP_SPOTAR)
#include "app_spotar_proj.h"
#endif

#if (BLE_APP_STREAM)
#include "app_stream_proj.h"
#endif

#if (BLE_APP_SMARTTAG)
#include "app_smarttag_proj.h"
#endif

#if (BLE_APP_DBEACON)
#include "app_dialog_beacon_proj.h"
#endif

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// List of Server Profile to Initialize
enum
{
    APP_PRF_LIST_START      = 0,

#if (BLE_APP_HT)
    APP_HT_TASK,
#endif //(BLE_APP_HT)
#if (BLE_DIS_SERVER)
    APP_DIS_TASK,
#endif //(BLE_DIS_SERVER)
#if (BLE_ACCEL)
    APP_ACCEL_TASK,
#endif //(BLE_APP_ACCEL)
#if (BLE_APP_NEB)
    APP_NEB_TASK,
#endif //(BLE_APP_NEB)
#if (BLE_PROX_REPORTER)
    APP_PROXR_TASK,
#endif //(BLE_PROX_REPORTER)
#if (BLE_STREAMDATA_DEVICE)
    APP_STREAM_TASK,
#endif //(BLE_STREAMDATA_DEVICE)
#if (BLE_BATT_SERVER)
    APP_BASS_TASK,
#endif //(BLE_BATT_SERVER)
#if (BLE_HID_DEVICE)
    APP_HOGPD_TASK,
#endif //(BLE_APP_KEYBOARD)
#if (BLE_SPOTA_RECEIVER)
    APP_SPOTAR_TASK,
#endif //(BLE_SPOTA_RECEIVER)
    APP_PRF_LIST_STOP,
};

/// APP Task messages
enum APP_MSG
{
    APP_MODULE_INIT_CMP_EVT = KE_FIRST_MSG(TASK_APP),
    
#if BLE_ACCEL
	APP_ACCEL_TIMER,
    APP_ACCEL_ADV_TIMER,
	APP_ACCEL_MSG,
#endif //BLE_ACCEL
    
#if BLE_PROX_REPORTER
    APP_PXP_TIMER,
#endif //BLE_PROX_REPORTER

#if (BLE_HID_DEVICE)
    APP_HID_TIMER,
    APP_HID_ADV_TIMER,
    APP_HID_ENC_TIMER,
    APP_HID_INACTIVITY_TIMER,
    APP_GREEN_LED_TIMER,
    APP_RED_LED_TIMER,
    APP_HID_MSG,
    APP_START_ADV_MSG,
    APP_TERMINATE_CONN_MSG,
    APP_ALT_PAIR_TIMER,
#endif //BLE_HID_DEVICE

#if (BLE_HID_REPORT_HOST)
    APP_HID_TIMER,
#endif //BLE_HID_REPORT_HOST

#if BLE_BATT_SERVER
    APP_BATT_TIMER,
    APP_BATT_ALERT_TIMER,
#endif //BLE_BATT_SERVER


#if BLE_APP_SMARTTAG
    APP_ADV_TIMER,
    APP_ADV_BLINK_TIMER,
    APP_WAKEUP_MSG,
#endif

#if BLE_APP_PROXR
    APP_ADV_TIMER,
    APP_WAKEUP_MSG,
#endif

};

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
 * @brief app_api function. Project's actions in app_disconnect
 *
 * @return void.
 ****************************************************************************************
*/

void app_disconnect_func(ke_task_id_t task_id, struct gapc_disconnect_ind const *param);

/**
 ****************************************************************************************
 * @brief app_api function. Project's actions in app_init
 *
 * @return void.
 ****************************************************************************************
*/

void app_init_func(void);

/**
 ****************************************************************************************
 * @brief app_api function. Project's actions in app_connect (device connection)
 *
 * @return void.
 ****************************************************************************************
*/

void app_connection_func(struct gapc_connection_req_ind const *param);

/**
 ****************************************************************************************
 * @brief app_api function. Project's actions in app_sec_init during system initialization
 *
 * @return void.
 ****************************************************************************************
*/

void app_sec_init_func(void);

/**
 ****************************************************************************************
 * @brief app_api function. Project's actions for profiles' database initialization
 *
 * @return void.
 ****************************************************************************************
*/

bool app_db_init_func(void);

/**
 ****************************************************************************************
 * @brief app_api function. Project configures GAPM. Called upon reset completion
 *
 * @return void.
 ****************************************************************************************
*/

void app_configuration_func(ke_task_id_t const task_id, struct gapm_set_dev_config_cmd *cmd);

/**
 ****************************************************************************************
 * @brief app_api function. Project advertising function. Setup advertise string.
 *
 * @return void.
 ****************************************************************************************
*/

void app_adv_func(struct gapm_start_advertise_cmd *cmd);

/**
 ****************************************************************************************
 * @brief app_api function. Sends request to update connection's paramaters.
 *
 * @return void.
 ****************************************************************************************
*/

void app_param_update_func(void);

/**
 ****************************************************************************************
 * @brief app_api function. Called upon device's configuration completion
 *
 * @return void.
 ****************************************************************************************
*/

void app_set_dev_config_complete_func(void);

/**
 ****************************************************************************************
 * @brief app_api function. Called upon connection param's update completion
 *
 * @return void.
 ****************************************************************************************
*/

void app_update_params_complete_func(void);

/**
 ****************************************************************************************
 * @brief app_api function. Called upon connection param's update rejection
 *
 * @return void.
 ****************************************************************************************
*/

void app_update_params_rejected_func(uint8_t status);

/**
 ****************************************************************************************
 * @brief app_api function. Handles undirect advertising completion.
 *
 * @return void.
 ****************************************************************************************
*/

void app_adv_undirect_complete(uint8_t status);

/**
 ****************************************************************************************
 * @brief app_api function. Handles direct advertising completion.
 *
 * @return void.
 ****************************************************************************************
*/

void app_adv_direct_complete(uint8_t status);

/**
 ****************************************************************************************
 * @brief app_api function. Handles Database creation. Start application.
 *
 * @return void.
 ****************************************************************************************
*/

void app_db_init_complete_func(void);


#if (BLE_APP_SEC)

/**
 ****************************************************************************************
 * @brief app_api function. Sends pairing response message Called upon pairing request message reception.
 *
 * @return void.
 ****************************************************************************************
*/

void app_send_pairing_rsp_func(struct gapc_bond_req_ind *param);

/**
 ****************************************************************************************
 * @brief app_api function. Send GAPC_TK_EXCH. Called in gapc_bond_req_ind_handler(tk_type == GAP_TK_KEY_DISPLAY)
 *
 * @return void.
 ****************************************************************************************
*/

void app_send_tk_exch_func(struct gapc_bond_req_ind *param);

/**
 ****************************************************************************************
 * @brief app_api function. Send GAPC_IRK_EXCH. Called in gapc_bond_req_ind_handler/GAPC_IRK_EXCH
 *
 * @return void.
 ****************************************************************************************
*/


void app_send_irk_exch_func(struct gapc_bond_req_ind *param);

/**
 ****************************************************************************************
 * @brief app_api function. Send GAPC_CSRK_EXCH. Called in gapc_bond_req_ind_handler/GAPC_CSRK_EXCH
 *
 * @return void.
 ****************************************************************************************
*/

void app_send_csrk_exch_func(struct gapc_bond_req_ind *param);

/**
 ****************************************************************************************
 * @brief app_api function. Send GAPC_LTK_EXCH. Called in gapc_bond_req_ind_handler/GAPC_LTK_EXCH
 *
 * @return void.
 ****************************************************************************************
*/

void app_send_ltk_exch_func(struct gapc_bond_req_ind *param);

/**
 ****************************************************************************************
 * @brief app_api function. Project's action when device is paired
 *
 * @return void.
 ****************************************************************************************
*/

void app_paired_func(void);

/**
 ****************************************************************************************
 * @brief app_api function. Validates encryption request message.
 *
 * @return void.
 ****************************************************************************************
*/

bool app_validate_encrypt_req_func(struct gapc_encrypt_req_ind *param);

/**
 ****************************************************************************************
 * @brief app_api function. Handles encryption indication.
 *
 * @return void.
 ****************************************************************************************
*/

void app_sec_encrypt_ind_func(void);

/**
 ****************************************************************************************
 * @brief app_api function. Project's actions when encryption request is completed succesfully
 *
 * @return void.
 ****************************************************************************************
*/

void app_sec_encrypt_complete_func(void);

/**
 ****************************************************************************************
 * @brief app_api function. Start passcode entry process. Called in gapc_bond_req_ind_handler(tk_type == GAP_TK_KEY_ENTRY)
 *
 * @return void.
 ****************************************************************************************
*/

void app_mitm_passcode_entry_func(ke_task_id_t const src_id, ke_task_id_t const dest_id);

#endif //BLE_APP_SEC

/// @} APP

#endif //APP_API_H_
