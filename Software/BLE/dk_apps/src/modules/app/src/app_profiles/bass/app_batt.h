/**
 ****************************************************************************************
 *
 * @file app_batt.h
 *
 * @brief Battery Service Application entry point
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

#ifndef APP_BATT_H_
#define APP_BATT_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Battery Service Application entry point.
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
#include "battery.h"

#if (BLE_BATT_SERVER)

#include <stdint.h>          // standard integer definition
#include <co_bt.h>

extern uint8_t cur_batt_level; 
extern uint8_t batt_alert_en; 
extern uint8_t bat_led_state;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize Battery Service Application
 ****************************************************************************************
 */
void app_batt_init(void);

/**
 ****************************************************************************************
 *
 * Battery Service Application Functions
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Enable the battery service profile
 *
 ****************************************************************************************
 */
void app_batt_enable_prf_sec(uint8_t sec_lvl, uint8_t batt_lvl, uint8_t lvl_alert_used, GPIO_PORT led_port, GPIO_PIN led_pin);
void app_batt_enable(uint8_t batt_lvl, uint8_t lvl_alert_used, GPIO_PORT led_port, GPIO_PIN led_pin);

/**
 ****************************************************************************************
 * @brief Enable the battery service profile
 *
 ****************************************************************************************
 */
void app_batt_create_db(void);
 
/**
 ****************************************************************************************
 * @brief Reads devices current battery level and sends battery level update request to Battery Service.
 *
 * @return void
 ****************************************************************************************
 */ 
void app_batt_lvl(void);

/**
 ****************************************************************************************
 * @brief Sends battery level update request to Battery Service.
 *
 * @return void.
 ****************************************************************************************
 */
void app_batt_set_level(uint8_t batt_level);

/**
 ****************************************************************************************
 * @brief Starts battery level polling.
 *
 * @return void.
 ****************************************************************************************
 */
 
void app_batt_poll_start(uint16_t poll_timeout);

/**
 ****************************************************************************************
 * @brief Stops battery level polling.
 *
 * @return void.
 ****************************************************************************************
 */
void app_batt_poll_stop(void);

/**
 ****************************************************************************************
 * @brief Starts battery alert. Battery Low.
 *
 * @return void.
 ****************************************************************************************
*/
void app_batt_alert_start(void);

/**
 ****************************************************************************************
 * @brief Stops battery alert. Battery Low.
 *
 * @return void.
 ****************************************************************************************
 */
void app_batt_alert_stop(void);

#endif //BLE_BATT_SERVER

/// @} APP

#endif // APP_BATT_H
