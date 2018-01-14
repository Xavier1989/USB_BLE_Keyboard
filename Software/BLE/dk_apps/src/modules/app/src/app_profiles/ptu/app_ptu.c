/**
 ****************************************************************************************
 *
 * @ile app_ptu.c
 *
 * @brief A4WP Wireless Power Transfer PTU role application.
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

#include "uart.h"

#include "gap.h"
#include "gapc_task.h"
#include "gapm_task.h"

#include "disc.h"
#include "disc_task.h"
#include "smpc_task.h"

#include "wptc.h"
#include "wptc_task.h"

#include "app.h"
#include "app_task.h"

#if (BLE_APP_PTU)
#include "app_ptu.h"

/**
 ****************************************************************************************
 * @brief Send read request for PRU Dynamic Param characteristic to WPT Client profile task.
 *
 * @return void.
 ****************************************************************************************
 */
void app_ptu_api_read_pru_dynamic_param(uint16_t ble_conn_hdl, uint8_t ble_conn_idx)
{
    struct wptc_rd_pru_dynamic_param_req * req = KE_MSG_ALLOC(
            WPTC_RD_PRU_DYNAMIC_PARAMETER_REQ, KE_BUILD_ID(TASK_WPTC, ble_conn_idx), TASK_APP,
            wptc_rd_pru_dynamic_param_req);

    req->conhdl = ble_conn_hdl;

    ke_msg_send((void *) req);
}

/**
 ****************************************************************************************
 * @brief Send read request for PRU Dynamic Param characteristic to WPT Client profile task.
 *
 * @return void.
 ****************************************************************************************
 */
void app_ptu_api_read_pru_static_param(uint16_t ble_conn_hdl, uint8_t ble_conn_idx)
{
    struct wptc_rd_pru_static_param_req * req = KE_MSG_ALLOC(
            WPTC_RD_PRU_STATIC_PARAMETER_REQ, KE_BUILD_ID(TASK_WPTC, ble_conn_idx), TASK_APP,
            wptc_rd_pru_static_param_req);


    req->conhdl = ble_conn_hdl;

    ke_msg_send((void *) req);
}

void app_ptu_api_write_pru_control(uint16_t ble_conn_hdl, uint8_t ble_conn_idx, uint8_t enables_val,
                                    uint8_t permission_val, uint8_t time_set_val)
{

    struct wptc_wr_pru_control_req * req = KE_MSG_ALLOC(WPTC_WR_PRU_CONTROL_REQ,
            KE_BUILD_ID(TASK_WPTC, ble_conn_idx), TASK_APP, wptc_wr_pru_control_req);

    req->conhdl = ble_conn_hdl;
    req->pru_control_val.enables = enables_val;
    req->pru_control_val.permission = permission_val;
    req->pru_control_val.time_set = time_set_val;

    ke_msg_send((void *) req);
}

void app_ptu_api_write_ptu_static_param(uint16_t ble_conn_hdl, uint8_t ble_conn_idx)
{
    struct wptc_wr_ptu_static_req * req = KE_MSG_ALLOC(
            WPTC_WR_PTU_STATIC_PARAMETER_REQ, KE_BUILD_ID(TASK_WPTC, ble_conn_idx), TASK_APP,
            wptc_wr_ptu_static_req);

    req->conhdl = ble_conn_hdl;
    memcpy(&(req->ptu_static_val), &(app_ptu_env.ptu_stat_val), WPTS_PTU_STATIC_CHAR_LEN);

    ke_msg_send((void *) req);
}

void app_ptu_api_enable(uint16_t conhdl, uint8_t connidx, uint16_t wpt_service_start_hdl,
                            uint8_t pru_rssi, uint8_t wpt_adv,
                            struct gapc_connection_req_ind const *param)
{
    // Allocate the message
    struct wptc_offline_enable_req * req  = KE_MSG_ALLOC(WPTC_OFFLINE_ENABLE_REQ, KE_BUILD_ID(TASK_WPTC, connidx),
                                                            TASK_APP, wptc_offline_enable_req);



    // Fill in the parameter structure
    req->conhdl = conhdl;
    req->wpt_uuid_start = wpt_service_start_hdl;

    // Send the message
    ke_msg_send((void *) req);
}

#endif //(BLE_APP_PTU)
