/**
 ****************************************************************************************
 *
 * @file app_kbd_fsm.c
 *
 * @brief HID Keyboard main FSM.
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
#include "rwip_config.h"             // SW configuration
#include "app.h" 
#include "app_task.h"
#include "app_sec.h"
#include "gap_cfg.h"
#include "llc_util.h"
#include "app_console.h"
#include "arch_sleep.h"
#include "gpio.h"

#include "app_kbd.h"
#include "app_kbd_proj.h"
#include "app_kbd_key_matrix.h"
#include "app_kbd_leds.h"
#include "app_kbd_fsm.h"
#include "app_kbd_debug.h"
#include "app_multi_bond.h"
#include "app_white_list.h"


enum main_fsm_states current_fsm_state  __attribute__((section("retention_mem_area0"), zero_init));
enum adv_states current_adv_state       __attribute__((section("retention_mem_area0"), zero_init));
bool is_bonded                          __attribute__((section("retention_mem_area0"), zero_init));
bool eeprom_is_read                     __attribute__((section("retention_mem_area0"), zero_init));
bool reset_bonding_request              __attribute__((section("retention_mem_area0"), zero_init));
int adv_timer_remaining                 __attribute__((section("retention_mem_area0"), zero_init));
int conn_timer_remaining                __attribute__((section("retention_mem_area0"), zero_init));

bool spota_on/* = false*/;

__attribute__((unused)) static const char state_names[8][24] = { "IDLE", "ADVERTISE", "CONNECTION_IN_PROGRESS", "CONNECTED_PAIRING", "CONNECTED",
                        "DISCONNECTED_IDLE", "DISCONNECTED_INIT", "DIRECTED_ADV" };

__attribute__((unused)) static const char events_names[15][17] = { "NO_EVENT", "KEY_PRESS", "TIMER_EXPIRED", "PAIRING_REQ", "CONN_REQ",
                         "CONN_CMP", "DISCONN", "CONN_UPD_RESP", "PASSKEY_ENTERED", "SWITCH_EVT", "NEW_HOST_EVT", "SPOTAR_START_EVT", "SPOTAR_END_EVT",
                         "ADV_TIMER_EXP", "INACT_TIMER_EXP" };

#if (DEVELOPMENT_DEBUG)
#define FSM_LOG_DEPTH 16
                         
struct fsm_log_ {
    enum main_fsm_events evt;
    enum main_fsm_states state;
    uint16_t time;
} fsm_log[FSM_LOG_DEPTH] __attribute__((section("retention_mem_area0"), zero_init));

int fsm_log_ptr __attribute__((section("retention_mem_area0"), zero_init));
#endif


/**
 ****************************************************************************************
 * @brief Handler of a dummy TASK_APP msg sent to trigger the adv timer
 *        In case of advertising start from IDLE_ST, a dummy message is sent to the TASK_APP. 
 *        This msg is put in the queue when the BLE is woken up. When the handler is called, 
 *        it is certain that the BLE is running and the timer may start.
 *
 * @param[in]   msgid    
 * @param[in]   param
 * @param[in]   dest_id
 * @param[in]   src_id
 *
 * @return  KE_MSG_CONSUMED
 ****************************************************************************************
 */
