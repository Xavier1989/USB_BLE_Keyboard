/**
 ****************************************************************************************
 *
 * @file app_kbd_proj_task.h
 *
 * @brief HID Keyboard handlers header file.
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

#ifndef APP_KBD_PROJ_TASK_H_
#define APP_KBD_PROJ_TASK_H_


// EXTENDED TIMER
extern int extended_timer_cnt;


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
 
/**
 ****************************************************************************************
 * @brief   Sets the HID Report Map value in the DB
 *          
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 * @remarks Called when the DB has been initialized.
 *****************************************************************************************
 */
int keyboard_create_db_cfm_handler(ke_msg_id_t const msgid,
                                    struct hogpd_create_db_cfm const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief   Called when the HID profile is being disabled
 *          
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 *****************************************************************************************
 */                                    
int keyboard_disable_ind_handler(ke_msg_id_t const msgid,
                                    struct hogpd_disable_ind const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief   Called when the HID report has been ACK'ed from the master
 *          
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 *****************************************************************************************
 */                                    
int keyboard_ntf_sent_cfm_handler(ke_msg_id_t const msgid,
                                    struct hogpd_ntf_sent_cfm const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief   Called when the Protocol Mode char is written by the Host
 *          
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 *****************************************************************************************
 */                                    
int keyboard_proto_mode_ind_handler(ke_msg_id_t const msgid,
                                    struct hogpd_proto_mode_ind const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);
/**
 ****************************************************************************************
 * @brief   Called when the CCC of a Report is written by the Host
 *          
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 *****************************************************************************************
 */                                    
int keyboard_ntf_config_ind_handler(ke_msg_id_t const msgid,
                                    struct hogpd_ntf_cfg_ind const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief   Called when the Service Changed indication has been successfully received by the Host
 *          
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 *****************************************************************************************
 */                                    
int keyboard_svc_chng_cmp_evt_handler(ke_msg_id_t const msgid,
                                    struct gattc_cmp_evt const *ind,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief   Called when the Control Point char is written by the Host
 *          
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 *****************************************************************************************
 */                                    
int keyboard_ctnl_pt_ind_handler(ke_msg_id_t const msgid,
                                    struct hogpd_ctnl_pt_ind const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief   Set a timer that supports timeout greater than 5 min
 *          Called when a timeout of more than 5 min (that stack supports) is needed.
 *          Only one such timer (only APP_HID_TIMER) can be on at a time!
 *          
 * @param[in] timer_id 
 * @param[in] task_id
 * @param[in] delay
 *
 * @return  void
 *
 * #remarks if delay is <= KE_TIMER_DELAY_MAX then the timer is "resumed". Thus, if
 *          extended_timer_cnt != 0, it continues counting 
 *          (extended_timer_cnt * KE_TIMER_DELAY_MAX) + delay
 *
 *****************************************************************************************
 */                                    
void app_extended_timer_set(ke_msg_id_t const timer_id, ke_task_id_t const task_id, uint32_t delay);

/**
 ****************************************************************************************
 * @brief   Clears a timer that was set with app_extended_timer_set()
 *          Called when a timeout of more than 5 min (that stack supports) was set.
 *          Do not directly use ke_timer_clear on such a timer!
 *          
 * @param[in] timer_id 
 * @param[in] task_id
 *
 * @return  void
 *
 *****************************************************************************************
 */
void app_extended_timer_clear(ke_msg_id_t const timer_id, ke_task_id_t const task_id);

#endif // APP_KBD_PROJ_TASK_H_
