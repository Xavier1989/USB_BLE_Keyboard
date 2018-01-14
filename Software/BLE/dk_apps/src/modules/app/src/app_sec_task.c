/**
 ****************************************************************************************
 *
 * @file app_sec_task.c
 *
 * @brief Application Security Task implementation
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APPSECTASK
 * @{
 ****************************************************************************************
 */

#define GAPC_ENCRYPT_REQ_IND_HANDLER_STATUS_BLOCKED       (0)
#define GAPC_ENCRYPT_REQ_IND_HANDLER_STATUS_KEY_NOT_FOUND (1)
#define GAPC_ENCRYPT_REQ_IND_HANDLER_STATUS_OK            (2)

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"

#if (BLE_APP_SEC)

#include "app_sec.h"                   // Application Security Definition
#include "app_sec_task.h"              // Application Security Definition
#include "gapc_task.h"                 // GAP Controller Task API
#include "app_api.h"                

#if (NVDS_SUPPORT)
#include "nvds.h"                      // NVDS Definitions
#endif //(NVDS_SUPPORT)

/*
 * GLOBAL VARIABLES DEFINITION
 ****************************************************************************************
 */



/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @brief Handles reception of bond request command
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int gapc_bond_req_ind_handler(ke_msg_id_t const msgid,
        struct gapc_bond_req_ind *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    
    switch(param->request)
    {
        // Bond Pairing request
        case GAPC_PAIRING_REQ:
        {
            app_send_pairing_rsp_func(param);
        }
        break;

        // Used to retrieve pairing Temporary Key
        case GAPC_TK_EXCH:
        {
            if(param->data.tk_type == GAP_TK_DISPLAY)
            {
                app_send_tk_exch_func(param);
            }
            else if (param->data.tk_type == GAP_TK_KEY_ENTRY) 
            {
                app_mitm_passcode_entry_func(src_id, dest_id);
            }
            else
            {
                ASSERT_ERR(0);
            }
        }
        break;

        case GAPC_IRK_EXCH:
            
            app_send_irk_exch_func(param);

            break;
            
        case GAPC_CSRK_EXCH:
        {
            app_send_csrk_exch_func(param);
        }
        break;


        // Used for Long Term Key exchange
        case GAPC_LTK_EXCH:
        {
            app_send_ltk_exch_func(param);
        }
        break;
        
        
        default:
        {
            ASSERT_ERR(0);
        }
        break;
    }
    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles reception of bond indication
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int gapc_bond_ind_handler(ke_msg_id_t const msgid,
        struct gapc_bond_ind *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    
    switch(param->info)
    {
        // Bond Pairing request
        case GAPC_PAIRING_SUCCEED:
        {
            // Save Authentication level
            bond_info.env.auth =  param->data.auth;

            if (bond_info.env.auth & GAP_AUTH_BOND)
            {
                bond_info.env.peer_addr_type = app_env.peer_addr_type;
                           
                bond_info.env.peer_addr = app_env.peer_addr;
            }
            app_paired_func();
        }
        break;


        case GAPC_PAIRING_FAILED:
        {
            // disconnect here
            app_disconnect();

            // no need to clear bond data
        }
        break;


        case (GAPC_LTK_EXCH):
        {
            // app_store_ltk_func();
        } 
        break;

        
        case (GAPC_IRK_EXCH):
        {
            bond_info.irk = param->data.irk.irk;
            bond_info.env.peer_addr = param->data.irk.addr.addr; // real BD addr
            bond_info.ext_info |= IRK_FLAG; // set IRK flag to indicate that an IRK is included
            app_alt_pair_store_bond_data();
        } 
        break;

        
        case (GAPC_CSRK_EXCH):
        {
            // app_store_csrk_func();
        } 
        break;


        default:
        {
            ASSERT_ERR(0);
        }
        break;
    }
    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief Handles reception of encrypt request command
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int gapc_encrypt_req_ind_handler(ke_msg_id_t const msgid,
        struct gapc_encrypt_req_ind *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    struct gapc_encrypt_cfm* cfm = KE_MSG_ALLOC(GAPC_ENCRYPT_CFM, src_id, dest_id, gapc_encrypt_cfm);
    int status;
    
    if (!app_validate_encrypt_req_func(param))
    {
        cfm->found = false;
        status = GAPC_ENCRYPT_REQ_IND_HANDLER_STATUS_BLOCKED;
    }
    else
    {
        if(((bond_info.env.auth & GAP_AUTH_BOND) != 0)
            && (memcmp(&(bond_info.env.rand_nb), &(param->rand_nb), RAND_NB_LEN) == 0)
            && (bond_info.env.ediv == param->ediv))
        {
            cfm->found = true;
            cfm->key_size = bond_info.env.key_size;
            memcpy(&(cfm->ltk), &(bond_info.env.ltk), KEY_LEN);
            // update connection auth
            app_connect_confirm(bond_info.env.auth);
            app_sec_encrypt_complete_func();
            status = GAPC_ENCRYPT_REQ_IND_HANDLER_STATUS_OK;
        }
        else
        {
            cfm->found = false;
            status = GAPC_ENCRYPT_REQ_IND_HANDLER_STATUS_KEY_NOT_FOUND;
        }
    }
    
    ke_msg_send(cfm);
    
    if (status != GAPC_ENCRYPT_REQ_IND_HANDLER_STATUS_OK)
    {
        if (!HAS_SECURITY_REQUEST_SEND)
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
        else if (status == GAPC_ENCRYPT_REQ_IND_HANDLER_STATUS_KEY_NOT_FOUND)
        {
            // An LL_REJECT_IND with reason "Pin or Key missing" will be sent by the stack.
            
            // When the APP_HID_END_TIMER expires, an L2CAP_SECURITY_REQUEST will be sent if SECURITY_REQUEST_SEND_ON is selected,
            // else the connection will be dropped.
        }
        else if (status == 0)
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
    }
    // else continue

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles reception of encrypt indication
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int gapc_encrypt_ind_handler(ke_msg_id_t const msgid,
        struct gapc_encrypt_ind *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    app_sec_encrypt_ind_func();

    return (KE_MSG_CONSUMED);
}

#endif //(BLE_APP_SEC)

/// @} APPSECTASK
