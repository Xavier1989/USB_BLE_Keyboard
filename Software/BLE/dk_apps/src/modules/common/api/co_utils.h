/**
 ****************************************************************************************
 *
 * @file co_utils.h
 *
 * @brief Common utilities definitions
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */
#ifndef _CO_UTILS_H_
#define _CO_UTILS_H_

/**
 ****************************************************************************************
 * @defgroup CO_UTILS Utilities
 * @ingroup COMMON
 * @brief  Common utilities
 *
 * This module contains the common utilities functions and macros.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdint.h>       // standard definitions
#include "co_bt.h"        // common bt definitions
#include "rwip_config.h"  // SW configuration
#include "compiler.h"     // for inline functions

#if (BT_EMB_PRESENT)
#include "ke_msg.h"       // kernel messages definitions
#endif //BT_EMB_PRESENT

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

#if (BT_EMB_PRESENT)

/// connection handle state
typedef enum
{
    /// Free connection handle
    CO_HDL_FREE,
    /// Connection handle reserved
    CO_HDL_RESERVED
} CO_HDL_STATE;

/// connection handle table
struct co_conhdl_tab_tag
{
    /// Connection handle state
    CO_HDL_STATE state;
    /// BD Address
    struct bd_addr bd_addr;
};
#endif //BT_EMB_PRESENT


/*
 * CONSTANT DECLARATIONS
 ****************************************************************************************
 */
/// Conversion table Sleep Clock Accuracy to PPM
extern const uint16_t co_sca2ppm[];

/// NULL BD address
extern const struct bd_addr co_null_bdaddr;

/// NULL BD address
extern const struct bd_addr co_default_bdaddr;

/*
 * MACROS
 ****************************************************************************************
 */

/// MACRO to build a subversion field from the Minor and Release fields
#define CO_SUBVERSION_BUILD(minor, release)     (((minor) << 8) | (release))

#if (BT_EMB_PRESENT)
#define CO_ACL_CONHDL_BITS        8
#define CO_ACL_CONHDL_MSK        ((0x01 << CO_ACL_CONHDL_BITS) - 1)
#define CO_BT_CONHDL_BIT          0x80
#define CO_BT_CONHDL_FIRST        CO_BT_CONHDL_BIT
#define CO_BT_CONHDL_MAX          CO_BT_CONHDL_BIT + BT_CONNECTION_MAX
#define CO_BT_CONHDL_INVALID_IDX  BT_CONNECTION_MAX
#define PTR_TO_EM_ADDR(ptr)    (uint16_t)((uint32_t)(ptr) & 0x7FFF)
#endif //BT_EMB_PRESENT

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Read an aligned 32 bit word.
 * @param[in] ptr32 The address of the first byte of the 32 bit word.
 * @return The 32 bit value.
 ****************************************************************************************
 */
__INLINE uint32_t co_read32(void const *ptr32)
{
    return *((uint32_t*)ptr32);
}

/**
 ****************************************************************************************
 * @brief Read an aligned 16 bits word.
 * @param[in] ptr16 The address of the first byte of the 16 bits word.
 * @return The 16 bits value.
 ****************************************************************************************
 */
__INLINE uint16_t co_read16(void const *ptr16)
{
    return *((uint16_t*)ptr16);
}

/**
 ****************************************************************************************
 * @brief Write an aligned 32 bits word.
 * @param[in] ptr32 The address of the first byte of the 32 bits word.
 * @param[in] value The value to write.
 ****************************************************************************************
 */
__INLINE void co_write32(void const *ptr32, uint32_t value)
{
    *(uint32_t*)ptr32 = value;
}

/**
 ****************************************************************************************
 * @brief Write an aligned 16 bits word.
 * @param[in] ptr16 The address of the first byte of the 16 bits word.
 * @param[in] value The value to write.
 ****************************************************************************************
 */
__INLINE void co_write16(void const *ptr16, uint32_t value)
{
    *(uint16_t*)ptr16 = value;
}

