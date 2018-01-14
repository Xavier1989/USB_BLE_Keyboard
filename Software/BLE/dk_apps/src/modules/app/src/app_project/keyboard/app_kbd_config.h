/**
 ****************************************************************************************
 *
 * @file app_kbd_config.h
 *
 * @brief Keyboard (HID) Application configuration header file.
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

#ifndef APP_KBD_CONFIG_H_
#define APP_KBD_CONFIG_H_

/****************************************************************************************
 * Use Deep Sleep when in IDLE_ST                                                       *
 ****************************************************************************************/
//#define DEEPSLEEP_ON


/****************************************************************************************
 * Use Privacy (implementation pending)                                                 *
 ****************************************************************************************/
//#define PRIVACY_ON


/****************************************************************************************
 * Use White List when exiting DIRECTED_ADV_ST unsuccessfully.                          *
 * Note 1: if White List is used, then the Device will be able to bond only to 1 Master!*
 *         This can be easily modified according to application's usage scenario.       *
 * Note 2: VIRTUAL_WHITE_LIST_ON must NOT be defined! Only 1 White List can exist at a  *
 *         time!                                                                        *
 ****************************************************************************************/
//#define WHITE_LIST_ON


/****************************************************************************************
 * Use a Virtual White List to support also hosts with Resolvable Random Addresses.     *
 * All addresses are filtered by SW and not by HW!                                      *
 * Note 1: if White List is used, then the Device will be able to bond only to 1 Master!*
 *         This can be easily modified according to application's usage scenario.       *
 * Note 2: WHITE_LIST_ON MUST NOT be defined! Only 1 White List can exist at a time!    *
 * Note 3: Bonding and Encryption MUST be enabled (bonding info is stored in a          *
 *         non-volatile storage area).                                                  *
 ****************************************************************************************/
//#define VIRTUAL_WHITE_LIST_ON


/****************************************************************************************
 * Continuous key scanning. The chip won't go to sleep in this case.                    *
 ****************************************************************************************/
//#define SCAN_ALWAYS_ACTIVE_ON


/****************************************************************************************
 * Delayed wakeup (requires pressing a key for X ms to start)                           *
 ****************************************************************************************/
//#define DELAYED_WAKEUP_ON


/****************************************************************************************
 * Extended timers support (of more than 5 min)                                         *
 ****************************************************************************************/
//#define EXTENDED_TIMERS_ON


/****************************************************************************************
 * Use BOOT MODE                                                                        *
 ****************************************************************************************/
#define HOGPD_BOOT_PROTO_ON


/****************************************************************************************
 * Include BATT in HID                                                                  *
 ****************************************************************************************/
//#define BATT_EXTERNAL_REPORT_ON


/****************************************************************************************
 * Use different Fast and Partial scan periods                                          *
 ****************************************************************************************/
//#define ALTERNATIVE_SCAN_TIMES_ON


/****************************************************************************************
 * Send a ConnUpdateParam request after connection completion                           *
 ****************************************************************************************/
#define USE_PREF_CONN_PARAMS_ON


/****************************************************************************************
 * Use a key combination to put the device permanently in extended sleep                *
 * (i.e. 'Fn'+'Space') for consumption measurement purposes.                            *
 ****************************************************************************************/
//#define KEYBOARD_MEASURE_EXT_SLP_ON


/****************************************************************************************
 * Use Connection status LEDs                                                           *
 ****************************************************************************************/
//#define KEYBOARD_LEDS_ON


/****************************************************************************************
 * Set NormallyConnectable mode ON                                                      *
 ****************************************************************************************/
#define NORMALLY_CONNECTABLE_ON


/****************************************************************************************
 * Set RemoteWakeup mode ON                                                             *
 ****************************************************************************************/
//#define REMOTE_WAKEUP_ON


/****************************************************************************************
 * Enable timeout checking during PassCode entry                                        *
 ****************************************************************************************/
