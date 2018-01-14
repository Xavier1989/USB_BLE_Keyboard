/*
 * Copyright (C) 2013 Dialog Semiconductor GmbH and its Affiliates, unpublished work
 * This computer program includes Confidential, Proprietary Information and is a Trade Secret 
 * of Dialog Semiconductor GmbH and its Affiliates. All use, disclosure, and/or 
 * reproduction is prohibited unless authorized in writing. All Rights Reserved.
 */

#include "arch.h"
#include "app.h"
#include "gpio.h"
#include "ke_event.h"

#if BLE_HID_DEVICE 
#include "rwip_config.h"
#include "rwip.h"
#include "co_buf.h"
#include "app_keyboard.h"

#if BLE_ALT_PAIR
#include "app_alt_pair.h"
#endif // BLE_ALT_PAIR

#if KEYBOARD_MEASURE_EXT_SLP   
#include "ke.h"
#endif // KEYBOARD_MEASURE_EXT_SLP

extern struct co_buf_env_tag co_buf_env;
#endif // BLE_HID_DEVICE

#if BLE_STREAMDATA_DEVICE
#include "app_stream.h"
#endif
#if BLE_PROX_REPORTER
#include "app_button_led.h"
#include "wkupct_driver.h"
#endif

/*
 ********************************* Hooks ************************************
 */

/*
 * Name         : app_asynch_trm - Hook #1
 *
 * Scope        : PUBLIC
 *
 * Arguments    : none
 *
 * Description  : See documentation.
 *
 * Returns      : true to force calling of schedule(), else false
 *
 */
static inline bool app_asynch_trm(void)
{
	bool ret = false;

	do {
#if BLE_HID_DEVICE 
        // Synchronize with the BLE here! The time window of requesting a packet trm at the upcoming
        // anchor point is from the CSCNT event until the FINEGTIM event. If you pass the FINEGTIM
        // event then the packet will be sent at the next anchor point!
        //
        // Note that this synchronization is only possible in sleep modes!
        if ( !ke_event_get(KE_EVENT_KE_MESSAGE) ) {
            // Since pkt reqs can be silently discarded if no Tx bufs are available, check first!
            if (kbd_trm_list && check_connection_status() && !co_list_is_empty(&co_buf_env.tx_free)) {
                if (app_kbd_send_key_report()) {
                    ret = true;
                    break;
                }
            }
        }
        
        if (user_disconnection_req) 
            if (!(app_env.app_flags & KBD_SCANNING)) {
                // clear trm queue if anything is left - FIXME
#if (BLE_ALT_PAIR)
                app_alt_pair_disconnect();
#endif                    
                user_disconnection_req = false;
            }
#endif
	} while(0);

	return ret;
}

/*
 * Name         : app_asynch_proc - Hook #2
 *
 * Scope        : PUBLIC
 *
 * Arguments    : none
 *
 * Description  : See documentation.
 *
 * Returns      : true to force calling of schedule(), else false
 *
 */
static inline bool app_asynch_proc(void)
{
	bool ret = false;

	do {
#if BLE_HID_DEVICE 
		if (app_env.app_flags & KBD_START_SCAN) {
			app_env.app_flags &= ~KBD_START_SCAN;
			app_kbd_start_scanning();
		}
		
		if (app_env.app_flags & KBD_SCANNING) {
			if (kbd_do_scan_param) 
				app_kbd_do_scan();
		}
		
        if (kbd_trm_list) {
            // If BLE is sleeping, wake it up!
            GLOBAL_INT_DISABLE();
            if (GetBits16(CLK_RADIO_REG, BLE_ENABLE) == 0) { // BLE clock is off
                SetBits16(GP_CONTROL_REG, BLE_WAKEUP_REQ, 1);
            } else
                ret = true;
            GLOBAL_INT_RESTORE();
        }
#endif
		
#if BLE_STREAMDATA_DEVICE
//		request_more_data_if_possible();

		app_stream_idle_handler();
#endif
        
#if BLE_PROX_REPORTER || BLE_FINDME_LOCATOR
        if (GPIO_GetPinStatus( GPIO_PORT_0, GPIO_PIN_6 ))
            wkupct_enable_irq(0x40, 0x40, 1, 0); // P0_6, polarity low , 1 event, debouncing time = 0
#endif
 	} while(0);

	return ret;
}

/*
 * Name         : app_asynch_sleep_proc - Hook #3
 *
 * Scope        : PUBLIC
 *
 * Arguments    : none
 *
 * Description  : See documentation.
 *
 * Returns      : none
 *
 */
