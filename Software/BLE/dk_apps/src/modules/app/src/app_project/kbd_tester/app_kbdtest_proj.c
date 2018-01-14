/**
 ****************************************************************************************
 *
 * @file app_kbdtest_proj.c
 *
 * @brief Keyboard Tester - HID over GATT Application entry point.
 *
 * Copyright (C) 2013. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"             // SW configuration
#include "app.h" 
#include "app_task.h"
#include "app_console.h"
#include "arch_sleep.h"

#include "app_sec.h"
#include "app_disc.h"
#include "app_basc.h"
#include "app_scppc.h"

#include "gapm_task.h"
#include "gapm_util.h"
#include "gapc.h"

#include "hogprh.h"
#include "hogprh_task.h"

#include "app_kbdtest_proj.h"
#include "app_kbdtest.h"


extern struct gap_cfg_table_struct gap_timeout_table;

uint8_t hids_nb  __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY
struct hogprh_hids_content hids[HOGPRH_NB_HIDS_INST_MAX]  __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
 
/*
 * Name         : app_update_gap_cfg - Update gap_cfg_table[]. 
 *
 * Scope        : PUBLIC
 *
 * Arguments    : none
 *
 * Description  : Updates gap_cfg_table[] with the preferred values (if any). Must be called
 *                at initialization and after a deep sleep wake up (because SysRAM contents
 *                are lost and gap_cfg_table[] is reset to the default values).
 *
 * Returns      : void
 *
 */
void app_update_gap_cfg(void)
{
    if (strlen(APP_DFLT_DEVICE_NAME) < 30)
        memcpy(gap_timeout_table.GAP_DEV_NAME_VAR, APP_DFLT_DEVICE_NAME, strlen(APP_DFLT_DEVICE_NAME));
#if  DEVELOPMENT_DEBUG
    else
        __asm("BKPT #0\n");
#endif // DEVELOPMENT_DEBUG
}


/*
 * Name         : app_init_func - Initialize Keyboard test app 
 *
 * Scope        : PUBLIC
 *
 * Arguments    : none
 *
 * Description  : Initialize state, GPIOs. Set GAP cfg and advertising data. Set keys (?). Set sleep mode.
 *
 * Returns      : void
 *
 */
void app_init_func(void)
{
    app_update_gap_cfg();
    
    // Initialize Device Information Client
    app_disc_init();
    
    // Initialize Battery Client
    app_basc_init();
    
    // Initialize Scan Parameters Client
    app_scppc_init();
        
//    app_set_deep_sleep();
//    app_set_extended_sleep();
    app_disable_sleep();
}


static void app_hogprh_enable_prf(uint16_t conhdl)
{
    // Allocate the message
    struct hogprh_enable_req *req = KE_MSG_ALLOC(HOGPRH_ENABLE_REQ,
                                               TASK_HOGPRH, TASK_APP,
                                               hogprh_enable_req);

    // Fill in the parameter structure
    req->conhdl             = conhdl;
    req->con_type           = PRF_CON_DISCOVERY;

    // Send the message
    ke_msg_send(req);
}


/*
 * Name         : app_configuration_func - Configure Keyboard BT settings 
 *
 * Scope        : PUBLIC
 *
 * Arguments    : task_id - id of the kernel task calling this function
 *                cmd - parameters to pass to the stack
 *
 * Description  : Configures Bluetooth settings for the Keyboard Test application.
 *
 * Returns      : void
 *
 */
void app_configuration_func(ke_task_id_t const task_id, struct gapm_set_dev_config_cmd *cmd)
{
    // Clear all previous initializations
    memset(cmd, 0, sizeof(struct gapm_set_dev_config_cmd));
    
    // set device configuration
    cmd->operation = GAPM_SET_DEV_CONFIG;
    // Device Role
    cmd->role = GAP_CENTRAL_MST;

    // Device IRK
    memcpy(cmd->irk.key, "0123456789012345", KEY_LEN);
    
    // Privacy settings bit field (0b1 = enabled, 0b0 = disabled)
    //  - [bit 0]: Privacy Support
    //  - [bit 1]: Multiple Bond Support (Peripheral only); If enabled, privacy flag is
    //             read only.
    //  - [bit 2]: Reconnection address visible.
    cmd->flags = 0;
}