/**
 ****************************************************************************************
 * @brief Write a 8 bits word.
 * @param[in] ptr8 The address of the first byte of the 8 bits word.
 * @param[in] value The value to write.
 ****************************************************************************************
 */
__INLINE void co_write8(void const *ptr8, uint32_t value)
{
    *(uint8_t*)ptr8 = value;
}

/**
 ****************************************************************************************
 * @brief Read a packed 16 bits word.
 * @param[in] ptr16 The address of the first byte of the 16 bits word.
 * @return The 16 bits value.
 ****************************************************************************************
 */
__INLINE uint16_t co_read16p(void const *ptr16)
{
    uint16_t value = ((uint8_t *)ptr16)[0] | ((uint8_t *)ptr16)[1] << 8;
    return value;
}

/**
 ****************************************************************************************
 * @brief Read a packed 24 bits word.
 * @param[in] ptr24 The address of the first byte of the 24 bits word.
 * @return The 24 bits value.
 ****************************************************************************************
 */
__INLINE uint32_t co_read24p(void const *ptr24)
{
    uint16_t addr_l, addr_h;
    addr_l = co_read16p((uint16_t *)ptr24);
    addr_h = *((uint16_t *)ptr24 + 1) & 0x00FF;
    return ((uint32_t)addr_l | (uint32_t)addr_h << 16);
}

/**
 ****************************************************************************************
 * @brief Write a packed 24 bits word.
 * @param[in] ptr24 The address of the first byte of the 24 bits word.
 * @param[in] value The value to write.
 ****************************************************************************************
 */
__INLINE void co_write24p(void const *ptr24, uint32_t value)
{
    uint8_t *ptr=(uint8_t*)ptr24;

    *ptr++ = (uint8_t)(value&0xff);
    *ptr++ = (uint8_t)((value&0xff00)>>8);
    *ptr++ = (uint8_t)((value&0xff0000)>>16);
}

/**
 ****************************************************************************************
 * @brief Read a packed 32 bits word.
 * @param[in] ptr32 The address of the first byte of the 32 bits word.
 * @return The 32 bits value.
 ****************************************************************************************
 */
__INLINE uint32_t co_read32p(void const *ptr32)
{
    uint16_t addr_l, addr_h;
    addr_l = co_read16p((uint16_t *)ptr32);
    addr_h = co_read16p((uint16_t *)ptr32 + 1);
    return ((uint32_t)addr_l | (uint32_t)addr_h << 16);
}
/**
 ****************************************************************************************
 * @brief Write a packed 32 bits word.
 * @param[in] ptr32 The address of the first byte of the 32 bits word.
 * @param[in] value The value to write.
 ****************************************************************************************
 */
__INLINE void co_write32p(void const *ptr32, uint32_t value)
{
    uint8_t *ptr=(uint8_t*)ptr32;

    *ptr++ = (uint8_t)(value&0xff);
    *ptr++ = (uint8_t)((value&0xff00)>>8);
    *ptr++ = (uint8_t)((value&0xff0000)>>16);
    *ptr = (uint8_t)((value&0xff000000)>>24);
}

/**
 ****************************************************************************************
 * @brief Write a packed 16 bits word.
 * @param[in] ptr16 The address of the first byte of the 16 bits word.
 * @param[in] value The value to write.
 ****************************************************************************************
 */
__INLINE void co_write16p(void const *ptr16, uint16_t value)
{
    uint8_t *ptr=(uint8_t*)ptr16;

    *ptr++ = value&0xff;
    *ptr = (value&0xff00)>>8;
}


#if (BT_EMB_PRESENT)
/****************************************************************************************
 * @brief Get the index of an ACL connection handle
 *
 * @param[in] conhdl ACL connection handle
 *
 * @return Connection index
 *
 ****************************************************************************************
 */
