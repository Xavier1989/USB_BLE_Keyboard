/**
 ****************************************************************************************
 *
 * @file app_kbd_proj.h
 *
 * @brief HID Keyboard hooks header file.
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

#ifndef APP_KBD_PROJ_H_
#define APP_KBD_PROJ_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdint.h>
#include "app.h"                    // application definitions
#include "gapc_task.h"              // gapc functions and messages
#include "gapm_task.h"              // gapm functions and messages
#include "gattc_task.h"             // gattc functions and messages
#include "hogpd_task.h"             // HOGP-Device functions and messages
#include "ke_task.h"                // kernel task
#include "ke_msg.h"                 // kernel message

#include "diss_task.h"              // diss message IDs
#include "app_dis_task.h"           // diss message handlers
#include "bass_task.h"              // bass message IDs
#include "app_batt_task.h"          // bass message handlers
#include "hogpd_task.h"             // hogpd message IDs
#include "app_kbd_proj_task.h"      // hogpd message handlers
#include "app_kbd_leds.h"           // leds message handlers
#include "app_kbd.h"
#include "app_kbd_key_matrix.h"
#include "app_multi_bond.h"         // multiple bonding message handlers

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
 

/*
 * DEFINES
 ****************************************************************************************
 */
#undef APP_DFLT_DEVICE_NAME 
#define APP_DFLT_DEVICE_NAME    "DA14580 Keyboard"

#if defined(CFG_PRF_SPOTAR)
#define APP_ADV_DATA            "\x03\x19\xc1\x03\x09\x02\x12\x18\x0f\x18\x0a\x18\xF5\xFE\x05\x16\x0A\x18\x00\x01"
#define APP_ADV_DATA_LENGTH     (20)
#else
#define APP_ADV_DATA            "\x03\x19\xc1\x03\x07\x02\x12\x18\x0f\x18\x0a\x18\x05\x16\x0A\x18\x00\x01"
#define APP_ADV_DATA_LENGTH     (18)
#endif
#define APP_SCNRSP_DATA         "\x00"
#define APP_SCNRSP_DATA_LENGTH  (0)

 
/*
 * EXPORTED VARIABLES
 ****************************************************************************************
 */
extern struct bonding_info_ bond_info;


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief   Report Passcode  
 *          Sends the passcode that the user entered to the Host
 *
 * @param[in] code  The code to report to the Host
 *
 * @return  void
 ****************************************************************************************
 */
void app_mitm_passcode_report(uint32_t code);

/**
 ****************************************************************************************
 * @brief   Sets the advertising and the scan response data in the GAP Start ADV command
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void set_adv_data(struct gapm_start_advertise_cmd *cmd);

/**
 ****************************************************************************************
 * @brief   Bypasses the normal disconnection procedure, in order to avoid sending the 
 *          LL_TERMINATE_IND. Used only when the HAS_SEND_LL_TERMINATE_IND is not defined
 *          
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_fake_disconnect(void);

/**
 ****************************************************************************************
 * @brief   Handler of the HID Timer - Action depends on the app state
 *
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 ****************************************************************************************
 */
int app_hid_timer_handler(ke_msg_id_t const msgid,
                                    void const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);
                                    
/**
 ****************************************************************************************
 * @brief   Handler of the HID Enc Timer
 *
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 * @remarks In case encryption is not activated by the remote host and the connection
 *           is still alive (if it wasn't then the timer would have been cleared),
 *           the handler will drop the connection. This situation appears in certain
 *           cases when pairing fails.
 ****************************************************************************************
 */
int app_hid_enc_timer_handler(ke_msg_id_t const msgid,
                                    void const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief   Handler of the HID ADV Timer 
 *
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 * @remarks If this timer expires then the advertising period is over. 
 *          If a connection is being setup, we will allow the procedure to
 *          complete. If it fails though, we do not return to ADVERTISE_ST
 *          but move on to IDLE_ST.
 ****************************************************************************************
 */                                    
int app_hid_adv_timer_handler(ke_msg_id_t const msgid,
                                    void const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief   Handler of the HID Inactivity Timer - Action depends on the app state
 *
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 ****************************************************************************************
 */
int app_hid_inactivity_timer_handler(ke_msg_id_t const msgid,
                                    void const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief   Handler of a dummy TASK_APP msg sent to trigger the timer
 *
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 * @remarks In case of delayed start, a dummy message is sent to the TASK_APP.
 *          This msg is put in the queue when the BLE is woken up. When the
 *          handler is called, it is certain that the BLE is running and 
 *          the timer may start.
 ****************************************************************************************
 */                                    
int app_hid_msg_handler(ke_msg_id_t const msgid,
                                    void const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief Handler of a dummy TASK_APP msg sent to trigger the adv timer
 *        In case of advertising start from IDLE_ST, a dummy message is sent to the TASK_APP. 
 *        This msg is put in the queue when the BLE is woken up. When the handler is called, 
 *        it is certain that the BLE is running and the timer may start.
 *
 * @param[in]   msgid    
 * @param[in]   param
 * @param[in]   dest_id
 * @param[in]   src_id
 *
 * @return  KE_MSG_CONSUMED
 ****************************************************************************************
 */                                    
int app_start_adv_msg_handler(ke_msg_id_t const msgid,
                                    void const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief  The Privacy flag has been altered by the remote host
 *
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 ****************************************************************************************
 */                                    
int app_updated_privacy_ind_handler(ke_msg_id_t const msgid,
                                    void const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief  The host updated the reconnection address 
 *
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 ****************************************************************************************
 */                                    
int app_updated_recon_addr_ind_handler(ke_msg_id_t const msgid,
                                    void const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief  Handler which checks the resolution procedure of the host's address
 *
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 ****************************************************************************************
 */                                    
int app_gapm_addr_solved_ind_handler(ke_msg_id_t const msgid,
                                    void const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief  The Client Char Config of Service Changed has been updated
 *
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 ****************************************************************************************
 */                                    
int app_service_changed_cfg_ind_handler(ke_msg_id_t const msgid,
                                    void const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief   Handler for the message sent by app_fake_disconnect() function.
 *          Used to bypasses the normal disconnection procedure, in order to avoid sending the 
 *          LL_TERMINATE_IND. Used only when the HAS_SEND_LL_TERMINATE_IND is not defined.
 *          
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  void
 ****************************************************************************************
 */                                    
int app_disconnect_cmd_handler(ke_msg_id_t const msgid,
                                    void const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);

/// @} APP

#endif // APP_KBD_PROJ_H_
