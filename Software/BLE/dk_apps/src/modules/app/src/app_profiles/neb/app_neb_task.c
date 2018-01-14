/**
 ****************************************************************************************
 *
 * @file app_neb_task.c
 *
 * @brief This file contains definitions related to the Nebulizer application
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_NEB
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

#if (BLE_APP_NEB)

#include <string.h>
#include "app_neb_task.h"
#include "app_neb.h"
#include "co_utils.h"
#include "gtl_task.h"
#include "ke_msg.h"          // Kernel message defines
#include "app_task.h"
#include "nbps_task.h"


/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * GLOBAL VARIABLES
 ****************************************************************************************
 */

/// Message Length Table
static const uint8_t app_neb_msg_par_len_tab[NEB_NB_RX_MSG] =
{
    NEB_MSG_PAR_LEN_CONFIRM       ,
    NEB_MSG_PAR_LEN_START         ,
    NEB_MSG_PAR_LEN_STOP          ,
    NEB_MSG_PAR_LEN_DURATION      ,
    NEB_MSG_PAR_LEN_RESP_CYCLE_DUR,
    NEB_MSG_PAR_LEN_RESP_FLOW     ,
    NEB_MSG_PAR_LEN_ACT_CNX_MODE  ,
};

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Check the length of message parameters
 * @param[in]     id            Identifier of message
 * @param[in]     length        Length of received parameters
 ****************************************************************************************
 */
static bool app_neb_msg_par_len_check(uint8_t id, uint8_t length)
{
    bool result = true;

    /// Parse the message length table
    if(id < NEB_NB_RX_MSG)
    {
        if(app_neb_msg_par_len_tab[id] != length)
        {
            result = false;
        }
    }

    return result;
}

/**
 ****************************************************************************************
 * @brief Checksum computation and checking
 * @param[in]     p_params      Pointer on message parameters
 * @param[in]     length        Length of received parameters (including ID and CRC byte)
 ****************************************************************************************
 */
static bool app_neb_msg_crc_check(uint8_t * p_params, uint16_t length)
{
//    uint8_t crc = GTL_NEB_MSG_TYPE;
    uint8_t crc = 0x10;
    uint8_t * p_crc = p_params + length - sizeof(uint8_t);
    uint8_t * p_start = p_params - sizeof(uint16_t);

    // Add all bytes in the packet
    while(p_start < p_crc)
    {
        crc += *(p_start++);
    }

    // 1s complement
    crc = ~crc;

    return (*p_crc == crc);
}


/*
 * MESSAGE HANDLERS
 ****************************************************************************************
 */

static int nbps_create_db_cfm_handler(ke_msg_id_t const msgid,
                                      struct nbps_create_db_cfm const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    if (ke_state_get(dest_id) == APP_NEB_DISABLED)
    {
        if (param->status == PRF_ERR_OK)
        {
            // Go to Idle state
            ke_state_set(TASK_APP_NEB, APP_NEB_IDLE);
        }

        // Inform the Application Manager
        struct app_module_init_cmp_evt *cfm = KE_MSG_ALLOC(APP_MODULE_INIT_CMP_EVT,
                                                           TASK_APP, TASK_APP_NEB,
                                                           app_module_init_cmp_evt);

        cfm->status = param->status;

        ke_msg_send(cfm);
    }

    return (KE_MSG_CONSUMED);
}

static int nbps_enable_cfm_handler(ke_msg_id_t const msgid,
                                   struct nbps_enable_cfm const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
    ASSERT_INFO(param->status == CO_ERROR_NO_ERROR, param->status, 0);

    if (ke_state_get(dest_id) == APP_NEB_IDLE)
    {
        // Go to Idle state
        ke_state_set(TASK_APP_NEB, APP_NEB_CONNECTED);
    }
    else
    {
        ASSERT_INFO(0, ke_state_get(dest_id), 0);
    }

    return (KE_MSG_CONSUMED);
}

static int nbps_disable_ind_handler(ke_msg_id_t const msgid,
                                    struct nbps_disable_ind const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    if (ke_state_get(dest_id) == APP_NEB_CONNECTED)
    {
        // Go to Idle state
        ke_state_set(TASK_APP_NEB, APP_NEB_IDLE);
    }

    return (KE_MSG_CONSUMED);
}