#define PASSCODE_TIMEOUT_ON


/****************************************************************************************
 * Send a SECURITY_REQUEST when connecting to a Host. If the Host is using a resolvable *
 * random address then check whether it is a known Host (i.e. its address can be        *
 * resolved). If it is a known Host then do not send the SECURITY_REQUEST message.      *
 ****************************************************************************************/
#define SECURITY_REQUEST_SEND_ON


/****************************************************************************************
 * Enable reporting of any key events happened while disconnected                       *
 ****************************************************************************************/
#define REPORT_HISTORY_ON


/****************************************************************************************
 * Enable sending of LL_TERMINATE_IND when dropping a connection                        *
 * Note: undefining this switch gives the option to silently drop a connection. The     *
 *       Host will notice it as if the device is lost or experienced a problem.         *
 ****************************************************************************************/
#define SEND_LL_TERMINATE_IND_ON 


/****************************************************************************************
 * Enable disconnection after a pre-defined inactivity timeout                          *
 ****************************************************************************************/
//#define INACTIVITY_TIMEOUT_ON


/****************************************************************************************
 * Use MITM authentication mode                                                         *
 ****************************************************************************************/
#define MITM_ON


/****************************************************************************************
 * Use EEPROM to store bonding data                                                     *
 * It depends on the Keyboard setup whether an EEPROM will be used eventually.          *
 ****************************************************************************************/
#define EEPROM_ON


/****************************************************************************************
 * Define EEPROM size (0 = 256 bytes, 1 = 8192 bytes)                                   *
 ****************************************************************************************/
#define EEPROM_IS_8K                            (0)


/****************************************************************************************
 * Choose keyboard layout                                                               *
 ****************************************************************************************/
#define MATRIX_SETUP                            (8)




/****************************************************************************************
 * Key scanning and Debouncing parameters                                               *
 ****************************************************************************************/
 
// HW debouncing time for key press
#define DEBOUNCE_TIME_PRESS                     (0)         // in msec
#define DEBOUNCE_TIME_RELEASE                   (0)         // in msec
#define DEBOUNCE_TIME_DELAYED_PRESS             (20)        // in msec
#define DEBOUNCE_TIME_DELAYED_RELEASE           (30)        // in msec

// SW debouncing times
#define ROW_SCAN_TIME                           (150)       // in usec

#ifdef ALTERNATIVE_SCAN_TIMES
#define FULL_SCAN_IN_MS                         (2)
#else
#define FULL_SCAN_IN_MS                         (3)
#endif
#define FULL_SCAN_TIME                          (FULL_SCAN_IN_MS * 1000)

#ifdef ALTERNATIVE_SCAN_TIMES
# define PARTIAL_SCAN_IN_MS                     (1)
#else
# define PARTIAL_SCAN_IN_MS                     (FULL_SCAN_IN_MS)
#endif
#define PARTIAL_SCAN_TIME                       (PARTIAL_SCAN_IN_MS * 1000)

// In general, debounce counters cannot be applied accurately. The reason is that 
// debouncing is done in SW, based on SysTick interrupt events that are used to 
// trigger the execution of the FSM.
// Thus, in the final application it is suggested to modify the definitions of the two
// counters (below) so that the real debouncing time is applied accurately in all
// cases. First, key presses need to be examined. It should be checked whether the
// debouncing period is correct both after wake-up (when the RC16 is used which is 
// approx. 14.5MHz) and when a 2nd key is pressed (assuming that in this case the BLE 
// core woke up to send the HID report of the 1st key press, which means that the 
// system runs with XTAL16).
// Release debouncing period must also be enforced. In all cases, though, the XTAL16 clk
// is used and there are no special cases.

#define DEBOUNCE_COUNTER_P_IN_MS                (12)
#define DEBOUNCE_COUNTER_R_IN_MS                (24)

