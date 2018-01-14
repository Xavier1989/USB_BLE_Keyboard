/**
 ****************************************************************************************
 *
 * @file gapm_util.h
 *
 * @brief Generic Access Profile Manager Tool Box Header.
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 * $Rev$
 *
 ****************************************************************************************
 */


#ifndef _GAPM_UTIL_H_
#define _GAPM_UTIL_H_

/**
 ****************************************************************************************
 * @addtogroup GAPM_UTIL Generic Access Profile Manager Tool Box
 * @ingroup GAPM
 * @brief Generic Access Profile Manager Tool Box
 *
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"
#include "ke_task.h"
#include "gapm.h"
#include "gapm_task.h"


/*
 * DEFINES
 ****************************************************************************************
 */
/// GAP Manager operation state - air commands
enum gapm_op_state
{
    /* state in non connected mode */
    /// Initial state of the operation
    GAPM_OP_INIT,
    /// Set operation parameters
    GAPM_OP_SET_PARAMS,
    /// Clear White List operation
    GAPM_OP_CLEAR_WL,
    /// Set White List operation
    GAPM_OP_SET_WL,
    /// Set operation advertise data
    GAPM_OP_SET_ADV_DATA,
    /// Set operation scan response data
    GAPM_OP_SET_SCAN_RSP_DATA,
    /// Generation of address
    GAPM_OP_ADDR_GEN,
    /// Set private address
    GAPM_OP_ADDR_SET,
    /// Start operation (advertising, scanning or connecting)
    GAPM_OP_START,
    /// Operation is in wait (advertising, scanning or connecting) state
    GAPM_OP_WAIT,
    /// Stop operation (advertising, scanning or connecting)
    GAPM_OP_STOP,
    /// Operation is finished
    GAPM_OP_FINISH,

    /// Error state
    GAPM_OP_ERROR,


    /* state in connected mode */
    /// Connection established
    GAPM_OP_CONNECT,
    /// Perform peer device name request
    GAPM_OP_NAME_REQ,
    /// Perform peer device disconnection
    GAPM_OP_DISCONNECT,


    /* TODO */
    /// Operation is in canceled state and shall be terminated.
    GAPM_OP_CANCEL,
    /// Operation is in timeout state and shall be terminated.
    GAPM_OP_TIMEOUT,
    /// Operation is terminated due to a timeout.
    GAPM_OP_TERM_TIMEOUT,
    /// Renew address generation
    GAPM_OP_ADDR_RENEW,
    /// initiate a connection request
    GAPM_OP_CONNECT_REQ,


    /// Mask used to retrieve operation state in state parameter
    GAPM_OP_MASK                 = 0x00FF,

    /* Specific bit fields into operation state machine */
    /// TODO [FBE] enhance comments
    /// Mask used to know in state parameter if message is in kernel queue
    GAPM_OP_QUEUED_MASK          = 0x0100,

    /// Operation is in canceled state and shall be terminated.
    GAPM_OP_CANCELED_MASK        = 0x0200,

    /// Operation is in timeout state and shall be terminated.
    GAPM_OP_TIMEOUT_MASK         = 0x0400,

    /// Renew address generation
    GAPM_OP_ADDR_RENEW_MASK      = 0x0800,

    /// Mask used to know in state parameter if a connection has been initiated
    GAPM_OP_CONNECTING_MASK      = 0x1000,

