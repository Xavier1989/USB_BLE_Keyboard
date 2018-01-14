/**
 ****************************************************************************************
 *
 * @file app_kbd_scan_fsm.h
 *
 * @brief Keyboard scan FSM header file.
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

#ifndef _APP_KBD_SCAN_FSM_H_
#define _APP_KBD_SCAN_FSM_H_

enum key_scan_states { 
    KEY_SCAN_INACTIVE,  // State 1
    KEY_SCAN_IDLE,      // State 2
    KEY_SCANNING,       // State 3
    KEY_STATUS_UPD,     // State 4
};

extern enum key_scan_states current_scan_state;

void fsm_scan_update(void);

#endif // _APP_KBD_SCAN_FSM_H_
