/**
 ****************************************************************************************
 *
 * @file app_ht.c
 *
 * @brief Health Thermometer Application entry point
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

#include "rwip_config.h"     // SW configuration

#if (BLE_APP_HT)

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app_ht.h"                  // Health Thermometer Application Definitions
#include "app_ht_task.h"             // Health Thermometer Application Task API
#include "app.h"                     // Application Definitions
#include "app_task.h"                // application task definitions
#include "htpt_task.h"               // health thermometer functions
#include "co_bt.h"
#include "prf_types.h"
#include "arch.h"                    // Platform Definitions

#if (DISPLAY_SUPPORT)
#include "app_display.h"
#include "display.h"
#endif //DISPLAY_SUPPORT

/*
 * DEFINES
 ****************************************************************************************
 */

/// Initial Temperature Value : 37c
#define APP_HT_TEMP_VALUE_INIT       (3700)
/// Temperature Step
#define APP_HT_TEMP_STEP_INIT        (20)
/// Measurement Interval Value Min
#define APP_HT_MEAS_INTV_MIN         (1)
/// Measurement Interval Value Max
#define APP_HT_MEAS_INTV_MAX         (10)

/*
 * LOCAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/// Health Thermometer Application Task Descriptor
static const struct ke_task_desc TASK_DESC_APP_HT = {NULL, &app_ht_default_handler, app_ht_state, APP_HT_STATE_MAX, APP_HT_IDX_MAX};

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// health thermometer application environment structure
struct app_ht_env_tag app_ht_env __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void app_ht_init(void)
{
    // Reset the environment
    memset(&app_ht_env, 0, sizeof(app_ht_env));

    /// Initial measurement interval : 0s
    app_ht_env.htpt_meas_intv   = 0;
    /// Initial temperature value : 37.00 C
    app_ht_env.temp_value       = APP_HT_TEMP_VALUE_INIT;
    /// Initial temperature step : 0.20 C
    app_ht_env.temp_step        = APP_HT_TEMP_STEP_INIT;
    /// Initial temperature type : NONE
    app_ht_env.temp_meas_type   = 1;

    // Create APP_HT task
    ke_task_create(TASK_APP_HT, &TASK_DESC_APP_HT);

    // Go to disabled state
    ke_state_set(TASK_APP_HT, APP_HT_DISABLED);
}

void app_ht_create_db_send(void)
{
    // Add HTS in the database
    struct htpt_create_db_req *req = KE_MSG_ALLOC(HTPT_CREATE_DB_REQ,
                                                  TASK_HTPT, TASK_APP_HT,
                                                  htpt_create_db_req);

    req->features = HTPT_ALL_FEAT_SUP;

    req->valid_range_min = APP_HT_MEAS_INTV_MIN;
    req->valid_range_max = APP_HT_MEAS_INTV_MAX;

    ke_msg_send(req);
}

void app_ht_enable_prf(uint16_t conhdl)
{
    // Allocate the message
    struct htpt_enable_req * req = KE_MSG_ALLOC(HTPT_ENABLE_REQ,
                                                TASK_HTPT, TASK_APP_HT,
                                                htpt_enable_req);

    // Fill in the parameter structure
    req->conhdl             = conhdl;
    req->sec_lvl            = PERM(SVC, ENABLE);
    req->con_type           = PRF_CON_DISCOVERY;

    // NTF/IND initial status - Disabled
    req->temp_meas_ind_en   = PRF_CLI_STOP_NTFIND;
    req->interm_temp_ntf_en = PRF_CLI_STOP_NTFIND;
    req->meas_intv_ind_en   = PRF_CLI_STOP_NTFIND;

    // Initial Measurement Interval value
    req->meas_intv          = 0;

    // Send the message
    ke_msg_send(req);

    // Go to Connected state
    ke_state_set(TASK_APP_HT, APP_HT_CONNECTED);
}

/**
 ****************************************************************************************
 * Health Thermometer Application Functions
 ****************************************************************************************
 */