static inline void app_asynch_sleep_proc(void)
{
#if BLE_HID_DEVICE 
    if (app_env.app_flags & KBD_START_SCAN) { // make sure we do not miss any wake up interrupts!
        app_env.app_flags &= ~KBD_START_SCAN;
        app_kbd_start_scanning();
    }
   
#if KEYBOARD_MEASURE_EXT_SLP   
    if ( (user_extended_sleep) && !(app_env.app_flags & KBD_SCANNING)) {
        // see if we can enter into sleep
        if (!ke_sleep_check())
            return;

        if (GetBits16(SYS_STAT_REG, RAD_IS_DOWN))
            return; // already sleeping, will wake up eventually. wait until this happens.
            
        if (rwip_prevent_sleep_get() != 0)
            return;
            
        rwip_rf.sleep();
		while ( !(GetWord32(BLE_CNTL2_REG) & RADIO_PWRDN_ALLOW) ) {};
        SetBits16(CLK_RADIO_REG, BLE_ENABLE, 0);
        SetBits16(PMU_CTRL_REG, RADIO_SLEEP, 1);        // turn off radio
        
        SCB->SCR |= 1<<2;
        SetBits16(SYS_CTRL_REG, RET_SYSRAM, 1);         // retain System RAM
        SetBits16(SYS_CTRL_REG, OTP_COPY, 0);           // disable OTP copy
        SetBits16(SYS_CTRL_REG, PAD_LATCH_EN, 0);       // activate PAD latches
        SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 1);       // turn off peripheral power domain
        
#if !(ES4_CODE)                
        if (GetBits16(ANA_STATUS_REG, BOOST_SELECTED) == 0x1) { // Boost-mode
            SetBits16(DCDC_CTRL2_REG, DCDC_CUR_LIM, 0xF);   // 150mA (will be lower due to auto-cal)
            SetBits16(DCDC_CTRL2_REG, DCDC_AUTO_CAL, 0x7);  // ON during active = OFF during deep-sleep
        } else { // Buck-mode
            SetWord16(DCDC_CAL1_REG, GetWord16(DCDC_CAL1_REG)); // Keep using the calibrated value
            SetBits16(DCDC_CTRL2_REG, DCDC_AUTO_CAL, 0x0);  // OFF during active and deep-sleep
        }
#endif

#if (ES4_CODE)
        SetBits16(CLK_16M_REG, XTAL16_BIAS_SH_DISABLE, 0);
#endif    

        WFI();
    }
#endif // KEYBOARD_MEASURE_EXT_SLP    
#endif // BLE_HID_DEVICE
}

/*
 * Name         : app_sleep_prepare_proc - Hook #4
 *
 * Scope        : PUBLIC
 *
 * Arguments    : none
 *
 * Description  : See documentation.
 *
 * Returns      : none
 *
 */
static inline void app_sleep_prepare_proc(sleep_mode_t *sleep_mode)
{
#if BLE_HID_DEVICE 
    if ( (app_env.app_flags & KBD_START_SCAN) || (app_env.app_flags & KBD_SCANNING) ) 
    {
        SCB->SCR &= ~(1<<2);                    // do not fall in deep or extended sleep!!!
        *sleep_mode = mode_idle;                // block power-off
    }
#endif
}

/*
 * Name         : app_sleep_entry_proc - Hook #5
 *
 * Scope        : PUBLIC
 *
 * Arguments    : none
 *
 * Description  : See documentation.
 *
 * Returns      : none
 *
 */
static inline void app_sleep_entry_proc(sleep_mode_t *sleep_mode)
{
#if BLE_PROX_REPORTER || BLE_FINDME_LOCATOR
    if (GPIO_GetPinStatus( GPIO_PORT_0, GPIO_PIN_6 ))
        wkupct_enable_irq(0x40, 0x40, 1, 0); // P0_6, polarity low , 1 event, debouncing time = 0
#endif
    
#if BLE_HID_DEVICE 
    if ( *sleep_mode == mode_idle ) 
    {
        /*
        * Use a lower clock to preserve power (i.e. 2MHz)
        */
        SetBits16(CLK_AMBA_REG, PCLK_DIV, 3);   // lowest is 2MHz (div 8, source is RC @16MHz)
        SetBits16(CLK_AMBA_REG, HCLK_DIV, 3);
    }
#endif // BLE_HID_DEVICE    
}

/*
 * Name         : app_sleep_exit_proc - Hook #6
 *
 * Scope        : PUBLIC
 *
 * Arguments    : none
 *
 * Description  : See documentation.
 *
 * Returns      : none
 *
 */
static inline void app_sleep_exit_proc(sleep_mode_t sleep_mode)
{
#if BLE_HID_DEVICE
    /*
     * Restore clock
     */
    SetBits16(CLK_AMBA_REG, PCLK_DIV, 0);  
    SetBits16(CLK_AMBA_REG, HCLK_DIV, 0);
#endif //BLE_HID_DEVICE
}
