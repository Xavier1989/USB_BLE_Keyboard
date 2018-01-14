/**
 ****************************************************************************************
 *
 * @file app_ht.h
 *
 * @brief Health Thermometer Application entry point
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */

#ifndef APP_HT_H_
#define APP_HT_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Health Thermometer Application entry point.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

#if (BLE_APP_HT)

#include <stdint.h>          // Standard Integer Definition
#include <co_bt.h>

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// health thermometer application environment structure
struct app_ht_env_tag
{
    /// Measurement interval
    uint16_t htpt_meas_intv;

    /// Temperature value
    uint32_t temp_value;
    /// Temperature step
    uint32_t temp_step;
    /// Temperature type
    uint8_t temp_meas_type;
    /// Temperature type string
    char temp_type_string[16];

    /// Measurement interval timer enable
    bool timer_enable;
};

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/// Health Thermomter Application environment
extern struct app_ht_env_tag app_ht_env;

/*
 * FUNCTIONS DECLARATION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *
 * Health Thermometer Application Functions
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize Health Thermometer Application
 ****************************************************************************************
 */
void app_ht_init(void);

/**
 ****************************************************************************************
 * @brief Send a new temperature
 ****************************************************************************************
 */
void app_htpt_temp_send(void);

/**
 ****************************************************************************************
 * @brief Add a Health Thermometer instance in the DB
 ****************************************************************************************
 */
void app_ht_create_db_send(void);

/**
 ****************************************************************************************
 * @brief Enable the health thermometer profile
 ****************************************************************************************
 */
void app_ht_enable_prf(uint16_t);

/**
 ****************************************************************************************
 * @brief Increase temp_value by temp_step
 ****************************************************************************************
 */
void app_htpt_temp_inc(void);

/**
 ****************************************************************************************
 * @brief Decrease temp_value by temp_step
 ****************************************************************************************
 */
void app_htpt_temp_dec(void);

/**
 ****************************************************************************************
 * @brief Decrease temp_type by 1
 ****************************************************************************************
 */
void app_htpt_temp_type_dec(void);

/**
 ****************************************************************************************
 * @brief Increase temp_type by 1
 ****************************************************************************************
 */
void app_htpt_temp_type_inc(void);

/**
 ****************************************************************************************
 * @brief Convert temp_type into a string. Update temp_type_string
 ****************************************************************************************
 */
void app_htpt_update_type_string(uint8_t temp_type);

#endif //(BLE_APP_HT)

/// @} APP

#endif // APP_HT_H_
