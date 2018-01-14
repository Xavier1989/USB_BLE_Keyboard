/**
 ****************************************************************************************
 *
 * @ile app_ptu.c
 *
 * @brief A4WP Wireless Power Transfer PRU role application.
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

#include "rwip_config.h"             // SW configuration

#if (BLE_APP_PRU)
#include "app_pru.h"
#include "app_pru_task.h"
#include "wpts.h"
#include "wpts_task.h"

/**
 ****************************************************************************************
 * @brief Send enable request to WPTS profile task.
 *
 * @return void.
 ****************************************************************************************
 */
void app_pru_enable(uint16_t conhdl)
{
    // Allocate the message
    struct wpts_enable_req * req = KE_MSG_ALLOC(WPTS_ENABLE_REQ, TASK_WPTS, TASK_APP,
                                                 wpts_enable_req);
    
    req->conhdl             = conhdl;
    req->sec_lvl            = PERM(SVC, ENABLE);
    
    // Send the message
    ke_msg_send(req);
}

/**
 ****************************************************************************************
 * @brief Send create data base request to WPTS profile task.
 *
 * @return void.
 ****************************************************************************************
 */
void app_pru_create_db_send(void)
{
    // Add DIS in the database
    struct wpts_create_db_req *req = KE_MSG_ALLOC(WPTS_CREATE_DB_REQ,
                                                  TASK_WPTS, TASK_APP,
                                                  wpts_create_db_req);
    req->features = 0;
    // Send the message
    ke_msg_send(req);
}

#endif // BLE_APP_PRU
