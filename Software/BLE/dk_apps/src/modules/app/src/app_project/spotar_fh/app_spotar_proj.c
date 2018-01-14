/**
****************************************************************************************
*
* @file app_spotar_proj.c
*
* @brief SPOTAR project source code.
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
#include "rwip_config.h"             // SW configuration

#if (BLE_APP_PRESENT)

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "app_sec.h"
#include "app_spotar_proj.h"

#include "co_math.h"                 // Common Maths Definition

#if (NVDS_SUPPORT)
#include "nvds.h"                    // NVDS Definitions
#endif //(NVDS_SUPPORT)

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
*/
void app_task_custom_init(void);
void app_set_adv_data(void);

/**
 ****************************************************************************************
 * @brief app_api function. Project's actions in app_init.
 * 
 * @param[in]   void
 *
 * @return      void
 ****************************************************************************************
*/
 
void app_init_func(void)
{	
#if (BLE_SPOTA_RECEIVER)    
	app_spotar_init();
#endif
    
#if (BLE_BATT_SERVER)    
    app_batt_init();
#endif
    
#if (BLE_DIS_SERVER)        
    app_dis_init();
#endif
}

/**
 ****************************************************************************************
 * @brief app_api function. Project's actions in app_sec_init
 *
 * @param[in]   void
 *
 * @return      void
 ****************************************************************************************
*/

void app_sec_init_func(void)
{

#if (BLE_APP_SEC)
	app_sec_env.auth = (GAP_AUTH_REQ_MITM_BOND);
#endif
}

/**
 ****************************************************************************************
 * @brief app_api function. Project's actions in app_connect (device connection)
 *
 * @param[in] taskid     App task's id.
 * @param[in] param      Received gapc_connection_req_ind.
 *
 * @return    void
 ****************************************************************************************
*/
void app_connection_func(struct gapc_connection_req_ind const *param)
{

    if (app_env.conidx != GAP_INVALID_CONIDX)
    {   
        /*--------------------------------------------------------------
        * ENABLE REQUIRED PROFILES
        *-------------------------------------------------------------*/

        #if (BLE_SPOTA_RECEIVER)
        app_spotar_enable();
        #endif //BLE_SPOTA_RECEIVER
        
        #if BLE_BATT_SERVER
        cur_batt_level = 0;
        app_batt_enable(cur_batt_level, 1); //Battery level Alert enabled.
        
        app_batt_poll_start(150); //15 Second timeout
        #endif // BLE_BATT_SERVER
        
        #if (BLE_APP_PRESENT)
        app_dis_enable_prf(app_env.conhdl);
        #endif

        ke_state_set(TASK_APP, APP_CONNECTED);
            
        // Retrieve the connection info from the parameters
        app_env.conhdl = param->conhdl;

        app_env.peer_addr_type = param->peer_addr_type;
        memcpy(app_env.peer_addr.addr, param->peer_addr.addr, BD_ADDR_LEN);

# if (BLE_APP_SEC)            
        // send connection confirmation
        app_connect_confirm(app_sec_env.auth);
        
# else // (BLE_APP_SEC)
        // send connection confirmation
        app_connect_confirm(GAP_AUTH_REQ_NO_MITM_NO_BOND);            
# endif // (BLE_APP_SEC)
    }
    else
    {
        // No connection has been establish, restart advertising
        app_adv_start();
    }
    
    return;
}

/**
 ****************************************************************************************
 * @brief app_api function. Project advertising function. Setup advertise string.
 *
 * @param[in]   void
 *
 * @return      void
 ****************************************************************************************
*/

