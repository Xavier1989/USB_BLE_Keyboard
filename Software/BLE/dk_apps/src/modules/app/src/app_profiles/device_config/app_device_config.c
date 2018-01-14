/**
****************************************************************************************
*
* @file app_device_config.c
*
* @brief device_config application.
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

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"

#if (BLE_APP_PRESENT)
#if (BLE_DEVICE_CONFIG)

#include "app_api.h"                // application task definitions
#include "device_config_task.h"              // device_config functions
#include "app_device_config.h"
#include "llc_task.h"
#include "gpio.h"
#include "nvds.h"
#include "co_endian.h"             // Common Maths Definition

struct app_device_config_tag app_device_config_env __attribute__((section("retention_mem_area0"),zero_init));

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
*/
 
 /**
 ****************************************************************************************
 * Beacon Configuration Application Functions
 ****************************************************************************************
 */
 
 /**
 ****************************************************************************************
 * @brief initilialization of device_config profile.
 *
 * @return void.
 ****************************************************************************************
 */

void app_device_config_init(struct app_device_config_param *params_struct, uint8_t params_num, uint8_t * version, uint8_t vesrion_length)
{
    uint8_t i;
    
    for (i = 0; i<params_num; i++)
    {
        if (params_struct[i].size > MAX_PARAM_SIZE)
        {
            params_struct[i].size = 64;
        }
    }

    app_device_config_env.params_num = params_num;
    app_device_config_env.params = params_struct;
    memcpy (app_device_config_env.version, version, 16);
    app_device_config_env.version_length =  vesrion_length;
}
 
/**
 ****************************************************************************************
 * @brief enable device_config profile.
 *
 * @return void.
 ****************************************************************************************
 */

void app_device_config_enable(void)
{
		
    // Allocate the message
    struct device_config_enable_req* req = KE_MSG_ALLOC(DEVICE_CONFIG_ENABLE_REQ, TASK_DEVICE_CONFIG, TASK_APP,
                                                 device_config_enable_req);

  	
    req->conhdl = app_env.conhdl;
	req->sec_lvl = PERM(SVC, ENABLE);
    memcpy(req->version, app_device_config_env.version, 16);
    req->version_length =  app_device_config_env.version_length;
	
    // Send the message
    ke_msg_send(req);

}


/**
 ****************************************************************************************
 * @brief Create Sample128 profile database.
 *
 * @return void.
 ****************************************************************************************
 */

void app_device_config_create_db_send(void)
{
    // 
    struct device_config_create_db_req *req = KE_MSG_ALLOC(DEVICE_CONFIG_CREATE_DB_REQ,
                                                  TASK_DEVICE_CONFIG, TASK_APP,
                                                  device_config_create_db_req);
    
    req->dummy = 0; // DUMMY NOT USED

    ke_msg_send(req);
}



/**
 ****************************************************************************************
 * @brief Send APP_DEVICE_CONFIG_UPDATE_IND to TASK_APP. A handler function in project 
 *        code must handle the message 
 *
 * @return void.
 ****************************************************************************************
 */

void app_device_config_update_ind_send(uint8_t param_id)
{
    // 
    struct app_device_config_update_ind *ind = KE_MSG_ALLOC(APP_DEVICE_CONFIG_UPDATE_IND,
                                                  TASK_APP, TASK_APP,
                                                  app_device_config_update_ind);
    
    ind->param_id = param_id; 

    ke_msg_send(ind);
}


#endif //BLE_DEVICE_CONFIG
#endif //BLE_APP_PRESENT

/// @} APP
