/**
****************************************************************************************
*
* @file app_adc_notify_task.c
*
* @brief ADC notify application Task Implemantation.
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
 * @addtogroup APPTASK
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"              // SW configuration

#if (BLE_APP_PRESENT)

#if (BLE_ADC_NOTIFY)

#include "app_adc_notify.h"
#include "app_adc_notify_task.h"
#include "app_task.h"                  // Application Task API
#include "gpio.h"
#include "adc.h"

#include "co_math.h"                 // Common Maths Definition

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Handles ADC notify  profile database creation confirmation.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance .
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_adc_notify_create_db_cfm_handler(ke_msg_id_t const msgid,
                                      struct adc_notify_create_db_cfm const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    // If state is not idle, ignore the message
    if (ke_state_get(dest_id) == APP_DB_INIT)
    {
							
        // Inform the Application Manager
        struct app_module_init_cmp_evt *cfm = KE_MSG_ALLOC(APP_MODULE_INIT_CMP_EVT,
                                                           TASK_APP, TASK_APP,
                                                           app_module_init_cmp_evt);

        cfm->status = param->status;

        ke_msg_send(cfm);
			
    }

    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief Handles disable indication from ADC notify profile.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance.
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_adc_notify_disable_ind_handler(ke_msg_id_t const msgid,
                                      struct adc_notify_disable_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    ke_timer_clear(APP_ADC_SAMPLING_TIMER, TASK_APP); //5 sec

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles write of characteristic's configuration event indication from adc_notify profile
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

int app_adc_notify_cfg_ind_handler(ke_msg_id_t const msgid,
                                      struct adc_notify_cfg_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{	
    
    if (param->val == PRF_CLI_START_NTF)
    {
        adc_init(GP_ADC_SE, 0); // Single ended mode
        adc_enable_channel(ADC_CHANNEL_P01);  //
    
        app_adc_notify_upd_char(SWAP(adc_get_sample()));
    
        app_timer_set(APP_ADC_SAMPLING_TIMER, TASK_APP, APP_ADC_SAMPLING_TIMEOUT); //5 sec
    }
    else if (param->val == PRF_CLI_STOP_NTFIND)
    {
        ke_timer_clear(APP_ADC_SAMPLING_TIMER, TASK_APP); //5 sec
    }
    
    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles APP_SAMPLING_TIMER's expiration message. Samples ADC and writes value to characteristic.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_adc_sampling_timer_handler(ke_msg_id_t const msgid,
                                      void *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    
    adc_init(GP_ADC_SE, ADC_POLARITY_UNSIGNED); // Single ended mode
	adc_enable_channel(ADC_CHANNEL_P01);  // 
    
    app_adc_notify_upd_char(SWAP(adc_get_sample()));
    
    app_timer_set(APP_ADC_SAMPLING_TIMER, TASK_APP, APP_ADC_SAMPLING_TIMEOUT); //5 sec
    
    return (KE_MSG_CONSUMED);
    
}

#endif //

#endif //(BLE_APP_PRESENT)

/// @} APPTASK
