/**
 ****************************************************************************************
 *
 * @file prf_utils_128.c
 *
 * @brief Implementation of Profile Utilities for attributes with 128-bit UUIDs
 *
 * Copyright (C) 2013 Dialog Semiconductor GmbH and its Affiliates, unpublished work
 * This computer program includes Confidential, Proprietary Information and is a Trade Secret
 * of Dialog Semiconductor GmbH and its Affiliates. All use, disclosure, and/or
 * reproduction is prohibited unless authorized in writing. All Rights Reserved.
 *
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @addtogroup PRF_UTILS
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"

#if (BLE_CLIENT_PRF)
#include <stdint.h>
#include <stdbool.h>
#include "ke_task.h"
#include "co_error.h"
#include "attm_util.h"
#include "gattc_task.h"
#include "prf_utils_128.h"
#include "ke_mem.h"
#include "gap.h"
#include "gapc.h"
#endif //(BLE_CLIENT_PRF)

#if (BLE_CLIENT_PRF)

void prf_search_chars_128(uint16_t svc_ehdl, uint8_t nb_chars,
                      struct prf_char_inf* chars, const struct prf_char_def_128* chars_req,
                      const struct gattc_disc_char_ind* param,
                      uint8_t* last_found_char)
{
    //Counters
    uint8_t cursor;
	
    //Sanity check
    if (param->uuid_len != ATT_UUID_128_LEN)
    {
        return;
    }

    //Look over requested characteristics
    for (cursor=0; cursor<nb_chars ; cursor++)
    {
        if(attm_uuid_match((uint8_t*)param->uuid, ATT_UUID_128_LEN,
                            (uint8_t*)chars_req[cursor].uuid, ATT_UUID_128_LEN))
        {
            //Save properties and handles
            chars[cursor].char_hdl       = param->attr_hdl;
            chars[cursor].val_hdl        = param->pointer_hdl;
            chars[cursor].prop           = param->prop;

            //Compute number of attribute in Char. using SVC edhl - Limited to 255
            chars[cursor].char_ehdl_off    = (uint8_t)(svc_ehdl - chars[cursor].char_hdl + 1);

            if (cursor != *last_found_char)
            {
                //Update number of attributes for the last found char.
                chars[*last_found_char].char_ehdl_off =
                        (uint8_t)(chars[cursor].char_hdl - chars[*last_found_char].char_hdl);
            }

            //Save code of the Char. which has been found.
            *last_found_char = cursor;
        }

    }
}

void prf_disc_svc_send_128(struct prf_con_info* con_info, uint8_t *uuid_128)
{
    //send GATT discover primary services by UUID request: find by type request
    struct gattc_disc_cmd * svc_req = KE_MSG_ALLOC_DYN(GATTC_DISC_CMD,
            KE_BUILD_ID(TASK_GATTC, con_info->conidx), con_info->prf_id,
            gattc_disc_cmd, ATT_UUID_128_LEN);

    //gatt request type: by UUID
    svc_req->req_type         = GATTC_DISC_BY_UUID_SVC;
    //start handle;
    svc_req->start_hdl        = ATT_1ST_REQ_START_HDL;
    //end handle
    svc_req->end_hdl          = ATT_1ST_REQ_END_HDL;

    // UUID search
    svc_req->uuid_len = ATT_UUID_128_LEN;

    // set 128 bit UUID for searching
    memcpy(&svc_req->uuid[0], uuid_128, (sizeof(uint8_t) * ATT_UUID_128_LEN));

    //send the message to GATT, which will send back the response when it gets it
    ke_msg_send(svc_req);
}

uint8_t prf_check_svc_char_validity_128(uint8_t nb_chars,
                                    const struct prf_char_inf* chars,
                                    const struct prf_char_def_128* chars_req)
{
    uint8_t status = PRF_ERR_OK;
    uint8_t i;

    for(i = 0; ((i < nb_chars) && (status == PRF_ERR_OK)); i++)
    {
        if (chars[i].char_hdl == ATT_INVALID_HANDLE)
        {
            //If Characteristic is not present, check requirements
            if (chars_req[i].req_flag == ATT_MANDATORY)
            {
                status = PRF_ERR_STOP_DISC_CHAR_MISSING;
            }
        }
        else
        {
            //If Characteristic is present, check properties
            if((chars[i].prop & chars_req[i].prop_mand) != chars_req[i].prop_mand)
            {
                status = PRF_ERR_STOP_DISC_WRONG_CHAR_PROP;
            }
        }
    }

    return status;
}

void prf_disc_char_by_uuid_128_send(struct prf_con_info* con_info, uint8_t *uuid_128)
{
    //send GATT discover primary services by UUID request: find by type request
    struct gattc_disc_cmd * svc_req = KE_MSG_ALLOC_DYN(GATTC_DISC_CMD,
            KE_BUILD_ID(TASK_GATTC, con_info->conidx), con_info->prf_id,
            gattc_disc_cmd, ATT_UUID_128_LEN);

    //gatt request type: by UUID
    svc_req->req_type         = GATTC_DISC_BY_UUID_CHAR;
    //start handle;
    svc_req->start_hdl        = ATT_1ST_REQ_START_HDL;
    //end handle
    svc_req->end_hdl          = ATT_1ST_REQ_END_HDL;

    // UUID search
    svc_req->uuid_len = ATT_UUID_128_LEN;

    // set 128 bit UUID for searching
    memcpy(&svc_req->uuid[0], uuid_128, (sizeof(uint8_t) * ATT_UUID_128_LEN));

    //send the message to GATT, which will send back the response when it gets it
    ke_msg_send(svc_req);
}

#endif /*(BLE_SAMPLE128_CLIENT) */

/// @} PRF_UTILS

