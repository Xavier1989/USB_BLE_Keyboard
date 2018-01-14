/**
 ****************************************************************************************
 *
 * @file app_white_list.h
 *
 * @brief White List mgmt header file.
 *
 * Copyright (C) 2013. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

#ifndef APP_WHITE_LIST_H_
#define APP_WHITE_LIST_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup White List
 *
 * @brief White List management entry point.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdint.h>
#include <stdbool.h>

#include "rwble_config.h"
#include "co_bt.h"


/*
 * Type declarations
 ****************************************************************************************
 */

enum wl_entry_status {
    UNUSED = 0,
    USED_ADDR_PUBLIC_or_PRIVATE,
    USED_ADDR_RAND
};

struct virtual_wl_entry {
    enum wl_entry_status status;
    union {
        struct bd_addr addr;
        int entry;
    } info;
};


/*
 * Public variables
 ****************************************************************************************
 */

extern uint8_t white_list_written;
extern enum adv_filter_policy virtual_wlist_policy;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
bool add_host_in_white_list(uint8_t peer_addr_type, struct bd_addr const *peer_addr, uint8_t bond_info_entry);

bool remove_host_from_white_list(uint8_t peer_addr_type, struct bd_addr const *peer_addr, uint8_t bond_info_entry);

bool lookup_rand_in_virtual_white_list(uint8_t entry);

bool lookup_public_in_virtual_white_list(uint8_t peer_addr_type, struct bd_addr const *peer_addr);

void clear_white_list(void);

/// @} APP

#endif // APP_WHITE_LIST_H_
