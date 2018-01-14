 /**
 ****************************************************************************************
 *
 * @file  app_sps_ble.c
 *
 * @brief SPS application.
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
 
#include <stdlib.h>
#include <stdint.h>
#include <string.h>                  				// string manipulation and functions

#include "rwip_config.h"               			// SW configuration

#if (BLE_APP_PRESENT)
#include "app.h"                     				// application definitions
#include "app_spsc.h"
#include "app_sps_scheduler.h"
#include "sps_client_task.h"    // SPS functions

/*
 * LOCAL FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief 			app_sps_init initialize the required functions
 * @param[in] 	void
 * @return 			none
 ****************************************************************************************
 */
void app_sps_init(void)
{
		// SPS application initialisation
		app_buffer_init();
		app_uart_init();
		app_flowcontrol_init();
}


/**
 ****************************************************************************************
 * @brief 		Function used to enable client role
 * @param[in] void
 * @return 		none
 ****************************************************************************************
 */
static void app_sps_client_enable(void)
{
    // Allocate the message
    struct sps_client_enable_req * req = KE_MSG_ALLOC(SPS_CLIENT_ENABLE_REQ, TASK_SPS_CLIENT, TASK_APP,
																				 sps_client_enable_req);
    
    // Fill in the parameter structure
    req->conhdl = app_env.conhdl;
    req->con_type = PRF_CON_DISCOVERY; 
    
    // Send the message
    ke_msg_send(req);	
}


/**
 ****************************************************************************************
 * @brief 			app_sps_enable(void) Sends enable message(s) when the connection is made.
 * @param[in] 	void
 * @return 			none
 ****************************************************************************************
 */
void app_sps_enable(void)
{
    app_sps_client_enable();
}

/**
 ****************************************************************************************
 * @brief 		Send flow control over Bluetooth
 * @param[in] 	flow control 		(UART_XON or UART_XOFF)
 * @return 		none
 ****************************************************************************************
 */
void sendFlowControlOverBluetooth(uint8_t flowcontrol)
{

		struct sps_client_send_notify_flow_control_state_req * req = KE_MSG_ALLOC(SPS_CLIENT_SEND_FLOW_CONTROL_REQ, TASK_SPS_CLIENT, TASK_APP,
																									sps_client_send_notify_flow_control_state_req);

		req->flow_control_state = (uint8_t) flowcontrol;
           
		ke_msg_send(req);
}

/**
 ****************************************************************************************
 * @brief 		Request data transmission over Bluetooth
 * @return 		none
 ****************************************************************************************
 */
void init_ble_tx(void)
{
    struct sps_client_init_ble_tx_req * req = KE_MSG_ALLOC(SPS_CLIENT_INIT_BLE_TX_REQ, TASK_SPS_CLIENT, TASK_APP, sps_client_init_ble_tx_req);

    ke_msg_send(req);
}
#endif // (BLE_APP_PRESENT)

/// @} APP
