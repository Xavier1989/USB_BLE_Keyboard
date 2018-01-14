/**
****************************************************************************************
*
* @file app_ptu.h
*
* @brief Header file Main A4WP Wireless Power Transfer PTU application.
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

#ifndef APP_PTU_H_
#define APP_PTU_H_

#include "co_bt.h"
#include "smpc_task.h"
#include "disc.h"

#include "wpt_common.h"

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>            // standard integer

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

void app_ptu_api_read_pru_dynamic_param(uint16_t ble_conn_hdl, uint8_t ble_conn_idx);

void app_ptu_api_read_pru_static_param(uint16_t ble_conn_hdl, uint8_t ble_conn_idx);

void app_ptu_api_write_pru_control(uint16_t ble_conn_hdl, uint8_t ble_conn_idx, uint8_t enables_val,
                                    uint8_t permission_val, uint8_t time_set_val);

void app_ptu_api_write_ptu_static_param(uint16_t ble_conn_hdl, uint8_t ble_conn_idx);

/**
 ****************************************************************************************
 * @brief Initialize the BLE demo application.
 *
 ****************************************************************************************
 */

/// Funtions called from app_ptu_task.c


/// @} APP

#endif // APP_PTU_H_
