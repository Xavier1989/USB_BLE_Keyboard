/**
****************************************************************************************
*
* @file app_streamdatah_task.h
*
* @brief streamdatah task header.
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

#ifndef APP_STREAMDATAH_TASK_H_
#define APP_STREAMDATAH_TASK_H_

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
 * @brief Handles enable profile confirmation for streamdata host profile.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int streamdatah_enable_cfm_handler(ke_msg_id_t const msgid,
                                      struct streamdatah_enable_cfm const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);
 
/**
 ****************************************************************************************
 * @brief Handles reception of data packet.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
 
int streamdatah_rcv_data_packet_ind_handler(ke_msg_id_t const msgid,
                                      struct streamdatah_rcv_data_packet_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);

/// @} APP

#endif // APP_FINDT_TASK_H_
