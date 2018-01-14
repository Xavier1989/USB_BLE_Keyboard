/**
 ****************************************************************************************
 *
 * @file app_dis.c
 *
 * @brief Device Information Service Application entry point
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

#if (BLE_DIS_SERVER)

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app_dis.h"                 // Device Information Service Application Definitions
#include "app_dis_task.h"            // Device Information Service Application Task API
#include "app.h"                     // Application Definitions
#include "app_task.h"                // Application Task Definitions
#include "diss_task.h"               // Health Thermometer Functions


/*
 * LOCAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void app_dis_init(void)
{
    return;
}

void app_dis_create_db_send(void)
{
    // Add DIS in the database
    struct diss_create_db_req *req = KE_MSG_ALLOC(DISS_CREATE_DB_REQ,
                                                  TASK_DISS, TASK_APP,
                                                  diss_create_db_req);

    req->features = APP_DIS_FEATURES;

    // Send the message
    ke_msg_send(req);
}

void app_dis_enable_prf_sec(uint16_t conhdl, uint8_t sec_lvl)
{
    // Allocate the message
    struct diss_enable_req *req = KE_MSG_ALLOC(DISS_ENABLE_REQ,
                                               TASK_DISS, TASK_APP,
                                               diss_enable_req);

    // Fill in the parameter structure
    req->conhdl             = conhdl;
    req->sec_lvl            = sec_lvl;
    req->con_type           = PRF_CON_DISCOVERY;

    // Send the message
    ke_msg_send(req);
}

void app_dis_enable_prf(uint16_t conhdl)
{
    app_dis_enable_prf_sec(conhdl, PERM(SVC, ENABLE));
}

#endif //BLE_DIS_SERVER

/// @} APP
