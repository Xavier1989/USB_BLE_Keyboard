/**
 ****************************************************************************************
 *
 * @file    arch_sleep.c
 *
 * @brief   Sleep control functions.
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

#include "arch.h"
#include "arch_sleep.h"
#include "app.h"
#include "rwip.h"

/// Application Environment Structure
extern struct arch_sleep_env_tag    sleep_env;    // __attribute__((section("retention_mem_area0")));
uint8_t sleep_md                    __attribute__((section("retention_mem_area0"), zero_init));
uint8_t sleep_pend                  __attribute__((section("retention_mem_area0"), zero_init));
uint8_t sleep_cnt                   __attribute__((section("retention_mem_area0"), zero_init));
bool sleep_ext_force                __attribute__((section("retention_mem_area0"), zero_init));

extern uint8_t rwble_last_event;


/**
 * @brief       Disable all sleep modes. 
 *
 * @param       void
 *
 * @details     Disable all sleep modes. The system operates in active / idle modes only.
 *
 * @return      void
 *
 */
void app_disable_sleep(void)
{
    if (sleep_md == 0)
    {
        sleep_env.slp_state = ARCH_SLEEP_OFF;
#if (DEEP_SLEEP)
        rwip_env.sleep_enable = false;
#endif
    }
    else
        sleep_pend = 0x80 | 0x00;
}


/**
 * @brief       Activates extended sleep mode.
 *
 * @param       void
 *
 * @details     Activates the extended sleep mode. The system operates in idle / active / extended sleep modes.
 *
 * @return      void
 *
 */
void app_set_extended_sleep(void)
{
    if (sleep_md == 0)
    {
        sleep_env.slp_state = ARCH_EXT_SLEEP_ON;
#if (DEEP_SLEEP)
        rwip_env.sleep_enable = true;
#endif
    }
    else
        sleep_pend = 0x80 | 0x01;
}


/**
 * @brief       Activates deep sleep mode.
 *
 * @param       void
 *
 * @details     Activates the deep sleep mode. The system operates in idle / active / deep sleep modes.
 *
 * @return      void
 *
 */
void app_set_deep_sleep(void)
{
    if (sleep_md == 0)
    {
        sleep_env.slp_state = ARCH_DEEP_SLEEP_ON;
#if (DEEP_SLEEP)
        rwip_env.sleep_enable = true;
#endif
    }
    else
        sleep_pend = 0x80 | 0x02;
}


#if 0
/**
 * @brief       Get the current sleep mode of operation.
 *
 * @param       void
 *
 * @return      The current sleep mode of operation. 
 *
 * @retval      The sleep mode.
 *              <ul>
 *                  <li> 0 if sleep is disabled
 *                  <li> 1 if extended sleep is enabled
 *                  <li> 2 if deep sleep is enabled
 *              </ul>
 *
 */
uint8_t app_get_sleep_mode(void)
{
	uint8_t ret = 0;

	switch(sleep_env.slp_state)
	{
		case ARCH_SLEEP_OFF: 
            ret = 0; break;
		case ARCH_EXT_SLEEP_ON:
            ret = 1; break;
		case ARCH_DEEP_SLEEP_ON: 
            ret = 2; break;
	}

	return ret;
}
#endif


/**
 * @brief       Restore the sleep mode to what it was before disabling.
 *
 * @param       void
 *
 * @details     Restores the sleep mode. App should not modify the sleep mode directly.
 *
 * @return      void
 *
 */
void app_restore_sleep_mode(void)
{
    uint8_t cur_mode;
    
    if (sleep_cnt > 0)
        sleep_cnt--;
    
    if (sleep_cnt > 0)
        return;     // cannot restore it yet. someone else has requested active mode and we'll wait him to release it.
        
    if (sleep_pend != 0)
    {
        sleep_md = sleep_pend & 0x3;
        sleep_pend = 0;
    }
    else if (sleep_md)
        sleep_md--;
    
    cur_mode = sleep_md;
    sleep_md = 0;
    
    switch(cur_mode) 
    {
       case 0:  break;
       case 1:  app_set_extended_sleep(); break;
       case 2:  app_set_deep_sleep(); break;
       default: break;
    }
}


