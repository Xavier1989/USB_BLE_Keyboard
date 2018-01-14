/**
 ****************************************************************************************
 *
 * @file app_disc_task.c
 *
 * @brief DIS Client - Task Handlers.
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

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"             // SW configuration

#if (BLE_DIS_CLIENT)
#include "ke_task.h"

#include "app.h" 
#include "app_console.h"
#include "prf_utils.h"
#include "disc.h"
#include "disc_task.h"

#include "app_disc.h"
#include "app_disc_task.h"

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

static int disc_enable_cfm_handler(ke_msg_id_t const msgid,
                                    struct disc_enable_cfm const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    switch(param->status) 
    {
        case PRF_ERR_OK:
            break;
        case PRF_ERR_STOP_DISC_CHAR_MISSING:
            arch_puts("DIS-C: Missing characteristic!\r\n");
            break;
        case PRF_ERR_MULTIPLE_SVC:
            arch_puts("DIS-C: Multiple services!\r\n");
            break;
        default:
            break;
    }
    
    if(param->status != PRF_ERR_OK)
    {
        // Go to the idle state
        ke_state_set(dest_id, APP_DISC_IDLE);
    } 
    else 
    {
        // Go to the connected state
        ke_state_set(dest_id, APP_DISC_CONNECTED);
        
        // Keep read values somewhere
        dis = param->dis;
    }

    return (KE_MSG_CONSUMED);
}

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Default State handlers definition
const struct ke_msg_handler app_disc_default_state[] =
{
    {DISC_ENABLE_CFM,               (ke_msg_func_t)disc_enable_cfm_handler},
};

/// Specifies the message handlers that are common to all states.
const struct ke_state_handler app_disc_default_handler = KE_STATE_HANDLER(app_disc_default_state);

/// Defines the place holder for the states of all the task instances.
ke_state_t app_disc_state[APP_DISC_IDX_MAX] __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY

#endif //(BLE_DIS_CLIENT)


