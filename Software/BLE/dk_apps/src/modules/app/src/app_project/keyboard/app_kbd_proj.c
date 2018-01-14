/**
 ****************************************************************************************
 *
 * @file app_kbd_proj.c
 *
 * @brief HID Keyboard hooks.
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

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */
 
#include "rwip_config.h"

#if (BLE_APP_PRESENT)

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"
#include "arch.h"
#include "app.h" 
#include "app_task.h"
#include "app_api.h"
#include "gapm_task.h"
#include "gapm_util.h"
#include "gattc_task.h"
#include "gapc.h"
#include "llc_util.h"
#include "rwble_hl_config.h"

#include "app_kbd.h"
#include "app_kbd_proj.h"
#include "app_kbd_key_matrix.h"
#include "app_kbd_fsm.h"
#include "app_kbd_leds.h"
#include "app_kbd_debug.h"
#include "i2c_eeprom.h"
#include "app_multi_bond.h"
#include "app_white_list.h"
#include "app_console.h"
#include "arch_sleep.h"
#include "gpio.h"
#include "nvds.h"

#include "app_sec.h"
#include "app_dis.h"
#include "app_batt.h"

#if DEVELOPMENT_DEBUG
#warning "Put manually APP_DISS_PNP_PRODUCT_VERSION in ADV data. Change \x00\x01 (0x0100) to a proper value!"
#endif

#if (HAS_VIRTUAL_WHITE_LIST && HAS_WHITE_LIST)
#error "Please choose one White List, either normal (WHITE_LIST_ON) or virtual (VIRTUAL_WHITE_LIST_ON)."
#endif

extern struct gap_cfg_table_struct gap_timeout_table;

/*
 * LOCAL VARIABLES
 ****************************************************************************************
 */

uint8_t app_adv_data_length                  __attribute__((section("retention_mem_area0"), zero_init));        // Advertising data length
uint8_t app_adv_data[ADV_DATA_LEN-3]         __attribute__((section("retention_mem_area0"), zero_init));        // Advertising data
uint8_t app_scanrsp_data_length              __attribute__((section("retention_mem_area0"), zero_init));        // Scan response data length
uint8_t app_scanrsp_data[SCAN_RSP_DATA_LEN]  __attribute__((section("retention_mem_area0"), zero_init));        // Scan response data
struct bonding_info_ bond_info               __attribute__((section("retention_mem_area0"), zero_init));        // Bonding info for current host
ke_task_id_t mitm_src_id, mitm_dest_id;



/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
*/
static void app_set_adv_data(void);
void app_spotar_callback(const uint8_t);


/**
 ****************************************************************************************
 * @brief       Initialize the application   
 *              Initialize state, GPIOs. Set advertising data. Set sleep mode.
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_init_func(void)
{
    app_keyboard_init();        // Initialize Keyboard env
    
    app_alt_pair_init();        // Initialize Multi-Bonding (if applicable)
    
    app_set_adv_data();         // Prepare Advertising data

    app_dis_init();             // Initialize Device Information Service
    
#if (BLE_SPOTA_RECEIVER)    
	app_spotar_init(app_spotar_callback);
#endif
    
  //  app_set_extended_sleep();
    app_disable_sleep();
}


/**
 ****************************************************************************************
 * @brief       Configures Bluetooth settings
 *
 * @param[in]   task_id - id of the kernel task calling this function
 * @param[in]   cmd - parameters to pass to the stack
 *
 * @return  void
 ****************************************************************************************
 */
void app_configuration_func(ke_task_id_t const task_id, struct gapm_set_dev_config_cmd *cmd)
{
    // set device configuration
    cmd->operation = GAPM_SET_DEV_CONFIG;
    // Device Role
    cmd->role = GAP_PERIPHERAL_SLV;
    // Device Appearance
    cmd->appearance = 961;          // Keyboard
    // Device Appearance write permission requirements for peer device
    cmd->appearance_write_perm = GAPM_WRITE_DISABLE;
    // Device Name write permission requirements for peer device
    cmd->name_write_perm = GAPM_WRITE_DISABLE;
    // Slave preferred Minimum of connection interval
    cmd->con_intv_min = 6;         // 7.5ms (6*1.25ms)
    // Slave preferred Maximum of connection interval
    cmd->con_intv_max = 16;        // 20ms (16*1.25ms)
    // Slave preferred Connection latency
    cmd->con_latency  = 31;
    // Slave preferred Link supervision timeout
    cmd->superv_to    = 200;

    // Device IRK
    memcpy(cmd->irk.key, "0123456789012345", KEY_LEN);
    
    // Privacy settings bit field (0b1 = enabled, 0b0 = disabled)
    //  - [bit 0]: Privacy Support
    //  - [bit 1]: Multiple Bond Support (Peripheral only); If enabled, privacy flag is
    //             read only.
    //  - [bit 2]: Reconnection address visible.
    if (HAS_PRIVACY)
    {
        cmd->flags = GAPM_CFG_VISIBLE_RECON_ADDR_EN | GAPM_CFG_PRIVACY_EN;
    }
    else
    {
        cmd->flags = 0;
    }

    if (HAS_PRIVACY)
    {
        // Set the reconnection address if privacy is set for this fearure
//        gapm_set_recon_addr(&my_addr);
    }
    
    if (HAS_KEYBOARD_LEDS)
    {
        // This is a good place to initialize the LEDs
        leds_init();
    }
}


/**
 ****************************************************************************************
 * @brief       Handles what needs to be done after the completion of the configuration phase
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_set_dev_config_complete_func(void)
{
    // We are now in Initialization State
    ke_state_set(TASK_APP, APP_DB_INIT);

    // Add the first required service in the database
    if (app_db_init())
    {
        app_state_update(NO_EVENT);
    }
}


/**
 ****************************************************************************************
 * @brief   Handles what needs to be done after the completion of the initialization
 *          of all modules
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_db_init_complete_func(void)
{
    app_state_update(NO_EVENT);
}


/**
 ****************************************************************************************
 * @brief   Handles what needs to be done after Undirected advertising finishes
 *
 * @param[in]   status
 *
 * @return  void
 ****************************************************************************************
 */
