/**
 ****************************************************************************************
 *
 * @file app_audio.h
 *
 * @brief AudioStreamer Application entry point
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

#ifndef APP_AUDIO_H_
#define APP_AUDIO_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief AudioStreamer Application entry point.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"

#if (BLE_STREAMDATA_DEVICE)

#include <stdint.h>          // standard integer definition
#include <co_bt.h>



/*
 * GLOBAL VARIABLE DECLARATION
 ****************************************************************************************
 */
 
extern uint8_t audio_adv_count;
extern uint16_t audio_adv_interval;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

void app_stream_create_db(void);

/**
 ****************************************************************************************
 * @brief Initialize AudioStreamer Application
 ****************************************************************************************
 */
void app_stream_init(void);

/**
 ****************************************************************************************
 *
 * AudioStreamer Application Functions
 *
 ****************************************************************************************
 */
void app_stream_start(void);
void stream_stop(void);
void stream_start(void);
void app_stream_idle_handler(void);
void app_stream_button_released(void);
void stream_start_button_init(void);
void stream_queue_more_data(uint16_t nr_packets);
void app_stream_enable(void);
/**
 ****************************************************************************************
 * @brief Enable the audioerometer profile
 *
 ****************************************************************************************
 */
void app_audio_enable(void);

/**
 ****************************************************************************************
 * @brief Start Timer to control Advertising interval.
 *
 ****************************************************************************************
 */
void app_audio_adv_started(void);

/**
 ****************************************************************************************
 * @brief Stop Timer that controls Advertising interval.
 *
 ****************************************************************************************
 */
void app_audio_adv_stopped(void);

/**
 ****************************************************************************************
 * @brief There is space for nr_packets more data to be transmitted
 *
 ****************************************************************************************
 */
void audio_queue_more_data(uint16_t nr_packets);



#endif //BLE_STREAMDATA_DEVICE

/// @} APP

#endif // APP_H_