    /// Mask used to know if device is currently in scanning state
    GAPM_OP_SCANNING_MASK        = 0x2000,

//    /// Stopping current operation.
//    GAPM_OP_STOPPING_MASK        = 0x2000,
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/*
 * MACROS
 ****************************************************************************************
 */
/// check if current role is supported by configuration
#define GAPM_IS_ROLE_SUPPORTED(role_type)\
    ((gapm_env.role & (role_type)) == (role_type))

/// Get current operation state
#define GAPM_GET_OP_STATE(operation) \
    ((operation).state & (GAPM_OP_MASK))

/// Set current operation state
#define GAPM_SET_OP_STATE(operation, new_state) \
    (operation).state = (((operation).state & ~(GAPM_OP_MASK)) | (new_state))


/// TODO [FBE] find better naming
/// Get if operation is in kernel queue
#define GAPM_IS_OP_FIELD_SET(operation, field) \
        ((((operation).state) & (GAPM_OP_##field##_MASK)) != 0)

/// Set operation in kernel queue
#define GAPM_SET_OP_FIELD(operation, field) \
        ((operation).state) |= (GAPM_OP_##field##_MASK)

/// Clear operation in kernel queue
#define GAPM_CLEAR_OP_FIELD(operation, field) \
        ((operation).state)  &= ~(GAPM_OP_##field##_MASK)


/// Bit setting
#define GAPM_SETBITS(flag, mask)                    ((flag)|=(mask))
/// Bit resetting
#define GAPM_RESETBITS(flag, mask)                  ((flag)&=(~(mask)))
/// Bit checking
#define GAPM_ISBITSET(flag, mask)                   (((flag)&(mask)) == mask)


/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Interface to get the device role.
 *
 * @return uint8_t device role
 *
 ****************************************************************************************
 */
uint8_t  gapm_get_role(void);


#if (BLE_PERIPHERAL)
/**
 ****************************************************************************************
 * @brief Retrieve privacy flag from attribute database.
 *
 * @return The privacy flag
 ****************************************************************************************
 */
uint8_t gapm_get_privacy_flag(void);

/**
 ****************************************************************************************
 * @brief Retrieve reconnection address property from attribute database.
 *
 * @return The reconnection address
 ****************************************************************************************
 */
struct bd_addr gapm_get_recon_addr(void);

/**
 ****************************************************************************************
 * @brief Modify privacy flag property into attribute database.
 *
 * @param priv_flag The privacy flag
 ****************************************************************************************
 */
void gapm_set_privacy_flag(uint8_t priv_flag);

/**
 ****************************************************************************************
 * @brief Modify reconnection address property into attribute database.
 *
 * @param recon_addr The reconnection address
 *
 * @return true if operation succeed, false else
 ****************************************************************************************
 */
bool gapm_set_recon_addr(struct bd_addr* recon_addr);
#endif /* #if (BLE_PERIPHERAL) */


#if (BLE_BROADCASTER || BLE_PERIPHERAL)
/**
 ****************************************************************************************
 * @brief Enable or disable advertising mode.
 *
 * @param[in] en_flag       enabling flag for advertising mode
 *
 ****************************************************************************************
 */
void gapm_set_adv_mode(uint8_t en_flag);
/**
 ****************************************************************************************
 * Execute Advertising operation according to current operation state.
 *
 * @param[in|out] adv Advertising operation data
 ****************************************************************************************
 */
void gapm_execute_adv_op(struct gapm_start_advertise_cmd *adv);


/**
 ****************************************************************************************
 * Perform a sanity check on advertising operation
 *
 * @param[in|out] adv Advertising operation data
 *
 * @return sanity check status: GAP_ERR_NO_ERROR if succeed, error code else.
 ****************************************************************************************
 */
uint8_t gapm_adv_op_sanity(struct gapm_start_advertise_cmd *adv);
#endif // (BLE_BROADCASTER || BLE_PERIPHERAL)


#if (BLE_OBSERVER || BLE_CENTRAL)
/**
 ****************************************************************************************
 * @brief Enable or disable scanning mode.
 *
 * @param[in] en_flag           enabling flag for scanning mode
 * @param[in] filter_duplic_en  enabling duplicate filtering mode
 *
 ****************************************************************************************
 */
void gapm_set_scan_mode(uint8_t en_flag, uint8_t filter_duplic_en);


/**
 ****************************************************************************************
 * @brief retrieve AD Type Flag information in advertising data.
 *
 * @param[in] data   Advertising data
 * @param[in] length Advertising data length
 *
 * @return Advertising AD Type flag value if found, 0x00 else.
 ****************************************************************************************
 */
uint8_t gapm_get_ad_type_flag(uint8_t *data, uint8_t length);


/**
 ****************************************************************************************
 * @brief Add device to unfiltered device
 *
 * @param[in] addr       Device address
 * @param[in] addr_type  Device address type
 ****************************************************************************************
 */
void gapm_add_to_filter(struct bd_addr * addr, uint8_t addr_type);

/**
 ****************************************************************************************
 * @brief Check if device is filtered or not when a scan response data is received
 *
 * If device is not filtered (present in filter list), in that case function returns false
 * and device is removed from filtered devices.
 *
 * @param[in] addr       Device address
 * @param[in] addr_type  Device address type
 *
 * @return true if device filtered, false else.
 ****************************************************************************************
 */
bool gapm_is_filtered(struct bd_addr * addr, uint8_t addr_type);

/**
 ****************************************************************************************
 * @brief Execute Scan operation according to current operation state.
 *
 * @param[in] scan Scan operation data
 ****************************************************************************************
 */
void gapm_execute_scan_op(struct gapm_start_scan_cmd *scan);

/**
 ****************************************************************************************
 * Perform a sanity check on Scanning operation
 *
 * @param[in|out] scan Scanning operation data
 *
 * @return sanity check status: GAP_ERR_NO_ERROR if succeed, error code else.
 ****************************************************************************************
 */
uint8_t gapm_scan_op_sanity(struct gapm_start_scan_cmd *scan);

#endif // (BLE_OBSERVER || BLE_CENTRAL)


#if (BLE_CENTRAL)
/**
 ****************************************************************************************
 * @brief Execute Connection operation according to current operation state.
 *
 * @param[in] connect Connection operation data
 ****************************************************************************************
 */
void gapm_execute_connect_op(struct gapm_start_connection_cmd *connect);


/**
 ****************************************************************************************
 * Perform a sanity check on connection operation
 *
 * @param[in|out] connect Connection operation data
 *
 * @return sanity check status: GAP_ERR_NO_ERROR if succeed, error code else.
 ****************************************************************************************
 */
uint8_t gapm_connect_op_sanity(struct gapm_start_connection_cmd *connect);

#endif // (BLE_CENTRAL)

/**
 ****************************************************************************************
 * @brief Requests current operation state to change.
 *
 *  1. Check if new state can be applied to ongoing operation.
 *  2. Verify that message is not in kernel message queue.
 *  3. If not push it into @ref TASK_GAPM message queue
 *
 * @param[in] op    Ongoing Air operation
 * @param[in] state New state to set
 *
 ****************************************************************************************
 */
void gapm_update_air_op_state(struct gapm_air_operation *op, uint8_t state);


/**
 ****************************************************************************************
 * @brief Retrieve device identity key.
 *
 * @return Device Identity Key
 ****************************************************************************************
 */
struct gap_sec_key* gapm_get_irk(void);


/**
 ****************************************************************************************
 * @brief Retrieve local public address.
 *
 * @return Return local public address
 ****************************************************************************************
 */
struct bd_addr* gapm_get_bdaddr(void);


/// @} GAPM_UTIL

#endif /* _GAPM_UTIL_H_ */
