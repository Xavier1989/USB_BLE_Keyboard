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

#include "ke_task.h"

#include "app.h" 
#include "app_console.h"
#include "prf_utils.h"
#include "basc.h"
#include "basc_task.h"

#include "app_basc.h"
#include "app_basc_task.h"

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

static int basc_enable_cfm_handler(ke_msg_id_t const msgid,
                                    struct basc_enable_cfm const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    uint8_t svc_inst;
    
    switch(param->status) 
    {
        case PRF_ERR_OK:
            break;
        case PRF_ERR_STOP_DISC_CHAR_MISSING:
            arch_puts("BAS-C: Missing characteristic!\r\n");
            break;
        default:
            break;
    }
    
    if(param->status != PRF_ERR_OK)
    {
        // Go to the idle state
        ke_state_set(dest_id, APP_BASC_IDLE);
    } 
    else 
    {
        // Go to the connected state
        ke_state_set(dest_id, APP_BASC_CONNECTED);
        
        // Keep read values somewhere
        bas_nb = param->bas_nb;
        for (svc_inst = 0; svc_inst < bas_nb; svc_inst++)
        {
            bas[svc_inst] = param->bas[svc_inst];

        }
    }

    return (KE_MSG_CONSUMED);
}


static int basc_error_ind_handler(ke_msg_id_t const msgid,
                                    struct basc_error_ind const *param,
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
                                        

static int basc_disable_ind_handler(ke_msg_id_t const msgid,
                                    struct prf_client_disable_ind const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    // Flush everything in this simple configuration
    bas_nb = 0;

    return (KE_MSG_CONSUMED);
}

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Default State handlers definition
const struct ke_msg_handler app_basc_default_state[] =
{
    {BASC_ERROR_IND,                (ke_msg_func_t)basc_error_ind_handler},
    {BASC_ENABLE_CFM,               (ke_msg_func_t)basc_enable_cfm_handler},
    {BASC_DISABLE_IND,              (ke_msg_func_t)basc_disable_ind_handler},
};

/// Specifies the message handlers that are common to all states.
const struct ke_state_handler app_basc_default_handler = KE_STATE_HANDLER(app_basc_default_state);

/// Defines the place holder for the states of all the task instances.
ke_state_t app_basc_state[APP_BASC_IDX_MAX] __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY


