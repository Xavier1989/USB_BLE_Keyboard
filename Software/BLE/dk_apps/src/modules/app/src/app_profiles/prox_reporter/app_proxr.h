/**
****************************************************************************************
*
* @file app_proxr.h
*
* @brief Proximity Reporter application.
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

#ifndef APP_PROXR_H_
#define APP_PROXR_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Proximity Reporter Application entry point.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"
#include "gpio.h"

#if BLE_PROX_REPORTER
#include "proxr.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
typedef struct
{
uint32_t blink_timeout;
	
uint8_t blink_toggle;
	
uint8_t lvl;
	
uint8_t ll_alert_lvl;
	
int8_t  txp_lvl;
    
GPIO_PORT port;

GPIO_PIN pin;

}app_alert_state;

extern app_alert_state alert_state;

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
 * @brief Initialize Proximity Application
 ****************************************************************************************
 */
void app_proxr_init(GPIO_PORT port, GPIO_PIN pin);

/**
 ****************************************************************************************
 *
 * Proximity Application Functions
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Enable the proximity profile
 *
 ****************************************************************************************
 */
void app_proxr_enable(void);

/**
 ****************************************************************************************
 * @brief Start proximity Alert
 *
 ****************************************************************************************
 */
void app_proxr_alert_start(uint8_t lvl);

/**
 ****************************************************************************************
 * @brief Stop proximity Alert.
 *
 ****************************************************************************************
 */
void app_proxr_alert_stop(void);


/**
 ****************************************************************************************
 * @brief Read Tx Power Level.
 *
 ****************************************************************************************
 */
void app_proxr_rd_tx_pwr(void);

/**
 ****************************************************************************************
 * @brief Create proximity reporter Database
 *
 ****************************************************************************************
 */

void app_proxr_create_db_send(void);

void app_proxr_port_reinit(GPIO_PORT port, GPIO_PIN pin);



#endif //BLE_PROX_REPORTER

/// @} APP

#endif // APP_H_
