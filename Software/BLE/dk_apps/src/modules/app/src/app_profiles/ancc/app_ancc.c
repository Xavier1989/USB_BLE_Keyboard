/**
****************************************************************************************
*
* @file app_ancc.c
*
* @brief ANCC application.
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
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"

#if (BLE_APP_PRESENT)
#if (BLE_ANC_CLIENT)

#include "app_api.h"                // application task definitions
#include "ancc_task.h"              // ANCC profile functions
#include "app_ancc.h"
#include "llc_task.h"
#include "gpio.h"

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
*/
 
 /**
 ****************************************************************************************
 * ANCS Client Application Functions
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize application and enable ANCC profile.
 *
 ****************************************************************************************
 */
void app_ancc_enable(void)
{
    // Allocate the message
    struct ancc_enable_req * req = KE_MSG_ALLOC(ANCC_ENABLE_REQ, TASK_ANCC, TASK_APP,
                                                ancc_enable_req);

    // Fill in the parameter structure
    req->conhdl = app_env.conhdl;
    
    // Send the message
    ke_msg_send(req);

}

/**
 ****************************************************************************************
 * @brief Subscribe to the Notification Source notifications
 *
 ****************************************************************************************
 */
void app_ancc_ntf_src_subscribe(void)
{
    // Allocate the message
    struct ancc_write_cmd * cmd = KE_MSG_ALLOC(ANCC_WRITE_CMD, TASK_ANCC, TASK_APP,
                                               ancc_write_cmd);

    // Fill in the parameter structure
    cmd->conhdl = app_env.conhdl;
    cmd->operation = NULL;
    cmd->write_code = ANCC_RD_WR_NTF_SRC_CFG;
    cmd->value = PRF_CLI_START_NTF;

    // Send the message
    ke_msg_send(cmd);

}

/**
 ****************************************************************************************
 * @brief Subscribe to the Data Source notifications
 *
 ****************************************************************************************
 */
void app_ancc_data_src_subscribe(void)
{
    // Allocate the message
    struct ancc_write_cmd * cmd = KE_MSG_ALLOC(ANCC_WRITE_CMD, TASK_ANCC, TASK_APP,
                                               ancc_write_cmd);

    // Fill in the parameter structure
    cmd->conhdl = app_env.conhdl;
    cmd->operation = NULL;
    cmd->write_code = ANCC_RD_WR_DATA_SRC_CFG;
    cmd->value = PRF_CLI_START_NTF;

    // Send the message
    ke_msg_send(cmd);
}


#endif //BLE_ANC_CLIENT
#endif //BLE_APP_PRESENT

/// @} APP
