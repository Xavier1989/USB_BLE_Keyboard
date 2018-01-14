/**
 ****************************************************************************************
 *
 * @file uds_common.h
 *
 * @brief Header File - User Data service common types.
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


#ifndef _UDS_COMMON_H_
#define _UDS_COMMON_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#if (BLE_UDS_CLIENT || BLE_UDS_SERVER)

#include "prf_types.h"
#include <stdint.h>

/*
 * DEFINES
 ****************************************************************************************
 */

/// User control point packet max length
#define UDS_USER_CTRL_PT_MAX_LEN     (20)

/// User consent code minimum and maximum values
#define UDS_CONSENT_CODE_MIN_VAL     (0x0000)
#define UDS_CONSENT_CODE_MAX_VAL     (0x270F)

/// User Data Service Error Code
enum uds_error_code
{
    /// UCP Procedure already in progress
    UDS_ERR_ACCESS_NOT_PERMITTED = (0x80),

};

/// User control point op codes
enum uds_ucp_op_code
{
    /// Register new user
    UDS_REQ_REG_NEW_USER  = 0x01,

    /// Register new user
    UDS_REQ_CONSENT       = 0x02,
    
    /// Register new user
    UDS_REQ_DEL_USER_DATA = 0x03,
    
    /// Response code
    UDS_REQ_RSP_CODE      = 0x20,
};

/// User control point response value
enum uds_ucp_rsp_val
{
    /// Success
    UDS_RSP_SUCCESS           = 0x01,

    /// Operation code not supported
    UDS_RSP_OP_CODE_NOT_SUP   = 0x02,
    
    /// Invalid parameter
    UDS_RSP_INVALID_PARAMETER = 0x03,
    
    /// Operation failed
    UDS_RSP_OPERATION_FAILED  = 0x04,
    
    /// User not authorized
    UDS_RSP_USER_NOT_AUTH     = 0x05,
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// User control point request
struct uds_ucp_req
{
    /// operation code
    uint8_t op_code;

    /// Parameter
    union
    {
        /// register new user parameter
        struct
        {
            /// consent code
            uint16_t consent_code;
        } reg_new_user;
        
        /// consent parameter
        struct
        {
            /// user index
            uint8_t user_idx;
            /// consent code
            uint16_t consent_code;
        } consent;
    } parameter;
};

/// User control point response
struct uds_ucp_rsp
{
    /// operation code
    uint8_t op_code;
    
    /// request operation code
    uint8_t req_op_code;
    
    /// response value
    uint8_t rsp_val;

    /// response parameter
    union
    {
        /// structure for register new user response parameter
        struct
        {
            /// User index
            uint8_t user_idx;
        } reg_new_user;
    } parameter;
};



#endif /* #if (BLE_UDS_CLIENT || BLE_UDS_SERVER) */

/// @} uds_common

#endif /* _UDS_COMMON_H_ */
