/**
 ****************************************************************************************
 *
 * @file udss.h
 *
 * @brief Header file - User Data Service Server.
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */

#ifndef UDSS_H_
#define UDSS_H_

/**
 ****************************************************************************************
 * @addtogroup UDSS User Data Service Server
 * @ingroup UDS
 * @brief User Data Service Server
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#if (BLE_UDS_SERVER)

#include "uds_common.h"
#include "atts.h"
#include "prf_types.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/// Maximal length for Characteristic values - 18
#define UDS_VAL_MAX_LEN                         (0x12)

///User height string length
#define UDS_SYS_ID_LEN                          (0x08)
///IEEE Certif length (min 6 bytes)
#define UDS_IEEE_CERTIF_MIN_LEN                 (0x06)
///PnP ID length
#define UDS_PNP_ID_LEN                          (0x07)

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

///Attributes State Machine
enum
{
    UDS_IDX_SVC,

    UDS_IDX_USER_HEIGHT_CHAR,
    UDS_IDX_USER_HEIGHT_VAL,

    UDS_IDX_USER_AGE_CHAR,
    UDS_IDX_USER_AGE_VAL,

    UDS_IDX_USER_DATE_OF_BIRTH_CHAR,
    UDS_IDX_USER_DATE_OF_BIRTH_VAL,

    UDS_IDX_USER_DB_CHANGE_INCR_CHAR,
    UDS_IDX_USER_DB_CHANGE_INCR_VAL,
    UDS_IDX_USER_DB_CHANGE_INCR_NTF_CFG,

    UDS_IDX_USER_INDEX_CHAR,
    UDS_IDX_USER_INDEX_VAL,

    UDS_IDX_USER_CTRL_POINT_CHAR,
    UDS_IDX_USER_CTRL_POINT_VAL,

    UDS_IDX_NB,
};

///Attribute Table Indexes
enum
{
    UDS_USER_HEIGHT_CHAR,
    UDS_USER_AGE_CHAR,
    UDS_USER_DATE_OF_BIRTH_CHAR,
    UDS_USER_DB_CHANGE_INCR_CHAR,
    UDS_USER_INDEX_CHAR,
    UDS_USER_CTRL_POINT_CHAR,

    UDS_CHAR_MAX,
};

///Characteristics Code for Write Indications
enum
{
    UDSS_ERR_CHAR,
    UDSS_USER_HEIGHT_CHAR,
    UDSS_USER_AGE_CHAR,
    UDSS_USER_DATE_OF_BIRTH_CHAR,
    UDSS_USER_DB_CHANGE_INCR_CHAR,
    UDSS_USER_CTRL_POINT_CHAR,
};

///User Data Service Server Environment Variable
struct udss_env_tag
{
    /// Connection Info
    struct prf_con_info con_info;

    /// Service Start HandleVAL
    uint16_t shdl;

    /////Application that requests to send UCP response indication.
    //ke_task_id_t ucp_ind_src;

    /// Attribute Table
    uint8_t att_tbl[UDS_CHAR_MAX];
};

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

extern const struct attm_desc udss_att_db[UDS_IDX_NB];

extern const att_svc_desc_t udss_dis_svc;

extern const struct att_char_desc udss_user_height_char;
extern const struct att_char_desc udss_user_age_char;
extern const struct att_char_desc udss_user_date_of_birth_char;
extern const struct att_char_desc udss_user_db_change_incr_char;
extern const struct att_char_desc udss_user_index_char;
extern const struct att_char_desc udss_user_ctrl_point_char;

extern struct udss_env_tag udss_env;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialization of the UDSS module.
 * This function performs all the initializations of the UDSS module.
 ****************************************************************************************
 */
void udss_init(void);

/**
 ****************************************************************************************
 * @brief Check if an attribute shall be added or not in the database
 * @param features UDS features
 ****************************************************************************************
 */
uint32_t udss_compute_cfg_flag(uint16_t features);

/**
 ****************************************************************************************
 * @brief Check if the provided value length matches characteristic requirements
 * @param char_code Characteristic Code
 * @param val_len   Length of the Characteristic value
 ****************************************************************************************
 */
uint8_t udss_check_val_len(uint8_t char_code, uint8_t val_len);

/**
 ****************************************************************************************
 * @brief Unpack User Control request
 *
 * @param[in] packed_val User Control Point value packed
 * @param[out] racp_req User Control Request value
 *
 * @return status of unpacking
 ****************************************************************************************
 */
uint8_t udss_unpack_ucp_req(uint8_t *packed_val, uint16_t length,
                             struct uds_ucp_req* ucp_req);


/**
 ****************************************************************************************
 * @brief Pack User Control response
 *
 * @param[out] packed_val User Control Point value packed
 * @param[in] ucp_rsp User Control Response value
 *
 * @return size of packed data
 ****************************************************************************************
 */
uint8_t udss_pack_ucp_rsp(uint8_t *packed_val,
                           struct uds_ucp_rsp* ucp_rsp);

/**
 ****************************************************************************************
 * @brief Send User Control Response Indication
 *
 * @param[in] ucp_rsp user Control Response value
 * @param[in] ucp_ind_src Application that requests to send UCP response indication.
 *
 * @return PRF_ERR_IND_DISABLED if indication disabled by peer device, PRF_ERR_OK else.
 ****************************************************************************************
 */
uint8_t udss_send_ucp_rsp(struct uds_ucp_rsp * ucp_rsp,
                           ke_task_id_t ucp_ind_src);

/**
 ****************************************************************************************
 * @brief Disable actions grouped in getting back to IDLE and sending configuration to requester task
 ****************************************************************************************
 */
void udss_disable(uint16_t conhdl); 

#endif //BLE_UDS_SERVER

/// @} UDSS

#endif // UDSS_H_