/**
 * @brief       Disable sleep but save the sleep mode status.
 *
 * @param       void
 *
 * @details     Disable sleep. Store the sleep mode used by the app.
 *
 * @return      void
 *
 */
void app_force_active_mode(void)
{
    uint8_t cur_mode;
    
    sleep_cnt++;
    
    if (sleep_md == 0)  // add this check for safety! If it's called again before restore happens then sleep_md won't get overwritten
    {
        cur_mode = app_get_sleep_mode();
        cur_mode++;     // 1: disabled, 2: extended, 3: deep sleep (!=0 ==> sleep is in forced state)
        app_disable_sleep();
        sleep_md = cur_mode;
    }
}


/**
 * @brief       Puts the BLE core to permanent sleep. Only an external event can wake it up.
 *
 * @param       void
 *
 * @details     BLE sleeps forever waiting a forced wakeup. After waking up from an external 
 *              event, if the system has to wake BLE up it needs to restore the default mode 
 *              of operation by calling app_ble_ext_wakeup_off() or the BLE won't be able to
 *              wake up in order to serve BLE events!
 *
 * @return      void
 *
 * @todo        Check why an external request wakes up the BLE core, even when 
 *              rwip_env.ext_wakeup_enable is 0.
 *
 * @exception   Warning Assertion
 *              if rwip_env.ext_wakeup_enable is 0 since it wouldn't be possible to wake-up the 
 *              BLE core in this case.
 *
 */
void app_ble_ext_wakeup_on(void)
{
//    ASSERT_WARNING(rwip_env.ext_wakeup_enable != 0);
    sleep_ext_force = true;
}


/**
 * @brief       Takes the BLE core out of the permanent sleep mode.
 *
 * @param       void
 *
 * @details     Restore BLE cores' operation to default mode. The BLE core will wake up every 
 *              10sec even if no BLE events are scheduled. If an event is to be scheduled 
 *              earlier, then BLE will wake up sooner to serve it.
 *
 * @return      void
 *
 */
void app_ble_ext_wakeup_off(void)
{
    sleep_ext_force = false;
}


/**
 * @brief       Checks whether the BLE core is in permanent sleep mode or not.
 *
 * @param       void
 *
 * @details     Returns the current mode of operation of the BLE core (external wakeup or 
 *              default).
 *
 * @return      The BLE core's mode of operation (extended wakeup or normal).
 *
 * @retval      The BLE core's sleep mode.
 *              <ul>
 *                  <li> false, if default mode is selected
 *                  <li> true, if BLE sleeps forever waiting for a forced wakeup
 *              </ul>
 *
 */
bool app_ble_ext_wakeup_get(void)
{
    return sleep_ext_force;
}


/**
 * @brief       Wake the BLE core via an external request.
 *
 * @param       void
 *
 * @details     If the BLE core is sleeping (permanently or not and external wake-up is enabled)
 *              then this function wakes it up. 
 *
 * @attention   A call to app_ble_ext_wakeup_off() should follow if the BLE core was in 
 *              permanent sleep.
 *
 * @return      status
 *
 * @retval      The status of the requested operation.
 *              <ul>
 *                  <li> false, if the BLE core is not sleeping
 *                  <li> true, if the BLE core was woken-up successfully
 *              </ul>
 *
 */
bool app_ble_force_wakeup(void)
{
    bool retval = false;
    
    // If BLE is sleeping, wake it up!
    GLOBAL_INT_DISABLE();
    if (GetBits16(CLK_RADIO_REG, BLE_ENABLE) == 0) { // BLE clock is off
        SetBits16(GP_CONTROL_REG, BLE_WAKEUP_REQ, 1);
        retval = true;
    }
    GLOBAL_INT_RESTORE();
    
    return retval;
}


/**
 * @brief       Used for application's tasks synchronisation with ble events. 
 *
 * @param       void
 *
 * @return      rwble_last_event
 *
 */
uint8_t app_last_rwble_evt_get(void)
{
    return rwble_last_event;
}
