/**
 ****************************************************************************************
 *
 * @file app_neb_task.h
 *
 * @brief This file contains definitions related to the Nebulizer application
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */

#ifndef APP_NEB_TASK_H_
#define APP_NEB_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup APP_NEB Nebulizer application
 *@{
 ****************************************************************************************
 */
 
/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

#if (BLE_APP_NEB)

#include "ke_task.h"         // kernel task

/*
 * INSTANCES
 ****************************************************************************************
 */
/// Maximum number of instances of the APP_NEB task
#define APP_NEB_IDX_MAX 1

/*
 * STATES
 ****************************************************************************************
 */
/// Possible states of the APP_NEB task
enum APP_NEB_STATE
{
    /// DISABLED state
    APP_NEB_DISABLED,
    /// IDLE state
    APP_NEB_IDLE,
    /// CONNECTED state
    APP_NEB_CONNECTED,
    
    /// Number of states.
    APP_NEB_STATE_MAX
};

/*
 * MESSAGES
 ****************************************************************************************
 */
/// Message API of the APP_NEB task
enum APP_NEB_MSG
{
    APP_NEB_MSG_ID_FIRST = KE_FIRST_MSG(TASK_APP_NEB),

    APP_NEB_MSG_RX = APP_NEB_MSG_ID_FIRST,

    APP_NEB_MSG_ID_LAST
};

/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */
extern const struct ke_state_handler app_neb_default_handler;
extern ke_state_t app_neb_state[APP_NEB_IDX_MAX];

#endif //BLE_APP_NEB

/// @} APP_NEB

#endif // APP_NEB_TASK_H_

