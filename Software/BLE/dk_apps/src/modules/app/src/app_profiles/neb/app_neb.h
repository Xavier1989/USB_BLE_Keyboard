/**
 ****************************************************************************************
 *
 * @file app_neb.h
 *
 * @brief Nebulizer Application entry point
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */

#ifndef APP_NEB_H_
#define APP_NEB_H_

/**
 ****************************************************************************************
 * @addtogroup APP_NEB
 * @ingroup APP
 *
 * @brief Nebulizer Application entry point.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"

#if (BLE_APP_NEB)

#include <stdint.h>          // standard integer definition
#include <co_bt.h>

/*
 * DEFINES
 ****************************************************************************************
 */

/// Nebulizer messages identifiers
#define NEB_MSG_ID_CONFIRM         0x00
#define NEB_MSG_ID_START           0x01
#define NEB_MSG_ID_STOP            0x02
#define NEB_MSG_ID_DURATION        0x03
#define NEB_MSG_ID_RESP_CYCLE_DUR  0x04
#define NEB_MSG_ID_RESP_FLOW       0x05
#define NEB_MSG_ID_ACT_CNX_MODE    0x06
#define NEB_MSG_ID_INSP_THR        0x07

/// Number of messages supported in reception
#define NEB_NB_RX_MSG               8

/// Maximum return parameters size
#define RET_PARAMS_SIZE_MAX         30

/// Nebulizer messages identifiers
#define NEB_MSG_PAR_LEN_CONFIRM          2 + RET_PARAMS_SIZE_MAX
#define NEB_MSG_PAR_LEN_START            1
#define NEB_MSG_PAR_LEN_STOP             1
#define NEB_MSG_PAR_LEN_DURATION         2
#define NEB_MSG_PAR_LEN_RESP_CYCLE_DUR   1
#define NEB_MSG_PAR_LEN_RESP_FLOW        2
#define NEB_MSG_PAR_LEN_ACT_CNX_MODE     0
#define NEB_MSG_PAR_LEN_INSP_THR         6

/// Nebulizer error codes
#define NEB_ERR_SUCCESS             0x00
#define NEB_ERR_UNSPECIFIED         0x01
#define NEB_ERR_UNKNOWN_MSG         0x02
#define NEB_ERR_INVALID_PARAMS      0x03
#define NEB_ERR_INCORRECT_LENGTH    0x04
#define NEB_ERR_INCORRECT_CRC       0x05


/// Nebulizer respiration flow sample pack number
#define NEB_RESP_FLOW_PCK_NB        4

/*
 * ENUMERATIONS DEFINITIONS
 ****************************************************************************************
 */

/// Nebulizer stop reason
enum NEB_MSG_STOP_REASON
{
    NEB_MSG_STOP_REASON_USER       = 0x00,
    NEB_MSG_STOP_REASON_LOW_BATT   = 0x01,
    NEB_MSG_STOP_REASON_AUTO       = 0x02,
};

/// Nebulizer start reason
enum NEB_MSG_START_REASON
{
    NEB_MSG_START_REASON_START     = 0x00,
    NEB_MSG_START_REASON_RESTART   = 0x01,
};

/*
 * MESSAGES DEFINITIONS
 ****************************************************************************************
 */

/// Nebulizer start message
struct app_neb_msg_start
{
    uint8_t reason;
};

/// Nebulizer stop message
struct app_neb_msg_stop
{
    uint8_t reason;
};

/// Nebulizer duration message
struct app_neb_msg_duration
{
    uint32_t duration;
};

/// Nebulizer respiratory flow message
struct app_neb_msg_resp_flow
{
    uint16_t flow;
};

/// Nebulizer inspiratory rate message
struct app_neb_msg_insp_rate
{
    uint8_t rate;
};

/// Nebulizer Respiratory Cycle Duration message
struct app_neb_msg_resp_cycle_dur
{
    uint8_t duration;
};

/// Nebulizer inspiratory thresholds message
struct app_neb_msg_insp_thr
{
    uint16_t flow_min;
    uint16_t flow_max;
    uint8_t duration_min;
    uint8_t duration_max;
};

