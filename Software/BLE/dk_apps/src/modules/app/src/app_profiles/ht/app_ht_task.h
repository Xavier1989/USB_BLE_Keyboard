/**
 ****************************************************************************************
 *
 * @file app_ht_task.h
 *
 * @brief Header file - APPHTTASK.
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */

#ifndef APP_HT_TASK_H_
#define APP_HT_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup APPHTTASK Task
 * @ingroup APPHT
 * @brief Health Thermometer Service Application Task
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

#if (BLE_APP_HT)

#include "ke_task.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/// Maximal number of APP HT Task instances
#define APP_HT_IDX_MAX        (1)

/// Possible states of the APP_HT task
enum
{
    /// Disabled state
    APP_HT_DISABLED,
    /// Idle state
    APP_HT_IDLE,
    /// Connected state
    APP_HT_CONNECTED,

    /// Number of defined states.
    APP_HT_STATE_MAX
};

/// APP Task messages
enum
{
    APP_HT_TIMER = KE_FIRST_MSG(TASK_APP_HT),
};

extern const struct ke_state_handler app_ht_default_handler;
extern ke_state_t app_ht_state[APP_HT_IDX_MAX];

#endif //(BLE_APP_HT)

/// @} APPHTTASK

#endif //APP_HT_TASK_H_
