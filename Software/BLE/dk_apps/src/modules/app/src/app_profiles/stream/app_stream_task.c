/**
****************************************************************************************
*
* @file app_proxr_task.c
*
* @brief Proximity Reporter application Message Handlers.
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

#include "rwip_config.h"               // SW configuration

#if (BLE_APP_PRESENT)


#include "app_task.h"                  // Application Task API
#include "app.h"                       // Application Definition
#include "gapc_task.h"                 // GAP Controller Task API
#include "gapm_task.h"                 // GAP Manager Task API
#include "gap.h"                       // GAP Definitions
#include "co_error.h"                  // Error Codes Definition
#include "arch.h"                      // Platform Definitions
#include "gpio.h"

#if (BLE_STREAMDATA_DEVICE)
#include "streamdatad_task.h"         // StreamData task functions
#include "app_stream.h" 
#include "prf_types.h"
#include "attm_db.h"
#include "l2cm.h"
#endif //BLE_STREAMDATA_DEVICE

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */


#if (BLE_STREAMDATA_DEVICE)


static inline void systick_stop( void )
{
 // leave systick in a known state
 SetWord32(0xE000E010, 0x00000000); // disable systick
 GetWord32(0xE000E010); // make sure COUNTFLAG is reset
}

static inline void systick_start(const int ticks, const int mode /* = 0*/)
{
 SetWord32(0xE000E010, 0x00000000); // disable systick
 SetWord32(0xE000E014, ticks); // set systick timeout based on 16MHz clock
 SetWord32(0xE000E018, ticks); // set systick timeout based on 16MHz clock
 GetWord32(0xE000E010); // make sure COUNTFLAG is reset
 SetWord32(0xE000E010, 1|/*4|*/mode); // enable systick on 1MHz clock (always ON) with interrupt (mode)
}

/**
 ****************************************************************************************
 * @brief .
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int stream_create_db_cfm_handler(ke_msg_id_t const msgid,
                                      struct streamdatad_create_db_cfm const *param,
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
 * @brief .
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int stream_more_data_handler(ke_msg_id_t const msgid,
                                           struct l2cc_data_send_rsp const *param,
                                           ke_task_id_t const dest_id,
                                           ke_task_id_t const src_id)
{
    stream_queue_more_data( dest_id);
    return (KE_MSG_CONSUMED);

} 


int started = 0;

int stream_start_ind_handler(ke_msg_id_t const msgid,
                                   struct streamdatad_start_ind const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
    // Start the stream Device
    if (1 == streamdatad_env.stream_enabled) stream_start(); // not for bwtest

	started = 1;
    // Start the timer
    systick_start(11250, 2);  // ConnInterval 11.25ms
	
    return (KE_MSG_CONSUMED);
}


void request_more_data_if_possible( void )
{
	if (0 == started) { streamdatad_env.nr_overflow_packets = 0; return; }
	const int nb_buffer_available = l2cm_get_nb_buffer_available();  //by ED TBC
	
	switch (streamdatad_env.stream_enabled)
	{
		default: 
			streamdatad_env.nr_overflow_packets = 0;
			break;
		case 1:
            stream_queue_more_data(nb_buffer_available);           
			break;
	}
}

short app_stream_get_enable(void)
{
	uint16_t *getstreamenable;
	
	// Get the current value from the attribute database
	att_size_t length = sizeof(uint16_t);
	attmdb_att_get_value(STREAMDATAD_HANDLE(STREAMDATAD_IDX_ENABLE_VAL), &length, (uint8_t**) &getstreamenable);
	return *getstreamenable;
}


void app_stream_send_enable(short value)
{
//	if (value) while (!cbuffer_read(&streambuffer, streampacketbuf)) {}

	// Update the value in the attribute database
	attmdb_att_set_value(STREAMDATAD_HANDLE(STREAMDATAD_IDX_ENABLE_VAL), sizeof(uint16_t), (uint8_t*) &value);        
	
	streamdatad_streamonoff();
    
    // Send notification
    //prf_server_send_event((prf_env_struct *)&(streamdatad_env.con_info), false, STREAMDATAD_HANDLE(STREAMDATAD_IDX_ENABLE_VAL));
}

extern int app_stream_check_for_button_release;

void SysTick_Handler(void) 
{ 
    //set_pxact_gpio();
    
    if(app_stream_check_for_button_release && GPIO_GetPinStatus(GPIO_PORT_0, GPIO_PIN_6))
	{
       app_stream_button_released();
	}
	
	if (started)
	{
		request_more_data_if_possible();
	}
}

/**
 ****************************************************************************************
 * @brief Handles Stop indication from the StreamData Device profile.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int stream_stop_ind_handler(ke_msg_id_t const msgid,
                                  void const *param,
                                  ke_task_id_t const dest_id,
                                  ke_task_id_t const src_id)
{
	started = 0;
    // Stop the stream Device
    stream_stop();
	
    systick_stop();	
	
    return (KE_MSG_CONSUMED);
}

#endif //BLE_STREAMDATA_DEVICE
#endif //(BLE_APP_PRESENT)

/// @} APPTASK