void app_adv_func(struct gapm_start_advertise_cmd *cmd)
{
    
    //  Device Name Length
    uint8_t device_name_length;
    int8_t device_name_avail_space;
    uint8_t device_name_temp_buf[64];

    cmd->op.code     = GAPM_ADV_UNDIRECT;
    cmd->op.addr_src = GAPM_PUBLIC_ADDR;
    cmd->intv_min    = APP_ADV_INT_MIN;
    cmd->intv_max    = APP_ADV_INT_MAX;
    cmd->channel_map = APP_ADV_CHMAP;

    cmd->info.host.mode = GAP_GEN_DISCOVERABLE;

    /*-----------------------------------------------------------------------------------
     * Set the Advertising Data and the Scan Response Data
     *---------------------------------------------------------------------------------*/
    cmd->info.host.adv_data_len       = APP_ADV_DATA_MAX_SIZE;
    cmd->info.host.scan_rsp_data_len  = APP_SCAN_RESP_DATA_MAX_SIZE;
    
    // Advertising Data
    #if (NVDS_SUPPORT)
    if(nvds_get(NVDS_TAG_APP_BLE_ADV_DATA, &cmd->info.host.adv_data_len,
                &cmd->info.host.adv_data[0]) != NVDS_OK)
    #endif //(NVDS_SUPPORT)
    {
        cmd->info.host.adv_data_len = APP_ADV_DATA_LEN;
        memcpy(&cmd->info.host.adv_data[0], APP_ADV_DATA, cmd->info.host.adv_data_len);
    }

    // Scan Response Data
    #if (NVDS_SUPPORT)
    if(nvds_get(NVDS_TAG_APP_BLE_SCAN_RESP_DATA, &cmd->info.host.scan_rsp_data_len,
                &cmd->info.host.scan_rsp_data[0]) != NVDS_OK)
    #endif //(NVDS_SUPPORT)
    {
        cmd->info.host.scan_rsp_data_len = APP_SCNRSP_DATA_LENGTH;
        memcpy(&cmd->info.host.scan_rsp_data[0], APP_SCNRSP_DATA, cmd->info.host.scan_rsp_data_len);
    }

    // Get remaining space in the Advertising Data - 2 bytes are used for name length/flag
    device_name_avail_space = APP_ADV_DATA_MAX_SIZE - cmd->info.host.adv_data_len - 2;

    // Check if data can be added to the Advertising data
    if (device_name_avail_space > 0)
    {
        // Get the Device Name to add in the Advertising Data (Default one or NVDS one)
        #if (NVDS_SUPPORT)
        device_name_length = NVDS_LEN_DEVICE_NAME;
        if (nvds_get(NVDS_TAG_DEVICE_NAME, &device_name_length, &device_name_temp_buf[0]) != NVDS_OK)
        #endif //(NVDS_SUPPORT)
        {
            // Get default Device Name (No name if not enough space)
            device_name_length = strlen(APP_DEVICE_NAME);
            memcpy(&device_name_temp_buf[0], APP_DEVICE_NAME, device_name_length);
        }

        if(device_name_length > 0)
        {
            // Check available space
            device_name_length = co_min(device_name_length, device_name_avail_space);

            // Fill Length
            cmd->info.host.adv_data[cmd->info.host.adv_data_len]     = device_name_length + 1;
            // Fill Device Name Flag
            cmd->info.host.adv_data[cmd->info.host.adv_data_len + 1] = '\x09';
            // Copy device name
            memcpy(&cmd->info.host.adv_data[cmd->info.host.adv_data_len + 2], device_name_temp_buf, device_name_length);

            // Update Advertising Data Length
            cmd->info.host.adv_data_len += (device_name_length + 2);
        }
    }

    return;
}

/**
 ****************************************************************************************
 * @brief app_api function. Project's actions in app_disconnect
 *
 * @param[in] taskid     App task's id.
 * @param[in] param      Received gapc_disconnect_ind msg.
 *
 * @return    void
 ****************************************************************************************
*/

