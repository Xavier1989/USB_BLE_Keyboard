/**
****************************************************************************************
*
* @file app_batt_task.c
*
* @brief Battery server application task.
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

#include "rwip_config.h"              

#if (BLE_APP_PRESENT)

#if (BLE_BATT_SERVER)

#include "app_task.h"                  // Application Task API
#include "bass_task.h"                      
#include "app_batt.h"
#include "gpio.h"


extern uint16_t bat_poll_timeout; 
extern uint8_t bat_lvl_alert_used; 
extern GPIO_PORT bat_led_port; 
extern GPIO_PIN bat_led_pin; 


#ifndef BASS_CCC_MASK
#define BASS_CCC_MASK   0
#endif

#ifndef BASS_CCC_POS
#define BASS_CCC_POS    0
#endif


/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Handles the Create DB confirmation message for the Device Information Service.
 *        Set Advertising Mode
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int batt_create_db_cfm_handler(ke_msg_id_t const msgid,
                                      struct bass_create_db_cfm const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    // Inform the Application Manager
    struct app_module_init_cmp_evt *cfm = KE_MSG_ALLOC(APP_MODULE_INIT_CMP_EVT,
                                                           TASK_APP, TASK_APP,
                                                           app_module_init_cmp_evt);

    cfm->status = param->status;

    ke_msg_send(cfm);

    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief Handles the confirmation for the Level update.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int batt_level_upd_cfm_handler(ke_msg_id_t const msgid,
                                      struct bass_batt_level_upd_cfm const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
	return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief Empty function to satisfy the compiler in case an EEPROM is not used.
 ****************************************************************************************
 */
__weak void app_alt_pair_update_bond_data(int mask, int pos, int value)
{
    // Satisfy the compiler. If an EEPROM is used then include a strong definition for this function!
}


/**
 ****************************************************************************************
 * @brief Handles the indication for the modification of the notification status.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] ind       Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int batt_level_ntf_cfg_ind_handler(ke_msg_id_t const msgid,
                                      struct bass_batt_level_ntf_cfg_ind const *ind,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    app_alt_pair_update_bond_data(BASS_CCC_MASK, BASS_CCC_POS, ind->ntf_cfg);

    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief Handles Battery Level polling timer
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_batt_timer_handler(ke_msg_id_t const msgid,
                                   void const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
    app_batt_lvl();
		
	app_timer_set(APP_BATT_TIMER, dest_id, bat_poll_timeout);
		
    return (KE_MSG_CONSUMED);
}

#ifndef CUSTOM_BATTERY_LEVEL_ALERT_LED_HANDLING   
/**
 ****************************************************************************************
 * @brief Handles Battery Alert timer
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_batt_alert_timer_handler(ke_msg_id_t const msgid,
                                        void const *param,
										ke_task_id_t const dest_id,
										ke_task_id_t const src_id)
{
    //Read LED GPIO state
    if(bat_lvl_alert_used)
    {		
		if (bat_led_state)
		{
            GPIO_SetInactive( bat_led_port, bat_led_pin);            	
			bat_led_state = 0;
			app_timer_set(APP_BATT_ALERT_TIMER, dest_id, 20);
		}
		else
		{
            GPIO_SetActive( bat_led_port, bat_led_pin);
			bat_led_state = 1;
			app_timer_set(APP_BATT_ALERT_TIMER, dest_id, 5);
		}
    }
		
    return (KE_MSG_CONSUMED);
}
#endif //CUSTOM_BATTERY_LEVEL_ALERT_LED_HANDLING            



#endif //(BLE_BATT_SERVER)

#endif //(BLE_APP_PRESENT)

/// @} APPTASK
