/**
 ****************************************************************************************
 *
 * @file app_ht_task.c
 *
 * @brief Health Thermometer Application Task
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

#include "rwip_config.h"

#if (BLE_APP_HT)

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app_task.h"           // Application Manager Task API
#include "htpt_task.h"          // Health Thermometer Profile Server Task API
#include "htpt.h"               // Health Thermometer Profile Definitions
#include "app_ht_task.h"        // Health Thermometer Application Task API
#include "app_ht.h"             // Health Thermometer Application Definitions
#include "app_display.h"        // Display Application Definitions

#include <stdlib.h>             // standard library

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

static int htpt_create_db_cfm_handler(ke_msg_id_t const msgid,
                                      struct htpt_create_db_cfm const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    if (ke_state_get(dest_id) == APP_HT_DISABLED)
    {
        if (param->status == PRF_ERR_OK)
        {
            // Go to Idle state
            ke_state_set(TASK_APP_HT, APP_HT_IDLE);
        }

        // Inform the Application Manager
        struct app_module_init_cmp_evt *cfm = KE_MSG_ALLOC(APP_MODULE_INIT_CMP_EVT,
                                                           TASK_APP, TASK_APP_HT,
                                                           app_module_init_cmp_evt);

        cfm->status = param->status;

        ke_msg_send(cfm);
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles measurement interval start indication from the Health Thermometer
 *        profile.
 *        Start or stop a timer following the value of the param intv.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int htpt_meas_intv_chg_ind_handler(ke_msg_id_t const msgid,
                                          struct htpt_meas_intv_chg_ind const *param,
                                          ke_task_id_t const dest_id,
                                          ke_task_id_t const src_id)
{
    if (ke_state_get(dest_id) == APP_HT_CONNECTED)
    {
        // Store the received Measurement Interval value
        app_ht_env.htpt_meas_intv = param->intv;

        // Check the new Measurement Interval Value
        if (app_ht_env.htpt_meas_intv != 0)
        {
            // Check if a Timer already exists
            if (!app_ht_env.timer_enable)
            {
                // Set a Timer
                ke_timer_set(APP_HT_TIMER, TASK_APP_HT, app_ht_env.htpt_meas_intv*100);
                app_ht_env.timer_enable = true;
            }
            else
            {
                // Clear the previous timer
                ke_timer_clear(APP_HT_TIMER, TASK_APP_HT);
                // Create a new timer with the received measurement interval
                ke_timer_set(APP_HT_TIMER, TASK_APP_HT, app_ht_env.htpt_meas_intv*100);
            }
        }
        else
        {
            // Check if a Timer exists
            if (app_ht_env.timer_enable)
            {
                // Measurement Interval is 0, clear the timer
                ke_timer_clear(APP_HT_TIMER, TASK_APP_HT);
                app_ht_env.timer_enable = false;
            }
        }
    }

    return (KE_MSG_CONSUMED);
}

static int htpt_disable_ind_handler(ke_msg_id_t const msgid,
                                    struct htpt_disable_ind const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    if (ke_state_get(dest_id) == APP_HT_CONNECTED)
    {
        // Stop the Health Thermometer timer if enabled
        if (app_ht_env.timer_enable)
        {
            ke_timer_clear(APP_HT_TIMER, TASK_APP_HT);
            app_ht_env.timer_enable = false;
        }

        // Go to Idle state
        ke_state_set(TASK_APP_HT, APP_HT_IDLE);
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles health thermometer timer
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int app_ht_timer_handler(ke_msg_id_t const msgid,
                                void const *param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id)
{
    if (ke_state_get(dest_id) == APP_HT_CONNECTED)
    {
        // Random generation of a temperature value
        uint32_t rand_temp_step;
        // Sign used to know if the tempererature will be increased or decreased
        int8_t sign;

        // Generate temperature step
        rand_temp_step = (uint32_t)(rand()%20);
        // Increase or decrease the temperature value
        sign = (int8_t)(rand_temp_step & 0x00000001);

        if (!sign)
        {
            sign = -1;
        }

        app_ht_env.temp_value += sign*rand_temp_step;

        // Send the new temperature
        app_htpt_temp_send();

        #if (DISPLAY_SUPPORT)
        app_display_update_temp_val_screen(app_ht_env.temp_value);
        #endif //DISPLAY_SUPPORT

        // Reset the Timer (Measurement Interval is not 0 if we are here)
        ke_timer_set(APP_HT_TIMER, TASK_APP_HT, app_ht_env.htpt_meas_intv*100);
    }

    return (KE_MSG_CONSUMED);
}

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Default State handlers definition
static const struct ke_msg_handler app_ht_default_state[] =
{
    {HTPT_CREATE_DB_CFM,            (ke_msg_func_t)htpt_create_db_cfm_handler},
    {HTPT_MEAS_INTV_CHG_IND,        (ke_msg_func_t)htpt_meas_intv_chg_ind_handler},
    {HTPT_DISABLE_IND,              (ke_msg_func_t)htpt_disable_ind_handler},

    {APP_HT_TIMER,                  (ke_msg_func_t)app_ht_timer_handler},
};

/// Specifies the message handlers that are common to all states.
const struct ke_state_handler app_ht_default_handler = KE_STATE_HANDLER(app_ht_default_state);
/// Defines the place holder for the states of all the task instances.
ke_state_t app_ht_state[APP_HT_IDX_MAX] __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY 

#endif //(BLE_APP_HT)

/// @} APP