void app_adv_undirect_complete(uint8_t status)
{
    if ( (status != GAP_ERR_NO_ERROR) && (status != GAP_ERR_CANCELED) )
    {
        ASSERT_ERROR(0); // unexpected error
    }
    
    if (status == GAP_ERR_CANCELED)
    {
        if (ke_state_get(TASK_APP) == APP_CONNECTABLE)
            app_state_update(TIMER_EXPIRED_EVT);
    }
}


/**
 ****************************************************************************************
 * @brief   Handles what needs to be done after Directed advertising finishes
 *
 * @param[in]   status
 *
 * @return  void
 ****************************************************************************************
 */
void app_adv_direct_complete(uint8_t status)
{
    if ( (status != GAP_ERR_NO_ERROR) && (status != GAP_ERR_TIMEOUT) )
    {
        ASSERT_ERROR(0); // unexpected error
    }
    
    if (status == GAP_ERR_TIMEOUT)
    {
        if (ke_state_get(TASK_APP) == APP_CONNECTABLE)
            app_state_update(TIMER_EXPIRED_EVT);
    }
}

/**
 ****************************************************************************************
 * @brief   Initialize security
 *
 * @param   status
 *
 * @return  void
 ****************************************************************************************
 */
void app_sec_init_func(void)
{

}


/**
 ****************************************************************************************
 * @brief   Prepares and sends the reply to the GAPC_PAIRING_REQ msg
 *
 * @param[in] param  The message from the host
 *
 * @return  void
 ****************************************************************************************
 */
void app_send_pairing_rsp_func(struct gapc_bond_req_ind *param)
{
    dbg_puts(DBG_CONN_LVL, "    GAPC_PAIRING_REQ ind\r\n");
    
    struct gapc_bond_cfm* cfm = KE_MSG_ALLOC(GAPC_BOND_CFM, TASK_GAPC, TASK_APP, gapc_bond_cfm);

    cfm->request = GAPC_PAIRING_RSP;
    
    // Feature check cannot be perfomed by the application. For example, the Host may have sent the
    // LL_CONNECT_REQ with No MITM and Bond. Although we require MITM and Bond we should reply. It is
    // left to the Host to check whether the requirements can be met and the Pairing procedure should
    // proceed or not.

    cfm->accept = true;
    
    if (current_fsm_state == CONNECTED_ST)
    {
        // special case: got a PAIRING_REQ when already paired and connected! Do not clear bond_info
        // in this case or else the DB will be corrupted (i.e. the notifications will be disabled
        // while the host had them enabled and won't re-enable them after the 2nd Pairing...).
    }
    else
    {
        // Clear bond data of previous connection (if any)
        memset(&bond_info, 0, sizeof(struct bonding_info_));
        bond_info.info = BOND_INFO_DEFAULT_VAL;
        updatedb_from_bonding_info(&bond_info);
        reset_active_peer_pos();
    }
    
    app_state_update(PAIRING_REQ_EVT);
    
    // OOB information
    cfm->data.pairing_feat.oob              = GAP_OOB_AUTH_DATA_NOT_PRESENT;
    // Encryption key size
    cfm->data.pairing_feat.key_size         = KEY_LEN;

    if (HAS_MITM)
    {
        // Authentication requirements
        // We do not return NO_BOND even if an EEPROM is not present. The reason is that
        // the Host may drop the Pairing procedure in case it requires bonding (i.e. Windows do that).
        // Of course, since an EEPROM is not present we fake we are bonding. We expect this to be used
        // in test or controlled environments only though.
            cfm->data.pairing_feat.auth     = GAP_AUTH_REQ_MITM_BOND;
        // IO capabilities
        cfm->data.pairing_feat.iocap        = GAP_IO_CAP_KB_ONLY;
        //Security requirements
        cfm->data.pairing_feat.sec_req      = GAP_SEC1_AUTH_PAIR_ENC;
    }
    else
    {
        // Authentication requirements
        // See above about NO_BOND...
            cfm->data.pairing_feat.auth     = GAP_AUTH_REQ_NO_MITM_BOND;
        // IO capabilities
        cfm->data.pairing_feat.iocap        = GAP_IO_CAP_NO_INPUT_NO_OUTPUT;    // Make sure that MITM won't be selected
        //Security requirements
        cfm->data.pairing_feat.sec_req      = GAP_SEC1_NOAUTH_PAIR_ENC;
    }
    
    //Initiator key distribution
    if ( (app_env.peer_addr_type == ADDR_RAND) && ((app_env.peer_addr.addr[BD_ADDR_LEN - 1] & 0xC0) == SMPM_ADDR_TYPE_PRIV_RESOLV) )
        cfm->data.pairing_feat.ikey_dist    = GAP_KDIST_IDKEY;
    else
        cfm->data.pairing_feat.ikey_dist    = GAP_KDIST_NONE;
    //Responder key distribution
    cfm->data.pairing_feat.rkey_dist        = GAP_KDIST_ENCKEY;

    ke_msg_send(cfm);
}


/**
 ****************************************************************************************
 * @brief   N/A for this application
 *
 * @param[in] param  The message from the host
 *
 * @return  void
 ****************************************************************************************
 */
void app_send_tk_exch_func(struct gapc_bond_req_ind *param)
{  
}


/**
 ****************************************************************************************
 * @brief   Starts keyboard scanning and sets the keyboard to passcode entry mode
 *
 * @param[in] src_id  id of the kernel task which caused this to happen
 * @param[in] dest_id id of the kernel task which received the message
 *
 * @return  void
 ****************************************************************************************
 */
