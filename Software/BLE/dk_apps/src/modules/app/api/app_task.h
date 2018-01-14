/**
 ****************************************************************************************
 *
 * @file app_task.h
 *
 * @brief Header file - APPTASK.
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */

#ifndef APP_TASK_H_
#define APP_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup APPTASK Task
 * @ingroup APP
 * @brief Routes ALL messages to/from APP block.
 *
 * The APPTASK is the block responsible for bridging the final application with the
 * RWBLE software host stack. It communicates with the different modules of the BLE host,
 * i.e. @ref SMP, @ref GAP and @ref GATT.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"             // SW configuration

#if (BLE_APP_PRESENT)

#include "ke_task.h"        // Kernel Task
#include "ke_msg.h"         // Kernel Message
#include "gapm_task.h"
#include "gapc_task.h"
#include "app_api.h"
#include <stdint.h>         // Standard Integer

/*
 * DEFINES
 ****************************************************************************************
 */

/// Number of APP Task Instances
#define APP_IDX_MAX                 (1)

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/// States of APP task
enum APP_STATE
{
    /// Disabled State
    APP_DISABLED,
    /// Database Initialization State
    APP_DB_INIT,
    /// Connectable state
    APP_CONNECTABLE,

    /**
     * CONNECTED STATES
     */
    /// Security state,
    APP_SECURITY,
    /// Connection Parameter Update State
    APP_PARAM_UPD,
    /// Connected state
    APP_CONNECTED,

    /// Number of defined states.
    APP_STATE_MAX
};



/*
 * API MESSAGES STRUCTURES
 ****************************************************************************************
 */

/// Parameters of the @ref APP_MODULE_INIT_CMP_EVT message
struct app_module_init_cmp_evt
{
    ///Status
    uint8_t status;
};

/// Parameters of the @ref APP_SECURITY_CMP_EVT message
struct app_pairing_cmp_evt
{
    ///Status
    uint8_t status;
};


/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

extern const struct ke_state_handler app_default_handler;

extern ke_state_t app_state[APP_IDX_MAX];



/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Handles ready indication from the GAP. - Reset the stack
 ****************************************************************************************
 */
int gapm_device_ready_ind_handler(ke_msg_id_t const msgid,
                                         void const *param,
                                         ke_task_id_t const dest_id,
                                         ke_task_id_t const src_id);
                                         
/**
 ****************************************************************************************
 * @brief Handles GAP manager command complete events.
 ****************************************************************************************
 */

int gapm_cmp_evt_handler(ke_msg_id_t const msgid,
                                struct gapm_cmp_evt const *param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id);

/**
 ****************************************************************************************
 * @brief Handles connection complete event from the GAP. Will enable profile.
 ****************************************************************************************
 */
int gapc_connection_req_ind_handler(ke_msg_id_t const msgid,
                                           struct gapc_connection_req_ind const *param,
                                           ke_task_id_t const dest_id,
                                           ke_task_id_t const src_id);
                                           

/**
 ****************************************************************************************
 * @brief Handles GAP controller command complete events.
 ****************************************************************************************
 */
int gapc_cmp_evt_handler(ke_msg_id_t const msgid,
                                struct gapc_cmp_evt const *param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id);


/**
 ****************************************************************************************
 * @brief Handles disconnection complete event from the GAP.
 ****************************************************************************************
 */
int gapc_disconnect_ind_handler(ke_msg_id_t const msgid,
                                      struct gapc_disconnect_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);
                                      
/**
 ****************************************************************************************
 * @brief Handles reception of the APP_MODULE_INIT_CMP_EVT messgage
 ****************************************************************************************
 */
int app_module_init_cmp_evt_handler(ke_msg_id_t const msgid,
                                           const struct app_module_init_cmp_evt *param,
                                           ke_task_id_t const dest_id,
                                           ke_task_id_t const src_id);


/// @} APPTASK

#endif //(BLE_APP_PRESENT)

#endif // APP_TASK_H_
