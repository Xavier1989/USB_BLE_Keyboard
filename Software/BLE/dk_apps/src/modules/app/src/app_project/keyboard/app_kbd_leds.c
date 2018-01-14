/**
 ****************************************************************************************
 *
 * @file app_kbd_leds.c
 *
 * @brief HID Keyboard LED functionality.
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
#include "rwip_config.h"
#include "app.h"
#include "app_task.h"
#include "app_console.h"
#include "arch_sleep.h"
#include "gpio.h"

#include "app_kbd.h"
#include "app_kbd_proj.h"
#include "app_kbd_key_matrix.h"
#include "app_kbd_leds.h"

#define __RETAINED __attribute__((section("retention_mem_area0"), zero_init))
enum HID_LED_ST green_led_st __RETAINED;
enum HID_LED_ST red_led_st __RETAINED;
bool high_priority_indications_active __RETAINED;

#define DBG_USES_GREEN_LED_PIN  ( (DEVELOPMENT_DEBUG == 1) \
                               && (((KBD_GREEN_LED_PORT == GPIO_PORT_1) && (KBD_GREEN_LED_PIN == GPIO_PIN_4)) \
                               ||  ((KBD_GREEN_LED_PORT == GPIO_PORT_1) && (KBD_GREEN_LED_PIN == GPIO_PIN_5))) )
#if ( (DEVELOPMENT_DEBUG == 0) \
   && (((KBD_GREEN_LED_PORT == GPIO_PORT_1) && (KBD_GREEN_LED_PIN == GPIO_PIN_4)) \
   ||  ((KBD_GREEN_LED_PORT == GPIO_PORT_1) && (KBD_GREEN_LED_PIN == GPIO_PIN_5))) )
#warning "Green LED uses a JTAG port pin. Make sure that debugger is not used or is turned off before the LEDs are initialized."
#endif

#define DBG_USES_RED_LED_PIN    ( (DEVELOPMENT_DEBUG == 1) \
                               && (((KBD_RED_LED_PORT == GPIO_PORT_1) && (KBD_RED_LED_PIN == GPIO_PIN_4)) \
                               ||  ((KBD_RED_LED_PORT == GPIO_PORT_1) && (KBD_RED_LED_PIN == GPIO_PIN_5))) )
#if ( (DEVELOPMENT_DEBUG == 0) \
   && (((KBD_RED_LED_PORT == GPIO_PORT_1) && (KBD_RED_LED_PIN == GPIO_PIN_4)) \
   ||  ((KBD_RED_LED_PORT == GPIO_PORT_1) && (KBD_RED_LED_PIN == GPIO_PIN_5))) )
#warning "Red LED uses a JTAG port pin. Make sure that debugger is not used or is turned off before the LEDs are initialized."
#endif

#define DBG_USES_LED_PINS       (DBG_USES_GREEN_LED_PIN || DBG_USES_RED_LED_PIN)


/**
 ****************************************************************************************
 * @brief Sets the Green LED to off
 *
 * @param   None    
 *
 * @return  void
 ****************************************************************************************
 */
static void green_led_off(void)
{
    if (HAS_KEYBOARD_LEDS)
    {
        if (LED_STATE_OFF == true)
        {
            GPIO_SetActive(KBD_GREEN_LED_PORT, KBD_GREEN_LED_PIN);      // green: high - off
        }
        else
        {
            GPIO_SetInactive(KBD_GREEN_LED_PORT, KBD_GREEN_LED_PIN);    // green: low - off
        }
        green_led_st = LED_OFF;
    }
}


/**
 ****************************************************************************************
 * @brief Sets the Green LED to on
 *
 * @param   None    
 *
 * @return  void
 ****************************************************************************************
 */
static void green_led_on(void)
{
    if (HAS_KEYBOARD_LEDS)
    {
        if (LED_STATE_OFF == true)
        {
            GPIO_SetInactive(KBD_GREEN_LED_PORT, KBD_GREEN_LED_PIN);    // green: low - on
        }
        else
        {
            GPIO_SetActive(KBD_GREEN_LED_PORT, KBD_GREEN_LED_PIN);      // green: high - on
        }
        green_led_st = LED_ON;
    }
}