void app_mitm_passcode_entry_func(ke_task_id_t const src_id, ke_task_id_t const dest_id)
{
    dbg_puts(DBG_CONN_LVL, "    GAPC_TK_EXCH\r\n");
    
    // store task IDs
    mitm_src_id = src_id;
    mitm_dest_id = dest_id;

    app_kbd_start_passcode();               // Set to 'Passcode' mode until the connection is established        
}


/**
 ****************************************************************************************
 * @brief   Sends the passcode that the user entered to the Host
 *
 * @param[in] code  The code to report to the Host
 *
 * @return  void
 ****************************************************************************************
 */
void app_mitm_passcode_report(uint32_t code)
{
    struct gapc_bond_cfm* cfm = KE_MSG_ALLOC(GAPC_BOND_CFM, mitm_src_id, mitm_dest_id, gapc_bond_cfm);
    
    cfm->request = GAPC_TK_EXCH;
    cfm->accept = true;

    memset(cfm->data.tk.key, 0, KEY_LEN);

    cfm->data.tk.key[3] = (uint8_t)((code & 0xFF000000) >> 24);
    cfm->data.tk.key[2] = (uint8_t)((code & 0x00FF0000) >> 16);
    cfm->data.tk.key[1] = (uint8_t)((code & 0x0000FF00) >>  8);
    cfm->data.tk.key[0] = (uint8_t)((code & 0x000000FF) >>  0);

    ke_msg_send(cfm);

    dbg_printf(DBG_CONN_LVL, "Code: %d\n\r", code);
    
    app_kbd_start_reporting();          // start sending notifications    
}


/**
 ****************************************************************************************
 * @brief   N/A for this application
 *
 * @param[in] param  The message from the host
 *
 * @return  void
 ****************************************************************************************
 */
void app_send_irk_exch_func(struct gapc_bond_req_ind *param)
{
}


/**
 ****************************************************************************************
 * @brief   N/A for this application
 *
 * @param[in] param  The message from the host
 *
 * @return  void
 ****************************************************************************************
 */
void app_send_csrk_exch_func(struct gapc_bond_req_ind *param)
{
}


/**
 ****************************************************************************************
 * @brief   LTK exchange - Handles the exchange of the LTK with the Host
 *
 * @param[in] param  The message from the host
 *
 * @return  void
 ****************************************************************************************
 */
void app_send_ltk_exch_func(struct gapc_bond_req_ind *param)
{
    struct gapc_bond_cfm* cfm = KE_MSG_ALLOC(GAPC_BOND_CFM, TASK_GAPC, TASK_APP, gapc_bond_cfm);

    dbg_puts(DBG_CONN_LVL, "--> GAPC_LTK_EXCH\r\n");
    
    // generate ltk
    app_sec_gen_ltk(param->data.key_size);

    cfm->request = GAPC_LTK_EXCH;

    cfm->accept = true;

    cfm->data.ltk.key_size = bond_info.env.key_size;
    cfm->data.ltk.ediv = bond_info.env.ediv;

    memcpy(&(cfm->data.ltk.randnb), &(bond_info.env.rand_nb), RAND_NB_LEN);
    memcpy(&(cfm->data.ltk.ltk), &(bond_info.env.ltk), KEY_LEN);

    ke_msg_send(cfm);
}


/**
 ****************************************************************************************
 * @brief   Pairing completed - Handles the completion of Pairing
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_paired_func(void)
{
    dbg_puts(DBG_CONN_LVL, "    GAPC_PAIRING_SUCCEED\r\n");
    
    // We may reach this point after getting an LL_ENC_REQ from an unbonded host 
    // with EDIV and RAND set to zero. Reject the Host in case of MITM since no Pairing has been perfomed.
    if ( (bond_info.env.auth == GAP_AUTH_NONE) && HAS_MITM)
    {
        // reset bond_info
        if (HAS_MITM)
        {
            bond_info.env.auth = GAP_AUTH_REQ_MITM_BOND;
        }
        else
        {
            bond_info.env.auth = GAP_AUTH_REQ_NO_MITM_BOND;
        }
        
        // disconnect
        if (HAS_SEND_LL_TERMINATE_IND)
        {
            app_disconnect();
        }
        else
        {
            app_fake_disconnect();                    
        }
        
        return;
    }
    
    if (HAS_MULTI_BOND)
    {
        multi_bond_enabled = 0;
        ke_timer_clear(APP_ALT_PAIR_TIMER, TASK_APP);
    }
    
    if (bond_info.env.auth & GAP_AUTH_BOND)
    {
        app_alt_pair_store_bond_data();
    }
            
    app_param_update_start();
}


/**
 ****************************************************************************************
 * @brief   Check if encryption will be accepted (LL_ENC_REQ received but not when in PAIRING mode)
 *          for this link
 *
 * @param[in] param  The message from the host
 *
 * @return  true, if everything is ok
 *          false, if the request is rejected
 ****************************************************************************************
 */
bool app_validate_encrypt_req_func(struct gapc_encrypt_req_ind *param)
{
    if (HAS_EEPROM)
    {
        if(((bond_info.env.auth & GAP_AUTH_BOND) != 0)
            && (memcmp(&(bond_info.env.rand_nb), &(param->rand_nb), RAND_NB_LEN) == 0)
            && (bond_info.env.ediv == param->ediv))
        {
            if (HAS_MULTI_BOND)
            {
                // the connecting host is the last host we connected to
                if (multi_bond_enabled)
                {
                    return false;
                }
            }
            // if it's not blocked then no EEPROM access is required to load keys.
            updatedb_from_bonding_info(&bond_info);
            if (update_usage_count(bond_info.env.nvds_tag & 0xF))
            {
                i2c_eeprom_write_data((uint8_t *)&bond_usage, EEPROM_USAGE_ADDR, sizeof(struct usage_array_));
            }
        }
        else if (app_alt_pair_load_bond_data(&param->rand_nb, param->ediv) == 1)
            app_state_update(NEW_HOST_EVT);
    }
    else
    {
        if(((bond_info.env.auth & GAP_AUTH_BOND) != 0)
            && (memcmp(&(bond_info.env.rand_nb), &(param->rand_nb), RAND_NB_LEN) == 0)
            && (bond_info.env.ediv == param->ediv))
        {
            // bond_info.info is OK
        }
        else
            bond_info.info = BOND_INFO_DEFAULT_VAL;
        
        updatedb_from_bonding_info(&bond_info);
    }
    
    return true;
}


