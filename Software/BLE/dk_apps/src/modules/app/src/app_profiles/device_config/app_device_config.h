/**
****************************************************************************************
*
* @file app_device_config.h
*
* @brief Beacon Configuration application.
*
* Copyright (C) 2012. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
*
* <bluetooth.support@diasemi.com> and contributors.
*
****************************************************************************************
*/

#ifndef APP_DEVICE_CONFIG_H_
#define APP_DEVICE_CONFIG_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Sample128 Application entry point.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"
#include "gpio.h"

#if BLE_DEVICE_CONFIG
#include "device_config.h"
#include "device_config_task.h"

/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

struct app_device_config_param 
{
    uint8_t  *p_data;
    uint8_t  size;
};

struct app_device_config_tag 
{
    uint8_t version[16];
    uint8_t version_length;
    uint8_t params_num;   
    struct app_device_config_param   *params;
    
};

struct app_device_config_update_ind
{
    uint8_t param_id;
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 *
 * Application Functions
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Enable the device_config profile
 *
 ****************************************************************************************
 */
void app_device_config_enable(void);

/**
 ****************************************************************************************
 * @brief Create Database
 *
 ****************************************************************************************
 */

void app_device_config_create_db_send(void);

void app_device_config_init(struct app_device_config_param *params_struct, uint8_t params_num, uint8_t *version, uint8_t vesrion_length);

void app_device_config_update(uint8_t *adv_data);

uint16_t app_get_device_adv_int(void);

void app_device_config_update_ind_send(uint8_t param_id);

#endif //DEVICE_CONFIG

/// @} APP

#endif // APP_H_
