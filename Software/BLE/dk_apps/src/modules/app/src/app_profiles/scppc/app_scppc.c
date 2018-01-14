/**
 ****************************************************************************************
 *
 * @file app_scppc.c
 *
 * @brief Scan Parameters Profile Client - Entry point.
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
#include "app.h" 
#include "app_console.h"
#include "scppc.h"
#include "scppc_task.h"

#include "app_scppc_task.h"

/// Device Information Service Application Task Descriptor
static const struct ke_task_desc TASK_DESC_APP_SCPPC = {NULL, &app_scppc_default_handler, app_scppc_state, APP_SCPPC_STATE_MAX, APP_SCPPC_IDX_MAX};

struct scps_content scps  __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
void app_scppc_init(void)
{
    // Create APP DIS(C) task
    ke_task_create(TASK_APP_SCPPC, &TASK_DESC_APP_SCPPC);

    // Go to idle state
    ke_state_set(TASK_APP_SCPPC, APP_SCPPC_IDLE);
}


void app_scppc_enable_prf(uint16_t conhdl)
{
    // Allocate the message
    struct scppc_enable_req *req = KE_MSG_ALLOC(SCPPC_ENABLE_REQ,
                                               TASK_SCPPC, TASK_APP_SCPPC,
                                               scppc_enable_req);

    // Fill in the parameter structure
    req->conhdl                         = conhdl;
    req->con_type                       = PRF_CON_DISCOVERY;
    req->scan_intv_wd.le_scan_window    = 320;  //320 * 0.625 = 200ms
    req->scan_intv_wd.le_scan_intv      = 336;  //336 * 0.625 = 210ms (scan for 200ms, wait 10ms, scan again for 200ms)

    // Send the message
    ke_msg_send(req);

    // Go to discovering state
    ke_state_set(TASK_APP_SCPPC, APP_SCPPC_DISCOVERING);
}