void app_disconnect_func(ke_task_id_t task_id, struct gapc_disconnect_ind const *param)
{
    uint8_t state = ke_state_get(task_id);

#if BLE_BATT_SERVER
    app_batt_poll_stop();
#endif // BLE_BATT_SERVER

    if ((state == APP_SECURITY) || (state == APP_CONNECTED)  || (state == APP_PARAM_UPD))
    {
        // Restart Advertising
        app_adv_start();
    }
    else
    {
        // We are not in a Connected State
        ASSERT_ERR(0);
    }
}

/**
 ****************************************************************************************
 * @brief app_api function. Project's actions for profiles' database initialization
 *
 * @param[in]   void
 *
 * @return      void
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
            #if (BLE_SPOTA_RECEIVER)
            case (APP_SPOTAR_TASK):
            {
                // Add spotar Service in the DB
                app_spotar_create_db();
            } break;
            #endif //BLE_SPOTA_RECEIVER
            #if (BLE_BATT_SERVER)
            case (APP_BASS_TASK):
            {   
                app_batt_create_db();
            } break;
            #endif //BLE_BATT_SERVER
            #if (BLE_DIS_SERVER)
            case (APP_DIS_TASK):
            {
                app_dis_create_db_send();
            } break;
            #endif //BLE_DIS_SERVER
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
 * @brief app_api function. Sends request to update connection's paramaters.
 *
 * @param[in]   void
 *
 * @return      void
 ****************************************************************************************
*/
void app_param_update_func(void)
{

    return;
}

/**
 ****************************************************************************************
 * @brief app_api function. Project configures GAPM. Called upon reset completion
 *
 * @param[in] task_id    id of the kernel task calling this function
 * @param[in] cmd        parameters to pass to the stack
 *
 * @return void.
 ****************************************************************************************
*/

void app_configuration_func(ke_task_id_t const task_id, struct gapm_set_dev_config_cmd *cmd)
{
    // set device configuration
    cmd->operation = GAPM_SET_DEV_CONFIG;
    // Device Role
    cmd->role = GAP_PERIPHERAL_SLV;
    // Device IRK
    // cmd->irk = ; TODO NOT set

    // Device Appearance
    #if (BLE_APP_HT)
    cmd->appearance = 728;
    #else
    // Device Appearance
    cmd->appearance = 0x0000;
    #endif

    // Device Appearance write permission requirements for peer device
    cmd->appearance_write_perm = GAPM_WRITE_DISABLE;
    // Device Name write permission requirements for peer device
    cmd->name_write_perm = GAPM_WRITE_DISABLE;

    // Peripheral only: *****************************************************************
    // Slave preferred Minimum of connection interval
    cmd->con_intv_min = 8;         // 10ms (8*1.25ms)
    // Slave preferred Maximum of connection interval
    cmd->con_intv_max = 16;        // 20ms (16*1.25ms)
    // Slave preferred Connection latency
    cmd->con_latency  = 0;
    // Slave preferred Link supervision timeout
    cmd->superv_to    = 100;

    // Privacy settings bit field
    cmd->flags = 0;
    
    return;   
}

/**
 ****************************************************************************************
 * @brief app_api function. Handles Set device's configuration complete message
 *
 * @param[in] void
 *
 * @return void.
 ****************************************************************************************
*/
void app_set_dev_config_complete_func(void)
{
    // We are now in Initialization State
    ke_state_set(TASK_APP, APP_DB_INIT);

    // Add the first required service in the database
    if (app_db_init())
    {
        // No service to add in the DB -> Start Advertising
        app_adv_start();
    }
 
    return;
}

/**
 ****************************************************************************************
 * @brief app_api function. Called upon connection param's update rejection
 *
 * @param[in] status        Error code
 *
 * @return void.
 ****************************************************************************************
*/

void app_update_params_rejected_func(uint8_t status)
{
    return;
}

/**
 ****************************************************************************************
 * @brief app_api function. Handles Update connection's params complete message
 *
 * @param[in] void
 *
 * @return void.
 ****************************************************************************************
*/
void app_update_params_complete_func(void)
{
    return;
}

