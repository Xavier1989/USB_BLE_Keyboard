/**
 ****************************************************************************************
 *
 * @file app_kbd_fsm.h
 *
 * @brief Keyboard (HID) Application FSM header file.
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

#ifndef APP_KBD_FSM_H_
#define APP_KBD_FSM_H_

#include <stdbool.h>


/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup HID
 *
 * @brief HID (Keyboard) Application FSM.
 *
 * @{
 ****************************************************************************************
 */


enum main_fsm_states {
    IDLE_ST,
    CONNECTED_ST,
    ADVERTISE_ST,
    CONNECTION_IN_PROGRESS_ST,
    CONNECTED_PAIRING_ST,
    DISCONNECTED_IDLE_ST,
    DISCONNECTED_INIT_ST,
    DIRECTED_ADV_ST,
};

enum adv_states {
    SLOW_ADV,
    UNBONDED_ADV,
    BONDED_ADV,
};

enum main_fsm_events {
    NO_EVENT,
    KEY_PRESS_EVT,
    TIMER_EXPIRED_EVT,
    PAIRING_REQ_EVT,
    CONN_REQ_EVT,
    CONN_CMP_EVT,
    DISCONN_EVT,
    CONN_UPD_RESP_EVT,
    PASSKEY_ENTERED,
    SWITCH_EVT,
    NEW_HOST_EVT,
    SPOTAR_START_EVT,
    SPOTAR_END_EVT,
    ADV_TIMER_EXP_EVT,
    INACT_TIMER_EXP_EVT
};

extern bool eeprom_is_read;
extern enum main_fsm_states current_fsm_state;
extern bool reset_bonding_request;

/**
 ****************************************************************************************
 * @brief Updates the main FSM with an event
 *
 * @param[in]   evt   The main FSM event
 *
 * @return  void
 ****************************************************************************************
 */
void app_state_update(enum main_fsm_events);

/**
 ****************************************************************************************
 * @brief Resets the bonding data and the white list entries
 *
 * @param   None    
 *
 * @return  KE_MSG_CONSUMED
 ****************************************************************************************
 */
void reset_bonding_data(void);

/**
 ****************************************************************************************
 * @brief Wakes-up the BLE
 *
 * @param   None    
 *
 * @return  KE_MSG_CONSUMED
 ****************************************************************************************
 */
void wake_ble_up(void);

#endif // APP_KBD_FSM_H_
