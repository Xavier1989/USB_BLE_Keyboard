/**
 ****************************************************************************************
 *
 * @file udss_task.c
 *
 * @brief User Data Service Task implementation.
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
 * @addtogroup UDSSTASK
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"

#if (BLE_UDS_SERVER)

#include "gap.h"
#include "gattc_task.h"
#include "uds_common.h"
#include "udss.h"
#include "udss_task.h"
#include "prf_utils.h"
#include "attm_util.h"
#include "atts_util.h"


/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

static int udss_create_db_req_handler(ke_msg_id_t const msgid,
                                      struct udss_create_db_req const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    //DB Creation Statis
    uint8_t status = ATT_ERR_NO_ERROR;

    //Save profile id
    udss_env.con_info.prf_id = TASK_UDSS;

    status = attm_svc_create_db(&udss_env.shdl, NULL, UDS_IDX_NB, &udss_env.att_tbl[0],
                                dest_id, &udss_att_db[0]);

    if (status == ATT_ERR_NO_ERROR)
    {
        //Disable service
        status = attmdb_svc_set_permission(udss_env.shdl, PERM(SVC, DISABLE));

        //If we are here, database has been fulfilled with success, go to idle test
        ke_state_set(TASK_UDSS, UDSS_IDLE);
    }

    //Send response to application
    struct udss_create_db_cfm * cfm = KE_MSG_ALLOC(UDSS_CREATE_DB_CFM, src_id, TASK_UDSS,
                                                   udss_create_db_cfm);
    cfm->status = status;
    ke_msg_send(cfm);

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref UDSS_SET_CHAR_VAL_REQ message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int udss_set_char_val_req_handler(ke_msg_id_t const msgid,
                                         struct udss_set_char_val_req const *param,
                                         ke_task_id_t const dest_id,
                                         ke_task_id_t const src_id)
{
    // Request status
    uint8_t status;
    // Characteristic Declaration attribute handle
    uint16_t handle;

    //Save the application task id
    udss_env.con_info.appid = src_id;

    // Check Characteristic Code
    if (param->char_code <= 0)//UDS_PNP_ID_CHAR)
    {
        // Get Characteristic Declaration attribute handle
        handle = udss_env.shdl + udss_env.att_tbl[param->char_code];

        // Check if the Characteristic exists in the database
        if (handle != PRF_ERR_INEXISTENT_HDL)
        {
            // Check the value length
//            status = udss_check_val_len(param->char_code, param->val_len);

            if (status == PRF_ERR_OK)
            {
                // Set value in the database
                attmdb_att_set_value(handle + 1, param->val_len, (uint8_t *)&param->val[0]);
            }
        }
        else
        {
            status = PRF_ERR_INEXISTENT_HDL;
        }
    }
    else
    {
        status = PRF_ERR_INVALID_PARAM;
    }

    if (status != PRF_ERR_OK)
    {
        // Status is PRF_ERR_INVALID_PARAM or PRF_ERR_UNEXPECTED_LEN or PRF_ERR_INEXISTENT_HDL
        prf_server_error_ind_send((prf_env_struct *)&udss_env, status,
                                  UDSS_ERROR_IND, UDSS_SET_CHAR_VAL_REQ);
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref UDSS_ENABLE_REQ message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int udss_enable_req_handler(ke_msg_id_t const msgid,
                                   struct udss_enable_req const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
    //Save the connection handle associated to the profile
    udss_env.con_info.conidx = gapc_get_conidx(param->conhdl);
    //Save the application id
    udss_env.con_info.appid = src_id;

    // Check if the provided connection exist
    if (udss_env.con_info.conidx == GAP_INVALID_CONIDX)
    {
        // The connection doesn't exist, request disallowed
        prf_server_error_ind_send((prf_env_struct *)&udss_env, PRF_ERR_REQ_DISALLOWED,
                                  UDSS_ERROR_IND, UDSS_ENABLE_REQ);
    }
    else
    {
        //Enable Attributes + Set Security Level
        attmdb_svc_set_permission(udss_env.shdl, param->sec_lvl);

        //Set User Height to specified value
        attmdb_att_set_value(udss_env.shdl + UDS_IDX_USER_HEIGHT_VAL,
                             sizeof(uint8_t), (uint8_t *)&param->user_height);

        //Set User Age to specified value
        attmdb_att_set_value(udss_env.shdl + UDS_IDX_USER_AGE_VAL,
                             sizeof(uint8_t), (uint8_t *)&param->user_age);
			
				//Set User Date of Birth to specified value
        attmdb_att_set_value(udss_env.shdl + UDS_IDX_USER_DATE_OF_BIRTH_VAL,
                             sizeof(struct date), (uint8_t *)&param->user_date_of_birth);
			
				//Set User DB Change Increment to specified value
        attmdb_att_set_value(udss_env.shdl + UDS_IDX_USER_DB_CHANGE_INCR_VAL,
                             sizeof(uint8_t), (uint8_t *)&param->user_db_change_incr);

				
        // Go to connected state
        ke_state_set(TASK_UDSS, UDSS_CONNECTED);
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GLPS_RACP_RSP_REQ message.
 * Send when a RACP requests is finished
 *
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int udss_ucp_rsp_req_handler(ke_msg_id_t const msgid,
                                     struct udss_ucp_rsp_req const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    struct uds_ucp_rsp ucp_rsp;
    // Status
    volatile uint8_t status = PRF_ERR_OK;
		volatile int x = sizeof(struct udss_ucp_rsp_req);
		volatile int xx = param->ucp_rsp.op_code;
    // check connection handle
    if(param->conhdl == gapc_get_conhdl(udss_env.con_info.conidx))
    {
        // check if op code valid
        if(param->ucp_rsp.op_code != UDS_REQ_RSP_CODE)
        {
            //Wrong op code
            status = PRF_ERR_INVALID_PARAM;
        }
        // check if UCP on going
//        else if((param->op_code != GLP_REQ_ABORT_OP) && !(GLPS_IS(RACP_ON_GOING)))
//        {
//            //Cannot send response since no RACP on going
//            status = PRF_ERR_REQ_DISALLOWED;
//        }
        else
        {
            /// Fill-in response code op code
            ucp_rsp.op_code = param->ucp_rsp.op_code;
            /// Fill-in request op code
            ucp_rsp.req_op_code = param->ucp_rsp.req_op_code;
						
            // Check if request op code is supported
            if((param->ucp_rsp.req_op_code < UDS_REQ_REG_NEW_USER)
               && ( param->ucp_rsp.req_op_code > UDS_REQ_DEL_USER_DATA))
            {
                /// Fill-in response value for not supported op code
                ucp_rsp.rsp_val = UDS_RSP_OP_CODE_NOT_SUP;
            }
            else
            {
                // Fill-in response value from application
                ucp_rsp.rsp_val = param->ucp_rsp.rsp_val;
                if((ucp_rsp.req_op_code == UDS_REQ_REG_NEW_USER)
                    &&(ucp_rsp.rsp_val == UDS_RSP_SUCCESS))
                {
                    ucp_rsp.parameter.reg_new_user.user_idx = param->ucp_rsp.parameter.reg_new_user.user_idx;
                }

            // There is no more UCP on going
//            UDSS_CLEAR(UCP_ON_GOING);

            // send UCP indication
            udss_send_ucp_rsp(&(ucp_rsp), udss_env.con_info.appid);
            }
        }
    }
    else
    {
        //Wrong Connection Handle
        status = PRF_ERR_INVALID_PARAM;
    }

    if (status != PRF_ERR_OK)
    {
        prf_server_error_ind_send((prf_env_struct *)&udss_env, status,
                                  UDSS_ERROR_IND, UDSS_UCP_RSP_REQ);
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GL2C_CODE_ATT_WR_CMD_IND message.
 * The handler will analyse what has been set and decide alert level
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
    uint8_t char_code = UDSS_ERR_CHAR;
    uint8_t status = PRF_APP_ERROR;

    if (KE_IDX_GET(src_id) == udss_env.con_info.conidx)
    {
//        uint8_t att_idx = GLPS_IDX(param->handle);
        status = PRF_ERR_OK;

        if (param->handle == udss_env.shdl + UDS_IDX_USER_HEIGHT_VAL)
        {
            char_code = UDSS_USER_HEIGHT_CHAR;
            //Set User Height to specified value
            attmdb_att_set_value(udss_env.shdl + UDS_IDX_USER_HEIGHT_VAL,
                             sizeof(uint8_t), (uint8_t *)&param->value[0]);
        }
        else if (param->handle == udss_env.shdl + UDS_IDX_USER_AGE_VAL)
        {
            char_code = UDSS_USER_AGE_CHAR;
            //Set User Age to specified value
            attmdb_att_set_value(udss_env.shdl + UDS_IDX_USER_AGE_VAL,
                                 sizeof(uint8_t), (uint8_t *)&param->value[0]);
        }
        else if (param->handle == udss_env.shdl + UDS_IDX_USER_DATE_OF_BIRTH_VAL)
        {
            char_code = UDSS_USER_DATE_OF_BIRTH_CHAR;
            //Set User Date of Birth to specified value
            attmdb_att_set_value(udss_env.shdl + UDS_IDX_USER_DATE_OF_BIRTH_VAL,
                                 sizeof(struct date), (uint8_t *)&param->value[0]);
        }
        else if (param->handle == udss_env.shdl + UDS_IDX_USER_DB_CHANGE_INCR_VAL)
        {
            char_code = UDSS_USER_DB_CHANGE_INCR_CHAR;
            //Set User DB Change Increment to specified value
            attmdb_att_set_value(udss_env.shdl + UDS_IDX_USER_DB_CHANGE_INCR_VAL,
                                 sizeof(uint8_t), (uint8_t *)&param->value[0]);
        }
        else if (param->handle == udss_env.shdl + UDS_IDX_USER_CTRL_POINT_VAL)
        {
            uint8_t reqstatus;
            uint8_t* value;
            uint16_t length;
            struct uds_ucp_req ucp_req;

            // Update the attribute value (note several write could be required since
            // attribute length > (ATT_MTU-3)
            attmdb_att_update_value(param->handle, param->length, param->offset,
                    (uint8_t*)&(param->value[0]));

            // retrieve full data.
            attmdb_att_get_value(param->handle, &length, &value);

            // unpack user control point value
            reqstatus = udss_unpack_ucp_req(value, length, &ucp_req);

            // check unpacked status
            switch(reqstatus)
            {
                case PRF_APP_ERROR:
                {
                    /* Do nothing, ignore request since it's not complete and maybe
                     * requires several peer write to be performed.
                     */
                }
                break;
                case PRF_ERR_OK:
                {
                    // check wich request shall be send to api task
                    switch(ucp_req.op_code)
                    {
                        case UDS_REQ_REG_NEW_USER:
                        case UDS_REQ_CONSENT:
                        case UDS_REQ_DEL_USER_DATA:
                        {
                            //forward request operation to application
                            struct udss_ucp_req_ind * req = KE_MSG_ALLOC(UDSS_UCP_REQ_IND,
                                    udss_env.con_info.appid, TASK_UDSS,
                                    udss_ucp_req_ind);

                            // UCP on going.
//                            UDSS_SET(UCP_ON_GOING);

                            req->conhdl = gapc_get_conhdl(udss_env.con_info.conidx);
                            req->ucp_req = ucp_req;

                            ke_msg_send(req);
                        }
                        break;
//                        case UDS_REQ_ABORT_OP:
//                        {
//                            // nothing to abort, send an error message.
//                            struct uds_ucp_rsp ucp_rsp;

//                            ucp_rsp.op_code = UDS_REQ_RSP_CODE;
//                            ucp_rsp.operand.rsp.op_code_req =
//                                    ucp_req.op_code;
//                            ucp_rsp.operand.rsp.status = UDS_RSP_ABORT_UNSUCCESSFUL;

//                            // send user control response indication
//                            udss_send_ucp_rsp(&(ucp_rsp),
//                                    TASK_UDSS);
//                        }
//                        break;
                        default:
                        {
                            // nothing to do since it's handled during unpack
                        }
                        break;
                    }
                }
                break;
                default:
                {
                    /* There is an error in user control request, inform peer
                     * device that request is incorrect. */
                    struct uds_ucp_rsp ucp_rsp;

                    ucp_rsp.op_code = UDS_REQ_RSP_CODE;
                    ucp_rsp.req_op_code = ucp_req.op_code;
                    ucp_rsp.rsp_val = reqstatus;

                    // send user control response indication
                    udss_send_ucp_rsp(&(ucp_rsp),
                            TASK_UDSS);
                }
                break;
            }
            
        }
        // not expected request
        else
        {
            status = ATT_ERR_WRITE_NOT_PERMITTED;
        }
    }
    
    if(param->response)
    {
        // Send Write Response
        atts_write_rsp_send(udss_env.con_info.conidx, param->handle, status);
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
    if (KE_IDX_GET(src_id) == udss_env.con_info.conidx)
    {
        udss_disable(param->conhdl);
    }

    return (KE_MSG_CONSUMED);
}