/**
 ****************************************************************************************
 * @brief app_api function. Handles undirect advertising completion.
 *
 * @param[in] status        Command complete message status
 *
 * @return void.
 ****************************************************************************************
*/
void app_adv_undirect_complete(uint8_t status)
{
    return;
}

/**
 ****************************************************************************************
 * @brief app_api function. Handles direct advertising completion.
 *
 * @param[in] status        Command complete message status
 *
 * @return void.
 ****************************************************************************************
*/
void app_adv_direct_complete(uint8_t status)
{
    return;
}

/**
 ****************************************************************************************
 * @brief app_api function. Handles Database creation. Start application.
 *
 * @param[in] void
 *
 * @return void.
 ****************************************************************************************
*/

void app_db_init_complete_func(void)
{
    
    app_adv_start();
    
    return;
}

/**
 ****************************************************************************************
 * @brief app_api function. Sends pairing response message Called upon pairing request message reception.
 *
 * @param[in] gapc_bond_req_ind message parameters
 *
 * @return void.
 ****************************************************************************************
*/
#if (BLE_APP_SEC)
void app_send_pairing_rsp_func(struct gapc_bond_req_ind *param)
{
    
    struct gapc_bond_cfm* cfm = KE_MSG_ALLOC(GAPC_BOND_CFM, TASK_GAPC, TASK_APP, gapc_bond_cfm);

    cfm->request = GAPC_PAIRING_RSP;
    cfm->accept = true;

    // OOB information
    cfm->data.pairing_feat.oob            = GAP_OOB_AUTH_DATA_NOT_PRESENT;
    // Encryption key size
    cfm->data.pairing_feat.key_size       = KEY_LEN;
    // IO capabilities
    cfm->data.pairing_feat.iocap          = GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
    // Authentication requirements
    cfm->data.pairing_feat.auth           = GAP_AUTH_REQ_NO_MITM_BOND;
    //Security requirements
    cfm->data.pairing_feat.sec_req        = GAP_NO_SEC;
    //Initiator key distribution
    //GZ cfm->data.pairing_feat.ikey_dist      = GAP_KDIST_NONE;
    cfm->data.pairing_feat.ikey_dist      = GAP_KDIST_SIGNKEY;
    //Responder key distribution
    cfm->data.pairing_feat.rkey_dist      = GAP_KDIST_ENCKEY;
    
    ke_msg_send(cfm);
}

/**
 ****************************************************************************************
 * @brief app_api function. Send GAPC_TK_EXCH. Called in gapc_bond_req_ind_handler(tk_type == GAP_TK_KEY_DISPLAY)
 *
 * @param[in] gapc_bond_req_ind message parameters
 *
 * @return void.
 ****************************************************************************************
*/
void app_send_tk_exch_func(struct gapc_bond_req_ind *param)
{
    struct gapc_bond_cfm* cfm = KE_MSG_ALLOC(GAPC_BOND_CFM, TASK_GAPC, TASK_APP, gapc_bond_cfm);
    uint32_t pin_code = app_sec_gen_tk();
    cfm->request = GAPC_TK_EXCH;
    cfm->accept = true;
    
    memset(cfm->data.tk.key, 0, KEY_LEN);
    
    cfm->data.tk.key[3] = (uint8_t)((pin_code & 0xFF000000) >> 24);
    cfm->data.tk.key[2] = (uint8_t)((pin_code & 0x00FF0000) >> 16);
    cfm->data.tk.key[1] = (uint8_t)((pin_code & 0x0000FF00) >>  8);
    cfm->data.tk.key[0] = (uint8_t)((pin_code & 0x000000FF) >>  0);
    
    ke_msg_send(cfm);
}