int app_start_adv_msg_handler(ke_msg_id_t const msgid,
                                    void const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    // Start Adv timer (if not running)
    if (current_adv_state != SLOW_ADV)
    {
        if (adv_timer_remaining > 0)
            app_timer_set(APP_HID_ADV_TIMER, TASK_APP, adv_timer_remaining);
        else
        {
            app_state_update(ADV_TIMER_EXP_EVT);
        }
    }
    
    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief Starts undirected advertising
 *
 * @param   None    
 *
 * @return  KE_MSG_CONSUMED
 ****************************************************************************************
 */
void start_adv_undirected(void)
{
    // Start Adv timer (if not running)
    ke_msg_send_basic(APP_START_ADV_MSG, TASK_APP, TASK_APP);
    
    // Allocate a message for GAP
    struct gapm_start_advertise_cmd *cmd = KE_MSG_ALLOC(GAPM_START_ADVERTISE_CMD,
                                                TASK_GAPM, TASK_APP,
                                                gapm_start_advertise_cmd);

    cmd->op.code = GAPM_ADV_UNDIRECT;
    cmd->op.addr_src = GAPM_PUBLIC_ADDR;
    cmd->channel_map = APP_ADV_CHMAP;

    cmd->info.host.mode = GAP_GEN_DISCOVERABLE;
    
    switch(current_adv_state)
    {
    case SLOW_ADV:
        cmd->intv_min = SLOW_BONDED_ADV_INT_MIN;
        cmd->intv_max = SLOW_BONDED_ADV_INT_MAX;
        break;
    case UNBONDED_ADV:
        cmd->intv_min = NORMAL_ADV_INT_MIN;
        cmd->intv_max = NORMAL_ADV_INT_MAX;
        break;
    case BONDED_ADV:
        cmd->intv_min = FAST_BONDED_ADV_INT_MIN;
        cmd->intv_max = FAST_BONDED_ADV_INT_MAX;
        break;
    default:
        break;
    }
    
    // The filter policy should be defined prior to the call to this function.
    // An application may wish to advertise using various filter policies, depending
    // on its state i.e. it may use ADV_ALLOW_SCAN_ANY_CON_WLST when it wants to
    // connect to known hosts only or ADV_ALLOW_SCAN_ANY_CON_ANY when it wants to
    // pair to new hosts.
    // So, the following code is placed here as a reference only.
    if (HAS_WHITE_LIST)
    {
        if (white_list_written)
        {
            cmd->info.host.adv_filt_policy = ADV_ALLOW_SCAN_WLST_CON_WLST;
        }
        else
        {
            cmd->info.host.adv_filt_policy = ADV_ALLOW_SCAN_ANY_CON_ANY;
        }
    }
    else if (HAS_VIRTUAL_WHITE_LIST)
    {
        if (white_list_written)
        {
            virtual_wlist_policy = ADV_ALLOW_SCAN_ANY_CON_WLST;
        }
        else
        {
            virtual_wlist_policy = ADV_ALLOW_SCAN_ANY_CON_ANY;
        }
        cmd->info.host.adv_filt_policy = ADV_ALLOW_SCAN_ANY_CON_ANY;
    }
    else
    {
        cmd->info.host.adv_filt_policy = ADV_ALLOW_SCAN_ANY_CON_ANY;
    }
    
    /*-----------------------------------------------------------------------------------
     * Set the Advertising Data and the Scan Response Data
     *---------------------------------------------------------------------------------*/
    set_adv_data(cmd);
    
    // Send the message
    ke_msg_send(cmd);

    // We are now connectable
    ke_state_set(TASK_APP, APP_CONNECTABLE);
}


/**
 ****************************************************************************************
 * @brief Starts directed advertising
 *
 * @param   None    
 *
 * @return  KE_MSG_CONSUMED
 ****************************************************************************************
 */
void start_adv_directed(void)
{
    // Allocate a message for GAP
    struct gapm_start_advertise_cmd *cmd = KE_MSG_ALLOC(GAPM_START_ADVERTISE_CMD,
                                                TASK_GAPM, TASK_APP,
                                                gapm_start_advertise_cmd);

    cmd->op.code = GAPM_ADV_DIRECT;
    cmd->op.addr_src = GAPM_PUBLIC_ADDR;
    cmd->channel_map = APP_ADV_CHMAP;
    cmd->intv_min = APP_ADV_INT_MIN;
    cmd->intv_max = APP_ADV_INT_MAX;
    cmd->info.host.mode = GAP_GEN_DISCOVERABLE;
    cmd->info.direct.addr_type = bond_info.env.peer_addr_type;
    memcpy((void *)cmd->info.direct.addr.addr, bond_info.env.peer_addr.addr, BD_ADDR_LEN);
    
    // Send the message
    ke_msg_send(cmd);

    // We are now connectable
    ke_state_set(TASK_APP, APP_CONNECTABLE);
}


/**
 ****************************************************************************************
 * @brief Sends connection update request
 *
 * @param   None    
 *
 * @return  KE_MSG_CONSUMED
 ****************************************************************************************
 */
void send_connection_upd_req(void)
{    
    ke_state_t app_state = ke_state_get(TASK_APP);
    
	// Modify Conn Params
	if (app_state == APP_SECURITY || app_state == APP_PARAM_UPD || app_state == APP_CONNECTED) 
	{
		struct gapc_param_update_cmd * req = KE_MSG_ALLOC(GAPC_PARAM_UPDATE_CMD, TASK_GAPC, TASK_APP, gapc_param_update_cmd);

		// Fill in the parameter structure
        req->operation = GAPC_UPDATE_PARAMS;
#ifndef __DA14581__
		req->params.intv_min = PREFERRED_CONN_INTERVAL_MIN; // N * 1.25ms
		req->params.intv_max = PREFERRED_CONN_INTERVAL_MAX; // N * 1.25ms
		req->params.latency  = PREFERRED_CONN_LATENCY;      // Conn Events skipped
		req->params.time_out = PREFERRED_CONN_TIMEOUT;      // N * 10ms
#else
		req->intv_min   = PREFERRED_CONN_INTERVAL_MIN;      // N * 1.25ms
		req->intv_max   = PREFERRED_CONN_INTERVAL_MAX;      // N * 1.25ms
		req->latency    = PREFERRED_CONN_LATENCY;           // Conn Events skipped
		req->time_out   = PREFERRED_CONN_TIMEOUT;           // N * 10ms
#endif        
		dbg_puts(DBG_FSM_LVL, "Send GAP_PARAM_UPDATE_REQ\r\n");
		ke_msg_send(req);
        
        ke_state_set(TASK_APP, APP_PARAM_UPD);
	}
}


/**
 ****************************************************************************************
 * @brief Wakes-up the BLE
 *
 * @param   None    
 *
 * @return  KE_MSG_CONSUMED
 ****************************************************************************************
 */
void wake_ble_up(void)
{
    app_ble_force_wakeup();
    app_ble_ext_wakeup_off();
}


#include "ke_env.h"
extern uint32_t ke_time(void);

/**
 ****************************************************************************************
 * @brief Removes a kernel timer from the timers list
 *
 * @param[in]   timer_id
 * @param[in]   task_id
 *
 * @return  the time (10ms intervals) that remained for the timer to elapse
 *          (0 if the timer was not removed)
 ****************************************************************************************
 */
int ke_timer_remove(ke_msg_id_t const timer_id, ke_task_id_t const task_id)
{
    int ret = 0;
    
    struct ke_timer *timer = (struct ke_timer *) ke_env.queue_timer.first;

    /* scan the timer queue to look for a message element with the same id and destination*/
    while (timer != NULL)
    {
        if ((timer->id == timer_id) &&
            (timer->task == task_id) )
        {
            /* Element has been found */
            break;
        }

        /* Check  next timer */
        timer = timer->next;
    }

    /* If the element has been found */
    if (timer != NULL)
    {
        ret = timer->time - ke_time();
        if (ret < 0)
            ret += (BLE_GROSSTARGET_MASK+1);
        
        ke_timer_clear(timer_id, task_id);
    }
    
    return ret;
}


/**
 ****************************************************************************************
 * @brief Updates the main FSM with an event
 *
 * @param[in]   evt   The main FSM event
 *
 * @return  void
 ****************************************************************************************
 */
void app_state_update(enum main_fsm_events evt)
{
    if ( (KEY_PRESS_EVT == evt) && (current_fsm_state > CONNECTED_ST) )     // only for IDLE_ST and CONNECTED_ST
        return;
        
#if (DEVELOPMENT_DEBUG)   
    fsm_log[fsm_log_ptr].state = current_fsm_state;
    fsm_log[fsm_log_ptr].evt = evt;
    fsm_log[fsm_log_ptr].time = ke_time() & BLE_GROSSTARGET_MASK;
    fsm_log_ptr++;
    if (fsm_log_ptr == FSM_LOG_DEPTH)
        fsm_log_ptr = 0;
#endif    
    
    dbg_printf(DBG_FSM_LVL, "\r\n**> %s\r\n", state_names[current_fsm_state]);
    dbg_printf(DBG_FSM_LVL, "--> %s\r\n", events_names[evt]);
    
    //Common code in all states
    if (SPOTAR_END_EVT == evt)
    {
        spota_on = false;
        // Re-configure EEPROM pins since they've been reset to GPIOs
        if (INIT_EEPROM_PINS)
        {
            GPIO_SetPinFunction(I2C_SCL_PORT, I2C_SCL_PIN, INPUT, PID_I2C_SCL);
            GPIO_SetPinFunction(I2C_SDA_PORT, I2C_SDA_PIN, INPUT, PID_I2C_SDA);
        }
        
        if (current_fsm_state != CONNECTED_ST)
            return; // extra functionality required only when connected!
    }

    switch(current_fsm_state) 
    {
    case IDLE_ST:
    
        if (HAS_EEPROM)
        {
            if (eeprom_is_read == false)
                is_bonded = false;
            
            if ( (is_bonded == false) && (eeprom_is_read == false) )
            {
                app_alt_pair_read_status();
                reset_active_peer_pos();
#if 0                
                is_bonded = app_alt_pair_get_next_bond_data(true);
#else                
                is_bonded = app_alt_pair_load_last_used();
                
                if (is_bonded)
                {
                    // has the last connected host public /*or static random address*/?
                    if (   (ADDR_PUBLIC == bond_info.env.peer_addr_type)
                        /*|| ( (bond_info.env.peer_addr_type == ADDR_RAND) && ((bond_info.env.peer_addr.addr[5] & SMPM_ADDR_TYPE_STATIC) == SMPM_ADDR_TYPE_STATIC) )*/ )
                    {
                        // do Directed advertising to this host if connection is lost
                    }
                    else
                        is_bonded = false;      // will be set to true if Privacy is enabled and the Reconnection address is written
                }
#endif                
                eeprom_is_read = true;
            }
        }
        else
            {
                bond_info.info = BOND_INFO_DEFAULT_VAL;
                updatedb_from_bonding_info(&bond_info);
                if (HAS_MITM)
                {
                    bond_info.env.auth = GAP_AUTH_REQ_MITM_BOND;
                }
                else
                {
                    bond_info.env.auth = GAP_AUTH_REQ_NO_MITM_BOND;
                }
            }
            
        
        if (HAS_INACTIVITY_TIMEOUT)
        {
            conn_timer_remaining = KBD_INACTIVITY_TIMEOUT / 10;     // reset inactivity timer here
        }
        
        switch(evt)
        {
        case NO_EVENT:
            if (HAS_NORMALLY_CONNECTABLE)
            {
                if (is_bonded && (bond_info.env.auth & GAP_AUTH_BOND))
                {
                    start_adv_directed();
                    current_fsm_state = DIRECTED_ADV_ST;
                }
                else
                {
                    current_adv_state = SLOW_ADV;
                    current_fsm_state = ADVERTISE_ST;
                    start_adv_undirected();
                }
                
                wake_ble_up();
                if (HAS_DEEPSLEEP)
                {
                    app_set_extended_sleep();
                }
            }
            else if (HAS_DEEPSLEEP)
            {
                app_set_deep_sleep();
                app_ble_ext_wakeup_on();
                if (HAS_DELAYED_WAKEUP)
                {
                    app_kbd_enable_delayed_scanning(true);
                }
            }
            else
            {
                app_ble_ext_wakeup_on();
                if (HAS_DELAYED_WAKEUP)
                {
                    app_kbd_enable_delayed_scanning(true);
                }
            }
            break;
            
        case KEY_PRESS_EVT:
            
            if (is_bonded && (bond_info.env.auth & GAP_AUTH_BOND))
            {
                current_fsm_state = DIRECTED_ADV_ST;
                start_adv_directed();
            }
            else
            {
                adv_timer_remaining = KBD_UNBONDED_DISCOVERABLE_TIMEOUT / 10;
                current_adv_state = UNBONDED_ADV;
                current_fsm_state = ADVERTISE_ST;
                start_adv_undirected();
            }
            
            wake_ble_up();
            if (HAS_DEEPSLEEP)
            {
                app_set_extended_sleep();
            }
            break;
            
        default:
            ASSERT_WARNING(0);
            break;
        }
        break;
        
        
    case ADVERTISE_ST:
        switch(evt)
        {
        case ADV_TIMER_EXP_EVT:
            adv_timer_remaining = 0;
            app_adv_stop();
            break;
        
        case TIMER_EXPIRED_EVT:
            if (HAS_NORMALLY_CONNECTABLE)
            {
                current_adv_state = SLOW_ADV;
                start_adv_undirected();
            }
            else
            {
#if 0 //DEBUG code                
                {
                    // Sanity check
                    uint8_t state = ke_state_get(TASK_GAPM);
                    if (state != GAPM_IDLE)
                        ASSERT_WARNING(0);
                }
#endif
                
                app_kbd_flush_buffer();
                app_kbd_flush_reports();

                // timer is cleared automatically
                
                if (HAS_DEEPSLEEP)
                {
                    app_set_deep_sleep();
                }
                app_ble_ext_wakeup_on();
                if (HAS_DELAYED_WAKEUP)
                {
                    app_kbd_enable_delayed_scanning(true);
                }
                current_fsm_state = IDLE_ST;
            }
            break;
            
        case CONN_REQ_EVT:
            if (adv_timer_remaining)
                adv_timer_remaining = ke_timer_remove(APP_HID_ADV_TIMER, TASK_APP);
            spota_on = false;
            current_fsm_state = CONNECTION_IN_PROGRESS_ST;
            break;
            
        default:
            ASSERT_WARNING(0);
            break;
        }
        break;
        
        
    case CONNECTION_IN_PROGRESS_ST:
        if ( (HAS_MITM) || (HAS_EEPROM) )
        {
            dbg_puts(DBG_FSM_LVL, "  (-) HID ENC timer\r\n");
            ke_timer_clear(APP_HID_ENC_TIMER, TASK_APP);        // Timer expire results in connection drop and is handled below.
        }
        
        switch(evt)
        {
        case PAIRING_REQ_EVT:
            if (HAS_PASSCODE_TIMEOUT)
            {
                dbg_puts(DBG_FSM_LVL, "  (+) passcode timer\r\n");
                app_timer_set(APP_HID_TIMER, TASK_APP, (KBD_PASSCODE_TIMEOUT / 10));
            }
            app_kbd_flush_buffer(); // entering "passcode" mode
            app_kbd_flush_reports();
            current_fsm_state = CONNECTED_PAIRING_ST;
            break;
            
        case NEW_HOST_EVT:
            app_kbd_flush_buffer();
            app_kbd_flush_reports();
            break;
        
        case CONN_CMP_EVT:
            // LEDs are controlled by the caller ("established")
            
            if (HAS_KBD_SWITCH_TO_PREFERRED_CONN_PARAMS)
            {
                // Timer for sending the CONN_PARAM_UPDATE is set by the caller
                dbg_puts(DBG_FSM_LVL, "  (+) update params timer\r\n");
            }
            
            if (HAS_INACTIVITY_TIMEOUT)
            {
                dbg_puts(DBG_FSM_LVL, "  (+) inactivity timer\r\n");
                if (conn_timer_remaining == 0)
                    conn_timer_remaining++; // in case it's zero, just go to CONNECTED_ST in order to reach IDLE_ST
                app_extended_timer_set(APP_HID_INACTIVITY_TIMER, TASK_APP, conn_timer_remaining);
            }
            
            if (HAS_WHITE_LIST || HAS_VIRTUAL_WHITE_LIST)
            {
                add_host_in_white_list(app_env.peer_addr_type, &app_env.peer_addr, app_alt_peer_get_active_index());
            }
            
            // has the host public /* or static random address */?
            if (   (ADDR_PUBLIC == app_env.peer_addr_type)
                /*|| ( (app_env.peer_addr_type == ADDR_RAND) && ((app_env.peer_addr.addr[5] & SMPM_ADDR_TYPE_STATIC) == SMPM_ADDR_TYPE_STATIC) )*/ )
            {
                is_bonded = true;       // do Directed advertising to this host if connection is lost
            }
            else
                is_bonded = false;      // will be set to true if Privacy is enabled and the Reconnection address is written
        
            current_fsm_state = CONNECTED_ST;
            break;
            
        case DISCONN_EVT:
            // LEDs are controlled by the caller ("disconnected")
            
            // Advertising settings remain unchanged!
            current_fsm_state = ADVERTISE_ST;
            start_adv_undirected();
            break;
            
        default:
            ASSERT_WARNING(0);
            break;
        }
        break;
        
        
    case CONNECTED_PAIRING_ST:
        if (HAS_PASSCODE_TIMEOUT)
        {
            dbg_puts(DBG_FSM_LVL, "  (-) passcode timer\r\n");
            ke_timer_clear(APP_HID_TIMER, TASK_APP);
        }
        
        switch(evt)
        {
        case PASSKEY_ENTERED:
            if (HAS_PASSCODE_TIMEOUT)
            {
                dbg_puts(DBG_FSM_LVL, "  (+) passcode timer\r\n");
                app_timer_set(APP_HID_TIMER, TASK_APP, (KBD_PASSCODE_TIMEOUT / 10));
            }
            break;
            
        case CONN_CMP_EVT:
            // LEDs are controlled by the caller ("established")
            
            if (HAS_KBD_SWITCH_TO_PREFERRED_CONN_PARAMS)
            {
                // Timer for sending the CONN_PARAM_UPDATE is set by the caller
                dbg_puts(DBG_FSM_LVL, "  (+) update params timer\r\n");
            }
            
            if (HAS_INACTIVITY_TIMEOUT)
            {
                dbg_puts(DBG_FSM_LVL, "  (+) inactivity timer\r\n");
                if (conn_timer_remaining == 0)
                    conn_timer_remaining++; // in case it's zero, just go to CONNECTED_ST in order to reach IDLE_ST
                app_extended_timer_set(APP_HID_INACTIVITY_TIMER, TASK_APP, conn_timer_remaining);
            }
            
            if (HAS_WHITE_LIST || HAS_VIRTUAL_WHITE_LIST)
            {
                add_host_in_white_list(app_env.peer_addr_type, &app_env.peer_addr, app_alt_peer_get_active_index());
            }
            
            // has the host public /* or static random address */?
            if (   (ADDR_PUBLIC == app_env.peer_addr_type)
                /*|| ( (app_env.peer_addr_type == ADDR_RAND) && ((app_env.peer_addr.addr[5] & SMPM_ADDR_TYPE_STATIC) == SMPM_ADDR_TYPE_STATIC) )*/ )
            {
                is_bonded = true;       // do Directed advertising to this host if connection is lost
            }
            else
                is_bonded = false;      // will be set to true if Privacy is enabled and the Reconnection address is written
        
            current_fsm_state = CONNECTED_ST;
            break;
            
        case DISCONN_EVT:
            // LEDs are controlled by the caller ("disconnected")
            
            // Advertising settings remain unchanged!
            current_fsm_state = ADVERTISE_ST;
            start_adv_undirected();
            break;
            
        case TIMER_EXPIRED_EVT:
            if (HAS_PASSCODE_TIMEOUT)
            {
                if (HAS_SEND_LL_TERMINATE_IND)
                {
                    app_disconnect();
                }
                else
                {
                    app_fake_disconnect(); 
                }
                
                if (__builtin_popcount(multi_bond_status) > 1)
                    current_adv_state = BONDED_ADV;
                else
                    current_adv_state = UNBONDED_ADV;
            
                current_fsm_state = DISCONNECTED_INIT_ST;
            }
            else 
                ASSERT_WARNING(0);
            break;
            
        default:
            ASSERT_WARNING(0);
            break;
        }
        break;
        
        
    case CONNECTED_ST:
        switch(evt)
        {
        case PAIRING_REQ_EVT:
            if ( (HAS_MITM) || (HAS_EEPROM) )
            {
                if (HAS_KEYBOARD_LEDS)
                {
                    leds_set_connection_in_progress();
                }
            }
                
            if (HAS_KBD_SWITCH_TO_PREFERRED_CONN_PARAMS)
            {
                // Clear update connection params timer or it may hit while we are in CONNECTED_PAIRING_ST...
                ke_timer_clear(APP_HID_TIMER, TASK_APP);
                dbg_puts(DBG_CONN_LVL, "(-) params update timer\r\n");
            }
                
            if (HAS_PASSCODE_TIMEOUT)
            {
                dbg_puts(DBG_FSM_LVL, "  (+) passcode timer\r\n");
                app_timer_set(APP_HID_TIMER, TASK_APP, (KBD_PASSCODE_TIMEOUT / 10));
            }
            app_kbd_flush_buffer(); // entering "passcode" mode
            app_kbd_flush_reports();
            if (HAS_INACTIVITY_TIMEOUT)
            {
                dbg_puts(DBG_FSM_LVL, "  (*) inactivity timer\r\n");
                conn_timer_remaining = ke_timer_remove(APP_HID_INACTIVITY_TIMER, TASK_APP);
                // in case of EXTENDED_TIMERS, extended_timer_cnt holds its value and can be used 
                // when the timer is reset later...
            }
            current_fsm_state = CONNECTED_PAIRING_ST;
            break;
            
        case SPOTAR_START_EVT:
            if (HAS_INACTIVITY_TIMEOUT)
            {
                dbg_puts(DBG_FSM_LVL, "  (*) inactivity timer\r\n");
                conn_timer_remaining = ke_timer_remove(APP_HID_INACTIVITY_TIMER, TASK_APP);
                // in case of EXTENDED_TIMERS, extended_timer_cnt holds its value and can be used 
                // when the timer is reset later...
            }
            spota_on = true;
            break;
            
        case SPOTAR_END_EVT:
            // Part of the handling of this event is done by the "common code" before the switch statement
            if (HAS_INACTIVITY_TIMEOUT)
            {
                dbg_puts(DBG_FSM_LVL, "  (+) inactivity timer\r\n");
                app_extended_timer_set(APP_HID_INACTIVITY_TIMER, TASK_APP, conn_timer_remaining);
            }
            break;
            
        case KEY_PRESS_EVT:
            if (HAS_INACTIVITY_TIMEOUT)
            {
                if (spota_on == false)
                {
                    dbg_puts(DBG_FSM_LVL, "  (!) inactivity timer\r\n");
                    conn_timer_remaining = KBD_INACTIVITY_TIMEOUT / 10;
                    app_extended_timer_set(APP_HID_INACTIVITY_TIMER, TASK_APP, conn_timer_remaining);
                }
            }
            break;
            
        case CONN_CMP_EVT:  // PARAM_UPDATE was completed!
            break;
            
        case SWITCH_EVT:
            // LEDs are controlled by the caller ("disconnected")
            
            if (HAS_INACTIVITY_TIMEOUT)
            {
                dbg_puts(DBG_FSM_LVL, "  (*) inactivity timer\r\n");
                conn_timer_remaining = ke_timer_remove(APP_HID_INACTIVITY_TIMER, TASK_APP);
                // in case of EXTENDED_TIMERS, extended_timer_cnt holds its value and can be used 
                // when the timer is reset later...
            }
            
            if (HAS_KBD_SWITCH_TO_PREFERRED_CONN_PARAMS)
            {
                // Clear update connection params timer
                ke_timer_clear(APP_HID_TIMER, TASK_APP);
                dbg_puts(DBG_CONN_LVL, "(-) params update timer\r\n");
            }
            
            if (__builtin_popcount(multi_bond_status) > 1)
            {
                adv_timer_remaining = (KBD_BONDED_DISCOVERABLE_TIMEOUT / 10) + ALT_PAIR_DISCONN_TIME;
                current_adv_state = BONDED_ADV;
            }
            else
            {
                adv_timer_remaining = (KBD_UNBONDED_DISCOVERABLE_TIMEOUT / 10) + ALT_PAIR_DISCONN_TIME;
                current_adv_state = UNBONDED_ADV;
            }
            
            current_fsm_state = DISCONNECTED_INIT_ST;
            break;
            
        case DISCONN_EVT:
            // LEDs are controlled by the caller ("disconnected")
            
            if (HAS_INACTIVITY_TIMEOUT)
            {
                dbg_puts(DBG_FSM_LVL, "  (*) inactivity timer\r\n");
                conn_timer_remaining = ke_timer_remove(APP_HID_INACTIVITY_TIMER, TASK_APP);
                // in case of EXTENDED_TIMERS, extended_timer_cnt holds its value and can be used 
                // when the timer is reset later...
            }

            if (HAS_KBD_SWITCH_TO_PREFERRED_CONN_PARAMS)
            {
                // Clear update connection params timer
                ke_timer_clear(APP_HID_TIMER, TASK_APP);
                dbg_puts(DBG_CONN_LVL, "(-) params update timer\r\n");
            }
                        
            if (is_bonded && (bond_info.env.auth & GAP_AUTH_BOND))
            {
                current_fsm_state = DIRECTED_ADV_ST;
                start_adv_directed();
            }
            else
            {
                adv_timer_remaining = KBD_BONDED_DISCOVERABLE_TIMEOUT / 10;
                current_adv_state = BONDED_ADV;
                current_fsm_state = ADVERTISE_ST;
                start_adv_undirected();
            }
            break;
            
        case TIMER_EXPIRED_EVT:
            if (HAS_KBD_SWITCH_TO_PREFERRED_CONN_PARAMS)
            {
                send_connection_upd_req();
            }
            else 
                ASSERT_WARNING(0);
            break;
            
        case INACT_TIMER_EXP_EVT:
            if (HAS_INACTIVITY_TIMEOUT)
            {
                if (HAS_SEND_LL_TERMINATE_IND)
                {
                    app_disconnect();
                }
                else
                {
                    app_fake_disconnect(); 
                }
                current_fsm_state = DISCONNECTED_IDLE_ST;
            }
            else 
                ASSERT_WARNING(0);
            break;
            
        default:
            ASSERT_WARNING(0);
            break;
        }
        break;
        
    case DISCONNECTED_IDLE_ST:
        switch(evt)
        {
        case DISCONN_EVT:
#if 0 //DEBUG code            
            {
                // Sanity check
                uint8_t state = ke_state_get(TASK_GAPM);
                if (state != GAPM_IDLE)
                    ASSERT_WARNING(0);
            }
#endif
        
            // LEDs are controlled by the caller ("disconnected")
        
            if (HAS_INACTIVITY_TIMEOUT)
            {
                conn_timer_remaining = KBD_INACTIVITY_TIMEOUT / 10;     // reset inactivity timer here
            }
        
            if (HAS_NORMALLY_CONNECTABLE)
            {
                current_adv_state = SLOW_ADV;
                current_fsm_state = ADVERTISE_ST;
                start_adv_undirected();
            }
            else
            {
                if (HAS_DEEPSLEEP)
                {
                    app_set_deep_sleep();
                }
                app_ble_ext_wakeup_on();
                if (HAS_DELAYED_WAKEUP)
                {
                    app_kbd_enable_delayed_scanning(true);
                }
                current_fsm_state = IDLE_ST;
            }
            break;

        case CONN_CMP_EVT:  // PARAM_UPDATE was completed!
            break;
        
        default:
            ASSERT_WARNING(0);
            break;
        }
        break;
        
    case DISCONNECTED_INIT_ST:
        switch(evt)
        {
        case DISCONN_EVT:
            // LEDs are controlled by the caller ("disconnected")
            
            current_fsm_state = ADVERTISE_ST;
            start_adv_undirected();
            break;

        case CONN_CMP_EVT:  // PARAM_UPDATE was completed!
            break;
        
        default:
            ASSERT_WARNING(0);
            break;
        }
        break;
        
        
    case DIRECTED_ADV_ST:
        switch(evt)
        {
        case TIMER_EXPIRED_EVT:
            adv_timer_remaining = KBD_BONDED_DISCOVERABLE_TIMEOUT / 10;
            current_adv_state = BONDED_ADV;
            current_fsm_state = ADVERTISE_ST;
            start_adv_undirected();
            break;
            
        case CONN_REQ_EVT:
            // prepare advertising settings in case connection setup fails
            adv_timer_remaining = KBD_BONDED_DISCOVERABLE_TIMEOUT / 10;
            current_adv_state = BONDED_ADV;
            spota_on = false;
            current_fsm_state = CONNECTION_IN_PROGRESS_ST;
            break;
        
        default:
            ASSERT_WARNING(0);
            break;
        }
        break;
        
    default:
        ASSERT_WARNING(0);
        break;
    }
    
    dbg_printf(DBG_FSM_LVL, "    (N) %s\r\n", state_names[current_fsm_state]);
}

/**
 ****************************************************************************************
 * @brief Resets the bonding data and the white list entries
 *
 * @param   None    
 *
 * @return  KE_MSG_CONSUMED
 ****************************************************************************************
 */
void reset_bonding_data(void)
{
    // CLRP clears bonding info unconditionnaly
    if (HAS_WHITE_LIST || HAS_VIRTUAL_WHITE_LIST)
    {
        clear_white_list();
    }  
    memset( (uint8_t *)&bond_info.env, 0, sizeof(struct app_sec_env_tag) );
    is_bonded = false;
}
