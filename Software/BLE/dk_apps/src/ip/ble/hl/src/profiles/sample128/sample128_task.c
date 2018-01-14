/**
 ****************************************************************************************
 *
 * @file sample128_task.c
 *
 * @brief Sample128 Task implementation.
 *
 * @brief 128 UUID service. sample code
 *
 * Copyright (C) 2013 Dialog Semiconductor GmbH and its Affiliates, unpublished work
 * This computer program includes Confidential, Proprietary Information and is a Trade Secret 
 * of Dialog Semiconductor GmbH and its Affiliates. All use, disclosure, and/or 
 * reproduction is prohibited unless authorized in writing. All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
*/

#include "rwble_config.h"

#if (BLE_SAMPLE128)

#include "gap.h"
#include "gapc.h"
#include "gattc_task.h"
#include "atts_util.h"
#include "sample128.h"
#include "sample128_task.h"
#include "attm_cfg.h"
#include "attm_db.h"
#include "prf_utils.h"

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref SAMPLE128_CREATE_DB_REQ message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int sample128_create_db_req_handler(ke_msg_id_t const msgid,
                                       struct sample128_create_db_req const *param,
                                       ke_task_id_t const dest_id,
                                       ke_task_id_t const src_id)
{
    //Database Creation Status
    uint8_t status;
    uint8_t nb_att_16;
    uint8_t nb_att_128;
    uint8_t nb_att_32;
    uint16_t att_decl_svc = ATT_DECL_PRIMARY_SERVICE;
    uint16_t att_decl_char = ATT_DECL_CHARACTERISTIC;
    uint16_t att_decl_cfg = ATT_DESC_CLIENT_CHAR_CFG;
    uint16_t val_hdl;
    uint16_t char_hdl; 

    //Save Profile ID
    sample128_env.con_info.prf_id = TASK_SAMPLE128;

    /*---------------------------------------------------*
     * Link Loss Service Creation
     *---------------------------------------------------*/

        //Add Service Into Database
        nb_att_16 = 4; // 2 UUID16 atts
        nb_att_32 = 0;// No UUID32 att
        nb_att_128 = 2; //1 UUID128 att
        status = attmdb_add_service(&(sample128_env.sample128_shdl), TASK_SAMPLE128,
                                             nb_att_16, nb_att_32, nb_att_128, 58); //total attributte size = 36, 16 (svc)  + 19 (desc_char) + 1 (att)

        if (status == ATT_ERR_NO_ERROR)
        {
            //add svc attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, ATT_UUID_128_LEN, //Data size = 16 (ATT_UUID_128_LEN)
                                               ATT_UUID_16_LEN, (uint8_t*)&att_decl_svc, PERM(RD, ENABLE),
                                               &(sample128_env.sample128_shdl));
            
            
            
            status = attmdb_att_set_value(sample128_env.sample128_shdl, ATT_UUID_128_LEN, (uint8_t *)sample128_svc.uuid);
            
            
//char 1     
            //add char attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, ATT_UUID_128_LEN + 3, //Data size = 19 (ATT_UUID_128_LEN + 3)
                                               ATT_UUID_16_LEN, (uint8_t*) &att_decl_char, PERM(RD, ENABLE),
                                               &(char_hdl));
            
            
            
            //add val attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, sizeof(uint8_t), //Data size = 1
                                               ATT_UUID_128_LEN, (uint8_t*)&sample128_1_val.uuid, PERM(RD, ENABLE) | PERM(WR, ENABLE),
                                               &(val_hdl));
            
            memcpy(sample128_1_char.attr_hdl, &val_hdl, sizeof(uint16_t));
            
            status = attmdb_att_set_value(char_hdl, sizeof(sample128_1_char), (uint8_t *)&sample128_1_char);
            

