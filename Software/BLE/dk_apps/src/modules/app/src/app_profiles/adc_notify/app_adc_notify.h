/**
****************************************************************************************
*
* @file app_adc_notify.h
*
* @brief sample182 application.
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

#ifndef APP_ADC_NOTIFY_H_
#define APP_ADC_NOTIFY_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief ADC notify Application entry point.
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

#if BLE_ADC_NOTIFY
#include "adc_notify.h"
#include "adc_notify_task.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

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
 *
 * ADC notify Application Functions
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Enable the adc_notify profile
 *
 ****************************************************************************************
 */
void app_adc_notify_enable(void);

/**
 ****************************************************************************************
 * @brief Create ADC notify Database
 *
 ****************************************************************************************
 */

void app_adc_notify_create_db_send(void);


/**
 ****************************************************************************************
 * @brief Update adc_notify characteristic value. 
 *
 * @return void.
 ****************************************************************************************
 */
void app_adc_notify_upd_char(uint16_t val);


#endif //ADC_NOTIFY

/// @} APP

#endif // APP_H_