/*
 * Name         : app_connect_func - Connect Keyboard Tester to DUT 
 *
 * Scope        : PUBLIC
 *
 * Arguments    : task_id - id of the kernel task calling this function
 *                cmd - parameters to pass to the stack
 *
 * Description  : Configures Bluetooth settings for the Keyboard Test application.
 *
 * Returns      : void
 *
 */
void app_connect_func(void)
{
    struct gapm_start_connection_cmd *msg;

    msg = (struct gapm_start_connection_cmd *) KE_MSG_ALLOC_DYN(GAPM_START_CONNECTION_CMD, 
                                                                TASK_GAPM, 
                                                                TASK_APP, 
                                                                gapm_start_connection_cmd, 
                                                                sizeof(struct gap_bdaddr)*2);

    msg->nb_peers           = 1;
    memcpy((void *) msg->peers[0].addr.addr, (void *)"\xA2\xA2\x45\x55\x23\x01", BD_ADDR_LEN);
    msg->peers[0].addr_type = GAPM_PUBLIC_ADDR;
    
    msg->con_intv_min       = 80;                   // 20ms
    msg->con_intv_max       = 80;                   // 20ms
    msg->ce_len_min         = 0x5; 
    msg->ce_len_max         = 0x8;
    msg->con_latency        = 0;
    msg->superv_to          = 0xC8;                 // 2sec
    
    msg->scan_interval      = 0x180;                // 240ms
    msg->scan_window        = 0x160;                // 220ms
    
    msg->op.addr_src        = GAPM_PUBLIC_ADDR;
    memcpy((void *) msg->op.addr.addr, (void *)"\x01\x23\x45\x55\x89\x01", BD_ADDR_LEN);
    msg->op.code            = GAPM_CONNECTION_DIRECT;

    // Send the message
    ke_msg_send(msg);    
}


/*
 * Name         : app_sec_init_func - Initialize security 
 *
 * Scope        : PUBLIC
 *
 * Arguments    : none
 *
 * Description  : Initialize security to "no man in the middle - no bonding".
 *
 * Returns      : void
 *
 */
#define HID_KEY_SIZE (16)

void app_sec_init_func(void)
{
#if (BLE_APP_SEC)
    app_sec_env.auth = GAP_AUTH_REQ_NO_MITM_NO_BOND;
//    app_sec_gen_ltk(HID_KEY_SIZE);
#endif
}


/*
 * Name         : app_sec_encrypt_complete_func - Enryption is completed -> Start normal keyboard operation
 *
 * Scope        : PUBLIC
 *
 * Arguments    : none
 *
 * Description  : After completion of the encryption of the link, it sets the keyboard to normal
 *                report mode.
 *
 * Returns      : void
 *
 */
void app_sec_encrypt_complete_func(void)
{
}


/*
 * Name         : app_connection_func - Configure app at connection establishment 
 *
 * Scope        : PUBLIC
 *
 * Arguments    : dest_id - id of the kernel task calling this function
 *                param - parameters passed from the stack
 *
 * Description  : Configures keyboard application when connection is established.
 *
 * Returns      : void
 *
 */
void app_connection_func(ke_task_id_t const dest_id, struct gapc_connection_req_ind const *param)
{
    /*--------------------------------------------------------------
    * ENABLE REQUIRED PROFILES
    *-------------------------------------------------------------*/
	arch_printf("gap_le_create_conn_req_cmp_evt_handler() (%d, %d, %d, %d)\r\n", 
			(int)param->con_interval,
			(int)param->con_latency,
			(int)param->sup_to,
			(int)param->clk_accuracy
		  );

    app_env.conhdl = param->conhdl;     // Store the connection handle
    
    app_disc_enable_prf(param->conhdl);
    app_basc_enable_prf(param->conhdl);
    app_scppc_enable_prf(param->conhdl);
    app_hogprh_enable_prf(param->conhdl);
}


/*
 * Name         : app_disconnect_func - Configure app at disconnection 
 *
 * Scope        : PUBLIC
 *
 * Arguments    : task_id - id of the kernel task calling this function
 *                param - parameters passed from the stack
 *
 * Description  : Configures keyboard application when connection is terminated.
 *
 * Returns      : void
 *
 */