/**
 ****************************************************************************************
 * @brief Sets the Green LED to blink
 *
 * @param   None    
 *
 * @return  void
 ****************************************************************************************
 */
static void green_led_blink(void)
{
    if (HAS_KEYBOARD_LEDS)
    {
        if (LED_STATE_OFF == true)
        {
            GPIO_SetInactive(KBD_GREEN_LED_PORT, KBD_GREEN_LED_PIN);    // green: low - on
        }
        else
        {
            GPIO_SetActive(KBD_GREEN_LED_PORT, KBD_GREEN_LED_PIN);      // green: high - on
        }
        green_led_st = BLINK_LED_IS_ON__TURN_OFF;
    }
}


/**
 ****************************************************************************************
 * @brief Sets the Red LED to off
 *
 * @param   None    
 *
 * @return  void
 ****************************************************************************************
 */
static void red_led_off(void)
{
    high_priority_indications_active = false;
    if (HAS_KEYBOARD_LEDS)
    {
        if (LED_STATE_OFF == true)
        {
            GPIO_SetActive(KBD_RED_LED_PORT, KBD_RED_LED_PIN);          // red: high - off
        }
        else
        {
            GPIO_SetInactive(KBD_RED_LED_PORT, KBD_RED_LED_PIN);        // red: low - off
        }
        red_led_st = LED_OFF;
    }
}


/**
 ****************************************************************************************
 * @brief Sets the Red LED to on
 *
 * @param   None    
 *
 * @return  void
 ****************************************************************************************
 */
static void red_led_on(void)
{
    if (HAS_KEYBOARD_LEDS)
    {
        if (LED_STATE_OFF == true)
        {
            GPIO_SetInactive(KBD_RED_LED_PORT, KBD_RED_LED_PIN);        // red: low - on
        }
        else
        {
            GPIO_SetActive(KBD_RED_LED_PORT, KBD_RED_LED_PIN);          // red: high - on
        }
        red_led_st = LED_ON;
    }
}


/**
 ****************************************************************************************
 * @brief Sets the Red LED to blink
 *
 * @param   None    
 *
 * @return  void
 ****************************************************************************************
 */
void red_led_blink(void)
{
    if (HAS_KEYBOARD_LEDS)
    {
        if (LED_STATE_OFF == true)
        {
            GPIO_SetInactive(KBD_RED_LED_PORT, KBD_RED_LED_PIN);    // red: low - on
        }
        else
        {
            GPIO_SetActive(KBD_RED_LED_PORT, KBD_RED_LED_PIN);      // red: high - on
        }
    }
}


/**
 ****************************************************************************************
 * @brief Initializes the LEDs
 *
 * @param   None    
 *
 * @return  void
 ****************************************************************************************
 */
void leds_init(void)
{
    if (HAS_KEYBOARD_LEDS)
    {
        if ( (GetWord16(SYS_STAT_REG) & DBG_IS_UP) && DBG_USES_LED_PINS )
        {
            //skip
        }
        else
        {
            GPIO_ConfigurePin(KBD_GREEN_LED_PORT, KBD_GREEN_LED_PIN, OUTPUT, PID_GPIO, LED_STATE_OFF);   // green: off
            green_led_st = LED_OFF;	
            GPIO_ConfigurePin(KBD_RED_LED_PORT, KBD_RED_LED_PIN, OUTPUT, PID_GPIO, LED_STATE_OFF);       // red: off
            red_led_st = LED_OFF;	
        }
    }
}


/**
 ****************************************************************************************
 * @brief Block sleep when a LED is up in BOOST mode for the period a LED is on.
 *
 * @param   None    
 *
 * @return  void
 ****************************************************************************************
 */
void leds_block_sleep(void)
{
    if (GetBits16(ANA_STATUS_REG, BOOST_SELECTED) == 0x1)           // needed in BOOST mode only!
    {
        if (app_get_sleep_mode())
            app_force_active_mode();                                // prevent sleep only if enabled
    }
}


