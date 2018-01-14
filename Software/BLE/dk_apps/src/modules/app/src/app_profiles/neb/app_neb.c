/**
 ****************************************************************************************
 *
 * @file app_neb.c
 *
 * @brief Nebulizer Application entry point
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
#include "app_neb.h"                  // Nebulizer application

#if (BLE_APP_NEB)

#include <string.h>                  // string manipulation and functions

#include "app_neb_task.h"            // application task definitions

#include "nbps_task.h"
#include "gtl_task.h"
#include "ke_msg.h"          // kernel message defines

#include "arch.h"                    // platform definitions

#include "co_bt.h"

#if RTC_SUPPORT
#include "rtc.h"
#endif //RTC_SUPPORT

#if (NVDS_SUPPORT)
#include "nvds.h"                    // NVDS Definitions
#endif //(NVDS_SUPPORT)

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
/// Nebulizer application environment structure
struct app_neb_env_tag app_neb_env __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY


/// APP task descriptor
static const struct ke_task_desc TASK_DESC_APP_NEB = {NULL, &app_neb_default_handler, app_neb_state, APP_NEB_STATE_MAX, APP_NEB_IDX_MAX};

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */


static void app_neb_insert_abs_time(struct nbps_neb_record_send_req *req)
{
    #if (RTC_SUPPORT)
    struct rtc_time time;

    // Get time stamp from RTC
    rtc_get_time(&time);

    // Format result
    req->neb_rec.timestamp.year     = time.tm_year;
    req->neb_rec.timestamp.month    = time.tm_mon;
    req->neb_rec.timestamp.day      = time.tm_mday;
    req->neb_rec.timestamp.hour     = time.tm_hour;
    req->neb_rec.timestamp.min      = time.tm_min;
    req->neb_rec.timestamp.sec      = time.tm_sec;

    #else //(RTC_SUPPORT)
    // Default time
    req->neb_rec.timestamp.year     = 2015;
    req->neb_rec.timestamp.month    = 10;
    req->neb_rec.timestamp.day      = 21;
    req->neb_rec.timestamp.hour     = 16;
    req->neb_rec.timestamp.min      = 29;
    req->neb_rec.timestamp.sec      = 0;
    #endif //(RTC_SUPPORT)
}


static void app_neb_msg_tx_insert_CRC(uint8_t * p_params, uint16_t length)
{
    uint8_t crc = 0x00;
    uint8_t * p_crc = p_params + length - sizeof(uint8_t);
    uint8_t * p_start = p_params - sizeof(uint16_t);

    // Add all bytes in the packet
    while(p_start < p_crc)
    {
        crc += *(p_start++);
    }

    // 1s complement
    crc = ~crc;

    *p_crc = crc;
}



/*
 * EXPORTED FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void app_neb_init(void)
{
    #if (NVDS_SUPPORT)
    uint8_t length = NVDS_LEN_NEB_ID;
    #endif // NVDS_SUPPORT

    // Reset the environment
    memset(&app_neb_env, 0, sizeof(app_neb_env));


    #if (NVDS_SUPPORT)
    // Get the current nebulizer ID from the storage
    if (nvds_get(NVDS_TAG_NEB_ID, &length, (uint8_t *)&app_neb_env.curr_neb_id) != NVDS_OK)
    #endif // NVDS_SUPPORT
    {
        // Initialize Nebulizer ID to 0
        app_neb_env.curr_neb_id = 0;
    }

    // Create APP task
    ke_task_create(TASK_APP_NEB, &TASK_DESC_APP_NEB);

    // Go to Disabled state
    ke_state_set(TASK_APP_NEB, APP_NEB_DISABLED);
}

void app_neb_create_db_send(void)
{
    // Add NBS in the database
    struct nbps_create_db_req *req = KE_MSG_ALLOC(NBPS_CREATE_DB_REQ,
                                                  TASK_NBPS, TASK_APP_NEB,
                                                  nbps_create_db_req);

    ke_msg_send(req);
}

void app_neb_enable_prf(uint16_t conhdl)
{
    // Allocate the message
    struct nbps_enable_req * req = KE_MSG_ALLOC(NBPS_ENABLE_REQ,
                                                TASK_NBPS, TASK_APP_NEB,
                                                nbps_enable_req);

    // Fill in the parameter structure
    req->conhdl             = conhdl;

    // Inspiratory Thresholds
    req->neb_insp_thres.min_insp_thres      = 12500; // 1 L/sec
    req->neb_insp_thres.max_insp_thres      = 37500; // 3 L/sec
    req->neb_insp_thres.min_insp_dur_thres  = 19;    // 1.5 sec
    req->neb_insp_thres.max_insp_dur_thres  = 34;    // 2.7 sec

    // Features
    req->neb_feat.unit_0 = 1;
    req->neb_feat.unit_1 = 2;

    ke_msg_send(req);

    // Save connection handle
    app_neb_env.conhdl = conhdl;
}


/*
 ****************************************************************************************
 * RX Messages API
 ****************************************************************************************
 */

