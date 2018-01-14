/**
 ****************************************************************************************
 *
 * @file app_scppc_task.c
 *
 * @brief Scan Parameters Profile Client - Task Handlers.
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

#include "ke_task.h"

#include "app.h" 
#include "app_console.h"
#include "prf_utils.h"
#include "scppc.h"
#include "scppc_task.h"

#include "app_scppc.h"
#include "app_scppc_task.h"

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

static int scppc_enable_cfm_handler(ke_msg_id_t const msgid,
                                    struct scppc_enable_cfm const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    switch(param->status) 
    {
        case PRF_ERR_OK:
            break;
        case PRF_ERR_STOP_DISC_CHAR_MISSING:
            arch_puts("SCPP-C: Missing characteristic!\r\n");
            break;
        case PRF_ERR_MULTIPLE_SVC:
            arch_puts("SCPP-C: Multiple services!\r\n");
            break;
        default:
            break;
    }
    
    if(param->status != PRF_ERR_OK)
    {
        // Go to the idle state
        ke_state_set(dest_id, APP_SCPPC_IDLE);
    } 
    else 
    {
        // Go to the connected state
        ke_state_set(dest_id, APP_SCPPC_CONNECTED);
        
        // Keep read values somewhere
        scps = param->scps;
    }

    return (KE_MSG_CONSUMED);
}


static int scppc_error_ind_handler(ke_msg_id_t const msgid,
                                    struct scppc_error_ind const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    switch(param->status)
    {
        case PRF_ERR_INEXISTENT_HDL:
            arch_puts("BAS-C: Handler not found!\r\n");
            break;
        case PRF_ERR_INVALID_PARAM:
            arch_puts("BAS-C: Invalid param!\r\n");
            break;
        default:
            break;
    }
    
    return (KE_MSG_CONSUMED);
}
                                        

static int scppc_wr_char_rsp_handler(ke_msg_id_t const msgid,
                                    struct scppc_wr_char_rsp const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    //Do nothing for now
    return (KE_MSG_CONSUMED);
}


static int scppc_scan_refresh_ntf_cfg_rd_rsp_handler(ke_msg_id_t const msgid,
                                    struct scppc_scan_refresh_ntf_cfg_rd_rsp const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    //Do nothing for now
    return (KE_MSG_CONSUMED);
}


/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Default State handlers definition
const struct ke_msg_handler app_scppc_default_state[] =
{
    {SCPPC_ERROR_IND,                   (ke_msg_func_t)scppc_error_ind_handler},
    {SCPPC_ENABLE_CFM,                  (ke_msg_func_t)scppc_enable_cfm_handler},
    {SCPPC_WR_CHAR_RSP,                 (ke_msg_func_t)scppc_wr_char_rsp_handler},
    {SCPPC_SCAN_REFRESH_NTF_CFG_RD_RSP, (ke_msg_func_t)scppc_scan_refresh_ntf_cfg_rd_rsp_handler},
};

/// Specifies the message handlers that are common to all states.
const struct ke_state_handler app_scppc_default_handler = KE_STATE_HANDLER(app_scppc_default_state);

/// Defines the place holder for the states of all the task instances.
ke_state_t app_scppc_state[APP_SCPPC_IDX_MAX] __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY
