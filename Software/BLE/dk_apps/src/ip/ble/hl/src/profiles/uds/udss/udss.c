/**
 ****************************************************************************************
 *
 * @file udss.c
 *
 * @brief User Data Service Server Implementation.
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
 * @addtogroup UDSS
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"

#if (BLE_UDS_SERVER)
#include "attm_util.h"
#include "atts_util.h"
#include "udss.h"
#include "udss_task.h"
#include "prf_utils.h"

/*
 * MACROS
 ****************************************************************************************
 */

/*
 * UDS ATTRIBUTES
 ****************************************************************************************
 */

/// Full UDS Database Description - Used to add attributes into the database
const struct attm_desc udss_att_db[UDS_IDX_NB] =
{
    // User Data Service Declaration
    [UDS_IDX_SVC]                           =   {ATT_DECL_PRIMARY_SERVICE, PERM(RD, ENABLE), sizeof(udss_dis_svc),
                                                 sizeof(udss_dis_svc), (uint8_t *)&udss_dis_svc},

    // User Height Characteristic Declaration
    [UDS_IDX_USER_HEIGHT_CHAR]              =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), sizeof(udss_user_height_char),
                                                 sizeof(udss_user_height_char), (uint8_t *)&udss_user_height_char},
    // User height Characteristic Value
    [UDS_IDX_USER_HEIGHT_VAL]               =   {ATT_CHAR_UDS_USER_HEIGHT, PERM(RD, ENABLE) | PERM(WR, ENABLE), sizeof(uint8_t), 0, NULL},

    // User Age Characteristic Declaration
    [UDS_IDX_USER_AGE_CHAR]                 =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), sizeof(udss_user_age_char),
                                                 sizeof(udss_user_age_char), (uint8_t *)&udss_user_age_char},
    // User Age Characteristic Value
    [UDS_IDX_USER_AGE_VAL]                  =   {ATT_CHAR_UDS_USER_AGE, PERM(RD, ENABLE) | PERM(WR, ENABLE), UDS_VAL_MAX_LEN, 0, NULL},

    // User Date of Birth Characteristic Declaration
    [UDS_IDX_USER_DATE_OF_BIRTH_CHAR]       =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), sizeof(udss_user_date_of_birth_char),
                                                 sizeof(udss_user_date_of_birth_char), (uint8_t *)&udss_user_date_of_birth_char},
    // User Date of Birth Characteristic Value
    [UDS_IDX_USER_DATE_OF_BIRTH_VAL]        =   {ATT_CHAR_UDS_USER_DATE_OF_BIRTH, PERM(RD, ENABLE) | PERM(WR, ENABLE), UDS_VAL_MAX_LEN, 0, NULL},

    // User Database Change Increment Characteristic Declaration
    [UDS_IDX_USER_DB_CHANGE_INCR_CHAR]      =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), sizeof(udss_user_db_change_incr_char),
                                                 sizeof(udss_user_db_change_incr_char), (uint8_t *)&udss_user_db_change_incr_char},
    // User Database Change Increment Characteristic Value
    [UDS_IDX_USER_DB_CHANGE_INCR_VAL]       =   {ATT_CHAR_UDS_USER_DB_CHANGE_INCR, PERM(RD, ENABLE) | PERM(WR, ENABLE) | PERM(NTF, ENABLE), UDS_VAL_MAX_LEN, 0, NULL},

    //  User Database Change Increment Characteristic - Client Characteristic Configuration Descriptor
    [UDS_IDX_USER_DB_CHANGE_INCR_NTF_CFG]   =   {ATT_DESC_CLIENT_CHAR_CFG, PERM(RD, ENABLE)|PERM(WR, ENABLE), sizeof(uint16_t), 0, NULL},

    // User Index Characteristic Declaration
    [UDS_IDX_USER_INDEX_CHAR]               =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), sizeof(udss_user_index_char),
                                                 sizeof(udss_user_index_char), (uint8_t *)&udss_user_index_char},
    // User Index Characteristic Value
    [UDS_IDX_USER_INDEX_VAL]                =   {ATT_CHAR_UDS_USER_DATA_INDEX, PERM(RD, ENABLE), UDS_VAL_MAX_LEN, 0, NULL},

    // User Control Point Characteristic Declaration
    [UDS_IDX_USER_CTRL_POINT_CHAR]          =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), sizeof(udss_user_ctrl_point_char),
                                                 sizeof(udss_user_ctrl_point_char), (uint8_t *)&udss_user_ctrl_point_char},
    // User Control Point Characteristic Value
    [UDS_IDX_USER_CTRL_POINT_VAL]           =   {ATT_CHAR_UDS_USER_CTRL_PT, PERM(WR, ENABLE) | PERM(IND, ENABLE), UDS_VAL_MAX_LEN, 0, NULL},
};

const att_svc_desc_t udss_dis_svc = ATT_SVC_USER_DATA;

const struct att_char_desc udss_user_height_char              = ATT_CHAR(ATT_CHAR_PROP_RD | ATT_CHAR_PROP_WR,
                                                                         UDS_USER_HEIGHT_CHAR,
                                                                         ATT_CHAR_UDS_USER_HEIGHT);
const struct att_char_desc udss_user_age_char                 = ATT_CHAR(ATT_CHAR_PROP_RD | ATT_CHAR_PROP_WR,
                                                                         UDS_USER_AGE_CHAR,
                                                                         ATT_CHAR_UDS_USER_AGE);
const struct att_char_desc udss_user_date_of_birth_char       = ATT_CHAR(ATT_CHAR_PROP_RD | ATT_CHAR_PROP_WR,
                                                                         UDS_USER_DATE_OF_BIRTH_CHAR,
                                                                         ATT_CHAR_UDS_USER_DATE_OF_BIRTH);
const struct att_char_desc udss_user_db_change_incr_char      = ATT_CHAR(ATT_CHAR_PROP_RD | ATT_CHAR_PROP_WR | ATT_CHAR_PROP_NTF,
                                                                         UDS_USER_DB_CHANGE_INCR_CHAR,
                                                                         ATT_CHAR_UDS_USER_DB_CHANGE_INCR);
const struct att_char_desc udss_user_index_char               = ATT_CHAR(ATT_CHAR_PROP_RD,
                                                                         UDS_USER_INDEX_CHAR,
                                                                         ATT_CHAR_UDS_USER_DATA_INDEX);
const struct att_char_desc udss_user_ctrl_point_char          = ATT_CHAR(ATT_CHAR_PROP_WR | ATT_CHAR_PROP_IND,
                                                                         UDS_USER_CTRL_POINT_CHAR,
                                                                         ATT_CHAR_UDS_USER_CTRL_PT);

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

struct udss_env_tag udss_env __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY

/// User Data Service task descriptor
static const struct ke_task_desc TASK_DESC_UDSS = {udss_state_handler, &udss_default_handler, udss_state, UDSS_STATE_MAX, UDSS_IDX_MAX};


/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void udss_init(void)
{
    // Reset environment
    memset(&udss_env, 0, sizeof(udss_env));

    // Create UDSS task
    ke_task_create(TASK_UDSS, &TASK_DESC_UDSS);

    // Set task in disabled state
    ke_state_set(TASK_UDSS, UDSS_DISABLED);
}

void udss_disable(uint16_t conhdl) 
{
    // Inform the application about the disconnection
    struct udss_disable_ind *ind = KE_MSG_ALLOC(UDSS_DISABLE_IND,
                                                udss_env.con_info.appid, TASK_UDSS,
                                                udss_disable_ind);

    ind->conhdl = conhdl;

    ke_msg_send(ind);

    //Disable UDS in database
    attmdb_svc_set_permission(udss_env.shdl, PERM(SVC, DISABLE));

    //Go to idle state
    ke_state_set(TASK_UDSS, UDSS_IDLE);
}

uint8_t udss_unpack_ucp_req(uint8_t *packed_val, uint16_t length,
                                        struct uds_ucp_req* ucp_req)
{
    uint8_t cursor = 0;