/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

///Disabled State handler definition.
const struct ke_msg_handler udss_disabled[] =
{
    {UDSS_CREATE_DB_REQ,         (ke_msg_func_t)udss_create_db_req_handler},
};

///Idle State handler definition.
const struct ke_msg_handler udss_idle[] =
{
    {UDSS_SET_CHAR_VAL_REQ,      (ke_msg_func_t)udss_set_char_val_req_handler},
    {UDSS_ENABLE_REQ,            (ke_msg_func_t)udss_enable_req_handler},
};

///Connected State handler definition.
const struct ke_msg_handler udss_connected[] =
{
    {UDSS_UCP_RSP_REQ,           (ke_msg_func_t) udss_ucp_rsp_req_handler},
    {GATTC_WRITE_CMD_IND,        (ke_msg_func_t) gattc_write_cmd_ind_handler},
};

/// Default State handlers definition
const struct ke_msg_handler udss_default_state[] =
{
    {GAPC_DISCONNECT_IND,        (ke_msg_func_t) gapc_disconnect_ind_handler},
};

///Specifies the message handler structure for every input state.
const struct ke_state_handler udss_state_handler[UDSS_STATE_MAX] =
{
    [UDSS_DISABLED]    = KE_STATE_HANDLER(udss_disabled),
    [UDSS_IDLE]        = KE_STATE_HANDLER(udss_idle),
    [UDSS_CONNECTED]   = KE_STATE_HANDLER(udss_connected),
};

///Specifies the message handlers that are common to all states.
const struct ke_state_handler udss_default_handler = KE_STATE_HANDLER(udss_default_state);

///Defines the place holder for the states of all the task instances.
ke_state_t udss_state[UDSS_IDX_MAX] __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY

#endif //BLE_UDS_SERVER

/// @} UDSSTASK