//////char 2
            //add char attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, ATT_UUID_128_LEN + 3, //Data size = 19 (ATT_UUID_128_LEN + 3)
                                               ATT_UUID_16_LEN, (uint8_t*) &att_decl_char, PERM(RD, ENABLE),
                                               &(char_hdl));
                        
            //add val attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, sizeof(uint8_t), //Data size = 1
                                               ATT_UUID_128_LEN, (uint8_t*)&sample128_2_val.uuid, PERM(RD, ENABLE) | PERM(NTF, ENABLE),
                                               &(val_hdl));
            
            memcpy(sample128_2_char.attr_hdl, &val_hdl, sizeof(uint16_t));
            
            status = attmdb_att_set_value(char_hdl, sizeof(sample128_2_char), (uint8_t *)&sample128_2_char);

            //add cfg attribute
            status = attmdb_add_attribute(sample128_env.sample128_shdl, sizeof(uint16_t), 
                                               ATT_UUID_16_LEN, (uint8_t*) &att_decl_cfg, PERM(RD, ENABLE) | PERM(WR, ENABLE),
                                               &(val_hdl));

            //Disable sample128 service
            attmdb_svc_set_permission(sample128_env.sample128_shdl, PERM(SVC, DISABLE));

            //Go to Idle State
    
            //If we are here, database has been fulfilled with success, go to idle state
            ke_state_set(TASK_SAMPLE128, SAMPLE128_IDLE);
        }
        
        //Send CFM to application
        struct sample128_create_db_cfm * cfm = KE_MSG_ALLOC(SAMPLE128_CREATE_DB_CFM, src_id,
                                                    TASK_SAMPLE128, sample128_create_db_cfm);
        cfm->status = status;
        ke_msg_send(cfm);
    

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Enable the Sample128 role, used after connection.
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int sample128_enable_req_handler(ke_msg_id_t const msgid,
                                    struct sample128_enable_req const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    
    uint16_t temp = 1;
    
    // Keep source of message, to respond to it further on
    sample128_env.con_info.appid = src_id;
    // Store the connection handle for which this profile is enabled
    sample128_env.con_info.conidx = gapc_get_conidx(param->conhdl);

    // Check if the provided connection exist
    if (sample128_env.con_info.conidx == GAP_INVALID_CONIDX)
    {
        // The connection doesn't exist, request disallowed
        prf_server_error_ind_send((prf_env_struct *)&sample128_env, PRF_ERR_REQ_DISALLOWED,
                 SAMPLE128_ERROR_IND, SAMPLE128_ENABLE_REQ);
    }
    else
    {
        // Sample128 1
        attmdb_svc_set_permission(sample128_env.sample128_shdl, param->sec_lvl);

        //set char 1 to specified value
        attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_1_IDX_VAL,
                             sizeof(uint8_t), (uint8_t *)&param->sample128_1_val);
        
        attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_2_IDX_VAL,
                             sizeof(uint8_t), (uint8_t *)&param->sample128_2_val);

        sample128_env.feature = param->feature; 
        
        if (!sample128_env.feature)
        {
               temp = 0;
        }
        
        attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_2_IDX_CFG,
                             sizeof(uint16_t), (uint8_t *)&temp);
        
        // Go to Connected state
        ke_state_set(TASK_SAMPLE128, SAMPLE128_CONNECTED);
    }

    return (KE_MSG_CONSUMED);
}



