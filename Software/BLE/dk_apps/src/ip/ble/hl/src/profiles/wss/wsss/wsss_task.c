/**
 ****************************************************************************************
 *
 * @file wsss_task.c
 *
 * @brief Weight Scale Service Server Task implementation.
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
 * @addtogroup WSSSTASK
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"

#if (BLE_WSS_SERVER)
    #include "attm_util.h"
    #include "atts_util.h"
    #include "ke_mem.h"
    #include "prf_utils.h"
    #include "wsss.h"

    #ifdef STORE_UNSENT_MEASUREMENTS
    /*
     * Unsent measurements queue
     ****************************************************************************************
     */
    struct wsss_unsent_meas_val *q_head __attribute__((section("retention_mem_area0"), zero_init)),
                                *q_tail __attribute__((section("retention_mem_area0"), zero_init));
    int                         q_size  __attribute__((section("retention_mem_area0"), zero_init));
    #endif

    /*
     * FUNCTION DEFINITIONS
     ****************************************************************************************
     */

    /**
     ****************************************************************************************
     * @brief Handles reception of the @ref WSSS_CREATE_DB_REQ message.
     * @param[in] msgid Id of the message received (probably unused).
     * @param[in] param Pointer to the parameters of the message.
     * @param[in] dest_id ID of the receiving task instance (probably unused).
     * @param[in] src_id ID of the sending task instance.
     * @return If the message was consumed or not.
     ****************************************************************************************
     */
    static int wsss_create_db_req_handler(ke_msg_id_t const msgid,
                                          struct wsss_create_db_req const *param,
                                          ke_task_id_t const dest_id,
                                          ke_task_id_t const src_id)
    {
        //DB Creation Status
        uint8_t status;

        //Save profile id
        wsss_env.con_info.prf_id = TASK_WSSS;

        /*---------------------------------------------------*
         * Weight Scale Service Creation
         *---------------------------------------------------*/
        status = attm_svc_create_db(&wsss_env.shdl, NULL, WSS_IDX_NB,
                                    wsss_env.att_tbl, dest_id, wsss_att_db);
        
        //Disable WSS
        attmdb_svc_set_permission(wsss_env.shdl, PERM(SVC, DISABLE));

        //Go to Idle State
        if (status == ATT_ERR_NO_ERROR)
            //If we are here, database has been fulfilled with success, go to idle test
            ke_state_set(TASK_WSSS, WSSS_IDLE);

        //Send response to application
        struct wsss_create_db_cfm *cfm = KE_MSG_ALLOC(WSSS_CREATE_DB_CFM, src_id,
                                                      TASK_WSSS,
                                                      wsss_create_db_cfm);
        cfm->status = status;
        ke_msg_send(cfm);

        return (KE_MSG_CONSUMED);
    }

    #ifdef STORE_UNSENT_MEASUREMENTS    
    /**
     ****************************************************************************************
     * @brief Removes an item from the queue.
     * @param[in] sent_meas_val Pointer to the item to be removed
     * @return void
     ****************************************************************************************
     */
    static void dequeue(struct wsss_unsent_meas_val *sent_meas_val)
    {
        if (sent_meas_val == NULL)
            return;
        
        if (sent_meas_val == q_head)
            q_head = sent_meas_val->next;
        else
            sent_meas_val->prev->next = sent_meas_val->next;
        
        if (sent_meas_val == q_tail)
            q_tail = sent_meas_val->prev;
        else
            sent_meas_val->next->prev = sent_meas_val->prev;
        
        ke_free(sent_meas_val);
        q_size--;
    }
    
    /**
     ****************************************************************************************
     * @brief Adds an item to the queue.
     * @param[in] packed_meas_val Pointer to the packed measurement values
     * @param[in] pval_size Size of the packed measurement values
     * @return void
     ****************************************************************************************
     */
    static void enqueue(uint8_t *packed_meas_val, uint8_t pval_size)
    {
        struct wsss_unsent_meas_val *unsent_meas_val = ke_malloc(pval_size +
                                                                 2*sizeof(struct wsss_unsent_meas_val *),
                                                                 KE_MEM_NON_RETENTION);
        memcpy(unsent_meas_val->packed_meas_val, packed_meas_val, pval_size);
        
        if (!q_size)
        {
            q_head = unsent_meas_val;
            q_head->prev = NULL;
        }
        else
        {
            if (q_size == 25)
                dequeue(q_head);
            q_tail->next = unsent_meas_val;
            unsent_meas_val->prev = q_tail;
        }
        
        q_tail = unsent_meas_val;
        q_tail->next = NULL;
        q_size++;
    }
    
    /**
     ****************************************************************************************
     * @brief Handles reception of the @ref WSSS_MEAS_SEND_REQ message.
     * @param[in] msgid Id of the message received (probably unused).
     * @param[in] param Pointer to the parameters of the message.
     * @param[in] dest_id ID of the receiving task instance (probably unused).
     * @param[in] src_id ID of the sending task instance.
     * @return If the message was consumed or not.
     ****************************************************************************************
     */
    static int wsss_meas_send_req_handler(ke_msg_id_t const msgid,
                                          struct wsss_meas_send_req const *param,
                                          ke_task_id_t const dest_id,
                                          ke_task_id_t const src_id)
    {   
        uint8_t status = PRF_ERR_OK,    // Request status
                pval_size,              // Packed value size
                *packed_meas_val;       // Pointer to unsent measurement
        
        struct prf_con_info *con_info = &wsss_env.con_info;
        
        //Save the application task id
        con_info->appid = src_id;
        
        // Calculate measurement value size
        pval_size = wsss_calc_meas_value_size(param->meas_val.flags);
        
        if(WSSS_IS_ENABLED(WSSS_WT_MEAS_IND_CFG))
        {   // Allocate memory space for the measurement element
            packed_meas_val = ke_malloc(pval_size, KE_MEM_NON_RETENTION);
            
            // Pack the Wt Measurement value
            wsss_pack_meas_value(packed_meas_val, &param->meas_val);

            if(!q_size && ke_state_get(TASK_WSSS) == WSSS_CONNECTED
                       && gapc_get_conidx(param->conhdl) == con_info->conidx)
            {
                uint16_t shdl = wsss_env.shdl;
                
                //Update value in DB
                attmdb_att_set_value(shdl + WSS_IDX_WT_MEAS_VAL, pval_size,
                                     packed_meas_val);
                
                //send notification through GATT
                prf_server_send_event((prf_env_struct*) &wsss_env, true,
                                      shdl + WSS_IDX_WT_MEAS_VAL);
            }
                
            wsss_meas_send_cfm_send(PRF_ERR_OK);
            enqueue(packed_meas_val, pval_size);
            ke_free(packed_meas_val);
        }
        else
            status = PRF_ERR_IND_DISABLED;
        
        // verify that no error occurs
        if (status != PRF_ERR_OK)
            // Inform app that value has not been sent
            wsss_meas_send_cfm_send(status);

        return (KE_MSG_CONSUMED);
    }    
    #else
    /**
     ****************************************************************************************
     * @brief Handles reception of the @ref WSSS_MEAS_SEND_REQ message.
     * @param[in] msgid Id of the message received (probably unused).
     * @param[in] param Pointer to the parameters of the message.
     * @param[in] dest_id ID of the receiving task instance (probably unused).
     * @param[in] src_id ID of the sending task instance.
     * @return If the message was consumed or not.
     ****************************************************************************************
     */
    static int wsss_meas_send_req_handler(ke_msg_id_t const msgid,
                                          struct wsss_meas_send_req const *param,
                                          ke_task_id_t const dest_id,
                                          ke_task_id_t const src_id)
    {
        // Request status
        uint8_t status = PRF_ERR_OK;
        
        // Packed value and its size
        uint8_t pval_size, *packed_meas_val;
        
        struct prf_con_info *con_info = &wsss_env.con_info;
        
        //Save the application task id
        con_info->appid = src_id;
        
        // Calculate measurement value size
        pval_size = wsss_calc_meas_value_size(param->meas_val.flags);
        
        if(WSSS_IS_ENABLED(WSSS_WT_MEAS_IND_CFG))
        {
            // Allocate memory space for the measurement list element
            packed_meas_val = ke_malloc(pval_size, KE_MEM_NON_RETENTION);
            
            // Pack the Wt Measurement value
            wsss_pack_meas_value(packed_meas_val, &param->meas_val);

            if(ke_state_get(TASK_WSSS) == WSSS_CONNECTED)
            {
                if(gapc_get_conidx(param->conhdl) == con_info->conidx)
                {
                    uint16_t shdl = wsss_env.shdl;
                    
                    //Update value in DB
                    status = attmdb_att_set_value(shdl + WSS_IDX_WT_MEAS_VAL,
                                                  pval_size, packed_meas_val);
                    
                    //send notification through GATT
                    prf_server_send_event((prf_env_struct*) &wsss_env, true,
                                          shdl + WSS_IDX_WT_MEAS_VAL);
                }
                else
                    //Wrong Connection Handle
                    status = PRF_ERR_INVALID_PARAM;
                
                ke_free(packed_meas_val);
            }
            else
                status = PRF_ERR_DISCONNECTED;
        }
        else
            status = PRF_ERR_IND_DISABLED;
        
        // verify that no error occurs
        if (status != PRF_ERR_OK)
            // Inform app that value has not been sent
            wsss_meas_send_cfm_send(status);

        return (KE_MSG_CONSUMED);
    }
    #endif


    /**
     ****************************************************************************************
     * @brief Handles reception of the @ref WSSS_ENABLE_REQ message.
     * @param[in] msgid Id of the message received (probably unused).
     * @param[in] param Pointer to the parameters of the message.
     * @param[in] dest_id ID of the receiving task instance (probably unused).
     * @param[in] src_id ID of the sending task instance.
     * @return If the message was consumed or not.
     ****************************************************************************************
     */
    static int wsss_enable_req_handler(ke_msg_id_t const msgid,
                                       struct wsss_enable_req const *param,
                                       ke_task_id_t const dest_id,
                                       ke_task_id_t const src_id)
    {
        //Value used to initialize all readable value in DB
        uint16_t indntf_cfg = 0;
        
        struct prf_con_info *con_info = &wsss_env.con_info;
        
        //Save the application id
        con_info->appid = src_id;
        //Save the connection handle associated to the profile
        con_info->conidx = gapc_get_conidx(param->conhdl);
        
        uint16_t shdl = wsss_env.shdl;

        // Check if the provided connection exist
        if (con_info->conidx == GAP_INVALID_CONIDX)
            // The connection doesn't exist, request disallowed
            prf_server_error_ind_send((prf_env_struct*) &wsss_env, PRF_ERR_REQ_DISALLOWED,
                                      WSSS_ERROR_IND, WSSS_ENABLE_REQ);
        else
        {
            // Set Weight Scale Feature Value in database - Not supposed to change during connection
            attmdb_att_set_value(shdl + WSS_IDX_FEATURE_VAL,
                                 sizeof(struct wss_feature), (uint8_t*) &param->ws_feature);

            // Configure Weight Scale Measuremment IND Cfg in DB
            if(param->con_type == PRF_CON_NORMAL)
            {
                indntf_cfg = param->wt_meas_ind_en;

                if (param->wt_meas_ind_en == PRF_CLI_START_IND)
                    wsss_env.evt_cfg |= WSSS_WT_MEAS_IND_CFG;
            }
            
            //Set Wt. Meas. Char. IND Configuration in DB - 0 if not normal connection
            attmdb_att_set_value(shdl + WSS_IDX_WT_MEAS_IND_CFG,
                                 sizeof(uint16_t), (uint8_t*) &indntf_cfg);
            
            //Enable Attributes + Set Security Level
            attmdb_svc_set_permission(shdl, param->sec_lvl);
            
            // Go to connected state
            ke_state_set(TASK_WSSS, WSSS_CONNECTED);
        }

        #ifdef STORE_UNSENT_MEASUREMENTS
        // Unsent measurement size and attmdb_att_set_value status
        uint8_t unsent_meas_size;
        
        /// Check if there are any unsent measurements
        if (q_size)
        {
            //Calculate size of unsent measurement
            unsent_meas_size = wsss_calc_meas_value_size(*q_head->packed_meas_val);
            
            //Update value in DB
            attmdb_att_set_value(shdl + WSS_IDX_WT_MEAS_VAL,
                                                    unsent_meas_size,
                                                    q_head->packed_meas_val);
            
            //send notification through GATT
            prf_server_send_event((prf_env_struct*) &wsss_env, true,
                                  shdl + WSS_IDX_WT_MEAS_VAL);
        }
        #endif
        
        return (KE_MSG_CONSUMED);
    }

    /**
     ****************************************************************************************
     * @brief Handles reception of the @ref GL2C_CODE_ATT_WR_CMD_IND message.
     * The handler compares the new values with current ones and notifies them if they changed.
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
        uint16_t value = 0x0000;
        uint8_t status = PRF_ERR_OK;
        struct prf_con_info *con_info = &wsss_env.con_info;

        if (KE_IDX_GET(src_id) == con_info->conidx)
        {
            //Extract value before check
            memcpy(&value, &param->value, sizeof(uint16_t));

            //Wt Measurement Char. - Client Char. Configuration
            if (param->handle == wsss_env.shdl + WSS_IDX_WT_MEAS_IND_CFG)
                if (value == PRF_CLI_STOP_NTFIND)
                    wsss_env.evt_cfg &= ~WSSS_WT_MEAS_IND_CFG;
                else if (value == PRF_CLI_START_IND)
                    wsss_env.evt_cfg |= WSSS_WT_MEAS_IND_CFG;
                else
                    status = PRF_APP_ERROR;

            if (status == PRF_ERR_OK)
            {
                //Update the attribute value
                status = attmdb_att_set_value(param->handle, sizeof(uint16_t),
                                              (uint8_t*) &value);
                if(param->last)
                {
                    //Inform APP of configuration change
                    struct wsss_cfg_indntf_ind *ind = KE_MSG_ALLOC(WSSS_CFG_INDNTF_IND,
                                                                   con_info->appid,
                                                                   TASK_BLPS,
                                                                   wsss_cfg_indntf_ind);

                    ind->conhdl = gapc_get_conhdl(con_info->conidx);
                    memcpy(&ind->cfg_val, &value, sizeof(uint16_t));

                    ke_msg_send(ind);
                }
            }
        }

        //Send write response
        atts_write_rsp_send(con_info->conidx, param->handle, status);

        return (KE_MSG_CONSUMED);
    }


    /**
     ****************************************************************************************
     * @brief Completion indication from GATT
     * @param[in] msgid     Id of the message received.
     * @param[in] param     Pointer to the parameters of the message.
     * @param[in] dest_id   ID of the receiving task instance
     * @param[in] src_id    ID of the sending task instance.
     * @return If the message was consumed or not.
     ****************************************************************************************
     */
    static int gattc_cmp_evt_handler(ke_msg_id_t const msgid,
                                     struct gattc_cmp_evt const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
    {   
        if(param->req_type == GATTC_INDICATE)
        {
            #ifdef STORE_UNSENT_MEASUREMENTS    
            // Unsent measurement size
            uint8_t unsent_meas_size;
            
            if(param->status == GATT_ERR_NO_ERROR)
            {
                dequeue(q_head);
            
                // Check if there are more unsent measurements
                if(q_size)
                {
                    //Calculate size of unsent measurement
                    unsent_meas_size = wsss_calc_meas_value_size(*q_head->packed_meas_val);
                    
                    uint16_t shdl = wsss_env.shdl;

                    //Update value in DB
                    attmdb_att_set_value(shdl + WSS_IDX_WT_MEAS_VAL,
                                         unsent_meas_size, q_head->packed_meas_val);
                    
                    //send notification through GATT
                    prf_server_send_event((prf_env_struct*) &wsss_env, true,
                                          shdl + WSS_IDX_WT_MEAS_VAL);
                }
            }
            #else
             wsss_meas_send_cfm_send(param->status);
            #endif         
        }
        
        return (KE_MSG_CONSUMED);
    }

    /**
     ****************************************************************************************
     * @brief Disconnection indication to HTPT.
     * @param[in] msgid     Id of the message received.
     * @param[in] param     Pointer to the parameters of the message.
     * @param[in] dest_id   ID of the receiving task instance
     * @param[in] src_id    ID of the sending task instance.
     * @return If the message was consumed or not.
     ****************************************************************************************
     */
    static int gapc_disconnect_ind_handler(ke_msg_id_t const msgid,
                                           struct gapc_disconnect_ind const *param,
                                           ke_task_id_t const dest_id,
                                           ke_task_id_t const src_id)
    {
        if (KE_IDX_GET(src_id) == wsss_env.con_info.conidx)
            wsss_disable(param->conhdl);

        return (KE_MSG_CONSUMED);
    }

    /*
     * GLOBAL VARIABLE DEFINITIONS
     ****************************************************************************************
     */

    ///Disabled State handler definition.
    const struct ke_msg_handler wsss_disabled[] =
    {
        {WSSS_CREATE_DB_REQ,    (ke_msg_func_t)wsss_create_db_req_handler},
    };

    ///Idle State handler definition.
    const struct ke_msg_handler wsss_idle[] =
    {
        {WSSS_ENABLE_REQ,       (ke_msg_func_t)wsss_enable_req_handler},
    };

    /// Default State handlers definition
    const struct ke_msg_handler wsss_default_state[] =
    {
        {WSSS_MEAS_SEND_REQ,    (ke_msg_func_t)wsss_meas_send_req_handler},
        {GAPC_DISCONNECT_IND,   (ke_msg_func_t)gapc_disconnect_ind_handler},
        {GATTC_WRITE_CMD_IND,   (ke_msg_func_t)gattc_write_cmd_ind_handler},
        {GATTC_CMP_EVT,         (ke_msg_func_t)gattc_cmp_evt_handler},
    };

    ///Specifies the message handler structure for every input state.
    const struct ke_state_handler wsss_state_handler[WSSS_STATE_MAX] =
    {
        [WSSS_DISABLED]    = KE_STATE_HANDLER(wsss_disabled),
        [WSSS_IDLE]        = KE_STATE_HANDLER(wsss_idle),
        [WSSS_CONNECTED]   = KE_STATE_HANDLER_NONE,
    };

    ///Specifies the message handlers that are common to all states.
    const struct ke_state_handler wsss_default_handler = KE_STATE_HANDLER(wsss_default_state);

    ///Defines the place holder for the states of all the task instances.
    ke_state_t wsss_state[WSSS_IDX_MAX] __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY

#endif //BLE_WSS_SERVER

/// @} WSSSTASK
