/**
 ****************************************************************************************
 *
 * @file app_kbd_scan_fsm.c
 *
 * @brief Keyboard scanning FSM implementation.
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

#include "app_kbd.h"
#include "app_kbd_key_matrix.h"
#include "app_kbd_fsm.h"
#include "app_kbd_scan_fsm.h"
#include "app_multi_bond.h"
#include "app_console.h"
#include "app_kbd_debug.h"

enum key_scan_states current_scan_state __attribute__((section("retention_mem_area0"), zero_init));
static int scanning_substate            __attribute__((section("retention_mem_area0"), zero_init));

//bool main_fsm_changed = true;   /* use if the key scanning needs to be disabled */

/*
 * Description  : Key scan FSM.
 *
 * Returns      : void
 *
 */
void fsm_scan_update(void)
{
    switch(current_scan_state)
    {
    case KEY_SCAN_INACTIVE:
        {
            if (systick_hit || wkup_hit)
                ASSERT_ERROR(0);
        }
        
//        if (main_fsm_changed)
        if (true)
        {
//            main_fsm_changed = false;
            if (HAS_DELAYED_WAKEUP)
            {
                app_kbd_enable_delayed_scanning(true);
            }
            else
            {
                app_kbd_enable_scanning();
            }
            current_scan_state = KEY_SCAN_IDLE;             // Transition from KEY_SCAN_INACTIVE -> KEY_SCAN_IDLE
        }
        
        break;
    case KEY_SCAN_IDLE:
        {
            if (systick_hit)
                ASSERT_ERROR(0);
        }
        
//        if (main_fsm_changed)
        if (false)
        {
//            main_fsm_changed = false;
            current_scan_state = KEY_SCAN_INACTIVE;         // Transition from KEY_SCAN_IDLE -> KEY_SCAN_INACTIVE
            app_kbd_disable_scanning();
        }
        else if (wkup_hit)
        {
            scanning_substate = 0;
            GLOBAL_INT_DISABLE();
            current_scan_state = KEY_SCANNING;              // Transition from KEY_SCAN_IDLE -> KEY_SCANNING
            app_kbd_start_scanning();
            wkup_hit = false;                                   
            GLOBAL_INT_RESTORE();
        }
        break;
    case KEY_STATUS_UPD:
        {
            if (wkup_hit)
                ASSERT_ERROR(0);
        }
        
        if (systick_hit)
        {
            systick_hit = false;
            if (app_kbd_update_status())
            {
                scanning_substate = 0;
                current_scan_state = KEY_SCANNING;          // Transition from KEY_STATUS_UPD -> KEY_SCANNING
                // scan once to save time!
                if (app_kbd_scan_matrix(&scanning_substate))
                    current_scan_state = KEY_STATUS_UPD;    // Transition from KEY_SCANNING -> KEY_STATUS_UPD
            }
            else
            {
                GLOBAL_INT_DISABLE();
                app_kbd_enable_scanning();
                current_scan_state = KEY_SCAN_IDLE;         // Transition from KEY_STATUS_UPD -> KEY_SCAN_IDLE
                GLOBAL_INT_RESTORE();
            }
        }
        break;
    case KEY_SCANNING:
        {
            if (wkup_hit)
                ASSERT_ERROR(0);
        }

        if (systick_hit)
        {
            systick_hit = false;
            if (app_kbd_scan_matrix(&scanning_substate))
                current_scan_state = KEY_STATUS_UPD;        // Transition from KEY_SCANNING -> KEY_STATUS_UPD
            // else the state remains unchanged and next time we will scan the next row
        }
        break;
    default:
        break;
    }
}


/*
 * Name         : app_use_lower_clocks_check
 *
 * Scope        : PUBLIC
 *
 * Arguments    : none
 *
 * Description  : Used to check if AMBA clocks can be reduced will waiting for an interrupt in IDLE state.
 *
 * Returns      : true if the clocks can be reduced else false
 *
 */
bool app_use_lower_clocks_check(void)
{
    return ( (current_scan_state != KEY_STATUS_UPD) && (current_scan_state != KEY_SCANNING) );
}

