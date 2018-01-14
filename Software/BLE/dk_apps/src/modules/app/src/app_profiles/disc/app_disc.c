/**
 ****************************************************************************************
 *
 * @file app_disc.c
 *
 * @brief DIS Client - Entry point.
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

#if BLE_DIS_CLIENT

#include "rwip.h"             
#include "app.h" 
#include "app_console.h"
#include "disc.h"
#include "disc_task.h"
#include "ke_task.h"

#include "app_disc_task.h"

/// Device Information Service Application Task Descriptor
static const struct ke_task_desc TASK_DESC_APP_DISC = {NULL, &app_disc_default_handler, app_disc_state, APP_DISC_STATE_MAX, APP_DISC_IDX_MAX};

struct disc_dis_content dis  __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void app_disc_init(void)
{
    // Create APP DIS(C) task
    ke_task_create(TASK_APP_DIS, &TASK_DESC_APP_DISC);

    // Go to idle state
    ke_state_set(TASK_APP_DIS, APP_DISC_IDLE);
}

void app_disc_enable_prf(uint16_t conhdl)
{
    // Allocate the message
    struct disc_enable_req *req = KE_MSG_ALLOC(DISC_ENABLE_REQ,
                                               TASK_DISC, TASK_APP_DIS,
                                               disc_enable_req);

    // Fill in the parameter structure
    req->conhdl             = conhdl;
    req->con_type           = PRF_CON_DISCOVERY;

    // Send the message
    ke_msg_send(req);

    // Go to discovering state
    ke_state_set(TASK_APP_DIS, APP_DISC_DISCOVERING);
}

#endif 