/// Nebulizer confirmation message
struct app_neb_msg_confirm
{
    /// Identifier of confirmed message
    uint8_t id;
    /// Status of confirmed message processing
    uint8_t status;
    /// Length of returned parameters
    uint8_t param_length;
    /// Pointer to returned parameters
    uint8_t ret_params[RET_PARAMS_SIZE_MAX];
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// Nebulizer application environment structure
struct app_neb_env_tag
{
    /// Connection handle
    uint16_t conhdl;
    /// Current Nebulizer session ID
    uint16_t curr_neb_id;
    /// Next Record ID
    uint16_t next_rec_id;

    /// Buffer for storing respiration flow samples
    uint16_t resp_flow_sample_buf[NEB_RESP_FLOW_PCK_NB];

    /// Number of respiration flow samples in buffer
    uint8_t resp_flow_sample_nb;
};

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/// Nebulizer Application environment
extern struct app_neb_env_tag app_neb_env;



/*
 ****************************************************************************************
 * Nebulizer Application Functions
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize Nebulizer Application
 ****************************************************************************************
 */
void app_neb_init(void);

/**
 ****************************************************************************************
 * @brief Add a Nebulizer instance in the DB
 ****************************************************************************************
 */
void app_neb_create_db_send(void);

/**
 ****************************************************************************************
 * @brief Enable the Nebulizer profile
 ****************************************************************************************
 */
void app_neb_enable_prf(uint16_t conhdl);


/*
 ****************************************************************************************
 * RX Messages API
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Handle start message
 * @param[in]     msg           Pointer to message parameters
 ****************************************************************************************
 */
void app_neb_msg_rx_start(struct app_neb_msg_start* msg);

/**
 ****************************************************************************************
 * @brief Handle stop message
 * @param[in]     msg           Pointer to message parameters
 ****************************************************************************************
 */
void app_neb_msg_rx_stop(struct app_neb_msg_stop* msg);

/**
 ****************************************************************************************
 * @brief Handle duration message
 * @param[in]     msg           Pointer to message parameters
 ****************************************************************************************
 */
void app_neb_msg_rx_duration(struct app_neb_msg_duration* msg);

/**
 ****************************************************************************************
 * @brief Handle respiratory flow message
 * @param[in]     msg           Pointer to message parameters
 ****************************************************************************************
 */
void app_neb_msg_rx_resp_flow(struct app_neb_msg_resp_flow* msg);

/**
 ****************************************************************************************
 * @brief Handle Respiratory Cycle Duration message
 * @param[in]     msg           Pointer to message parameters
 ****************************************************************************************
 */
void app_neb_msg_rx_resp_cycle_dur(struct app_neb_msg_resp_cycle_dur* msg);

/**
 ****************************************************************************************
 * @brief Handle active connection mode message
 ****************************************************************************************
 */
void app_neb_msg_rx_act_cnx_mode(void);

/**
 ****************************************************************************************
 * @brief Handle confirmation message
 * @param[in]     msg           Pointer to message parameters
 ****************************************************************************************
 */
void app_neb_msg_rx_confirm(struct app_neb_msg_confirm* msg);



/*
 ****************************************************************************************
 * TX Messages API
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Send inspiratory thresholds message
 * @param[in]     msg           Pointer to message parameters
 ****************************************************************************************
 */
void app_neb_msg_tx_insp_thr(struct app_neb_msg_insp_thr* msg);

/**
 ****************************************************************************************
 * @brief Send confirmation message
 * @param[in]     msg           Pointer to message parameters
 ****************************************************************************************
 */
void app_neb_msg_tx_confirm(struct app_neb_msg_confirm* msg);

/**
 ****************************************************************************************
 * @brief Send a basic confirmation message (no return params)
 * @param[in]     id            Identifier of confirmed message
 * @param[in]     status        Status of confirmed message processing
 ****************************************************************************************
 */
void app_neb_msg_tx_confirm_basic(uint8_t id, uint8_t status);


#endif //BLE_APP_NEB

/// @} APP_NEB

#endif // APP_NEB_H_