void app_disconnect_func(ke_task_id_t const task_id, struct gapc_disconnect_ind const *param)
{
    arch_printf("** Clear param update timer\r\n");
    ke_timer_clear(APP_HID_TIMER, task_id);
    
    // Call test code here
    stop_kbd_single_test();
}


/*
 * Name         : app_param_update_func - Request update of connection params 
 *
 * Scope        : PUBLIC
 *
 * Arguments    : task_id - id of the kernel task calling this function
 *                param - parameters passed from the stack
 *
 * Description  : After connection and, optionally, pairing is completed, this function 
 *                is called to (optionally) modify the connection parameters.
 *
 * Returns      : void
 *
 */
void app_param_update_func(void)
{
    //Set a timer to update connection params after i.e. 10sec
    ke_timer_set(APP_HID_TIMER, TASK_APP, 100);
    arch_printf("** Set param update timer to 1 sec\r\n");
    
#ifndef MITM_REQUIRED
    // Call test code here    
#endif    
}


/*
 * Name         : app_hid_timer_handler - Handler of the HID Timer 
 *
 * Scope        : PUBLIC
 *
 * Arguments    : <various>
 *
 * Description  : Sends a Connection Parameters Update Request to the Host
 *
 * Returns      : KE_MSG_CONSUMED
 *
 */
int app_hid_timer_handler(ke_msg_id_t const msgid,
                           void const *param,
                           ke_task_id_t const dest_id,
                           ke_task_id_t const src_id)
{
//	arch_puts("app_hid_timer_handler()\r\n");
	
    ke_state_t app_state = ke_state_get(TASK_APP);
    
	// Modify Conn Params
	if (app_state == APP_SECURITY || app_state == APP_PARAM_UPD || app_state == APP_CONNECTED) 
	{
		struct gapc_param_update_cmd * req = KE_MSG_ALLOC(GAPC_PARAM_UPDATE_CMD, TASK_GAPC, TASK_APP, gapc_param_update_cmd);

        req->operation = GAPC_UPDATE_PARAMS;
		// Fill in the parameter structure
		req->params.intv_min = 8;	    // N * 1.25ms
		req->params.intv_max = 8;	    // N * 1.25ms
		req->params.latency  = 23;		// Conn Events skipped
		req->params.time_out = 200;		// N * 10ms
		arch_puts("Send GAP_PARAM_UPDATE_REQ\r\n");
		ke_msg_send(req);
        
        ke_state_set(TASK_APP, APP_PARAM_UPD);
	}
	
    // Keyboard test can start here!
    uint32_t press = 100000;    // 100 msec
    uint32_t release = 235000;  // 235 msec
    uint32_t press_incr = 0;    // 0 usec
    uint32_t release_incr = 10; // 10 usec
    uint32_t limit = 275000;    // 275 msec
    
    if ( !start_kbd_single_test(press, press_incr, release, release_incr, limit) ) {
        arch_puts("### Failed to start test!\r\n");
    } else {
        arch_printf("### Started test (press: %d, incr: %d, release: %d, incr: %d, limit: %d)\r\n",
            press, press_incr, release, release_incr, limit);
    }
            
	return (KE_MSG_CONSUMED);
}


/*
 * Name         : app_mitm_passcode_entry - NULL 
 *
 * Scope        : PUBLIC
 *
 * Arguments    : src_id - id of the kernel task caused this to happen
 *                dest_id - id of the kernel task that received the message
 *
 * Description  : Starts keyboard scanning and sets the keyboard to passcode entry mode
 *
 * Returns      : void
 *
 */
void app_mitm_passcode_entry(ke_task_id_t const src_id, ke_task_id_t const dest_id)
{
}


/*
 * Name         : app_adv_func - NULL
 *
 * Scope        : PUBLIC
 *
 * Arguments    : cmd - message to GAPM 
 *
 * Description  : If the advertising and scan response data set by app_adv_start are not correct
 *                for this application, this function overwrites them with the ones prepared in 
 *                app_set_adv_data().
 *                Note that this costs in retention memory (since the data are not prepared
 *                dynamically but are stored in the retention memory). (FIXME)
 *
 * Returns      : none
 *
 */
