/**
 ****************************************************************************************
 *
 * @file app_task.c
 *
 * @brief A4WP WPT Power Transmit Unit (PTU) implementation file used for
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

#include "app_task.h"
#include "gapm_task.h"
#include "gapc_task.h"
#include "disc.h"
#include "disc_task.h"
#include "smpc_task.h"
#include "wptc.h"
#include "wptc_task.h" 

/* for basic APP API, eg. connecting, scanning...*/
#include "app.h"
#include "app_task.h"

#if (BLE_APP_PTU)
#include "app_ptu.h"
#include "app_ptu_task.h"
#include "app_ptu_engine_task.h"

/**
 ****************************************************************************************
 * @brief Handles WPTC profile enable confirmation.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_ptu_task_enable_cfm_handler(ke_msg_id_t msgid, struct wptc_enable_cfm *param,
        ke_task_id_t dest_id, ke_task_id_t src_id)
{
    app_ptu_task_enable_cfm_impl(msgid, param, dest_id, src_id);

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles read characteristic response.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_ptu_task_rd_char_rsp_handler(ke_msg_id_t msgid, struct wptc_rd_char_rsp *param,
        ke_task_id_t dest_id, ke_task_id_t src_id)
{
    app_ptu_task_rd_char_rsp_impl(msgid, param, dest_id, src_id);

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles write characteristic response.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_ptu_task_wr_char_rsp_handler(ke_msg_id_t msgid, struct wptc_wr_char_rsp *param,
        ke_task_id_t dest_id, ke_task_id_t src_id)
{
    app_ptu_task_wr_char_rsp_impl(msgid, param, dest_id, src_id);

    return (KE_MSG_CONSUMED);
}

int app_ptu_task_alert_ntf_handler(ke_msg_id_t msgid, struct wptc_alert_report_ntf *param,
        ke_task_id_t dest_id, ke_task_id_t src_id)
{
    app_ptu_task_alert_ntf_impl(msgid, param, dest_id, src_id);

    return (KE_MSG_CONSUMED);
}
#endif // (BLE_APP_PTU)
