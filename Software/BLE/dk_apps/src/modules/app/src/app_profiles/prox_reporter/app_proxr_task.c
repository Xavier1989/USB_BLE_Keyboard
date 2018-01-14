/**
****************************************************************************************
*
* @file app_proxr_task.c
*
* @brief Proximity Reporter application Task Implemantation.
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

#if (BLE_PROX_REPORTER)

#include "app_proxr.h"
#include "app_proxr_task.h"
#include "app_task.h"                  // Application Task API
#include "gpio.h"

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Handles Proximity reporter's profile database creation confirmation.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance .
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int proxr_create_db_cfm_handler(ke_msg_id_t const msgid,
                                      struct proxr_create_db_cfm const *param,
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
 * @brief Handles disable indication from the Proximity Reporter profile.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance.
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int proxr_disable_ind_handler(ke_msg_id_t const msgid,
                                      struct proxr_disable_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    
		if (app_env.conhdl == param->conhdl)
			alert_state.ll_alert_lvl = param->lls_alert_lvl;
    
    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief Handles Read Tx Power Level response from llc task.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int llc_rd_tx_pw_lvl_cmp_evt_handler(ke_msg_id_t const msgid,
                                      struct llc_rd_tx_pw_lvl_cmd_complete const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    if (param->status == CO_ERROR_NO_ERROR)
		{
			if (app_env.conhdl == param->conhdl)
				alert_state.txp_lvl = param->tx_pow_lvl;
		}
		else
			alert_state.txp_lvl = 0x00;
			
    // Enable the Proximity Reporter profile
		app_proxr_enable();
    
    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief Handles Alert indication from proximity reporter profile
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

int proxr_alert_ind_handler(ke_msg_id_t const msgid,
                                      struct proxr_alert_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{	
	
		if (param->alert_lvl)
			app_proxr_alert_start(param->alert_lvl); 
		else
			app_proxr_alert_stop();

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles proximitty alert timer
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

int app_proxr_timer_handler(ke_msg_id_t const msgid,
                                   void const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
		 
		if (alert_state.blink_toggle)
		{
            GPIO_SetInactive( alert_state.port, alert_state.pin);
			alert_state.blink_toggle = 0;
		}
		else
		{
            GPIO_SetActive( alert_state.port, alert_state.pin);
			alert_state.blink_toggle = 1;
		}
		
		ke_timer_set(APP_PXP_TIMER, dest_id, alert_state.blink_timeout);
		
    return (KE_MSG_CONSUMED);
}

#endif //BLE_PROX_REPORTER

#endif //(BLE_APP_PRESENT)

/// @} APPTASK
