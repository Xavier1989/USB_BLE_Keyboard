/**
****************************************************************************************
*
* @file app_multi_bond.h
*
* @brief Special (multi) bonding procedure header file.
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

#ifndef APP_MULTI_BOND_H_
#define APP_MULTI_BOND_H_

/*
 * USAGE
 *
 * To use this module the following must be properly defined in your project and include the header file in app_multi_bond.c:
 *     ALT_PAIR_DISCONN_TIME : Time to block previous host during a "host-switch" (in 10th of msec) 
 *
 * and the following must be defined as (1) if used or (0) if not used:
 *     HAS_MULTI_BOND : set to (1) if multiple bonding is to be used.
 *     HAS_EEPROM : if there's no EEPROM then using this module is useless.
 *     HAS_MITM : set to (1) if MITM is used or (0) if no MITM is supported.
 *
 * Note that the following configuration makes sense:
 *     HAS_MULTI_BOND (0) and HAS_EEPROM (1)
 * meaning that only one bond will exist in EEPROM without the possibility of switching.
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>
#include <stdbool.h>
#include "rwip_config.h"
#include "co_bt.h"
#include "smpm.h"
#include "gap.h"
#include "app.h" 
#include "app_sec.h"
#include "app_kbd_key_matrix.h"


/*
 * DEFINES
 ****************************************************************************************
 */

// Load IRKs in irk_array. Depends on the MBOND_LOAD_INFO_AT_INIT setting.
// IRKs won't be loaded if MBOND_LOAD_INFO_AT_INIT is 1 since the info is already available.
// They will be loaded if MBOND_LOAD_INFO_AT_INIT is 0 though.
// Do not modify!
#if (MBOND_LOAD_INFO_AT_INIT)
#define MBOND_LOAD_IRKS_AT_INIT     (0)
#else
#define MBOND_LOAD_IRKS_AT_INIT     (1)
#endif

// EEPROM_BASE_ADDR(3:0) are left unused
#define EEPROM_MAGIC_ADDR           (EEPROM_BASE_ADDR + 0x04)
#define EEPROM_STATUS_ADDR          (EEPROM_BASE_ADDR + 0x08)
#define EEPROM_USAGE_ADDR           (EEPROM_BASE_ADDR + 0x09)
#define EEPROM_BOND_DATA_ADDR       (EEPROM_BASE_ADDR + 0x10)   // Up to 7 bonds (0x10 - 0x09 = 7)

#define EEPROM_GUARD_NUMBER         (0xA55A5AA5)
#define EEPROM_MAGIC_NUMBER         (0xDEADBEEF)

/*
 * EEPROM Structure
 ****************************************************************************************
 *
 *                            7         6         5         4         3         2         1         0
 *                       |---------|---------|---------|---------|---------|---------|---------|---------|
 *                       |         |         |         |         |         |         |         |         |
 * EEPROM_STATUS_ADDR    |                                    STATUS                                     |
 *                       |         |         |         |         |         |         |         |         |
 *                       |---------|---------|---------|---------|---------|---------|---------|---------|
 *                                                              ...
 *                       |---------|---------|---------|---------|---------|---------|---------|---------|
 *                       |         |         |         |         |         |         |         |         |
 * EEPROM_USAGE_ADDR     |                                   Usage #0                                    |
 *                       |         |         |         |         |         |         |         |         |
 *                       |---------|---------|---------|---------|---------|---------|---------|---------|
 *                       |         |         |         |         |         |         |         |         |
 *                       |                                   Usage #1                                    |
 *                       |         |         |         |         |         |         |         |         |
 *                       |---------|---------|---------|---------|---------|---------|---------|---------|
 *                       |         |         |         |         |         |         |         |         |
 *                       |                                      ...                                      |
 *                       |         |         |         |         |         |         |         |         |
 *                       |---------|---------|---------|---------|---------|---------|---------|---------|
 *                       |         |         |         |         |         |         |         |         |
 *                       |                               Usage #N (< = 15)                               |
 *                       |         |         |         |         |         |         |         |         |
 *                       |---------|---------|---------|---------|---------|---------|---------|---------|
 *                                                              ...
 *                       |---------|---------|---------|---------|---------|---------|---------|---------|
 *                       |         |         |         |         |         |         |         |         |
 * EEPROM_BOND_DATA_ADDR |                                Bonding info #0                                |
 *                       |         |         |         |         |         |         |         |         |
 *                       |---------|---------|---------|---------|---------|---------|---------|---------|
 *                       |         |         |         |         |         |         |         |         |
 *                       |                                      ...                                      |
 *                       |         |         |         |         |         |         |         |         |
 *                       |---------|---------|---------|---------|---------|---------|---------|---------|
 *                       |         |         |         |         |         |         |         |         |
 *                (+60)  |                                Bonding info #1                                |
 *                       |         |         |         |         |         |         |         |         |
 *                       |---------|---------|---------|---------|---------|---------|---------|---------|
 *                       |         |         |         |         |         |         |         |         |
 *                       |                                      ...                                      |
 *                       |         |         |         |         |         |         |         |         |
 *                       |---------|---------|---------|---------|---------|---------|---------|---------|
 *                       |         |         |         |         |         |         |         |         |
 *               (+120)  |                                Bonding info #2                                |
 *                       |         |         |         |         |         |         |         |         |
 *                       |---------|---------|---------|---------|---------|---------|---------|---------|
 *                                                              ...
 */

