/**
****************************************************************************************
*
* @file app_ancc_task.h
*
* @brief ANCS Client task header. ANCC Handlers declaration.
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

#include "rwip_config.h"

#ifndef APP_ANCC_TASK_H_
#define APP_ANCC_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup APP
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


#if BLE_ANC_CLIENT

#include "ancc_task.h"

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
 * @brief Handles disable indication from the ANCS profile.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
                                   
int ancc_disable_ind_handler(ke_msg_id_t const msgid,
                             struct ancc_disable_ind const *param,
                             ke_task_id_t const dest_id,
                             ke_task_id_t const src_id);


/**
 ****************************************************************************************
 * @brief Handles ANCC_CMP_EVT message from ANCS profile
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int ancc_cmp_evt_handler(ke_msg_id_t const msgid,
                         struct ancc_cmp_evt const *param,
                         ke_task_id_t const dest_id,
                         ke_task_id_t const src_id);


/**
 ****************************************************************************************
 * @brief Handles Notification Attribute indication from ANCS profile
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int ancc_ntf_att_ind_handler(ke_msg_id_t const msgid,
                             struct ancc_ntf_att_ind const *param,
                             ke_task_id_t const dest_id,
                             ke_task_id_t const src_id);


/**
 ****************************************************************************************
 * @brief Handles App Attribute indication from ANCS profile
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int ancc_app_att_ind_handler(ke_msg_id_t const msgid,
                             struct ancc_app_att_ind const *param,
                             ke_task_id_t const dest_id,
                             ke_task_id_t const src_id);


/**
 ****************************************************************************************
 * @brief Handles ANCC_ANC_CONTENT_IND indication from ANCS profile
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int ancc_anc_content_ind_handler(ke_msg_id_t const msgid,
                                 struct ancc_anc_content_ind const *param,
                                 ke_task_id_t const dest_id,
                                 ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief Handles Client Characteristic Configuration Descriptor value indication 
 *        from ANCC profile
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
 
int ancc_cl_cfg_val_ind_handler(ke_msg_id_t const msgid,
                                struct ancc_cl_cfg_val_ind const *param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id);


/**
 ****************************************************************************************
 * @brief Handles Notification Source indication from ANCC profile
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
 
int ancc_ntf_src_ind_handler(ke_msg_id_t const msgid,
                             struct ancc_ntf_src_ind const *param,
                             ke_task_id_t const dest_id,
                             ke_task_id_t const src_id);
                                   


/**
 ****************************************************************************************
 * @brief Handles Notification Attribute indication from ANCC profile
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

int ancc_ntf_att_ind_handler(ke_msg_id_t const msgid,
                             struct ancc_ntf_att_ind const *param,
                             ke_task_id_t const dest_id,
                             ke_task_id_t const src_id);


/**
 ****************************************************************************************
 * @brief Handles App Attribute indication from ANCC profile
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

int ancc_app_att_ind_handler(ke_msg_id_t const msgid,
                                      struct ancc_app_att_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);


#endif //BLE_ANC_CLIENT

/// @} APP

#endif // APP_ANCC_TASK_H_
