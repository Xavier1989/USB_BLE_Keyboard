/**
****************************************************************************************
*
* @file app_streamdatad_task.h
*
* @brief streamdatad task header.
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

#ifndef APP_STREAMDATAD_TASK_H_
#define APP_STREAMDATAD_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Findme Application task handlers.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"
#include "streamdatah_task.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
 /**
 ****************************************************************************************
 * @brief Handle Streamdata Device database creation completion.
 ****************************************************************************************
 */
 
int stream_create_db_cfm_handler(ke_msg_id_t const msgid,
                                      struct streamdatad_create_db_cfm const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);
                
/**
 ****************************************************************************************
 * @brief Handles reception of data packet.
 ****************************************************************************************
 */

int stream_rcv_data_packet_ind_handler(ke_msg_id_t const msgid,
                                      struct streamdatad_rcv_data_packet_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);

/// @} APP

#endif // APP_FINDT_TASK_H_