/**
 ****************************************************************************************
 * @brief   Received and accepted LL_ENC_REQ 
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_sec_encrypt_complete_func(void)
{
}


/**
 ****************************************************************************************
 * @brief   Received LL_START_ENC_RSP : Enryption is completed -> Start normal keyboard
 *          operation. After completion of the encryption of the link, it sets the keyboard
 *          to normal report mode
 *          
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_sec_encrypt_ind_func(void)
{
    if (HAS_MULTI_BOND)
    {
        multi_bond_enabled = 0;
        ke_timer_clear(APP_ALT_PAIR_TIMER, TASK_APP);
    }
    
    app_kbd_start_reporting();          // start sending notifications    
    
    if ( (HAS_MITM) || (HAS_EEPROM) )
        ke_timer_clear(APP_HID_ENC_TIMER, TASK_APP);

    app_param_update_start();
        
    // no need to store anything to the EEPROM
}


/**
 ****************************************************************************************
 * @brief   Configures keyboard application when connection is established.
 *          
 * @param[in] param  parameters passed from the stack
 *
 * @return  void
 ****************************************************************************************
 */
void app_connection_func(struct gapc_connection_req_ind const *param)
{
    // Check if the received Connection Handle was valid
    if (app_env.conidx != GAP_INVALID_CONIDX)
    {
       /*--------------------------------------------------------------
        * ENABLE REQUIRED PROFILES
        *-------------------------------------------------------------*/
        dbg_printf(DBG_ALL, "gap_le_create_conn_req_cmp_evt_handler() (%d, %d, %d, %d)\r\n", 
                (int)param->con_interval,
                (int)param->con_latency,
                (int)param->sup_to,
                (int)param->clk_accuracy
              );

        dbg_printf(DBG_ALL, "Peer addr %02x:%02x:%02x:%02x:%02x:%02x\r\n", 
            param->peer_addr.addr[0], param->peer_addr.addr[1], param->peer_addr.addr[2], 
            param->peer_addr.addr[3], param->peer_addr.addr[4], param->peer_addr.addr[5]);
        
        app_state_update(CONN_REQ_EVT);
                
        ke_state_set(TASK_APP, APP_CONNECTED);  // Update TASK_APP state (MUST always be done!)

        if (HAS_VIRTUAL_WHITE_LIST)
        {
            // Public and Static Random Addresses are checked here. Resolvable Random Addresses are checked after
            // they are resolved.
            if (   (ADDR_PUBLIC == param->peer_addr_type)
                || ( (param->peer_addr_type == ADDR_RAND) && ((param->peer_addr.addr[5] & SMPM_ADDR_TYPE_STATIC) == SMPM_ADDR_TYPE_STATIC) ) )
            {
                if (!lookup_public_in_virtual_white_list(param->peer_addr_type, &param->peer_addr))
                {
                    app_disconnect();
                    return;
                }
            }
        }
        
        if (HAS_MULTI_BOND)
        {
            if (app_alt_pair_check_peer((struct bd_addr *) &param->peer_addr, param->peer_addr_type) == false)
                return;            
        }
        // Connection is accepted!
        
        app_env.conhdl = param->conhdl;         // Store the connection handle

        // Enable DIS for this conhdl
#ifdef MITM_ON
        app_dis_enable_prf_sec(param->conhdl, PERM(SVC, AUTH));     
#else
# ifdef EEPROM_ON        
        app_dis_enable_prf_sec(param->conhdl, PERM(SVC, UNAUTH));
# else        
        app_dis_enable_prf_sec(param->conhdl, PERM(SVC, ENABLE));
# endif        
#endif                                                   
                
        // Enable BATT for this conhdl (with fake levels)
#ifdef MITM_ON
        app_batt_enable_prf_sec(PERM(SVC, AUTH), 99, true, GPIO_PORT_0, GPIO_PIN_0);
#else
# ifdef EEPROM_ON        
        app_batt_enable_prf_sec(PERM(SVC, UNAUTH), 99, true, GPIO_PORT_0, GPIO_PIN_0);
# else        
        app_batt_enable_prf_sec(PERM(SVC, ENABLE), 99, true, GPIO_PORT_0, GPIO_PIN_0);
# endif        
#endif                                                   
        app_batt_poll_start(BATTERY_LEVEL_POLLING_PERIOD/10);  // Start polling
        
        app_keyboard_enable();                  // Enable HOGPD for this conhdl
                
        // Retrieve the connection info from the parameters (MUST always be done!)
        app_env.conhdl = param->conhdl;
        app_env.peer_addr_type = param->peer_addr_type;
        app_env.peer_addr = param->peer_addr;
        
        multi_bond_resolved_peer_pos = 0;
                
#if (BLE_SPOTA_RECEIVER)
        app_spotar_enable();
#endif //BLE_SPOTA_RECEIVER
        
# if (BLE_APP_SEC)
        // send connection confirmation
        app_connect_confirm(bond_info.env.auth);
# else
        // send connection confirmation
        app_connect_confirm(GAP_AUTH_REQ_NO_MITM_NO_BOND);            
# endif

        if ( (HAS_MITM) || (HAS_EEPROM) )
        {
            // set a timer in case pairing/encyption does not follow
            app_timer_set(APP_HID_ENC_TIMER, TASK_APP, (KBD_ENC_SAFEGUARD_TIMEOUT / 10));
            dbg_puts(DBG_FSM_LVL, "  (+) HID ENC timer\r\n");

            if (HAS_KEYBOARD_LEDS)
            {
                leds_set_connection_in_progress();
            }
            
            if (HAS_SECURITY_REQUEST_SEND)
            {
                if ( (app_env.peer_addr_type == ADDR_RAND) && ((app_env.peer_addr.addr[BD_ADDR_LEN - 1] & 0xC0) == SMPM_ADDR_TYPE_PRIV_RESOLV) )
                {
                    //Resolve address
                    struct gapm_resolv_addr_cmd *cmd = (struct gapm_resolv_addr_cmd *)KE_MSG_ALLOC_DYN(GAPM_RESOLV_ADDR_CMD, 
                                    TASK_GAPM, TASK_APP, gapm_resolv_addr_cmd, 
                                    MAX_BOND_PEER * sizeof(struct gap_sec_key) );
                    
                    cmd->operation = GAPM_RESOLV_ADDR; // GAPM requested operation
                    cmd->nb_key = MAX_BOND_PEER; // Number of provided IRK 
                    cmd->addr = app_env.peer_addr; // Resolvable random address to solve
                    if (HAS_EEPROM)
                    {
                        if (MBOND_LOAD_IRKS_AT_INIT)
                        {
                            memcpy(cmd->irk, irk_array.irk, MAX_BOND_PEER * sizeof(struct gap_sec_key)); // Array of IRK used for address resolution (MSB -> LSB)
                        }
                        
                        if (MBOND_LOAD_INFO_AT_INIT)
                        {
                            for (int i = 0; i < MAX_BOND_PEER; i++)
                            {
                                cmd->irk[i] = bond_array[i].irk;
                            }
                        }
                    }
                    else
                    {
                        cmd->irk[0] = bond_info.irk; // Only one member in the "array", the "previous" host, if any.
                    }

                    ke_msg_send(cmd);
                }
                else
                {
                    app_security_start();
                }
            }
        }
        else
        {
            // no bonding --> connection setup proceeds directly
            app_param_update_start();
        } 
    }
    else
    {
        // No connection has been established. Update state.
        app_state_update(TIMER_EXPIRED_EVT);
    }
}


