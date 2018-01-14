/**
 ****************************************************************************************
 *
 * @file udss_task.h
 *
 * @brief Header file - UDSSTASK.
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */

#ifndef UDSS_TASK_H_
#define UDSS_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup UDSSTASK Task
 * @ingroup UDSS
 * @brief User Data Service Server Task
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#if (BLE_UDS_SERVER)

#include <stdint.h>
#include "ke_task.h"
#include "udss.h"
#include "prf_types.h"

/*
 * DEFINES
 ****************************************************************************************
 */

#define UDSS_IDX_MAX        (1)

/// Possible states of the UDSS task
enum
{
    ///Disabled state
    UDSS_DISABLED,
    ///Idle state
    UDSS_IDLE,
    ///Connected state
    UDSS_CONNECTED,

    ///Number of defined states.
    UDSS_STATE_MAX
};

/// Messages for User Data Service Server
enum
{
    ///Add a UDS instance into the database
    UDSS_CREATE_DB_REQ = KE_FIRST_MSG(TASK_UDSS),
    ///Inform APP of database creation status
    UDSS_CREATE_DB_CFM,


    ///Start the User Data Service Task - at connection
    UDSS_ENABLE_REQ,
    /// Inform the application that the profile service role task has been disabled after a disconnection
    UDSS_DISABLE_IND,

    ///Set the value of an attribute
    UDSS_SET_CHAR_VAL_REQ,	
    
    /// User Control Point Request
    UDSS_UCP_REQ_IND,
    /// User Control Point Response
    UDSS_UCP_RSP_REQ,
    ///Inform that requested action has been performed
    UDSS_REQ_CMP_EVT,
    
    ///Error indication to Host
    UDSS_ERROR_IND,
};

/*
 * API MESSAGES STRUCTURES
 ****************************************************************************************
 */

struct date
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
};

/// Parameters of the @ref UDSS_CREATE_DB_REQ message
struct udss_create_db_req
{
    ///Database configuration
    uint16_t features;
};

/// Parameters of the @ref UDSS_CREATE_DB_CFM message
struct udss_create_db_cfm
{
    ///Status
    uint8_t status;
};

/// Parameters of the @ref UDSS_SET_CHAR_VAL_REQ message - shall be dynamically allocated
struct udss_set_char_val_req
{
    /// Characteristic Code
    uint8_t char_code;
    /// Value length
    uint8_t val_len;
    /// Value
    uint8_t val[1];
};

/// Parameters of the @ref UDSS_ENABLE_REQ message
struct udss_enable_req
{
    ///Connection handle
    uint16_t conhdl;
    /// security level: b0= nothing, b1=unauthenticated, b2=authenticated, b3=authorized; b1 or b2 and b3 can go together
    uint8_t sec_lvl;
    ///Type of connection
    uint8_t con_type;
    ///User height
    uint8_t user_height;
    ///User age
    uint8_t user_age;
    ///User date of birth
    struct date user_date_of_birth;
    ///User DB change increment
    uint8_t user_db_change_incr;
    ///User control point
//    struct udss_user_control_point user_ctrl_point;
};

/// Parameters of the @ref UDSS_DISABLE_IND message
struct udss_disable_ind
{
    ///Connection handle
    uint16_t conhdl;
};

/// Parameters of the @ref UDSS_UCP_REQ_IND message
struct udss_ucp_req_ind
{
    ///Connection handle
    uint16_t conhdl;
    ///User control point (UCP) request structure
    struct uds_ucp_req ucp_req;
};

///Parameters of the @ref UDSS_UCP_RSP_REQ message
struct udss_ucp_rsp_req
{
    ///Connection handle
    uint16_t conhdl;
    ///User control point (ucp) response structure
    struct uds_ucp_rsp ucp_rsp;

//    ///Number of records.
//    uint16_t num_of_record;

//    /// operation code
//    uint8_t op_code;

//    ///Command status
//    uint8_t status;
};

/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */
extern const struct ke_state_handler udss_state_handler[UDSS_STATE_MAX];
extern const struct ke_state_handler udss_default_handler;
extern ke_state_t udss_state[UDSS_IDX_MAX];

#endif //BLE_UDS_SERVER

/// @} UDSSTASK
#endif // UDSS_TASK_H_