void app_adv_func(struct gapm_start_advertise_cmd *cmd)
{
}


/*
 * Name         : app_db_init_func - NULL
 *
 * Scope        : PUBLIC
 *
 * Arguments    : none
 *
 * Description  : Initializes the database for the keyboard application.
 *
 * Returns      : true
 *
 */
bool app_db_init_func(void)
{
#if DEVELOPMENT_DEBUG
    __asm("BKPT #0\n"); // we should not be in here!        
#endif    

    return true; 
}



/*
 * HOGPRH Handlers
 ****************************************************************************************
 */
 
int hogprh_enable_cfm_handler(ke_msg_id_t const msgid,
                                    struct hogprh_enable_cfm const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    uint8_t i;
    
    switch(param->status) 
    {
        case PRF_ERR_OK:
            break;
        case PRF_ERR_STOP_DISC_CHAR_MISSING:
            arch_puts("HOGPRH: Missing characteristic!\r\n");
            break;
        case PRF_ERR_MULTIPLE_SVC:
            arch_puts("HOGPRH: Multiple services!\r\n");
            break;
        default:
            break;
    }
    
    if(param->status == PRF_ERR_OK)
    {
        // Keep read values somewhere
        hids_nb = param->hids_nb;
        for (i = 0; i < hids_nb; i++)
        {
            hids[i] = param->hids[i];
        }
        
        if (hids_nb)
        {
            // Read the Report Map Characteristic now
            struct hogprh_rd_char_req *req = KE_MSG_ALLOC(HOGPRH_RD_CHAR_REQ,
                                                       TASK_HOGPRH, TASK_APP,
                                                       hogprh_rd_char_req);

            // Fill in the parameter structure
            req->conhdl             = app_env.conhdl;
            req->hids_nb            = 0; // check
            req->report_nb          = 0;
            req->read_code          = HOGPRH_RD_HIDS_REPORT_MAP;

            // Send the message
            ke_msg_send(req);
        }
    }

    return (KE_MSG_CONSUMED);
}


int hogprh_err_rsp_handler(ke_msg_id_t const msgid,
                                    struct hogprh_char_req_rsp const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    arch_printf("HOGPRH: Error for msgid=%d, conhdl=%d, att_code=%d\r\n", msgid, param->conhdl, param->att_code);
    
    switch(param->status)
    {
        case PRF_ERR_INEXISTENT_HDL:
            arch_puts("Handle not found!\r\n");
            break;
        case PRF_ERR_INVALID_PARAM:
            arch_puts("Invalid param!\r\n");
            break;
        case PRF_ERR_NOT_WRITABLE:
            arch_puts("Not writable!\r\n");
            break;
        default:
            arch_printf("--> Status=%d\r\n", param->status);
            if ( (param->status == ATT_ERR_INSUFF_AUTHEN) || (param->status == ATT_ERR_INSUFF_ENC) ){
#if MITM_REQUIRED
                struct gapc_bond_cmd *req = KE_MSG_ALLOC(GAPC_BOND_CMD,
                                                           KE_BUILD_ID(TASK_GAPC, app_env.conidx), TASK_APP,
                                                           gapc_bond_cmd);

                // Fill in the parameter structure
                req->operation          = GAPC_BOND;
                // OOB information
                req->pairing.oob        = GAP_OOB_AUTH_DATA_NOT_PRESENT;
                // Encryption key size
                req->pairing.key_size   = KEY_LEN;
                // IO capabilities
                req->pairing.iocap      = GAP_IO_CAP_KB_DISPLAY;
                // Authentication requirements
                req->pairing.auth       = GAP_AUTH_REQ_MITM_BOND;
                //Security requirements
                req->pairing.sec_req    = GAP_SEC1_AUTH_PAIR_ENC;
                //Initiator key distribution
                req->pairing.ikey_dist  = GAP_KDIST_SIGNKEY;
                //Responder key distribution
                req->pairing.rkey_dist  = GAP_KDIST_ENCKEY;

                // Send the message
                ke_msg_send(req);
#else
                struct gapc_bond_cmd *req = KE_MSG_ALLOC(GAPC_BOND_CMD,
                                                           KE_BUILD_ID(TASK_GAPC, app_env.conidx), TASK_APP,
                                                           gapc_bond_cmd);

                // Fill in the parameter structure
                req->operation          = GAPC_BOND;
                // OOB information
                req->pairing.oob        = GAP_OOB_AUTH_DATA_NOT_PRESENT;
                // Encryption key size
                req->pairing.key_size   = KEY_LEN;
                // IO capabilities
                req->pairing.iocap      = GAP_IO_CAP_KB_DISPLAY;
                // Authentication requirements
                req->pairing.auth       = GAP_AUTH_REQ_NO_MITM_BOND;
                //Security requirements
                req->pairing.sec_req    = GAP_NO_SEC;
                //Initiator key distribution
                req->pairing.ikey_dist  = GAP_KDIST_SIGNKEY;
                //Responder key distribution
                req->pairing.rkey_dist  = GAP_KDIST_ENCKEY;

                // Send the message
                ke_msg_send(req);
#endif
            }
            break;
    }
    
    return (KE_MSG_CONSUMED);
}