/**
 ****************************************************************************************
 * @brief   Configures the application when the connection is terminated.
 *          
 * @param[in] task_id   id of the kernel task calling this function
 * @param[in] param     parameters passed from the stack
 *
 * @return  void
 ****************************************************************************************
 */
void app_disconnect_func(ke_task_id_t const task_id, struct gapc_disconnect_ind const *param)
{
    uint8_t state = ke_state_get(task_id);

    if ( (state == APP_CONNECTED) || (state == APP_PARAM_UPD) || (state == APP_SECURITY) )
    {
        ke_timer_clear(APP_HID_TIMER, TASK_APP);
        
        if ( (HAS_MITM) || (HAS_EEPROM) )
        {
            ke_timer_clear(APP_HID_ENC_TIMER, TASK_APP);
        }
        
        app_kbd_stop_reporting();

        app_batt_poll_stop();    // stop battery polling
        
        if (HAS_KEYBOARD_LEDS)
        {
            if (HAS_MULTI_BOND)
            {
                if (!multi_bond_enabled)
                    leds_set_disconnected();
                else
                    leds_init(); // turn off
            }
            else
            {
                leds_set_disconnected();
            }
        }
        user_disconnection_req = false; // clear any multi-bond switch
        
        app_state_update(DISCONN_EVT);
        
        ke_state_set(TASK_APP, APP_CONNECTABLE); // APP_CONNECTABLE means "Idle"
    }
    // There is an extreme case where this message is received twice. This happens when 
    // both the device and the host decide to terminate the connection at the same time.
    // In this case, when the device sends the LL_TERMINATE_IND, it also gets the same
    // message from the Host. This is not an erroneous situation provided that the 
    // device has already cleared its state.
    //    else
    //        ASSERT_ERROR(0);
}


/**
 ****************************************************************************************
 * @brief   Handler for the message sent by app_fake_disconnect() function
 *          Used to bypasses the normal disconnection procedure, in order to avoid sending the 
 *          LL_TERMINATE_IND. Used only when the HAS_SEND_LL_TERMINATE_IND is not defined
 *          
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 ****************************************************************************************
 */
