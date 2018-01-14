/**
 ****************************************************************************************
 *
 * @file app_pru_task.h
 *
 * @brief A4WP WPT Power Receiver Unit (PRU) implementation file used for
 * handling of ble events and responses that coming from WPTS (Wireless Power
 * Transfer Server) module.
 *
 * Copyright (C) 2014. Dialog Semiconductor Ltd, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

#ifndef APP_PRU_TASK_H
#define APP_PRU_TASK_H

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "ke_msg.h"          // kernel message
#include "wpts_task.h"
#include <stdint.h>          // standard integer

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Handles WPTS profile enable confirmation.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_pru_enable_cfm_handler(ke_msg_id_t msgid,
                                void *param,
                                ke_task_id_t dest_id,
                                ke_task_id_t src_id);

/**
 ****************************************************************************************
 * @brief Handles WPTS profile data base creation confirmation.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_pru_create_db_cfm_handler(ke_msg_id_t const msgid,
                              struct wpts_create_db_cfm const *param,
                              ke_task_id_t const dest_id,
                              ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief Handles WPTS profile disable indication.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_pru_disable_ind_handler(ke_msg_id_t const msgid,
                            struct wpts_disable_ind const *param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief Handles WPTS profile alert configuration indication.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_pru_alert_cfg_indntf_ind_handler(ke_msg_id_t const msgid,
                                     struct wpts_alert_cfg_indntf_ind const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief Handles WPTS profile control request indication.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_pru_ctrl_change_ind_handler(ke_msg_id_t const msgid,
                                struct wpts_ctrl_change_ind const *param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief Handles WPTS profile PTU Static charateristic indication.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_pru_ptu_static_val_ind_handler(ke_msg_id_t const msgid,
                                   struct wpts_ptu_static_val_ind const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief Handles WPTS profile send alert info confirmation.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_pru_alert_send_cfm_handler(ke_msg_id_t const msgid,
                               struct wpts_alert_send_cfm const *param,
                               ke_task_id_t const dest_id,
                               ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief Handles WPTS profile update static charateristic confirmation.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_pru_static_val_cfm_handler(ke_msg_id_t const msgid,
                               struct wpts_pru_static_val_cfm const *param,
                               ke_task_id_t const dest_id,
                               ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief Handles WPTS profile update dynamic charateristic confirmation.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_pru_dynamic_val_cfm_handler(ke_msg_id_t const msgid,
                                struct wpts_pru_dynamic_val_cfm const *param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id);
                               
/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

#endif // APP_PRU_TASK_H