__INLINE uint8_t co_conhdl_to_index(uint16_t conhdl)
{
    // BT connection handle is the index or'red with the BT connection handle bit
    return (conhdl & ~CO_BT_CONHDL_BIT);
}
#endif //BT_EMB_PRESENT


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Compares two Bluetooth device addresses
 *
 * This function checks if the two bd address are equal.
 *
 * @param[in] bd_address1        Pointer on the first bd address to be compared.
 * @param[in] bd_address2        Pointer on the second bd address to be compared.
 *
 * @return result of the comparison (true or false).
 *
 ****************************************************************************************
 */
bool co_bdaddr_compare(struct bd_addr const *bd_address1,
                          struct bd_addr const *bd_address2);


#if (BT_EMB_PRESENT)

/**
 ******************************************************************************
 * @brief Convert an duration in baseband slot to a duration in number of ticks.
 * @param[in]  slot_cnt  Duration in number of baseband slot
 * @return  Duration (in number of ticks).
 ******************************************************************************
 */
uint32_t co_slot_to_duration(uint16_t slot_cnt);

/****************************************************************************************
 * @brief Initialisation of the PLC Pid Table.
 ****************************************************************************************
 */
void co_conhdl_init(void);

/**
 ****************************************************************************************
 * @brief This function is used by LL to release a connection handle.
 * @param[in]  conhdl          Connection Handle
 ****************************************************************************************
 */
void co_conhdl_acl_delete(uint16_t conhdl);

/**
 ****************************************************************************************
 * @brief This function is to get LL PID from BdAddr. For ACL links only.
 * @param[in]  BdAddr          Bd address
 * @return  LL Pid or null if not found
 ****************************************************************************************
 */
ke_task_id_t co_conhdl_bdaddr_to_tid(struct bd_addr *BdAddr);

/**
 ****************************************************************************************
 * @brief This function is used by LL to create a connection handle and associate
 *        a BdAddr and a PID.
 * @param[in]  bd_addr         Bd address
 * @param[in]  task_id         LC task id
 * @return ACL Connection Handle created. If no conHdl can be attributed,
 *         NO_CON_HDL is returned.
 ****************************************************************************************
 */
uint16_t co_conhdl_acl_create(struct bd_addr *bd_addr, ke_task_id_t task_id);

/****************************************************************************************
 * @brief Check if the data base is empty.
 * @return Boolean.
 ****************************************************************************************
 */
bool co_conhdl_database_empty(void);

/**
 ****************************************************************************************
 * @brief This function is used by the BMC to get PID from Sync or ACL conhdl.
 *        If the HCI is present, the addressing cannot be done using the PID
 *        of the destination process. In this case the signals to LL are sent
 *        to ENV and the HCI dispatches them to their destination process
 * @param[in]  conhdl          Connection Handle
 * @return Task ID or null if error.
 ****************************************************************************************
 */
ke_task_id_t co_conhdl_to_tid(uint16_t conhdl);

/**
 ****************************************************************************************
 * @brief Convert link index to connection handle
 * @param[in]  index
 * @return Connection Handle or null if error.
 ****************************************************************************************
 */
uint16_t co_conhdl_index_to_conhdl(uint8_t index);

/**
 ****************************************************************************************
 * @brief This function is used to get the status of connection identified by its
 * connection handle.
 * @param[in]   conhdl
 * @return  Connection handle or NO_CON_HDL, if no further ACL link is found.
 ****************************************************************************************
 */
bool co_conhdl_active(uint16_t conhdl);

/**
 ****************************************************************************************
 * @brief Return conhdl, an element in the co_conhdl_res_tab structure.
 *
 * @param[in] bd_addr
 *
 * @return ACL connection handle or NO_CON_HDL if not found
 *
 ****************************************************************************************
 */
uint16_t co_conhdl_get(struct bd_addr* bd_addr);

/**
 ****************************************************************************************
 * @brief Return link type
 *
 * @param[in] conhdl connection handle
 *
 * @return Link type
 *
 ****************************************************************************************
 */
uint8_t co_conhdl_type_get(uint16_t conhdl);

#endif //BT_EMB_PRESENT

/// @} CO_UTILS

#endif // _CO_UTILS_H_