int app_disconnect_cmd_handler(ke_msg_id_t const msgid,
        void const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    if (HAS_SEND_LL_TERMINATE_IND)
    {
        ASSERT_WARNING(0);
    }
    else
    {
        uint8_t status = CO_ERROR_COMMAND_DISALLOWED;

        switch(ke_state_get(dest_id))
        {
            case LLC_FREE:
            case LLC_DISC:
            case LLC_STOPPING:
                break;
            case LLCP_WAIT_ACK:
                return (KE_MSG_SAVED);
            default:
                // Termination procedure can be started at any time
                status = CO_ERROR_NO_ERROR;
                // Execute the termination procedure
                llc_util_dicon_procedure(app_env.conhdl, CO_ERROR_REMOTE_USER_TERM_CON); 
                break;
        }

        llc_common_cmd_status_send(LLC_DISCON_STAT_EVT, src_id, dest_id, status, app_env.conhdl);
    }

    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief   Bypasses the normal disconnection procedure, in order to avoid sending the 
 *          LL_TERMINATE_IND. Used only when the HAS_SEND_LL_TERMINATE_IND is not defined.
 *          
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_fake_disconnect(void)
{
    if (HAS_SEND_LL_TERMINATE_IND)
    {
        ASSERT_WARNING(0);
    }
    else
    {
        ke_msg_send_basic(APP_TERMINATE_CONN_MSG, TASK_APP, TASK_APP);
    }
}


/**
 ****************************************************************************************
 * @brief   Requests update of connection params
 *          After connection and, optionally, pairing is completed, this function 
 *          is called to (optionally) modify the connection parameters.
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_param_update_func(void)
{
    if (!HAS_MITM)
    {
        app_kbd_start_reporting();          // start sending notifications   
        // Clear all buffers if reporting "old" keys is not wanted
        if (!HAS_REPORT_HISTORY)
        {
            app_kbd_flush_buffer();
            app_kbd_flush_reports();
        }
        app_kbd_prepare_keyreports();       // prepare any (if required)
    }
    
    if (HAS_KEYBOARD_LEDS)
    {
        leds_set_connection_established();
    }
    
    app_state_update(CONN_CMP_EVT);
    
    if (HAS_KBD_SWITCH_TO_PREFERRED_CONN_PARAMS)
    {
        //Set a timer to update connection params
        app_timer_set(APP_HID_TIMER, TASK_APP, (TIME_TO_REQUEST_PARAM_UPD / 10));
        dbg_puts(DBG_CONN_LVL, "** Set param update timer\r\n");
    }
}


/**
 ****************************************************************************************
 * @brief   Handles the rejection of connection params from the host
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_update_params_rejected_func(uint8_t status)
{
    if (status != GAP_ERR_REJECTED)
    {
        ASSERT_INFO(0, param->status, APP_PARAM_UPD);

        // Disconnect
        app_disconnect();
    }
    else
    {
        // it's application specific what to do when the Param Upd request is rejected
        
        // Go to Connected State
        ke_state_set(TASK_APP, APP_CONNECTED);
        app_state_update(CONN_CMP_EVT);
    }
}


/**
 ****************************************************************************************
 * @brief   Handles the completion of connection params update
 *          Actions taken after the reception of the host's reply to a connection update 
 *          params we sent.
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_update_params_complete_func(void)
{
    app_state_update(CONN_CMP_EVT);
}


/**
 ****************************************************************************************
 * @brief   Sets the advertising and the scan response data
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
static void app_set_adv_data(void)
{
    int8_t device_name_length;      // Device Name Length

    /*-----------------------------------------------------------------------------
     * Set the Advertising Data
     *-----------------------------------------------------------------------------*/
    #if (NVDS_SUPPORT)
    if(nvds_get(NVDS_TAG_APP_BLE_ADV_DATA, &app_adv_data_length,
                &app_adv_data[0]) != NVDS_OK)
    #endif //(NVDS_SUPPORT)
    {
        app_adv_data_length = APP_ADV_DATA_LENGTH;
        memcpy(&app_adv_data[0], APP_ADV_DATA, app_adv_data_length);
    }
        
    /*-----------------------------------------------------------------------------
     * Set the Scan Response Data
     *-----------------------------------------------------------------------------*/
    #if (NVDS_SUPPORT)
    if(nvds_get(NVDS_TAG_APP_BLE_SCAN_RESP_DATA, &app_scanrsp_data_length,
                &app_scanrsp_data[0]) != NVDS_OK)
    #endif //(NVDS_SUPPORT)
    {
        app_scanrsp_data_length = APP_SCNRSP_DATA_LENGTH;
        if (app_scanrsp_data_length > 0) 
            memcpy(&app_scanrsp_data[0], APP_SCNRSP_DATA, app_scanrsp_data_length);
    }

#ifdef APP_DFLT_DEVICE_NAME
    /*-----------------------------------------------------------------------------
     * Add the Device Name in the Advertising Data
     *-----------------------------------------------------------------------------*/
    // Get available space in the Advertising Data
    device_name_length = APP_ADV_DATA_MAX_SIZE - app_adv_data_length - 2;

    // Check if data can be added to the Advertising data
    if (device_name_length > 0)
    {
        // Get default Device Name (No name if not enough space)
        int8_t temp_len;
        
        temp_len = (strlen(APP_DFLT_DEVICE_NAME) <= device_name_length) ? strlen(APP_DFLT_DEVICE_NAME) : 0;
        
        if (temp_len > 0)
            device_name_length = temp_len;
        // else device_name_length shows the available space in the ADV pkt
        
        memcpy(&app_adv_data[app_adv_data_length + 2], APP_DFLT_DEVICE_NAME, device_name_length);
        app_adv_data[app_adv_data_length]     = device_name_length + 1;         // Length
        
        if (temp_len > 0) 
        {
            app_adv_data[app_adv_data_length + 1] = '\x09';                     // Complete Local Name Flag
            app_adv_data_length += (device_name_length + 2);                    // Update Advertising Data Length
        }
        else
        {
            app_adv_data[app_adv_data_length + 1] = '\x08';                     // Shortened Local Name Flag
            app_adv_data_length += (device_name_length + 2);                    // Update Advertising Data Length
            device_name_length = 0;                                             // To add the full name in the Scan Response data
        }
    }
        
    if (device_name_length > 0)
        return; // device name has been added

    /*-----------------------------------------------------------------------------
     * Add the Device Name in the Advertising Scan Response Data
     *-----------------------------------------------------------------------------*/
    // Get available space in the Advertising Data
    device_name_length = APP_ADV_DATA_MAX_SIZE - app_scanrsp_data_length - 2;

    // Check if data can be added to the Advertising data
    if (device_name_length > 0)
    {
        // Get default Device Name (No name if not enough space)
        device_name_length = (strlen(APP_DFLT_DEVICE_NAME) <= device_name_length) ? strlen(APP_DFLT_DEVICE_NAME) : 0;
        if (device_name_length > 0) {
            memcpy(&app_scanrsp_data[app_scanrsp_data_length + 2], APP_DFLT_DEVICE_NAME, device_name_length);

            app_scanrsp_data[app_scanrsp_data_length]     = device_name_length + 1; // Length
            app_scanrsp_data[app_scanrsp_data_length + 1] = '\x09';                 // Device Name Flag
            
            app_scanrsp_data_length += (device_name_length + 2);                    // Update Scan response Data Length
        }
    }
#endif // APP_DFLT_DEVICE_NAME
}


