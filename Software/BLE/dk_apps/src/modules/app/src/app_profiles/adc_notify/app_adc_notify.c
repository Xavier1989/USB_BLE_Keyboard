/**
****************************************************************************************
*
* @file app_adc_notify.c
*
* @brief adc_notify application.
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
#if (BLE_ADC_NOTIFY)

#include "app_api.h"                // application task definitions
#include "adc_notify_task.h"              // adc_notify functions
#include "app_adc_notify.h"
#include "llc_task.h"
#include "gpio.h"
#include "adc.h"

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
*/
 

/**
 ****************************************************************************************
 * @brief enable adc notify profile.
 *
 * @return void.
 ****************************************************************************************
 */

void app_adc_notify_enable(void)
{
		
    // Allocate the message
    struct adc_notify_enable_req* req = KE_MSG_ALLOC(ADC_NOTIFY_ENABLE_REQ, TASK_ADC_NOTIFY, TASK_APP,
                                                 adc_notify_enable_req);
  	
    req->conhdl = app_env.conhdl;
	req->sec_lvl = PERM(SVC, ENABLE);
    
	adc_init(GP_ADC_SE, ADC_POLARITY_UNSIGNED); // Single ended mode
	adc_enable_channel(ADC_CHANNEL_P01);  //
    
    req->adc_notify_val = SWAP(adc_get_sample());//dummy value
    req->feature = 0x00; //client CFG notif/ind disable
    
    // Send the message
    ke_msg_send(req);

}

/**
 ****************************************************************************************
 * @brief Create ADC Notify profile database.
 *
 * @return void.
 ****************************************************************************************
 */

void app_adc_notify_create_db_send(void)
{
    // 
    struct adc_notify_create_db_req *req = KE_MSG_ALLOC(ADC_NOTIFY_CREATE_DB_REQ,
                                                  TASK_ADC_NOTIFY, TASK_APP,
                                                  adc_notify_create_db_req);
    
    req->features = 0; // DUMMY NOT USED

    ke_msg_send(req);
}

/**
 ****************************************************************************************
 * @brief Update adc_notify characteristic value. 
 *
 * @return void.
 ****************************************************************************************
 */

void app_adc_notify_upd_char(uint16_t val)
{	
    struct adc_notify_upd_char_req *req = KE_MSG_ALLOC(ADC_NOTIFY_UPD_CHAR_REQ,
                                                  TASK_ADC_NOTIFY, TASK_APP,
                                                  adc_notify_upd_char_req);
    
    req->val = val; 
    req->conhdl = app_env.conhdl;

    ke_msg_send(req);
}

#endif //BLE_ADC_NOTIFY
#endif //BLE_APP_PRESENT

/// @} APP