/*
 * BONDING INFO
 ****************************************************************************************
 *
 * Byte 0
 * ------
 *      7         6         5         4         3         2         1         0
 * |---------|---------|---------|---------|---------|---------|---------|---------|
 * |         |         |         |         |         |         |         |         |
 * |        SCF        |   CPS   |                  NOS                  |   MODE  |
 * |         |         |         |         |         |         |         |         |
 * |---------|---------|---------|---------|---------|---------|---------|---------|
 *
 * MODE                        : 0 = Boot mode, 1 = Report mode
 * NOS (Notifications' status) : Bit value    : 0 = Notifications are disabled, 1 = Notifications are enabled
 *                               Bit position : 1 = Battery, 2 = Boot Report (Input), 3 = Report #0 (Input), 4 = Report #2 (Input)
 * CPS (Control Point Status)  : Value of the control point characteristic
 * SCF (Service Changed flag)  : 0 = Not notified, Not indicated
 *                               1 = error
 *                               2 = Indicated
 *                               3 = error
 *                               The rangle of the affected handles is in Byte 1. If it's not zero then the characteristic must be updated.
 *
 *
 * Byte 1
 * ------
 * Set after any SW update that results in a modification of the DB (at all entries).
 * Cleared (in a specific entry) after informing the corresponding Host.
 *
 *      7         6         5         4         3         2         1         0
 * |---------|---------|---------|---------|---------|---------|---------|---------|
 * |         |         |         |         |         |         |         |         |
 * |                      Start of affected handle attr range                      |
 * |         |         |         |         |         |         |         |         |
 * |---------|---------|---------|---------|---------|---------|---------|---------|
 *
 *
 *
 * Byte 2
 * ------
 * Set after any SW update that results in a modification of the DB (at all entries).
 * Cleared (in a specific entry) after informing the corresponding Host.
 *
 *      7         6         5         4         3         2         1         0
 * |---------|---------|---------|---------|---------|---------|---------|---------|
 * |         |         |         |         |         |         |         |         |
 * |                       End of affected handle attr range                       |
 * |         |         |         |         |         |         |         |         |
 * |---------|---------|---------|---------|---------|---------|---------|---------|
 *
 *
 * Bytes 3 - 6 : RESERVED
 * ------
 *
 *
 * Byte 7
 * ------
 *      7         6         5         4         3         2         1         0
 * |---------|---------|---------|---------|---------|---------|---------|---------|
 * |         |         |         |         |         |         |         |         |
 * | IRK flg |                          RESERVED                                   |
 * |         |         |         |         |         |         |         |         |
 * |---------|---------|---------|---------|---------|---------|---------|---------|
 *
 *
 * Bytes 8 - 23 : IRK
 * ------------
 *
 *
 * Bytes 24 - 59 : app_sec_env_tag
 * ------
 */

#define PROTOCOL_MODE_POS       (0)
#define PROTOCOL_MODE_MASK      (1 << PROTOCOL_MODE_POS)
#define BASS_CCC_POS            (1)
#define BASS_CCC_MASK           (1 << BASS_CCC_POS)
#define HID_BOOT_CCC_POS        (2)
#define HID_BOOT_CCC_MASK       (1 << HID_BOOT_CCC_POS)
#define HID_REPORT_0_CCC_POS    (3)
#define HID_REPORT_0_CCC_MASK   (1 << HID_REPORT_0_CCC_POS)
#define HID_REPORT_2_CCC_POS    (4)
#define HID_REPORT_2_CCC_MASK   (1 << HID_REPORT_2_CCC_POS)
#define HID_CTNL_PT_CCC_POS     (5)
#define HID_CTNL_PT_CCC_MASK    (1 << HID_CTNL_PT_CCC_POS)
#define SVC_CHNG_CCC_POS        (6)
#define SVC_CHNG_CCC_MASK       (3 << SVC_CHNG_CCC_POS)

//#define BOND_INFO_DEFAULT_VAL   (0x79)
#define BOND_INFO_DEFAULT_VAL   (0xA1)
 
#define IRK_FLAG                (1 << 7)
struct bonding_info_
{
    int info;
    int ext_info;
    struct gap_sec_key irk;
    struct app_sec_env_tag env;
};

struct usage_array_
{
    uint8_t pos[MAX_BOND_PEER];
};

struct irk_array_
{
    struct gap_sec_key irk[MAX_BOND_PEER];
};

/*
 * VARIABLES
 ****************************************************************************************
 */

extern uint8_t multi_bond_enabled;
extern uint8_t multi_bond_status;
extern uint8_t multi_bond_resolved_peer_pos;
extern struct usage_array_ bond_usage;
extern struct irk_array_ irk_array;
extern struct bonding_info_ bond_array[MAX_BOND_PEER];

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

void app_alt_pair_init(void);

bool app_alt_pair_disconnect(void);

int app_alt_pair_timer_handler(void);

bool app_alt_pair_check_peer(struct bd_addr *peer_addr, uint8_t peer_addr_type);

void app_alt_pair_read_status(void);

void app_alt_pair_store_bond_data(void);

void app_alt_pair_update_bond_data(int mask, int pos, int value);

int app_alt_pair_load_bond_data(struct rand_nb *rand_nb, uint16_t ediv);

bool app_alt_pair_get_next_bond_data(bool init);

bool app_alt_pair_load_last_used(void);

bool app_alt_pair_load_entry(int entry);

void app_alt_pair_delete_entry(int entry);

void app_alt_pair_clear_all_bond_data(void);

int app_alt_peer_get_active_index(void);

void updatedb_from_bonding_info(struct bonding_info_ *inf);

int update_usage_count(int idx);

void reset_active_peer_pos(void);

void update_active_peer_pos(int index);

#endif // APP_MULTI_BOND_H_
