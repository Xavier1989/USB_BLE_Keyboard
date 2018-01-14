/**
 ****************************************************************************************
 *
 * @file app_ptu_task.h
 *
 * @brief A4WP WPT Power Transmit Unit (PTU) header file used for
 * handling of ble events and responses that coming from WPTC (Wireless Power
 * Transfer Client) module.
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

#ifndef APP_PTU_TASK_H
#define APP_PTU_TASK_H

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "ke_task.h"         // kernel task
#include "ke_msg.h"          // kernel message#include "gapm_task.h"
#include "gapc_task.h"
#include "wptc_task.h"
#include "app_ptu.h"
#include <stdint.h>          // standard integer

int app_ptu_task_enable_cfm_handler(ke_msg_id_t msgid, struct wptc_enable_cfm *param,
        ke_task_id_t dest_id, ke_task_id_t src_id);

int app_ptu_task_rd_char_rsp_handler(ke_msg_id_t msgid, struct wptc_rd_char_rsp *param,
        ke_task_id_t dest_id, ke_task_id_t src_id);

int app_ptu_task_wr_char_rsp_handler(ke_msg_id_t msgid, struct wptc_wr_char_rsp *param,
        ke_task_id_t dest_id, ke_task_id_t src_id);

int app_ptu_task_alert_ntf_handler(ke_msg_id_t msgid, struct wptc_alert_report_ntf *param,
        ke_task_id_t dest_id, ke_task_id_t src_id);
#endif // APP_TASK_H_
