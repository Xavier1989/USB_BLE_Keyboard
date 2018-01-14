/**
 ****************************************************************************************
 *
 * @file app_disc_task.h
 *
 * @brief DIS Client - Task Handlers header file.
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

#ifndef APP_DISC_TASK_H_
#define APP_DISC_TASK_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

#if (BLE_DIS_CLIENT)

#include "ke_task.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/// Maximal number of APP DISC Task instances
#define APP_DISC_IDX_MAX        (1)

/// Possible states of the APP_DIS task
enum
{
    /// Disabled state
    APP_DISC_DISABLED,
    /// Idle state
    APP_DISC_IDLE,
    /// Discovering state
    APP_DISC_DISCOVERING,
    /// Connected state
    APP_DISC_CONNECTED,

    /// Number of defined states.
    APP_DISC_STATE_MAX
};

/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */

extern const struct ke_state_handler app_disc_default_handler;
extern ke_state_t app_disc_state[APP_DISC_IDX_MAX];

#endif //(CFG_PRF_DISS)

/// @} APPDISTASK

#endif //APP_DISC_TASK_H_
