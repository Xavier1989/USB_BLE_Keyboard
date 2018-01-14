/**
****************************************************************************************
*
* @file app_ancc_task.c
*
* @brief ANCS Client application Task Implementation.
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

/**
 ****************************************************************************************
 * @addtogroup APPTASK
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"              // SW configuration

#if (BLE_APP_PRESENT)

#if (BLE_ANC_CLIENT)

#include "app_ancc.h"
#include "app_ancc_task.h"
#include "app_task.h"                  // Application Task API
#include "gpio.h"
#include "app_sec.h"

#include "app_console.h"

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Handles disable indication from the ANCS profile.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance.
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int ancc_disable_ind_handler(ke_msg_id_t const msgid,
                             struct ancc_disable_ind const *param,
                             ke_task_id_t const dest_id,
                             ke_task_id_t const src_id)
{
    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles ANCC_CMP_EVT from ANC client profile
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int ancc_cmp_evt_handler(ke_msg_id_t const msgid,
                         struct ancc_cmp_evt const *param,
                         ke_task_id_t const dest_id,
                         ke_task_id_t const src_id)
{   
    // Check for Insufficient Authentication status 
    if( (param->operation == ANCC_WRITE_CL_CFG_OP_CODE) && (param->status == ATT_ERR_INSUFF_AUTHEN) )
    {
        // Initiate pairing procedure with iOS device
        app_security_start();
    }
    
    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles ANCS discovery from ANCC profile
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

int ancc_anc_content_ind_handler(ke_msg_id_t const msgid,
                                 struct ancc_anc_content_ind const *param,
                                 ke_task_id_t const dest_id,
                                 ke_task_id_t const src_id)
{
    arch_printf("Discovered ANCS and related attributes.\r\n");
    
    // Subscribe to Notification Source characteristic notifications
    app_ancc_ntf_src_subscribe();
    // Subscribe to Data Source characteristic notifications
    app_ancc_data_src_subscribe();
    
    return (KE_MSG_CONSUMED);
}

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
                                ke_task_id_t const src_id)
{

    return (KE_MSG_CONSUMED);
}

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
                             ke_task_id_t const src_id)
{
    arch_printf("Rcvd notification. event_id=0x%X, event_flags=0x%X, cat_id=0x%X, cat_cnt=0x%X, ntf_uuid=0x%X \r\n", 
                 param->value.event_id,
                 param->value.event_flags,
                 param->value.cat_id,
                 param->value.cat_cnt,
                 param->value.ntf_uid
               );
    if (param->value.event_id == EVT_ID_NTF_ADDED)
    {
        // get notification attributes
        {
            struct ancc_get_ntf_atts_cmd *cmd = KE_MSG_ALLOC(ANCC_GET_NTF_ATTS_CMD,
                                                             TASK_ANCC, TASK_APP,
                                                             ancc_get_ntf_atts_cmd);
            cmd->conhdl = param->conhdl;
            cmd->operation = ANCC_GET_NTF_ATTS_CMD_OP_CODE;
            cmd->ntf_uid = param->value.ntf_uid;
            
            // Fill requested notification attributes
            cmd->atts = NTF_ATT_ID_APP_ID_PRESENT
                       | NTF_ATT_ID_TITLE_PRESENT         
                       | NTF_ATT_ID_SUBTITLE_PRESENT      
                       | NTF_ATT_ID_MSG_PRESENT           
                       | NTF_ATT_ID_MSG_SIZE_PRESENT      
                       | NTF_ATT_ID_DATE_PRESENT          
                       ;
            // Set maximum lengths for requested notification attributes
            cmd->title_len    = 25;
            cmd->subtitle_len = 25;
            cmd->message_len  = 25;
            
            ke_msg_send(cmd);
        }
    }
    
    arch_printf("\r\n");

    return (KE_MSG_CONSUMED);
}

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
                             ke_task_id_t const src_id)
{
    // Temporary buffer to form a printable string of the Notification Attribute
    uint8_t buf[param->length + 1];
    // Copy Notification Attribute value
    memcpy(buf, &param->value, param->length);
    // Append NULL character
    buf[param->length] = NULL; 
    
    arch_printf("Notification UID: 0x%X,  attribute ID=0x%X length=0x%X value=%s \r\n", param->ntf_uid, param->att_id, param->length, buf);
    
    if (param->att_id == NTF_ATT_ID_APP_ID)
    {
        // get notification attributes

        struct ancc_get_app_atts_cmd *cmd = KE_MSG_ALLOC_DYN(ANCC_GET_APP_ATTS_CMD,
                                                             TASK_ANCC, TASK_APP,
                                                             ancc_get_app_atts_cmd,
                                                             param->length);
        cmd->conhdl = param->conhdl;
        cmd->operation = ANCC_GET_APP_ATTS_CMD_OP_CODE;
        cmd->atts = APP_ATT_ID_DISPLAY_NAME_PRESENT;
        arch_printf("Req. atts = 0x%X\r\n",cmd->atts);

        cmd->app_id_length = param->length;  // Fill in App ID length
        memcpy(&cmd->app_id[0],&param->value[0],param->length);
        
        ke_msg_send(cmd);
    }
    
    return (KE_MSG_CONSUMED);
}

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
                                      ke_task_id_t const src_id)
{
    // Temporary buffers to form a printable string of the Notification Attribute
    uint8_t att[param->att_length + 1];
    uint8_t app_id[param->app_id_length];

    // Copy App Identifier value
    memcpy(app_id, &param->values, param->app_id_length);
    // Copy Attribute value
    memcpy(att, &param->values[param->app_id_length], param->att_length);
    // Append NULL character
    att[param->att_length] = NULL; 

    arch_printf("App ID: %s,  attribute ID=0x%X length=0x%X value=%s \r\n", app_id, param->att_id, param->att_length, att);
    
    return (KE_MSG_CONSUMED);
}

#endif //BLE_ANC_CLIENT

#endif //(BLE_APP_PRESENT)

/// @} APPTASK