    // verify that enough data present
    if(length < 1)
    {
        return PRF_APP_ERROR;
    }

    // retrieve function op code
    ucp_req->op_code = packed_val[cursor];
    cursor++;

    // clear user control point parameter structure
    memset(&(ucp_req->parameter), 0, sizeof(ucp_req->parameter));

    // check if op code is supported
    if((ucp_req->op_code < UDS_REQ_REG_NEW_USER)
            || (ucp_req->op_code > UDS_REQ_DEL_USER_DATA))
    {
        return UDS_RSP_OP_CODE_NOT_SUP;
    }

    // Delete user data operation doesn't require any other parameter
    if(ucp_req->op_code == UDS_REQ_DEL_USER_DATA)
    {
        return PRF_ERR_OK;
    }

//    // Abort operation don't require any other parameter
//    if(ucp_req->op_code == UDS_REQ_ABORT_OP)
//    {
//        return PRF_ERR_OK;
//    }

    // Register new user function
    if(ucp_req->op_code == UDS_REQ_REG_NEW_USER)
    {
        // check sufficient data available
        if((length - cursor) < 2)
        {        
            return UDS_RSP_INVALID_PARAMETER;
        }
        
        // retrieve consent code
        ucp_req->parameter.reg_new_user.consent_code = co_read16p(packed_val + cursor);
        cursor +=2;
        
        if(ucp_req->parameter.reg_new_user.consent_code > UDS_CONSENT_CODE_MAX_VAL)
        {
            return UDS_RSP_INVALID_PARAMETER;
        }
    }

    // Consent function
    if(ucp_req->op_code == UDS_REQ_CONSENT)
    {
        // check sufficient data available
        if((length - cursor) < 3)
        {        
            return UDS_RSP_INVALID_PARAMETER;
        }
        // retrieve user index
        ucp_req->parameter.consent.user_idx = *(packed_val + cursor);
        cursor++;
        // retrieve consent code
        ucp_req->parameter.consent.consent_code = co_read16p(packed_val + cursor);
        cursor +=2;

        if(ucp_req->parameter.consent.consent_code > UDS_CONSENT_CODE_MAX_VAL)
        {
            return UDS_RSP_INVALID_PARAMETER;
        }
    }

    // no errors
    return PRF_ERR_OK;
}

uint8_t udss_pack_ucp_rsp(uint8_t *packed_val,
                          struct uds_ucp_rsp* ucp_rsp)
{
    uint8_t cursor = 0;

    // set response op code
    packed_val[cursor] = ucp_rsp->op_code;
    cursor++;

    // set request op code
    packed_val[cursor] = ucp_rsp->req_op_code;
    cursor++;

    // set response value
    packed_val[cursor] = ucp_rsp->rsp_val;
    cursor++;

    // fill in parameter with user index for register new user function
    if((ucp_rsp->req_op_code == UDS_REQ_REG_NEW_USER)
       &&(ucp_rsp->rsp_val == UDS_RSP_SUCCESS))
    {
        *(packed_val+cursor) = ucp_rsp->parameter.reg_new_user.user_idx;
        cursor ++;
    }

    return cursor;
}

uint8_t udss_send_ucp_rsp(struct uds_ucp_rsp * ucp_rsp,
                                      ke_task_id_t ucp_ind_src)
{
    uint8_t status = PRF_ERR_IND_DISABLED;
    
    struct attm_elmt * attm_elmt;

    // save response requester task
//    udss_env.ucp_ind_src = ucp_ind_src;
    
    // retrieve attdb data value pointers
    attm_elmt = attmdb_get_attribute(udss_env.shdl + UDS_IDX_USER_CTRL_POINT_VAL);
    
    // pack data (updates database)
    attm_elmt->length = udss_pack_ucp_rsp(attm_elmt->value, ucp_rsp);
    
    // send indication through GATT
    prf_server_send_event((prf_env_struct *)&udss_env, true,
            udss_env.shdl + UDS_IDX_USER_CTRL_POINT_VAL);
    
    status = PRF_ERR_OK;

    return status;
}

#endif //BLE_UDS_SERVER

/// @} UDSS