/**
 ****************************************************************************************
 * @brief  Sets-up the Red LED to indicate battery low level warning
 *
 * @param   None    
 *
 * @return  void
 ****************************************************************************************
 */
void leds_set_battery_low_warning(void)
{
    if (HAS_KEYBOARD_LEDS)
    {
        if ( (high_priority_indications_active) || (red_led_st != LED_OFF) )
            return;
        
        if ( (GetWord16(SYS_STAT_REG) & DBG_IS_UP) && DBG_USES_LED_PINS )
        {
            //skip
        }
        else
        {
            red_led_blink();
            red_led_st = DOUBLE_BLINK_LED_IS_ON__TURN_OFF_A;
            app_timer_set(APP_RED_LED_TIMER, TASK_APP, DOUBLE_BLINK_RED_ON_A);
            leds_block_sleep();
        }
    }
}


/**
 ****************************************************************************************
 * @brief  Stops the battery low level warning indication on the Red LED
 *
 * @param   None    
 *
 * @return  void
 ****************************************************************************************
 */
void leds_clear_battery_low_warning(void)
{
    if (HAS_KEYBOARD_LEDS)
    {
		// if high priority indication is active
		// the LED status is left unchanged
		// the LED is left to be handled by the
		// high priority indications handler
        if (high_priority_indications_active)
            return;
        red_led_off();
    }
}


/**
 ****************************************************************************************
 * @brief  Sets-up the LEDS to indicate disconnection
 *
 * @param   None    
 *
 * @return  void
 ****************************************************************************************
 */
void leds_set_disconnected(void)
{
    high_priority_indications_active = true;
    if (HAS_KEYBOARD_LEDS)
    {
        if ( (GetWord16(SYS_STAT_REG) & DBG_IS_UP) && DBG_USES_LED_PINS )
        {
            //skip
        }
        else
        {
            green_led_off();
            red_led_on();
            app_timer_set(APP_RED_LED_TIMER, TASK_APP, RED_ON);
            leds_block_sleep();
        }
    }
}


/**
 ****************************************************************************************
 * @brief  Sets-up the LEDS to indicate connection in progress status
 *
 * @param   None    
 *
 * @return  void
 ****************************************************************************************
 */
void leds_set_connection_in_progress(void)
{
    if ((GetWord16(SYS_STAT_REG) & DBG_IS_UP) && DBG_USES_LED_PINS) 
    {
        //skip
    }
    else
    {
        red_led_off();
        green_led_blink();
        app_timer_set(APP_GREEN_LED_TIMER, TASK_APP, BLINK_GREEN_ON);
        leds_block_sleep();
    }
}

/**
 ****************************************************************************************
 * @brief  Sets-up the LEDS to indicate connection astablished status
 *
 * @param   None    
 *
 * @return  void
 ****************************************************************************************
 */
