/**
 ****************************************************************************************
 *
 * @file app_task_handlers.c
 *
 * @brief APP Task handlers definition
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


#include "rwip_config.h"               // SW configuration
#include <string.h> 

#if (BLE_APP_PRESENT)

#include "app_task.h"                  // Application Task API
#include "app.h"                       // Application Definition
#include "gapc_task.h"                 // GAP Controller Task API
#include "gapm_task.h"                 // GAP Manager Task API
#include "gattc_task.h"                // GATT Controller Task API
#include "app_sec_task.h"              // Application Security Task API
#include "app_api.h"                    

#ifdef APP_TASK_HANDLERS_INCLUDE
#define EXTERN 
#else
#define EXTERN extern
#endif

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */


/*
 * GLOBAL VARIABLES DEFINITION
 ****************************************************************************************
 */

/* Default State handlers definition. */
EXTERN KBD_TYPE_QUALIFIER struct ke_msg_handler app_default_state[] KBD_ARRAY_ATTRIBUTE =
{
    {GAPM_DEVICE_READY_IND,                 (ke_msg_func_t)gapm_device_ready_ind_handler},
    {GAPM_CMP_EVT,                          (ke_msg_func_t)gapm_cmp_evt_handler},
    {GAPC_CMP_EVT,                          (ke_msg_func_t)gapc_cmp_evt_handler},
    {GAPC_CONNECTION_REQ_IND,               (ke_msg_func_t)gapc_connection_req_ind_handler},
    {GAPC_DISCONNECT_IND,                   (ke_msg_func_t)gapc_disconnect_ind_handler},
    {APP_MODULE_INIT_CMP_EVT,               (ke_msg_func_t)app_module_init_cmp_evt_handler},

#if (BLE_APP_SEC)
    {GAPC_BOND_REQ_IND,                     (ke_msg_func_t)gapc_bond_req_ind_handler},
    {GAPC_BOND_IND,                         (ke_msg_func_t)gapc_bond_ind_handler},
    {GAPC_ENCRYPT_REQ_IND,                  (ke_msg_func_t)gapc_encrypt_req_ind_handler},
    {GAPC_ENCRYPT_IND,                      (ke_msg_func_t)gapc_encrypt_ind_handler},
#endif

#if (BLE_DIS_SERVER)
    {DISS_CREATE_DB_CFM,                    (ke_msg_func_t)diss_create_db_cfm_handler},
    {DISS_DISABLE_IND,                      (ke_msg_func_t)diss_disable_ind_handler},
#endif //BLE_DIS_SERVER

#if BLE_ACCEL
    {ACCEL_START_IND,                       (ke_msg_func_t)accel_start_ind_handler},
    {ACCEL_STOP_IND,                        (ke_msg_func_t)accel_stop_ind_handler},
    {ACCEL_WRITE_LINE_IND,                  (ke_msg_func_t)accel_write_line_ind_handler},
    {APP_ACCEL_TIMER,                       (ke_msg_func_t)app_accel_timer_handler},
    {APP_ACCEL_ADV_TIMER,                   (ke_msg_func_t)app_accel_adv_timer_handler},
    {ACCEL_CREATE_DB_CFM,                   (ke_msg_func_t)accel_create_db_cfm_handler},
    {APP_ACCEL_MSG,							(ke_msg_func_t)accel_msg_handler},

#endif //BLE_ACCEL

#if BLE_PROX_REPORTER
    {PROXR_ALERT_IND,                       (ke_msg_func_t)proxr_alert_ind_handler},
    {APP_PXP_TIMER,                         (ke_msg_func_t)app_proxr_timer_handler},
    {PROXR_CREATE_DB_CFM,                   (ke_msg_func_t)proxr_create_db_cfm_handler},
    {PROXR_DISABLE_IND,                     (ke_msg_func_t)proxr_disable_ind_handler},
    {LLC_RD_TX_PW_LVL_CMP_EVT,				(ke_msg_func_t)llc_rd_tx_pw_lvl_cmp_evt_handler},			
#endif //BLE_PROXR_REPORTER	

#if (BLE_APP_KEYBOARD)
    {GATTC_CMP_EVT,                         (ke_msg_func_t)keyboard_svc_chng_cmp_evt_handler},
    {HOGPD_CREATE_DB_CFM,                   (ke_msg_func_t)keyboard_create_db_cfm_handler},
    {HOGPD_DISABLE_IND,                     (ke_msg_func_t)keyboard_disable_ind_handler},
    {HOGPD_NTF_SENT_CFM,                    (ke_msg_func_t)keyboard_ntf_sent_cfm_handler},
    {HOGPD_PROTO_MODE_IND,                  (ke_msg_func_t)keyboard_proto_mode_ind_handler},
    {HOGPD_NTF_CFG_IND,                     (ke_msg_func_t)keyboard_ntf_config_ind_handler},
    {HOGPD_CTNL_PT_IND,                     (ke_msg_func_t)keyboard_ctnl_pt_ind_handler},
	{APP_HID_TIMER,                         (ke_msg_func_t)app_hid_timer_handler},
    {APP_GREEN_LED_TIMER,                   (ke_msg_func_t)app_green_led_timer_handler},
    {APP_RED_LED_TIMER,                     (ke_msg_func_t)app_red_led_timer_handler},
    {APP_HID_ENC_TIMER,                     (ke_msg_func_t)app_hid_enc_timer_handler},
    {APP_HID_ADV_TIMER,                     (ke_msg_func_t)app_hid_adv_timer_handler},
    {APP_HID_INACTIVITY_TIMER,              (ke_msg_func_t)app_hid_inactivity_timer_handler},
    {GAPM_UPDATED_PRIVACY_IND,              (ke_msg_func_t)app_updated_privacy_ind_handler},
    {GAPM_UPDATED_RECON_ADDR_IND,           (ke_msg_func_t)app_updated_recon_addr_ind_handler},
    {GAPM_ADDR_SOLVED_IND,                  (ke_msg_func_t)app_gapm_addr_solved_ind_handler},
    {GATTC_SVC_CHANGED_CFG_IND,             (ke_msg_func_t)app_service_changed_cfg_ind_handler},
    {APP_HID_MSG,                           (ke_msg_func_t)app_hid_msg_handler},
    {APP_START_ADV_MSG,                     (ke_msg_func_t)app_start_adv_msg_handler},
    {APP_TERMINATE_CONN_MSG,                (ke_msg_func_t)app_disconnect_cmd_handler},
    {APP_ALT_PAIR_TIMER,                    (ke_msg_func_t)app_alt_pair_timer_handler},
#endif    

#if (BLE_APP_KEYBOARD_TESTER)
    {HOGPRH_ENABLE_CFM,                     (ke_msg_func_t)hogprh_enable_cfm_handler},
    {HOGPRH_RD_CHAR_ERR_RSP,                (ke_msg_func_t)hogprh_err_rsp_handler},
    {HOGPRH_WR_CHAR_RSP,                    (ke_msg_func_t)hogprh_err_rsp_handler},
    {HOGPRH_REPORT_IND,                     (ke_msg_func_t)hogprh_report_ind_handler},
    {HOGPRH_DISABLE_IND,                    (ke_msg_func_t)hogprh_disable_ind_handler},
	{APP_HID_TIMER,                         (ke_msg_func_t)app_hid_timer_handler},
    {HOGPRH_REPORT_MAP_RD_RSP,              (ke_msg_func_t)hopgrh_report_map_rd_rsp_handler},
    {GAPC_PARAM_UPDATE_REQ_IND,             (ke_msg_func_t)gapc_param_update_req_ind_handler},
#ifndef MITM_ON
    {APP_HID_ENC_TIMER,                     (ke_msg_func_t)app_hid_enc_timer_handler},
#endif
#endif

#if BLE_BATT_SERVER
    {BASS_CREATE_DB_CFM,                    (ke_msg_func_t)batt_create_db_cfm_handler},
    {BASS_BATT_LEVEL_UPD_CFM,               (ke_msg_func_t)batt_level_upd_cfm_handler},
    {BASS_BATT_LEVEL_NTF_CFG_IND,           (ke_msg_func_t)batt_level_ntf_cfg_ind_handler},
    {APP_BATT_TIMER,                        (ke_msg_func_t)app_batt_timer_handler},
#ifndef CUSTOM_BATTERY_LEVEL_ALERT_LED_HANDLING     
    {APP_BATT_ALERT_TIMER,                  (ke_msg_func_t)app_batt_alert_timer_handler},
#endif //CUSTOM_BATTERY_LEVEL_ALERT_LED_HANDLING     
#endif //(BLE_BATT_SERVER)

#if BLE_FINDME_TARGET
    {FINDT_ALERT_IND,                       (ke_msg_func_t)findt_alert_ind_handler},
#endif //BLE_FINDME_TARGET
	
#if BLE_FINDME_LOCATOR
    {FINDL_ENABLE_CFM,					    (ke_msg_func_t)findl_enable_cfm_handler},
#endif //BLE_FINDME_LOCATOR

#if (BLE_STREAMDATA_DEVICE)
	{STREAMDATAD_CREATE_DB_CFM,             (ke_msg_func_t)stream_create_db_cfm_handler},
    {L2CC_DATA_SEND_RSP,                    (ke_msg_func_t)stream_more_data_handler},
    {STREAMDATAD_START_IND,                 (ke_msg_func_t)stream_start_ind_handler}, // tell the app that the host has enabled notifications
    {STREAMDATAD_STOP_IND,                  (ke_msg_func_t)stream_stop_ind_handler},  // tell the app that the host has disabled notifications
#endif //BLE_STREAMDATA_DEVICE    
 
#if BLE_SPOTA_RECEIVER
    {SPOTAR_PATCH_MEM_DEV_IND,              (ke_msg_func_t)spotar_patch_mem_dev_ind_handler},
    {SPOTAR_GPIO_MAP_IND,                   (ke_msg_func_t)spotar_gpio_map_ind_handler},
    {SPOTAR_PATCH_LEN_IND,                  (ke_msg_func_t)spotar_patch_len_ind_handler}, 
    {SPOTAR_PATCH_DATA_IND,                 (ke_msg_func_t)spotar_patch_data_ind_handler},   
    {SPOTAR_CREATE_DB_CFM,                  (ke_msg_func_t)spotar_create_db_cfm_handler},
#endif //BLE_SPOTA_RECEIVER

#if BLE_APP_SMARTTAG    
    {APP_ADV_TIMER,							(ke_msg_func_t)app_adv_timer_handler},
    {APP_ADV_BLINK_TIMER,					(ke_msg_func_t)app_adv_blink_timer_handler},  
    {APP_WAKEUP_MSG,    					(ke_msg_func_t)app_wakeup_handler},
#endif

#if BLE_APP_PROXR    
    {APP_ADV_TIMER,							(ke_msg_func_t)app_adv_timer_handler},
    {APP_WAKEUP_MSG,    					(ke_msg_func_t)app_wakeup_handler},
#endif    
}; 

#define APP_DEFAULT_STATE_SIZE  sizeof(app_default_state);

#endif //(BLE_APP_PRESENT)

/// @} APPTASK
