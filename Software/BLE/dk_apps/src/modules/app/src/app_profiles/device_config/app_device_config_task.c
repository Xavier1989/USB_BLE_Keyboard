/**
****************************************************************************************
*
* @file app_device_config_task.c
*
* @brief Device  Configuration application Task Implemantation.
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

#if (BLE_DEVICE_CONFIG)

#include "app_device_config.h"
#include "app_device_config_task.h"
#include "app_task.h"                  // Application Task API
#include "gpio.h"

#include "co_math.h"                 // Common Maths Definition
#include "co_endian.h"                 // Common Maths Definition



extern struct app_device_config_tag app_device_config_env;
/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Handles Device Configuration  profile database creation confirmation.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance .
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_device_config_create_db_cfm_handler(ke_msg_id_t const msgid,
                                      struct device_config_create_db_cfm const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    // If state is not idle, ignore the message
    if (ke_state_get(dest_id) == APP_DB_INIT)
    {
							
        // Inform the Application Manager
        struct app_module_init_cmp_evt *cfm = KE_MSG_ALLOC(APP_MODULE_INIT_CMP_EVT,
                                                           TASK_APP, TASK_APP,
                                                           app_module_init_cmp_evt);

        cfm->status = param->status;

        ke_msg_send(cfm);
			
    }

    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief Handles disable indication from Sample128 profile.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance.
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_device_config_disable_ind_handler(ke_msg_id_t const msgid,
                                      struct device_config_disable_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles write configuration characteristic event indication from device_config profile
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

int app_device_config_val_ind_handler(ke_msg_id_t const msgid,
                                      struct device_config_val_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{	
    
    if (param->param_id >=  app_device_config_env.params_num)
    {
            return (KE_MSG_CONSUMED);
    }
    else if (param->size != app_device_config_env.params[param->param_id].size)
    {
        return (KE_MSG_CONSUMED);
    }
    
    memcpy(app_device_config_env.params[param->param_id].p_data, param->val, param->size); 
    
    app_device_config_update_ind_send(param->param_id);
    
    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles read command characteristic event indication from device_config profile
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_device_read_request_ind_handler(ke_msg_id_t const msgid,
                                      struct device_config_read_request_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    uint8_t status = 0;
    
    // Allocate message
   struct device_config_read_response_cmd *cmd = KE_MSG_ALLOC(DEVICE_READ_RESPONSE_CMD,
                                              TASK_DEVICE_CONFIG, TASK_APP,
                                              device_config_read_response_cmd);

    if (param->param_id >=  app_device_config_env.params_num)
    {
        status = 1;
    }
    
    if (!status)
    {
        
        cmd->size = app_device_config_env.params[param->param_id].size;
        memcpy(cmd->val, app_device_config_env.params[param->param_id].p_data, cmd->size );
        
    }
    else
    {
        cmd->size = 0;
    }
    
    cmd->param_id = param->param_id;
    cmd->conhdl = app_env.conhdl;    
    cmd->status = status;
    
    ke_msg_send(cmd);
    
    return (KE_MSG_CONSUMED);
}


#endif 

#endif //(BLE_APP_PRESENT)

/// @} APPTASK