/**
 ****************************************************************************************
 * @brief   Sets the advertising and the scan response data in the GAP Start ADV command
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void set_adv_data(struct gapm_start_advertise_cmd *cmd)
{
    if (app_adv_data_length != 0)
    {
        memcpy(&cmd->info.host.adv_data[0], app_adv_data, app_adv_data_length);
        cmd->info.host.adv_data_len = app_adv_data_length;
    }

    if (app_scanrsp_data_length != 0)
    {
        memcpy(&cmd->info.host.scan_rsp_data[0], app_scanrsp_data, app_scanrsp_data_length);
        cmd->info.host.scan_rsp_data_len = app_scanrsp_data_length;
    }
}


/**
 ****************************************************************************************
 * @brief   N/A for this application
 *
 * @param[in] cmd   message to GAPM
 *
 * @return  void
 ****************************************************************************************
 */
void app_adv_func(struct gapm_start_advertise_cmd *cmd)
{
}


/**
 ****************************************************************************************
 * @brief   Initializes the HID server DB 
 *
 * @param[in] cmd   message to GAPM
 *
 * @return  true, if the initialization is done
 *          false, if the initialization fails
 ****************************************************************************************
 */
bool app_db_init_func(void)
{
    // Indicate if more services need to be added in the database
    bool end_db_create = false;
    
    // Check if another should be added in the database
    if (app_env.next_prf_init < APP_PRF_LIST_STOP)
    {
        switch (app_env.next_prf_init)
        {
            
            case (APP_DIS_TASK):
            {
                app_dis_create_db_send();
            } break;
            case (APP_BASS_TASK):
            {
                app_batt_create_db();
            } break;
            case (APP_HOGPD_TASK):
            {
                // Add HID Service in the DB
                app_hid_create_db();
            } break;
#if (BLE_SPOTA_RECEIVER)
            case (APP_SPOTAR_TASK):
            {
                // Add spotar Service in the DB
                app_spotar_create_db();
            } break;
#endif //BLE_SPOTA_RECEIVER
            default:
            {
                ASSERT_ERR(0);
            } break;

        }

        // Select following service to add
        app_env.next_prf_init++;
    }
    else
    {
        end_db_create = true;
    }

    return end_db_create;
}


/**
 ****************************************************************************************
 * @brief   Handler of the HID Timer - Action depends on the app state
 *
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 ****************************************************************************************
 */
