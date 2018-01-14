/**
 ****************************************************************************************
 *
 * @file app_kbd_leds.h
 *
 * @brief HID Keyboard LEDs header file.
 *
 * Copyright (C) 2014. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

#ifndef APP_CUSTOM_KBD_LEDS_H_
#define APP_CUSTOM_KBD_LEDS_H_

#include "ke_task.h"        // kernel task
#include "ke_msg.h"         // kernel message

enum HID_LED_ST {
    LED_OFF = 0,
    BLINK_LED_IS_ON__TURN_OFF,
    BLINK_LED_IS_OFF__TURN_ON,
    DOUBLE_BLINK_LED_IS_ON__TURN_OFF_A,
    DOUBLE_BLINK_LED_IS_OFF__TURN_ON_B,
    DOUBLE_BLINK_LED_IS_ON__TURN_OFF_B,
    LED_ON,
};

extern enum HID_LED_ST green_led_st;
extern enum HID_LED_ST red_led_st;

#define BLINK_GREEN_ON              10      // 100ms
#define BLINK_GREEN_OFF             40      // 400ms
#define GREEN_ON                   150      // 1.5s

#define BLINK_RED_ON                10      // 100ms
#define BLINK_RED_OFF               40      // 400ms
#define DOUBLE_BLINK_RED_ON_A       20      // 200ms
#define DOUBLE_BLINK_RED_OFF_A      10      // 100ms
#define DOUBLE_BLINK_RED_ON_B       20      // 200ms
#define DOUBLE_BLINK_RED_OFF_B      100     // 1s
#define RED_ON                      150     // 1.5s


/**
 ****************************************************************************************
 * @brief Initializes the LEDs
 *
 * @param   None    
 *
 * @return  void
 ****************************************************************************************
 */
void leds_init(void);

/**
 ****************************************************************************************
 * @brief  Sets-up the LEDS to indicate disconnection
 *
 * @param   None    
 *
 * @return  void
 ****************************************************************************************
 */
void leds_set_disconnected(void);

/**
 ****************************************************************************************
 * @brief  Sets-up the LEDS to indicate connection in progress status
 *
 * @param   None    
 *
 * @return  void
 ****************************************************************************************
 */
void leds_set_connection_in_progress(void);

/**
 ****************************************************************************************
 * @brief  Sets-up the LEDS to indicate connection astablished status
 *
 * @param   None    
 *
 * @return  void
 ****************************************************************************************
 */
void leds_set_connection_established(void);

/**
 ****************************************************************************************
 * @brief  Sets-up the Red LED to indicate battery low level warning
 *
 * @param   None    
 *
 * @return  void
 ****************************************************************************************
 */
void leds_set_battery_low_warning(void);

/**
 ****************************************************************************************
 * @brief  Stops the battery low level warning indication on the Red LED
 *
 * @param   None    
 *
 * @return  void
 ****************************************************************************************
 */
void leds_clear_battery_low_warning(void);

/**
 ****************************************************************************************
 * @brief  Handler of the Green LED Timer
 *
 * @param[in]   msgid
 * @param[in]   param 
 * @param[in]   dest_id
 * @param[in]   src_id 
 *
 * @return  KE_MSG_CONSUMED
 ****************************************************************************************
 */
int app_green_led_timer_handler(ke_msg_id_t const msgid,
                                   void const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id);
                                   
/**
 ****************************************************************************************
 * @brief  Handler of the Red LED Timer
 *
 * @param[in]   msgid
 * @param[in]   param 
 * @param[in]   dest_id
 * @param[in]   src_id 
 *
 * @return  KE_MSG_CONSUMED
 ****************************************************************************************
 */
int app_red_led_timer_handler(ke_msg_id_t const msgid,
                                   void const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id);

#endif // APP_CUSTOM_KBD_LEDS_H_
