/**
 ****************************************************************************************
 *
 * @file app_scppc_task.h
 *
 * @brief Scan Parameter Profile Client - Task Handlers header file.
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

#ifndef APP_SCPPC_TASK_H_
#define APP_SCPPC_TASK_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

#include "ke_task.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/// Maximal number of APP DISC Task instances
#define APP_SCPPC_IDX_MAX        (1)

/// Possible states of the APP_DIS task
enum
{
    /// Disabled state
    APP_SCPPC_DISABLED,
    /// Idle state
    APP_SCPPC_IDLE,
    /// Discovering state
    APP_SCPPC_DISCOVERING,
    /// Connected state
    APP_SCPPC_CONNECTED,

    /// Number of defined states.
    APP_SCPPC_STATE_MAX
};

/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */

extern const struct ke_state_handler app_scppc_default_handler;
extern ke_state_t app_scppc_state[APP_SCPPC_IDX_MAX];

/// @} APPDISTASK

#endif //APP_SCPPC_TASK_H_