int app_hid_timer_handler(ke_msg_id_t const msgid,
                           void const *param,
                           ke_task_id_t const dest_id,
                           ke_task_id_t const src_id)
{
    dbg_puts(DBG_FSM_LVL, "HID timer exp\r\n");
    
    // Timer elapsed!
    if (HAS_DELAYED_WAKEUP)
    {
        if (current_fsm_state != IDLE_ST)
        {
            app_state_update(TIMER_EXPIRED_EVT);
        }
        else
        {
            ASSERT_ERROR(monitor_kbd_delayed_start_st == MONITOR_RELEASE);
            
            GLOBAL_INT_DISABLE();
            // Stop WKUP
            SetBits16(WKUP_CTRL_REG, WKUP_ENABLE_IRQ, 0); //No more interrupts of this kind
            GLOBAL_INT_RESTORE();
            // Any WKUP int would hit here
            
            trigger_kbd_delayed_start_st = NO_TRIGGER;
            
            wkup_hit = true;
        }
    }
    else
        app_state_update(TIMER_EXPIRED_EVT);
    
    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief   Handler of the HID Inactivity Timer - Action depends on the app state
 *
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 ****************************************************************************************
 */
int app_hid_inactivity_timer_handler(ke_msg_id_t const msgid,
                           void const *param,
                           ke_task_id_t const dest_id,
                           ke_task_id_t const src_id)
{
    dbg_puts(DBG_FSM_LVL, "HID Inactivity timer exp\r\n");
    
    if (HAS_EXTENDED_TIMERS)
    {
        if (extended_timer_cnt)
        {
            extended_timer_cnt--;
            app_timer_set(APP_HID_INACTIVITY_TIMER, TASK_APP, KE_TIMER_DELAY_MAX);    // i.e. 5 min
            return (KE_MSG_CONSUMED);
        }
    }
	
    // Timer elapsed!
    app_state_update(INACT_TIMER_EXP_EVT);
    
    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief   Handler of the HID Enc Timer
 *
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 * @remarks In case encryption is not activated by the remote host and the connection
 *           is still alive (if it wasn't then the timer would have been cleared),
 *           the handler will drop the connection. This situation appears in certain
 *           cases when pairing fails.
 ****************************************************************************************
 */
int app_hid_enc_timer_handler(ke_msg_id_t const msgid,
                           void const *param,
                           ke_task_id_t const dest_id,
                           ke_task_id_t const src_id)
{
    dbg_puts(DBG_FSM_LVL, "HID_ENC timer exp\r\n");
    
    if ( (HAS_MITM) || (HAS_EEPROM) )
    {
        if (HAS_SEND_LL_TERMINATE_IND)
        {
            app_disconnect();
        }
        else
        {
            app_fake_disconnect(); 
        }
    }
    else
    {
        ASSERT_WARNING(0);
    }
    
    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief   Handler of the HID ADV Timer 
 *
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 * @remarks If this timer expires then the advertising period is over. 
 *          If a connection is being setup, we will allow the procedure to
 *          complete. If it fails though, we do not return to ADVERTISE_ST
 *          but move on to IDLE_ST.
 ****************************************************************************************
 */
int app_hid_adv_timer_handler(ke_msg_id_t const msgid,
                           void const *param,
                           ke_task_id_t const dest_id,
                           ke_task_id_t const src_id)
{
    dbg_puts(DBG_FSM_LVL, "ADV timer exp\r\n");

    app_state_update(ADV_TIMER_EXP_EVT);
    
    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief   Handler of a dummy TASK_APP msg sent to trigger the timer
 *
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 * @remarks In case of delayed start, a dummy message is sent to the TASK_APP.
 *          This msg is put in the queue when the BLE is woken up. When the
 *          handler is called, it is certain that the BLE is running and 
 *          the timer may start.
 ****************************************************************************************
 */
int app_hid_msg_handler(ke_msg_id_t const msgid,
                           void const *param,
                           ke_task_id_t const dest_id,
                           ke_task_id_t const src_id)
{
    struct hid_msg* msg = (struct hid_msg*)param;
    
    if (HAS_DELAYED_WAKEUP)
    {
        dbg_puts(DBG_FSM_LVL, "HID_MSG handler\r\n");
        
        if (msg->trigger == PRESSED_TRIGGER_SYNCED)
        {
            ASSERT_WARNING(current_fsm_state == IDLE_ST);
            
            trigger_kbd_delayed_start_st = NO_TRIGGER;
            
            app_timer_set(APP_HID_TIMER, TASK_APP, (KBD_DELAY_TIMEOUT / 10));
            
            // Set WKUPCT to monitor key release
            app_kbd_enable_delayed_scanning(false);
        }

        if (msg->trigger == RELEASED_TRIGGER_SYNCED)
        {
            ASSERT_WARNING(current_fsm_state == IDLE_ST);
            
            ke_timer_clear(APP_HID_TIMER, TASK_APP);
            
            trigger_kbd_delayed_start_st = NO_TRIGGER;
            
            // Put BLE in permanent sleep mode
            app_ble_ext_wakeup_on();
            
            // Set WKUPCT to monitor key press
            app_kbd_enable_delayed_scanning(true);
        }   
    }
    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief  The Privacy flag has been altered by the remote host
 *
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 ****************************************************************************************
 */
int app_updated_privacy_ind_handler(ke_msg_id_t const msgid,
                                   void const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
    // 1. If disabled, use the public address in advertising
    // 2. If enabled generate a reconnection address (if 0:0:0:0:0:0) and write it.
    
    if (HAS_PRIVACY)
    {
        #warning "Full feature implementation (Privacy) is pending..."
        
        struct gapm_updated_privacy_ind *privacy_ind = (struct gapm_updated_privacy_ind *)param;
        
        if (privacy_ind->privacy)
            ASSERT_WARNING(0);
    }
    
    return (KE_MSG_CONSUMED);
}                                       

                              
/**
 ****************************************************************************************
 * @brief  The host updated the reconnection address 
 *
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 ****************************************************************************************
 */
int app_updated_recon_addr_ind_handler(ke_msg_id_t const msgid,
                                   void const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
    if (HAS_PRIVACY)
    {
        ASSERT_WARNING(0);
    }
    
    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief  Handler which checks the resolution procedure of the host's address
 *
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 ****************************************************************************************
 */
int app_gapm_addr_solved_ind_handler(ke_msg_id_t const msgid,
                                    void const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    int i;
    struct gapm_addr_solved_ind *ind = (struct gapm_addr_solved_ind *)param;
    
    if (MBOND_LOAD_IRKS_AT_INIT || MBOND_LOAD_INFO_AT_INIT)
    {
        // The Host has been found! We don't care about the key and the BD address. 
        // The entry will be located again when EDIV & RAND are provided.
        
        // Since we have the IRK, we can find the real address
        if (MBOND_LOAD_INFO_AT_INIT)
        {
            for (i = 0; i < MAX_BOND_PEER; i++)
            {
                if (!memcmp(&bond_array[i].irk.key[0], &ind->irk.key[0], KEY_LEN))
                {
                    multi_bond_resolved_peer_pos = i+1;
                    break;
                }
            }
        }
        else
        {
            for (i = 0; i < MAX_BOND_PEER; i++)
            {
                if (!memcmp(&irk_array.irk[i].key[0], &ind->irk.key[0], KEY_LEN))
                    break;
            }
            
            if (i != MAX_BOND_PEER) // found
            {
                multi_bond_resolved_peer_pos = i+1;
            }
        }
    }
    else
        ASSERT_WARNING(0);
    
    if (HAS_VIRTUAL_WHITE_LIST)
    {
        if (!lookup_rand_in_virtual_white_list(multi_bond_resolved_peer_pos-1))
            app_disconnect();
    }
    
    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief  The Client Char Config of Service Changed has been updated
 *
 * @param[in] msgid 
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return  KE_MSG_CONSUMED
 *
 ****************************************************************************************
 */
int app_service_changed_cfg_ind_handler(ke_msg_id_t const msgid,
                                   void const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
    struct gattc_svc_changed_cfg *ind = (struct gattc_svc_changed_cfg *)param;
        
    // ind->ind_cfg holds the value
    if ( ((bond_info.info & SVC_CHNG_CCC_MASK) >> SVC_CHNG_CCC_POS) != ind->ind_cfg)
    {
        app_alt_pair_store_bond_data();
    }
    
    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief  SPOTAR start/stop callback
 *         Gets called when SPOTAR starts or stops to update the main FSM.
 *
 * @param[in] status  The status of SPOTAR process
 *
 * @return  void
 *
 ****************************************************************************************
 */
void app_spotar_callback(const uint8_t status)
{
    if (status == SPOTAR_START)
        app_state_update(SPOTAR_START_EVT);
    else if (status == SPOTAR_END)
        app_state_update(SPOTAR_END_EVT);
    else
        ASSERT_WARNING(0);
}


#endif  //BLE_APP_PRESENT
/// @} APP