/**
 ****************************************************************************************
 * @brief app_api function. Send GAPC_IRK_EXCH. Called in gapc_bond_req_ind_handler/GAPC_IRK_EXCH
 *
 * @param[in] gapc_bond_req_ind message parameters
 *
 * @return void.
 ****************************************************************************************
*/
void app_send_irk_exch_func(struct gapc_bond_req_ind *param)
{
    return;
}

/**
 ****************************************************************************************
 * @brief app_api function. Send GAPC_CSRK_EXCH. Called in gapc_bond_req_ind_handler/GAPC_CSRK_EXCH
 *
 * @param[in] gapc_bond_req_ind message parameters
 *
 * @return void.
 ****************************************************************************************
*/
void app_send_csrk_exch_func(struct gapc_bond_req_ind *param)
{
    struct gapc_bond_cfm* cfm = KE_MSG_ALLOC(GAPC_BOND_CFM, TASK_GAPC, TASK_APP, gapc_bond_cfm);

    // generate ltk
    app_sec_gen_ltk(param->data.key_size);

    cfm->request = GAPC_CSRK_EXCH;

    cfm->accept = true;

    memset((void *) cfm->data.csrk.key, 0, KEY_LEN);
    memcpy((void *) cfm->data.csrk.key, (void *)"\xAB\xAB\x45\x55\x23\x01", 6);

    ke_msg_send(cfm);

}

/**
 ****************************************************************************************
 * @brief app_api function. Send GAPC_LTK_EXCH. Called in gapc_bond_req_ind_handler/GAPC_LTK_EXCH
 *
 * @param[in] gapc_bond_req_ind message parameters
 *
 * @return void.
 ****************************************************************************************
*/
void app_send_ltk_exch_func(struct gapc_bond_req_ind *param)
{
    
    struct gapc_bond_cfm* cfm = KE_MSG_ALLOC(GAPC_BOND_CFM, TASK_GAPC, TASK_APP, gapc_bond_cfm);

    // generate ltk
    app_sec_gen_ltk(param->data.key_size);

    cfm->request = GAPC_LTK_EXCH;

    cfm->accept = true;

    cfm->data.ltk.key_size = app_sec_env.key_size;
    cfm->data.ltk.ediv = app_sec_env.ediv;

    memcpy(&(cfm->data.ltk.randnb), &(app_sec_env.rand_nb) , RAND_NB_LEN);
    memcpy(&(cfm->data.ltk.ltk), &(app_sec_env.ltk) , KEY_LEN);

    ke_msg_send(cfm);

}

/**
 ****************************************************************************************
 * @brief app_api function. Project's action when device is paired
 *
 * @param[in] void
 *
 * @return void.
 ****************************************************************************************
*/
void app_paired_func(void)
{
    return;
}

/**
 ****************************************************************************************
 * @brief app_api function. Validates encryption request message.
 *
 * @param[in] void
 *
 * @return void.
 ****************************************************************************************
*/
bool app_validate_encrypt_req_func(struct gapc_encrypt_req_ind *param)
{
    return true;
}

/**
 ****************************************************************************************
 * @brief app_api function. Handles encryption indication.
 *
 * @param[in] void
 *
 * @return void.
 ****************************************************************************************
*/
void app_sec_encrypt_ind_func(void)
{
    return; 
}

/**
 ****************************************************************************************
 * @brief app_api function. Project's actions when encryption request is completed succesfully
 *
 * @param[in] void
 *
 * @return void.
 ****************************************************************************************
*/
void app_sec_encrypt_complete_func(void)
{
    return;
}

/**
 ****************************************************************************************
 * @brief app_api function. Start passcode entry process. Called in gapc_bond_req_ind_handler(tk_type == GAP_TK_KEY_ENTRY)
 *
 * @param[in] void
 *
 * @return void.
 ****************************************************************************************
*/
void app_mitm_passcode_entry_func(ke_task_id_t const src_id, ke_task_id_t const dest_id)
{
    return;
}

#endif //BLE_APP_SEC
#endif  //BLE_APP_PRESENT
/// @} APP
