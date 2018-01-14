/**
 ****************************************************************************************
 *
 * @file app_basc_task.h
 *
 * @brief BAS Client - Task Handlers header file.
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

#ifndef APP_BASC_TASK_H_
#define APP_BASC_TASK_H_

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

/// Maximal number of APP BASC Task instances
#define APP_BASC_IDX_MAX        (1)

/// Possible states of the APP_BASC task
enum
{
    /// Disabled state
    APP_BASC_DISABLED,
    /// Idle state
    APP_BASC_IDLE,
    /// Discovering state
    APP_BASC_DISCOVERING,
    /// Connected state
    APP_BASC_CONNECTED,

    /// Number of defined states.
    APP_BASC_STATE_MAX
};

/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */

extern const struct ke_state_handler app_basc_default_handler;
extern ke_state_t app_basc_state[APP_BASC_IDX_MAX];

/// @} APPDISTASK

#endif //APP_BASC_TASK_H_