void leds_set_connection_established(void)
{
    if ((GetWord16(SYS_STAT_REG) & DBG_IS_UP) && DBG_USES_LED_PINS) 
    {
        //skip
    }
    else
    {
        red_led_off();
        green_led_on();
        app_timer_set(APP_GREEN_LED_TIMER, TASK_APP, GREEN_ON);
        leds_block_sleep();
    }
}


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
                           ke_task_id_t const src_id)
{
    if (HAS_KEYBOARD_LEDS)
    {
        do {
            if (DBG_USES_LED_PINS && ((GetWord16(SYS_STAT_REG) & DBG_IS_UP) == DBG_IS_UP))
                break; // GPIOs are being used by the debugger
                
            switch(green_led_st)
            {
                case LED_OFF:
                    green_led_off();
                    break;
                
                case BLINK_LED_IS_ON__TURN_OFF:
                    GPIO_SetActive(KBD_GREEN_LED_PORT, KBD_GREEN_LED_PIN);              // high - off
                    app_timer_set(APP_GREEN_LED_TIMER, TASK_APP, BLINK_GREEN_OFF);
                    green_led_st = BLINK_LED_IS_OFF__TURN_ON;
                    if ((GetBits16(ANA_STATUS_REG, BOOST_SELECTED) == 0x1) && ((red_led_st == LED_OFF) || (red_led_st == BLINK_LED_IS_OFF__TURN_ON)))
                        app_restore_sleep_mode();                                       // restore sleep
                    break;
                    
                case BLINK_LED_IS_OFF__TURN_ON:
                    green_led_blink();
                    app_timer_set(APP_GREEN_LED_TIMER, TASK_APP, BLINK_GREEN_ON);
                    leds_block_sleep();
                    break;
                
                case LED_ON:
                    green_led_off();
                    if ((GetBits16(ANA_STATUS_REG, BOOST_SELECTED) == 0x1) && ((red_led_st == LED_OFF) || (red_led_st == BLINK_LED_IS_OFF__TURN_ON)))
                        app_restore_sleep_mode();                                       // restore sleep
                    break;
                
                default:
                    break;
            }
        } while(0);
    }
	
	return (KE_MSG_CONSUMED);
}


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
                           ke_task_id_t const src_id)
{
    if (HAS_KEYBOARD_LEDS)
    {
        do {
            if (DBG_USES_LED_PINS && ((GetWord16(SYS_STAT_REG) & DBG_IS_UP) == DBG_IS_UP))
                break;  // GPIOs are being used by the debugger
                
            switch(red_led_st)
            {
                case LED_OFF:
                    red_led_off();
                    break;
                
                
                case DOUBLE_BLINK_LED_IS_ON__TURN_OFF_A:
                    red_led_off();
                    app_timer_set(APP_RED_LED_TIMER, TASK_APP, DOUBLE_BLINK_RED_OFF_A);
                    red_led_st = DOUBLE_BLINK_LED_IS_OFF__TURN_ON_B;
                    if ((GetBits16(ANA_STATUS_REG, BOOST_SELECTED) == 0x1) && ((green_led_st == LED_OFF) || (green_led_st == BLINK_LED_IS_OFF__TURN_ON)))
                        app_restore_sleep_mode(); 
                    break;
                    
                case DOUBLE_BLINK_LED_IS_OFF__TURN_ON_B:
                    red_led_blink();
                    app_timer_set(APP_RED_LED_TIMER, TASK_APP, DOUBLE_BLINK_RED_ON_B);
                    red_led_st = DOUBLE_BLINK_LED_IS_ON__TURN_OFF_B;
                    leds_block_sleep();
                    break;                   

                case DOUBLE_BLINK_LED_IS_ON__TURN_OFF_B:
                    red_led_off();
                    app_timer_set(APP_RED_LED_TIMER, TASK_APP, DOUBLE_BLINK_RED_OFF_B);
                    red_led_st = LED_OFF;
                    if ((GetBits16(ANA_STATUS_REG, BOOST_SELECTED) == 0x1) && ((green_led_st == LED_OFF) || (green_led_st == BLINK_LED_IS_OFF__TURN_ON)))
                        app_restore_sleep_mode();  
                    break;                        

                case BLINK_LED_IS_ON__TURN_OFF:
                    red_led_off();
                    app_timer_set(APP_RED_LED_TIMER, TASK_APP, BLINK_RED_OFF);
                    red_led_st = BLINK_LED_IS_OFF__TURN_ON;
                    if ((GetBits16(ANA_STATUS_REG, BOOST_SELECTED) == 0x1) && ((green_led_st == LED_OFF) || (green_led_st == BLINK_LED_IS_OFF__TURN_ON)))
                        app_restore_sleep_mode();                                       // restore sleep
                    break;
                    
                case BLINK_LED_IS_OFF__TURN_ON:
                    red_led_blink();
                    app_timer_set(APP_RED_LED_TIMER, TASK_APP, BLINK_RED_ON);
                    leds_block_sleep();
                    break;
                
                case LED_ON:
                    red_led_off();
                    if ((GetBits16(ANA_STATUS_REG, BOOST_SELECTED) == 0x1) && ((green_led_st == LED_OFF) || (green_led_st == BLINK_LED_IS_OFF__TURN_ON)))
                        app_restore_sleep_mode();                                       // restore sleep
                    high_priority_indications_active = false;
                    break;
                
                default:
                    break;
            }
        } while(0);
    }
		
	return (KE_MSG_CONSUMED);
}