void app_neb_msg_rx_start(struct app_neb_msg_start* msg)
{
    uint8_t status = NEB_ERR_SUCCESS;

    // Check parameters
    switch (msg->reason)
    {
        case NEB_MSG_START_REASON_START:
        {
            // Increment nebulizer ID
            app_neb_env.curr_neb_id++;

            #if (NVDS_SUPPORT)
            // Put the current nebulizer ID into storage
            nvds_put(NVDS_TAG_NEB_ID, NVDS_LEN_NEB_ID, (uint8_t *)&app_neb_env.curr_neb_id);
            #endif // NVDS_SUPPORT

            // Clear record ID
            app_neb_env.next_rec_id = 0;
        }
        break;
        case NEB_MSG_START_REASON_RESTART:
        break;
        default:
        {
            status = NEB_ERR_INVALID_PARAMS;
        }
        break;
    }

    if ((status == NEB_ERR_SUCCESS) && (ke_state_get(TASK_APP_NEB) == APP_NEB_CONNECTED))
    {
        // Allocate message to profile
        struct nbps_neb_record_send_req *req = KE_MSG_ALLOC(NBPS_NEB_RECORD_SEND_REQ,
                TASK_NBPS, TASK_APP_NEB,
                nbps_neb_record_send_req);
        req->conhdl = app_neb_env.conhdl;
        req->neb_rec.neb_id = app_neb_env.curr_neb_id;
        req->neb_rec.rec_id = app_neb_env.next_rec_id;
        req->neb_rec.flags = NEB_REC_START_FLAG;

        // Parameters
        req->neb_rec.reason = msg->reason;

        // Get absolute time stamp
        app_neb_insert_abs_time(req);

        ke_msg_send(req);

        // Increment record ID
        app_neb_env.next_rec_id++;
    }

    // Confirm the message
    app_neb_msg_tx_confirm_basic(NEB_MSG_ID_START, status);
}

void app_neb_msg_rx_stop(struct app_neb_msg_stop* msg)
{
    uint8_t status = NEB_ERR_SUCCESS;

    // Check parameters
    switch (msg->reason)
    {
        case NEB_MSG_STOP_REASON_USER:
        case NEB_MSG_STOP_REASON_LOW_BATT:
        case NEB_MSG_STOP_REASON_AUTO:
            break;
        default:
        {
            status = NEB_ERR_INVALID_PARAMS;
        }
        break;
    }

    if ((status == NEB_ERR_SUCCESS) && (ke_state_get(TASK_APP_NEB) == APP_NEB_CONNECTED))
    {
        // Allocate message to profile
        struct nbps_neb_record_send_req *req = KE_MSG_ALLOC(NBPS_NEB_RECORD_SEND_REQ,
                TASK_NBPS, TASK_APP_NEB,
                nbps_neb_record_send_req);
        req->conhdl = app_neb_env.conhdl;
        req->neb_rec.neb_id = app_neb_env.curr_neb_id;
        req->neb_rec.rec_id = app_neb_env.next_rec_id;
        req->neb_rec.flags = NEB_REC_STOP_FLAG;

        // Parameters
        req->neb_rec.reason = msg->reason;

        // Get absolute time stamp
        app_neb_insert_abs_time(req);

        ke_msg_send(req);

        // Increment record ID
        app_neb_env.next_rec_id++;
    }

    // Confirm the message
    app_neb_msg_tx_confirm_basic(NEB_MSG_ID_STOP, status);
}

void app_neb_msg_rx_duration(struct app_neb_msg_duration* msg)
{
    uint8_t status = NEB_ERR_SUCCESS;

    if(ke_state_get(TASK_APP_NEB) == APP_NEB_CONNECTED)
    {
        // Allocate message to profile
        struct nbps_neb_record_send_req *req = KE_MSG_ALLOC(NBPS_NEB_RECORD_SEND_REQ,
                TASK_NBPS, TASK_APP_NEB,
                nbps_neb_record_send_req);
        req->conhdl = app_neb_env.conhdl;
        req->neb_rec.neb_id = app_neb_env.curr_neb_id;
        req->neb_rec.rec_id = app_neb_env.next_rec_id;
        req->neb_rec.flags = NEB_REC_DUR_FLAG;

        // Parameters
        req->neb_rec.session_dur = msg->duration;

        ke_msg_send(req);

        // Increment record ID
        app_neb_env.next_rec_id++;
    }

    // Confirm the message
    app_neb_msg_tx_confirm_basic(NEB_MSG_ID_DURATION, status);
}