void app_htpt_temp_send(void)
{
    struct prf_date_time timestamp;

    //Allocate the message
    struct htpt_temp_send_req * req = KE_MSG_ALLOC(HTPT_TEMP_SEND_REQ,
                                                   TASK_HTPT, TASK_APP_HT,
                                                   htpt_temp_send_req);

    int32_t value = (int32_t)(app_ht_env.temp_value);
    value |= 0xFE000000;

    timestamp.year      = 0x0000;
    timestamp.month     = 0x00;
    timestamp.day       = 0x00;
    timestamp.hour      = 0x00;
    timestamp.min       = 0x00;
    timestamp.sec       = 0x00;

    // Connection Handle
    req->conhdl                 = app_env.conhdl;
    // Stable => Temperature Measurement Char.
    req->flag_stable_meas       = 0x01;

    // Temperature Measurement Value
    req->temp_meas.flags        = 0x04;             //Celcius, no timestamp, type
    req->temp_meas.temp         = value;
    req->temp_meas.time_stamp   = timestamp;
    req->temp_meas.type         = app_ht_env.temp_meas_type;

    ke_msg_send(req);
}

void app_htpt_temp_inc(void)
{
    app_ht_env.temp_value += app_ht_env.temp_step;

    #if (DISPLAY_SUPPORT)
    app_display_update_temp_val_screen(app_ht_env.temp_value);
    #endif //DISPLAY_SUPPORT

    app_htpt_temp_send();
}

void app_htpt_temp_dec(void)
{
    app_ht_env.temp_value -= app_ht_env.temp_step;

    #if (DISPLAY_SUPPORT)
    app_display_update_temp_val_screen(app_ht_env.temp_value);
    #endif //DISPLAY_SUPPORT

    app_htpt_temp_send();
}

void app_htpt_temp_type_inc(void)
{
    app_ht_env.temp_meas_type = (uint8_t)(((int)app_ht_env.temp_meas_type + 1)%10);

    #if (DISPLAY_SUPPORT)
    app_htpt_update_type_string(app_ht_env.temp_meas_type);
    app_display_update_temp_type_screen(app_ht_env.temp_type_string);
    #endif //DISPLAY_SUPPORT
}

void app_htpt_temp_type_dec(void)
{
    if (((int)app_ht_env.temp_meas_type-1) < 0)
    {
        app_ht_env.temp_meas_type = 0x09;
    }
    else
    {
        app_ht_env.temp_meas_type = app_ht_env.temp_meas_type - 1;
    }

    #if DISPLAY_SUPPORT
    app_htpt_update_type_string(app_ht_env.temp_meas_type);
    app_display_update_temp_type_screen(app_ht_env.temp_type_string);
    #endif //DISPLAY_SUPPORT
}

void app_htpt_update_type_string(uint8_t temp_type)
{
    switch (temp_type)
    {
        case 0:
            strcpy(app_ht_env.temp_type_string, "NONE");
            break;
        case 1:
            strcpy(app_ht_env.temp_type_string, "ARMPIT");
            break;
        case 2:
            strcpy(app_ht_env.temp_type_string, "BODY");
            break;
        case 3:
            strcpy(app_ht_env.temp_type_string, "EAR");
            break;
        case 4:
            strcpy(app_ht_env.temp_type_string, "FINGER");
            break;
        case 5:
            strcpy(app_ht_env.temp_type_string, "GASTRO-INT");
            break;
        case 6:
            strcpy(app_ht_env.temp_type_string, "MOUTH");
            break;
        case 7:
            strcpy(app_ht_env.temp_type_string, "RECTUM");
            break;
        case 8:
            strcpy(app_ht_env.temp_type_string, "TOE");
            break;
        case 9:
            strcpy(app_ht_env.temp_type_string, "TYMPANUM");
            break;
        default:
            strcpy(app_ht_env.temp_type_string, "UNKNOWN");
            break;
    }
}

#endif //BLE_APP_HT

/// @} APP