static int nbps_neb_ind_cfm_handler(ke_msg_id_t const msgid,
                                    struct nbps_neb_ind_cfm const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    ASSERT_INFO(param->status == PRF_ERR_OK
            || param->status == PRF_ERR_DISCONNECTED
            || param->status == PRF_ERR_REQ_DISALLOWED, param->status, 0);

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles message from Nebulizer system.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int app_neb_msg_rx_handler(ke_msg_id_t const msgid,
                                 void const *param,
                                 ke_task_id_t const dest_id,
                                 ke_task_id_t const src_id)
{
    //extract the ke_msg pointer from the param passed
    struct ke_msg * p_msg = ke_param2msg(param);

    uint8_t* p_params = (uint8_t*) param;

    do
    {
        if(p_msg->param_len == 0)
        {
            // Send dummy neb message in response
            ke_msg_send_basic(GTL_NEB_MSG_TX, TASK_GTL, TASK_APP_NEB);

            break;
        }

        // Check CRC
        if(!app_neb_msg_crc_check(p_params, p_msg->param_len))
        {
            // Reject message
            app_neb_msg_tx_confirm_basic(*p_params, NEB_ERR_INCORRECT_CRC);

            break;
        }

        // Check parameters length (excluding ID and CRC bytes)
        if(!app_neb_msg_par_len_check(*p_params, p_msg->param_len - 2))
        {
            // Reject message
            app_neb_msg_tx_confirm_basic(*p_params, NEB_ERR_INCORRECT_LENGTH);

            break;
        }


        switch(*p_params++)
        {
            case NEB_MSG_ID_START:
            {
                // Decode message
                struct app_neb_msg_start msg;

                msg.reason = *(p_params);

                // Handle message
                app_neb_msg_rx_start(&msg);
            }
            break;
            case NEB_MSG_ID_STOP:
            {
                // Decode message
                struct app_neb_msg_stop msg;

                msg.reason = *(p_params);

                // Handle message
                app_neb_msg_rx_stop(&msg);
            }
            break;
            case NEB_MSG_ID_DURATION:
            {
                // Decode message
                struct app_neb_msg_duration msg;

                msg.duration = co_read16p(p_params);

                // Handle message
                app_neb_msg_rx_duration(&msg);
            }
            break;
            case NEB_MSG_ID_RESP_FLOW:
            {
                // Decode message
                struct app_neb_msg_resp_flow msg;

                msg.flow = co_read16p(p_params);

                // Handle message
                app_neb_msg_rx_resp_flow(&msg);
            }
            break;
            case NEB_MSG_ID_RESP_CYCLE_DUR:
            {
                // Decode message
                struct app_neb_msg_resp_cycle_dur msg;

                msg.duration = *(p_params);

                // Handle message
                app_neb_msg_rx_resp_cycle_dur(&msg);
            }
            break;
            case NEB_MSG_ID_ACT_CNX_MODE:
            {
                app_neb_msg_rx_act_cnx_mode();
            }
            break;
            case NEB_MSG_ID_CONFIRM:
            {
                // Decode message
                struct app_neb_msg_confirm msg;

                msg.id = *(p_params);
                msg.status = *(p_params+1);
                msg.param_length = p_msg->param_len - 2;
                memcpy(&msg.ret_params[0], p_params + 2, p_msg->param_len - 2);

                // Handle message
                app_neb_msg_rx_confirm(&msg);
            }
            break;

            default:
            {
                // Unknown message
                app_neb_msg_tx_confirm_basic(*(p_params-1), NEB_ERR_UNKNOWN_MSG);
            }
            break;
        }


    } while(0);

    return (KE_MSG_CONSUMED);
}



/*
 * TASK DESCRIPTOR DEFINITIONS
 ****************************************************************************************
 */

/// Specifies the message handlers that are common to all states.
const struct ke_msg_handler app_neb_default_state[] =
{
    {NBPS_CREATE_DB_CFM,    (ke_msg_func_t)nbps_create_db_cfm_handler},
    {NBPS_ENABLE_CFM,       (ke_msg_func_t)nbps_enable_cfm_handler},
    {NBPS_DISABLE_IND,      (ke_msg_func_t)nbps_disable_ind_handler},
    {NBPS_NEB_IND_CFM,      (ke_msg_func_t)nbps_neb_ind_cfm_handler},

    {APP_NEB_MSG_RX,        (ke_msg_func_t)app_neb_msg_rx_handler},
};

/// Specifies the message handlers that are common to all states.
const struct ke_state_handler app_neb_default_handler = KE_STATE_HANDLER(app_neb_default_state);

/// Defines the placeholder for the states of all the task instances.
ke_state_t app_neb_state[APP_NEB_IDX_MAX] __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY 


#endif //BLE_APP_NEB

/// @} APP_NEB
