/**
 ****************************************************************************************
 *
 * @file app_basc.c
 *
 * @brief BAS Client - Entry point.
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
#include "basc.h"
#include "basc_task.h"

#include "app_basc_task.h"

/// Device Information Service Application Task Descriptor
static const struct ke_task_desc TASK_DESC_APP_BASC = {NULL, &app_basc_default_handler, app_basc_state, APP_BASC_STATE_MAX, APP_BASC_IDX_MAX};

uint8_t bas_nb  __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY
struct bas_content bas[BASC_NB_BAS_INSTANCES_MAX]  __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void app_basc_init(void)
{
    bas_nb = 0;
    
    // Create APP DIS(C) task
    ke_task_create(TASK_APP_BASC, &TASK_DESC_APP_BASC);

    // Go to idle state
    ke_state_set(TASK_APP_BASC, APP_BASC_IDLE);
}


void app_basc_enable_prf(uint16_t conhdl)
{
    // Allocate the message
    struct basc_enable_req *req = KE_MSG_ALLOC(BASC_ENABLE_REQ,
                                               TASK_BASC, TASK_APP_BASC,
                                               basc_enable_req);

    // Fill in the parameter structure
    req->conhdl             = conhdl;
    req->con_type           = PRF_CON_DISCOVERY;
    req->bas_nb             = bas_nb;

    // Send the message
    ke_msg_send(req);

    // Go to discovering state
    ke_state_set(TASK_APP_BASC, APP_BASC_DISCOVERING);
}