uint8_t prev_rep[8];
int hogprh_report_ind_handler(ke_msg_id_t const msgid,
                                    struct hogprh_report_ind const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    int i;
    
//    arch_printf("HOGPRH: Report (conhdl=%d, hids_nb=%d, report_nb=%d, type=%d)\r\n", 
//                param->conhdl, param->hids_nb, param->report_nb, param->ind_type);
//    arch_printf("        Data: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\r\n",
//                    param->report[0],
//                    param->report[1],
//                    param->report[2],
//                    param->report[3],
//                    param->report[4],
//                    param->report[5],
//                    param->report[6],
//                    param->report[7]);

    // Call test code here
    
    // if all data are equal zero then it is a release report else a press
    for (i = 0; i < 8; i++)
        if (param->report[i] != 0)
            break;
            
    if (i < 8) { // press report
        int ret;
        
        ret = report_press();
        if (ret) 
        {
            arch_printf("Report (#%d): 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\r\n",
                            ret,
                            param->report[0],
                            param->report[1],
                            param->report[2],
                            param->report[3],
                            param->report[4],
                            param->report[5],
                            param->report[6],
                            param->report[7]);
            arch_printf("Previous    : 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\r\n",
                            prev_rep[0],
                            prev_rep[1],
                            prev_rep[2],
                            prev_rep[3],
                            prev_rep[4],
                            prev_rep[5],
                            prev_rep[6],
                            prev_rep[7]);
        }
    } else {     // release report
        int ret;
        
        ret = report_release();
        if (ret) 
        {
            arch_printf("Report (#%d): 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\r\n",
                            ret,
                            param->report[0],
                            param->report[1],
                            param->report[2],
                            param->report[3],
                            param->report[4],
                            param->report[5],
                            param->report[6],
                            param->report[7]);
            arch_printf("Previous    : 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\r\n",
                            prev_rep[0],
                            prev_rep[1],
                            prev_rep[2],
                            prev_rep[3],
                            prev_rep[4],
                            prev_rep[5],
                            prev_rep[6],
                            prev_rep[7]);
        }
    }
    memcpy(prev_rep, param->report, 8);
                    
    return (KE_MSG_CONSUMED);
}


int hogprh_disable_ind_handler(ke_msg_id_t const msgid,
                                    struct prf_client_disable_ind const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    // Flush everything in this simple configuration
    hids_nb = 0;
    
    return (KE_MSG_CONSUMED);
}


int hopgrh_report_map_rd_rsp_handler(ke_msg_id_t const msgid,
                                    struct hogprh_report_map_rd_rsp const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    arch_puts("Got Report Map from the Device Under Test\r\n");
    
    return (KE_MSG_CONSUMED);
}

int gapc_param_update_req_ind_handler(ke_msg_id_t const msgid,
                                    struct gapc_param_update_req_ind const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    struct gapc_param_update_cfm *req = KE_MSG_ALLOC(GAPC_PARAM_UPDATE_CFM,
                                               KE_BUILD_ID(TASK_GAPC, app_env.conidx), TASK_APP,
                                               gapc_param_update_cfm);
    
    req->accept = false;
    
    // Send the message
    ke_msg_send(req);
    
    return (KE_MSG_CONSUMED);
}
    