#define KEYCODE_BUFFER_SIZE                     (64)	// if set to more than 255, change the type of the rd & wr pointers from 8- to 16-bit

#define DEBOUNCE_BUFFER_SIZE                    (16)


/****************************************************************************************
 * Timeouts                                                                             *
 ****************************************************************************************/
 
// Time in Limited Discoverable mode in ADVERTISE_ST:UNBONDED           (when NORMALLY_CONNECTABLE_ON is undefined)
#define KBD_UNBONDED_DISCOVERABLE_TIMEOUT       (180000)    // in msec

// Time in Limited Discoverable mode in ADVERTISE_ST:BONDED
#define KBD_BONDED_DISCOVERABLE_TIMEOUT         (30000)     // in msec

// Time to wait for a PAIRING_REQ or ENC_REQ from the Host when BONDING is enabled
#define KBD_ENC_SAFEGUARD_TIMEOUT               (5000)      // in msec

// Time in CONNECTED_NO_PAIR_ST until passcode is entered               (when PASSCODE_TIMEOUT_ON is defined)
#define KBD_PASSCODE_TIMEOUT                    (60000)     // in msec

// Idle time in CONNECTED_ST until disconnection is requested           (when INACTIVITY_TIMEOUT_ON is defined)
#define KBD_INACTIVITY_TIMEOUT                  (300000)    // in msec

// Time to request update of connection parameters                      (when USE_PREF_CONN_PARAMS_ON is defined)
#define TIME_TO_REQUEST_PARAM_UPD               (40000)	    // in msec

// Time to block previous host during a "host-switch"
#define ALT_PAIR_DISCONN_TIME                   (6000)      // in 10msec

// ADVERTISE_ST:UNBONDED : minimum advertising interval (* 0.625ms)
#define NORMAL_ADV_INT_MIN                      (0x30)      // 30 msec  (+ pseudo random advDelay from 0 to 10msec)

// ADVERTISE_ST:UNBONDED : maximum advertising interval (* 0.625ms)
#define NORMAL_ADV_INT_MAX                      (0x40)      // 40 msec  (+ pseudo random advDelay from 0 to 10msec)

// ADVERTISE_ST:BONDED : minimum advertising interval   (* 0.625ms)
#define FAST_BONDED_ADV_INT_MIN                 (0x20)      // 20 msec  (+ pseudo random advDelay from 0 to 10msec)

// ADVERTISE_ST:BONDED : maximum advertising interval   (* 0.625ms)
#define FAST_BONDED_ADV_INT_MAX                 (0x20)      // 20 msec  (+ pseudo random advDelay from 0 to 10msec)

// ADVERTISE_ST:SLOW : minimum advertising interval     (* 0.625ms)     (when NORMALLY_CONNECTABLE_ON is undefined) 
#define SLOW_BONDED_ADV_INT_MIN                 (0x640)     // 1 s      (+ pseudo random advDelay from 0 to 10msec)

// ADVERTISE_ST:SLOW : maximum advertising interval     (* 0.625ms)     (when NORMALLY_CONNECTABLE_ON is undefined)
#define SLOW_BONDED_ADV_INT_MAX                 (0xFA0)     // 2.5 s    (+ pseudo random advDelay from 0 to 10msec)

// Time to hold down a key for the system to wake-up (DELAYED_WAKEUP_ON must be set)
#define KBD_DELAY_TIMEOUT                       (0x7D0)     // 2 s


/****************************************************************************************
 * Prefered connection parameters                                                       *
 ****************************************************************************************/
#define PREFERRED_CONN_INTERVAL_MIN             (6)         //N * 1.25ms
#define PREFERRED_CONN_INTERVAL_MAX             (6)         //N * 1.25ms
#define	PREFERRED_CONN_LATENCY                  (31)
#define PREFERRED_CONN_TIMEOUT                  (200)       //N * 10ms

#endif // APP_KBD_CONFIG_H_