/**
 ****************************************************************************************
 * @brief Updates characteristic's 2 value. Sends notification to peer if property is enabled.
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

static int sample128_upd_char2_req_handler(ke_msg_id_t const msgid,
                                    struct sample128_upd_char2_req const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    uint8_t status = PRF_ERR_OK;

    // Check provided values
    if(param->conhdl == gapc_get_conhdl(sample128_env.con_info.conidx))
    {
        // Update value in DB
        attmdb_att_set_value(sample128_env.sample128_shdl + SAMPLE128_2_IDX_VAL,
                             sizeof(uint8_t), (uint8_t *)&param->val);

        if((sample128_env.feature & PRF_CLI_START_NTF))
            // Send notification through GATT
            prf_server_send_event((prf_env_struct *)&sample128_env, false,
                    sample128_env.sample128_shdl + SAMPLE128_2_IDX_VAL);
        
    }
    else
    {
        status = PRF_ERR_INVALID_PARAM;
    }

    if (status != PRF_ERR_OK)
    {
        sample128_upd_char2_cfm_send(status);
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GATT_WRITE_CMD_IND message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gattc_write_cmd_ind_handler(ke_msg_id_t const msgid,
                                      struct gattc_write_cmd_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    uint8_t char_code = SAMPLE128_ERR_CHAR;
    uint8_t status = PRF_APP_ERROR;

    if (KE_IDX_GET(src_id) == sample128_env.con_info.conidx)
    {
        if (param->handle == sample128_env.sample128_shdl + SAMPLE128_1_IDX_VAL)
        {
            char_code = SAMPLE128_1_CHAR;
        }
        
        if (param->handle == sample128_env.sample128_shdl + SAMPLE128_2_IDX_CFG)
        {
            char_code = SAMPLE128_2_CFG;
        }
        
        if (char_code == SAMPLE128_1_CHAR)
        {
            
            //Save value in DB
            attmdb_att_set_value(param->handle, sizeof(uint8_t), (uint8_t *)&param->value[0]);
            
            if(param->last)
            {
                sample128_send_val(param->value[0]);
            }

            status = PRF_ERR_OK;
               
        }
        else if (char_code == SAMPLE128_2_CFG)
        {
            
            // Written value
            uint16_t ntf_cfg;

            // Extract value before check
            ntf_cfg = co_read16p(&param->value[0]);
        
            // Only update configuration if value for stop or notification enable
            if ((ntf_cfg == PRF_CLI_STOP_NTFIND) || (ntf_cfg == PRF_CLI_START_NTF))
            {
                //Save value in DB
                attmdb_att_set_value(param->handle, sizeof(uint16_t), (uint8_t *)&param->value[0]);
                
                // Conserve information in environment
                if (ntf_cfg == PRF_CLI_START_NTF)
                {
                    // Ntf cfg bit set to 1
                    sample128_env.feature |= PRF_CLI_START_NTF;
                }
                else
                {
                    // Ntf cfg bit set to 0
                    sample128_env.feature &= ~PRF_CLI_START_NTF;
                }                
                
                status = PRF_ERR_OK; 
                
            }
        }
    }

    // Send Write Response
    atts_write_rsp_send(sample128_env.con_info.conidx, param->handle, status);
    
    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Disconnection indication to sample128.
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gap_disconnnect_ind_handler(ke_msg_id_t const msgid,
                                        struct gapc_disconnect_ind const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{

    // Check Connection Handle
    if (KE_IDX_GET(src_id) == sample128_env.con_info.conidx)
    {
        
        // In any case, inform APP about disconnection
        sample128_disable();
    }

    return (KE_MSG_CONSUMED);
}

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Disabled State handler definition.
const struct ke_msg_handler sample128_disabled[] =
{
    {SAMPLE128_CREATE_DB_REQ,   (ke_msg_func_t) sample128_create_db_req_handler },
};

/// Idle State handler definition.
const struct ke_msg_handler sample128_idle[] =
{
    {SAMPLE128_ENABLE_REQ,      (ke_msg_func_t) sample128_enable_req_handler },
};

/// Connected State handler definition.
const struct ke_msg_handler sample128_connected[] =
{
    {GATTC_WRITE_CMD_IND,       (ke_msg_func_t) gattc_write_cmd_ind_handler},
    {SAMPLE128_UPD_CHAR2_REQ,   (ke_msg_func_t) sample128_upd_char2_req_handler},
};

/// Default State handlers definition
const struct ke_msg_handler sample128_default_state[] =
{
    {GAPC_DISCONNECT_IND,    (ke_msg_func_t) gap_disconnnect_ind_handler},
};

/// Specifies the message handler structure for every input state.
const struct ke_state_handler sample128_state_handler[SAMPLE128_STATE_MAX] =
{
    [SAMPLE128_DISABLED]    = KE_STATE_HANDLER(sample128_disabled),
    [SAMPLE128_IDLE]        = KE_STATE_HANDLER(sample128_idle),
    [SAMPLE128_CONNECTED]   = KE_STATE_HANDLER(sample128_connected),
};

/// Specifies the message handlers that are common to all states.
const struct ke_state_handler sample128_default_handler = KE_STATE_HANDLER(sample128_default_state);

/// Defines the place holder for the states of all the task instances.
ke_state_t sample128_state[SAMPLE128_IDX_MAX] __attribute__((section("retention_mem_area0"),zero_init));

#endif //BLE_SAMPLE128