void app_neb_msg_rx_resp_cycle_dur(struct app_neb_msg_resp_cycle_dur* msg)
{
    uint8_t status = NEB_ERR_SUCCESS;

    if(msg->duration != 0 && (ke_state_get(TASK_APP_NEB) == APP_NEB_CONNECTED))
    {
        // Allocate message to profile
        struct nbps_neb_record_send_req *req = KE_MSG_ALLOC(NBPS_NEB_RECORD_SEND_REQ,
                TASK_NBPS, TASK_APP_NEB,
                nbps_neb_record_send_req);
        req->conhdl = app_neb_env.conhdl;
        req->neb_rec.neb_id = app_neb_env.curr_neb_id;
        req->neb_rec.rec_id = app_neb_env.next_rec_id;
        req->neb_rec.flags = NEB_REC_RESP_DUR_FLAG;

        // Parameters
        req->neb_rec.resp_cycle_dur = msg->duration;

        ke_msg_send(req);

        // Increment record ID
        app_neb_env.next_rec_id++;
    }

    // Confirm the message
    app_neb_msg_tx_confirm_basic(NEB_MSG_ID_RESP_CYCLE_DUR, status);
}

void app_neb_msg_rx_resp_flow(struct app_neb_msg_resp_flow* msg)
{
    uint8_t status = NEB_ERR_SUCCESS;

    if(ke_state_get(TASK_APP_NEB) == APP_NEB_CONNECTED)
    {
        ASSERT_INFO(app_neb_env.resp_flow_sample_nb < NEB_RESP_FLOW_PCK_NB, app_neb_env.resp_flow_sample_nb, NEB_RESP_FLOW_PCK_NB);

        // Store sample
        app_neb_env.resp_flow_sample_buf[app_neb_env.resp_flow_sample_nb++] = msg->flow;

        // Check the buffer size
        if(app_neb_env.resp_flow_sample_nb == NEB_RESP_FLOW_PCK_NB)
        {
            // Allocate message to profile
            struct nbps_neb_record_send_req *req = KE_MSG_ALLOC(NBPS_NEB_RECORD_SEND_REQ,
                    TASK_NBPS, TASK_APP_NEB,
                    nbps_neb_record_send_req);
            req->conhdl = app_neb_env.conhdl;
            req->neb_rec.neb_id = app_neb_env.curr_neb_id;
            req->neb_rec.rec_id = app_neb_env.next_rec_id;
            req->neb_rec.flags = NEB_REC_RESP_RATE_FLAG;

            // Parameters
            req->neb_rec.nb_resp_rate = NEB_RESP_FLOW_PCK_NB;
            memcpy(&req->neb_rec.resp_rate[0], &app_neb_env.resp_flow_sample_buf[0], NEB_RESP_FLOW_PCK_NB * sizeof(uint16_t));

            ke_msg_send(req);

            // Clear number of samples
            app_neb_env.resp_flow_sample_nb = 0;

            // Increment record ID
            app_neb_env.next_rec_id++;
        }
    }

    // Confirm the message
    app_neb_msg_tx_confirm_basic(NEB_MSG_ID_RESP_FLOW, status);
}

void app_neb_msg_rx_act_cnx_mode(void)
{
    uint8_t status = NEB_ERR_SUCCESS;

    // Confirm the message
    app_neb_msg_tx_confirm_basic(NEB_MSG_ID_ACT_CNX_MODE, status);
}

void app_neb_msg_rx_confirm(struct app_neb_msg_confirm* msg)
{

}


/*
 ****************************************************************************************
 * TX Messages API
 ****************************************************************************************
 */

void app_neb_msg_tx_insp_thr(struct app_neb_msg_insp_thr* msg)
{
    uint8_t * p_msg = ke_msg_alloc(GTL_NEB_MSG_TX, TASK_GTL, TASK_APP_NEB, NEB_MSG_PAR_LEN_INSP_THR + 1);

    // Pack parameters
    *(p_msg + 0) = NEB_MSG_ID_INSP_THR;
    *(p_msg + 1) = msg->flow_min;
    *(p_msg + 2) = msg->flow_max;
    *(p_msg + 3) = msg->duration_min;
    *(p_msg + 4) = msg->duration_max;

    // Send message
    ke_msg_send(p_msg);
}

void app_neb_msg_tx_confirm(struct app_neb_msg_confirm* msg)
{
    uint8_t * p_msg = ke_msg_alloc(GTL_NEB_MSG_TX, TASK_GTL, TASK_APP_NEB, msg->param_length + 4);

    // Pack parameters
    *(p_msg + 0) = NEB_MSG_ID_CONFIRM;
    *(p_msg + 1) = msg->id;
    *(p_msg + 2) = msg->status;

    if(msg->param_length > 0)
    {
        memcpy(p_msg + 3, &msg->ret_params[0], msg->param_length);
    }

    // Add CRC
    app_neb_msg_tx_insert_CRC(p_msg ,msg->param_length + 4);

    // Send message
    ke_msg_send(p_msg);
}

void app_neb_msg_tx_confirm_basic(uint8_t id, uint8_t status)
{
    uint8_t * p_msg = ke_msg_alloc(GTL_NEB_MSG_TX, TASK_GTL, TASK_APP_NEB, 4);

    // Pack parameters
    *(p_msg + 0) = NEB_MSG_ID_CONFIRM;
    *(p_msg + 1) = id;
    *(p_msg + 2) = status;

    // Add CRC
    app_neb_msg_tx_insert_CRC(p_msg ,4);

    // Send message
    ke_msg_send(p_msg);
}


#endif //BLE_APP_NEB

/// @} APP_NEB
