/**
****************************************************************************************
*
* @file app_streamdatah_task.c
*
* @brief Streamdata Host application task.
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
#if BLE_STREAMDATA_HOST
#include "streamdatah_task.h"                  // Application Task API
#include "app_streamdatah_task.h"                  // Application Task API
#include "app_task.h"                  
#include "app_stream_queue.h"                  

uint16_t min_hdl, max_hdl;

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Handles enable profile confirmation for streamdata host profile.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance.
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

int streamdatah_enable_cfm_handler(ke_msg_id_t const msgid,
                                      struct streamdatah_enable_cfm const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{	

    if (param->status != PRF_ERR_OK)
        app_disconnect();
    
    
    min_hdl = param->streamdatah.chars[STREAMDATAH_D0_CHAR].val_hdl; 
    max_hdl = param->streamdatah.chars[STREAMDATAH_D9_CHAR].val_hdl;
    
    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles reception of data packet.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance.
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

int streamdatah_rcv_data_packet_ind_handler(ke_msg_id_t const msgid,
                                      struct streamdatah_rcv_data_packet_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    
#ifdef METRICS           
    add_packets_rx(param->size);
#endif 
    
    return (KE_MSG_CONSUMED);
}

#endif

#endif //(BLE_APP_PRESENT)

/// @} APPTASK
