/**
****************************************************************************************
*
* @file app_multi_bond.c
*
* @brief Special (multi) bonding procedure.
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

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <string.h>                     // string manipulation and functions
#include <stddef.h>

#include "app.h"                        // application definitions
#include "app_task.h"                   // application task definitions
#include "app_api.h"                
#include "gattc_task.h"
#include "app_console.h"
#include "app_sec.h"                
#include "co_bt.h"
#include "arch.h"                       // platform definitions
#include "ke_timer.h"                   // kernel timer 
#include "rwble_config.h"
#include "gpio.h"

#include "i2c_eeprom.h"
#include "periph_setup.h"

#include "app_kbd.h"
#include "app_kbd_key_matrix.h"
#include "app_kbd_debug.h"
#include "app_kbd_fsm.h"

#include "app_multi_bond.h"
#include "app_white_list.h"

/*
 * ROM functions
 ****************************************************************************************
 */

extern uint8_t atts_find_uuid(uint16_t *start_hdl, uint16_t end_hdl, uint8_t uuid_len, uint8_t *uuid);
extern uint8_t attmdb_att_get_value(uint16_t handle, att_size_t* length, uint8_t** value);
extern uint8_t attmdb_att_set_value(uint16_t handle, att_size_t length, uint8_t* value);


#if (BLE_APP_PRESENT)

#define ATTR_TYPE   0
#define CCC_TYPE    1

/*
 * Retained variables
 ****************************************************************************************
 */

uint8_t multi_bond_status                       __attribute__((section("retention_mem_area0"), zero_init));
uint8_t multi_bond_enabled                      __attribute__((section("retention_mem_area0"), zero_init)); 
uint8_t multi_bond_active_peer_pos              __attribute__((section("retention_mem_area0"), zero_init)); 
uint8_t multi_bond_next_peer_pos                __attribute__((section("retention_mem_area0"), zero_init));
uint8_t multi_bond_resolved_peer_pos            __attribute__((section("retention_mem_area0"), zero_init));
struct usage_array_ bond_usage                  __attribute__((section("retention_mem_area0"), zero_init));
struct irk_array_ irk_array                     __attribute__((section("retention_mem_area0"), zero_init)); // stored in RetRAM for power saving reasons
struct bonding_info_ bond_array[MAX_BOND_PEER]  __attribute__((section("retention_mem_area0"), zero_init)); // stored in RetRAM for power saving reasons

// Only one of irk_array and bond_array will be used eventually.
#if (MBOND_LOAD_INFO_AT_INIT && MBOND_LOAD_IRKS_AT_INIT)
#error "Either the IRKs will be loaded into the RetRAM or the whole bonding info. Not both!"
#endif


/*
 * Local variables
 ****************************************************************************************
 */

int attribute_handle;

void clear_eeprom(void);

/**
 * @brief       Refresh usage counters.
 *
 * @details     Refresh usage counters (decrement) for all entries apart from idx.
 *              idx is the current entry being used. Thus, it gets the maximum value
 *              currently available while all other entries are decremented by 1.
 *
 * @param[in]   idx     index of currently connected host
 *
 * @return      void
 *
 */
void update_usage_count_values(int idx)
{
    int i;

    if (HAS_MULTI_BOND)
    {
        for (i = 0; i < MAX_BOND_PEER; i++)
        {
            if ( (i == idx) || (bond_usage.pos[i] < bond_usage.pos[idx]) )
                continue;

            if (bond_usage.pos[i])
                bond_usage.pos[i]--;
        }
    }
}


/**
 * @brief       Refresh usage counters.
 *
 * @details     Refresh usage counters. idx is the current entry being used.
 *              Thus, it gets the maximum value currently available while all
 *              other entries are decremented by 1.
 *
 * @param       void
 *
 * @return      int 
 *
 * @retval      Status of operation.
 *              <ul>
 *                  <li> 0 if nothing has changed
 *                  <li> 1 if an update has been performed and the EEPROM has to be written.
 *              </ul>
 *
 */
int update_usage_count(int idx)
{
    if (HAS_MULTI_BOND)
    {
        int i;
        int max = 0;
        int max_idx = -1;
        
        // find maximum
        for (i = 0; i < MAX_BOND_PEER; i++)
        {
            if (bond_usage.pos[i] > max)
            {
                max = bond_usage.pos[i];
                max_idx = i;
            }
        }

        if (max_idx == idx)
            return 0;

        // max > MAX?
        if (max < (MAX_BOND_PEER-1))
        {
            if (bond_usage.pos[idx] != 0)
                update_usage_count_values(idx);

            if (bond_usage.pos[idx] == 0)
                bond_usage.pos[idx] = max + 1;
            else
                bond_usage.pos[idx] = max;
        }
        else
        {
            update_usage_count_values(idx);
            bond_usage.pos[idx] = (MAX_BOND_PEER-1);
        }
    
        return 1;
    }
    else
        return 0;
}


/**
 * @brief       Get an entry to use (write).
 *
 * @details     Gets the next available bonding entry for storing the bonding info.
 *              The one that hasn't been used for the longest period is selected, or
 *              an empty one (if available). 
 *
 * @param       void
 *
 * @return      int
 *
 * @retval      The index of the usage entry to be deleted / written.
 *
 */
int get_entry_to_delete(void)
{
    int i;

    if (MBOND_LOAD_INFO_AT_INIT)
    {
        if (app_env.peer_addr_type == ADDR_PUBLIC)
        {
            // Look if there's an older entry for this host (use Public address)
            
            for (i = 0; i < MAX_BOND_PEER; i++)
            {
                if ((bond_array[i].env.peer_addr_type == ADDR_PUBLIC) && !memcmp(&bond_array[i].env.peer_addr, &app_env.peer_addr, BD_ADDR_LEN))
                    return i;
            }
        }
        else if (multi_bond_resolved_peer_pos != 0) //resolved addresses
            return (multi_bond_resolved_peer_pos - 1);
    }
    else //resolved addresses (MBOND_LOAD_IRKS_AT_INIT case)
    {
        if (app_env.peer_addr_type == ADDR_PUBLIC)
        {
            // Look if there's an older entry for this host (use Public address)
            
            struct bonding_info_ info;
            int addr = EEPROM_BOND_DATA_ADDR;

            i2c_eeprom_init(I2C_SLAVE_ADDRESS, I2C_SPEED_MODE, I2C_ADDRESS_MODE, I2C_ADRESS_BYTES_CNT);
            
            for (i = 0; i < MAX_BOND_PEER; i++)
            {
                i2c_eeprom_read_data( (uint8_t *) &info, addr, sizeof(struct bonding_info_));
        
                if ( (info.env.peer_addr_type == ADDR_PUBLIC) 
                   && (!memcmp(&info.env.peer_addr, &app_env.peer_addr, BD_ADDR_LEN)) ) // no need to check if the entry is valid
                    break;
            
                addr += sizeof(struct bonding_info_);
            }
            
            i2c_eeprom_release();
            
            if (i != MAX_BOND_PEER)
                return i;
        }
        else if (multi_bond_resolved_peer_pos != 0) 
            return (multi_bond_resolved_peer_pos - 1);
    }

    if (HAS_MULTI_BOND)
    {
        // Find oldest entry
        for (i = 0; i < MAX_BOND_PEER; i++)
        {
            if (bond_usage.pos[i] == 0)
                return i;
        }
    }

    return 0;
}


/**
 * @brief       Get the last used entry (if any).
 *
 * @details     Gets the position of the last used bonding entry.
 *
 * @param       void
 *
 * @return      int
 *
 * @retval      The index of the last used bonding entry or MAX_BOND_PEER if none if found.
 *
 */
int get_last_used_entry(void)
{
    int i;
    int ret = 0;
    int max = 0;

    if (HAS_MULTI_BOND)
    {
        ret = MAX_BOND_PEER;
        
        for (i = 0; i < MAX_BOND_PEER; i++)
        {
            if (bond_usage.pos[i] > max)
            {
                max = bond_usage.pos[i];
                ret = i;
            }
        }
    }
    else
    {
        if (multi_bond_status == 0)     // nothing has been stored in the EEPROM
            ret = MAX_BOND_PEER;
        // else return the 1st entry
    }

    return ret;
}


/**
 * @brief       Initialize EEPROM.
 *
 * @details     Checks the existance of the MAGIC number and the sanity of the usage counters.
 *              If any of the checks fails, it deletes the area of the EEPROM which is used
 *              to store the bonding info. 
 *              It initializes bond_usage (usage counters) from the data stored in the EEPROM.
 *
 * @param       void
 *
 * @return      void
 *
 */
void app_alt_pair_init(void)
{
    if (HAS_EEPROM)
    {
        int i;
        int magic;
        /*volatile*/ int usage_mask = (1 << MAX_BOND_PEER) - 1; // if not volatile the compiler mess things up due to optimizations!
        bool flush = false;
        
        /*** Sanity tests ***/
        
        // MAGIC number first
        i2c_eeprom_init(I2C_SLAVE_ADDRESS, I2C_SPEED_MODE, I2C_ADDRESS_MODE, I2C_ADRESS_BYTES_CNT);
#if (DEVELOPMENT_DEBUG)
        volatile int security;
        i2c_eeprom_read_data((uint8_t *)&security, EEPROM_BASE_ADDR, sizeof(int));        
#endif        
        i2c_eeprom_read_data((uint8_t *)&magic, EEPROM_MAGIC_ADDR, sizeof(int));
        if (magic != EEPROM_MAGIC_NUMBER)
            flush = true;
        else
        {
            // MAGIC number is OK
            
            if (HAS_MULTI_BOND)
            {
                i2c_eeprom_read_data((uint8_t *)&bond_usage, EEPROM_USAGE_ADDR, sizeof(struct usage_array_));
                for (i = 0; i < MAX_BOND_PEER; i++)
                {
                    // usage must be within limits
                    if (bond_usage.pos[i] > MAX_BOND_PEER)
                    {
                        flush = true;
                        break;
                    }
                    
                    // there can't be two entries that have the same usage count unless the count is zero
                    if (bond_usage.pos[i])
                    {
                        if ( usage_mask & (1 << bond_usage.pos[i]) )
                            usage_mask &= ~(1 << bond_usage.pos[i]);
                        else
                        {
                            flush = true;
                            break;
                        }
                    }
                }
            }
            
            if (MBOND_LOAD_IRKS_AT_INIT)
            {
                struct bonding_info_ info;
                int addr = EEPROM_BOND_DATA_ADDR;

                for (i = 0; i < MAX_BOND_PEER; i++)
                {
                    i2c_eeprom_read_data( (uint8_t *) &info, addr, sizeof(struct bonding_info_));
            
                    if ((info.env.nvds_tag >> 4) == 0x5)
                    {
                        if (HAS_WHITE_LIST || HAS_VIRTUAL_WHITE_LIST)
                        {
                            add_host_in_white_list(info.env.peer_addr_type, &info.env.peer_addr, i);
                        }
                        if (info.ext_info & IRK_FLAG)
                            irk_array.irk[i] = info.irk;
                    }
                
                    addr += sizeof(struct bonding_info_);
                }
            }
            
            if (MBOND_LOAD_INFO_AT_INIT)
            {
                struct bonding_info_ info;
                int addr = EEPROM_BOND_DATA_ADDR;

                for (i = 0; i < MAX_BOND_PEER; i++)
                {
                    i2c_eeprom_read_data( (uint8_t *) &info, addr, sizeof(struct bonding_info_));
            
                    if ((info.env.nvds_tag >> 4) == 0x5)
                    {
                        bond_array[i] = info;
                        if (HAS_WHITE_LIST || HAS_VIRTUAL_WHITE_LIST)
                        {
                            add_host_in_white_list(info.env.peer_addr_type, &info.env.peer_addr, i);
                        }
                    }
                
                    addr += sizeof(struct bonding_info_);
                }
            }
        }
            
        if (flush)
        {
            clear_eeprom();
        }
        
        i2c_eeprom_release();
    }
}


/**
 * @brief       Disconnect for a Host switch.
 *
 * @details     Called on a Host switch to disconnect from the current Host and
 *              start the "switching period" of 60 sec.
 *
 * @param       void
 *
 * @return      bool
 *
 * @retval      Status of operation.
 *              <ul>
 *                  <li> true if the requested operation has been performed
 *                  <li> false if the operation is not supported (no Multi-Bond)
 *              </ul>
 *
 */
bool app_alt_pair_disconnect(void)
{
    bool ret = false;
    
    if (HAS_MULTI_BOND)
    {
        if((bond_info.env.auth & GAP_AUTH_BOND))
        {        
            multi_bond_enabled = 1;

            app_timer_set(APP_ALT_PAIR_TIMER, TASK_APP, ALT_PAIR_DISCONN_TIME);	//60 seconds
        }
        
        if (HAS_SEND_LL_TERMINATE_IND)
        {
            app_disconnect();
        }
        else
        {
            app_fake_disconnect();                    
        }
        
        ret = true;
    }

    return ret;
}


/**
 * @brief       Handler of the "Host switch" timer.
 *
 * @details     Called when the "switching period" of 60 sec expires.
 *
 * @param       void
 *
 * @return      int
 *
 * @retval      KE_MSG_CONSUMED (message handler)
 *
 */
int app_alt_pair_timer_handler(void)
{
    if (HAS_MULTI_BOND)
    {
        multi_bond_enabled = 0;
    }
    
    return (KE_MSG_CONSUMED);
}


/**
 * @brief       Filter a Host during a Host switch upon reception of a CONNECT_REQ message.
 *
 * @details     Called when a CONNECT_REQ is received during the "switching period" of 60 sec.
 *              It simply checks if the connecting Host is the same with the last one, the one
 *              being blocked for 60 sec. Since it checks the BD_ADDR only, it won't work in case
 *              of Host with Random addresses. In this case, the filtering is based on EDIV and RAND
 *              and is done in app_alt_pair_load_bond_data().
 *
 * @param[in]   peer_addr       BD_ADDR of the connecting host
 * @param[in]   peer_addr_type  Address Type of the connecting host's address (i.e. Public)
 *
 * @return      bool
 *
 * @retval      Status of operation.
 *              <ul>
 *                  <li> true if Multi-Bond is not supported or the connecting Host is (or may be,
 *                       in case of Hosts with Random addresses) different than the last one
 *                  <li> false if Multi-Bond is supported and the connecting Host is the same with
 *                       the last one
 *              </ul>
 *
 */
bool app_alt_pair_check_peer(struct bd_addr *peer_addr, uint8_t peer_addr_type)
{
    if (HAS_MULTI_BOND)
    {
        if (multi_bond_enabled)
        {
            if ( (peer_addr_type == bond_info.env.peer_addr_type) && 
                 (!memcmp(bond_info.env.peer_addr.addr, peer_addr->addr, BD_ADDR_LEN)) &&
                 (bond_info.env.auth & GAP_AUTH_BOND) )
            {
                if (HAS_SEND_LL_TERMINATE_IND)
                {
                    app_disconnect();
                }
                else
                {
                    app_fake_disconnect();                    
                }
                
                return false;
            }
        }
    }
    
    return true;
}


/**
 * @brief       Read Multi-Bond status.
 *
 * @details     Initializes multi_bond_status from the data stored in the EEPROM.
 *              multi_bond_status is a a flag which indicates the validity of host data 
 *              in the available slots. A '1' at a bit position indicates that the corresponding
 *              slot has valid host data. This field also informs about the number of the used slots
 *              (and, consequently, the number of bonded hosts).
 *              If selective delete is added then this field must be updated properly.
 *
 * @param       void
 *
 * @return      void
 *
 */
void app_alt_pair_read_status(void)
{
    if (HAS_EEPROM)
    {
        i2c_eeprom_init(I2C_SLAVE_ADDRESS, I2C_SPEED_MODE, I2C_ADDRESS_MODE, I2C_ADRESS_BYTES_CNT);
        
        i2c_eeprom_read_data(&multi_bond_status, EEPROM_STATUS_ADDR, sizeof(uint8_t));
        
        i2c_eeprom_release();
    }
}


/**
 * @brief       Write Multi-Bond status.
 *
 * @details     Writes the multi_bond_status to the EEPROM.
 *              See app_alt_pair_read_status() of info about Multi-Bond status.
 *
 * @param       void
 *
 * @return      void
 *
 */
void app_alt_pair_store_status(void)
{
    if (HAS_EEPROM)
    {
        i2c_eeprom_init(I2C_SLAVE_ADDRESS, I2C_SPEED_MODE, I2C_ADDRESS_MODE, I2C_ADRESS_BYTES_CNT);
        
        i2c_eeprom_write_byte(EEPROM_STATUS_ADDR, multi_bond_status);
        
        i2c_eeprom_release();
    }
}


/**
 * @brief       Process a UUID in the DataBase.
 *
 * @details     Finds the given UUID in the DB and returns its handle.
 *              If length is 0, a read is preformed and value holds the DB value.
 *              If length is not 0, a write is performed (value is written).
 *
 * @param[out]  handle      The handle of the UUID being processed
 * @param[in]   uuid        The UUID to be processed
 * @param       length      The size of value.
 * @param       value       The value to be read or written.
 *
 * @return      int
 *
 * @retval      Status of operation.
 *              <ul>
 *                  <li> 0 if the UUID does not exist in the DB.
 *                  <li> 1 if the UUID was found and the read/write operation was successful
 *              </ul>
 *
 */
int process_value_of_uuid(uint16_t *handle, uint8_t *uuid, int *length, uint8_t **value)
{
    if (atts_find_uuid(handle, 0xFFFF, ATT_UUID_16_LEN, uuid) == ATT_ERR_NO_ERROR)
    {
        if (*length)
        {
            if (attmdb_att_set_value(*handle, *length, *value) == ATT_ERR_NO_ERROR)
                return 1;
        }
        else
        {
            if (attmdb_att_get_value(*handle, (att_size_t *)length, value) == ATT_ERR_NO_ERROR)
                return 1;
        }
    }
    
    ASSERT_WARNING(0);
    return 0;
}


/**
 * @brief       Process the Client Characteristic Configuration of a UUID in the DataBase.
 *
 * @details     Finds the given UUID in the DB. Then it finds the Client Characteristic
 *              Configuration descriptor for this handle and returns its handle.
 *              If length is 0, a read is preformed and value holds the DB value.
 *              If length is not 0, a write is performed (value is written).
 *
 * @param[out]  handle      The handle of the CCC UUID being processed
 * @param[in]   uuid        The UUID to be processed
 * @param       length      The size of value.
 * @param       value       The value to be read or written.
 *
 * @return      int
 *
 * @retval      Status of operation.
 *              <ul>
 *                  <li> 0 if the UUID does not exist in the DB.
 *                  <li> 1 if the UUID was found and the read/write operation was successful
 *              </ul>
 *
 */
int process_value_of_ccc_uuid(uint16_t *handle, uint8_t *uuid, int *length, uint8_t **value)
{
    if (atts_find_uuid(handle, 0xFFFF, ATT_UUID_16_LEN, uuid) == ATT_ERR_NO_ERROR)
    {
        int tmp_uuid = ATT_DESC_CLIENT_CHAR_CFG;
        
        if (atts_find_uuid(handle, 0xFFFF, ATT_UUID_16_LEN, (uint8_t *)&tmp_uuid) == ATT_ERR_NO_ERROR)
        {
            if (*length)
            {
                if (attmdb_att_set_value(*handle, *length, *value) == ATT_ERR_NO_ERROR)
                    return 1;
            }
            else
            {
                if (attmdb_att_get_value(*handle, (att_size_t *)length, value) == ATT_ERR_NO_ERROR)
                    return 1;
            }
        }
    }
    
    ASSERT_WARNING(0);
    return 0;
}


/**
 * @brief       Update bonding information based on the current configuration of the DataBase.
 *
 * @param[in]   inf     The bonding information
 *
 * @return      void
 *
 */
void prepare_bonding_info(struct bonding_info_ *inf)
{
    int i, handle, uuid, *value, length;
    
    // Read Protocol mode
    handle = 0;
    uuid = ATT_CHAR_PROTOCOL_MODE; 
    length = 0; // read
    if (process_value_of_uuid((uint16_t *)&handle, (uint8_t *)&uuid, &length, (uint8_t **)&value))
        inf->info = (*(uint8_t *)value ? PROTOCOL_MODE_MASK : 0);
    
    // Read Notifications' status
    // Battery Level
    handle = 0;
    uuid = ATT_CHAR_BATTERY_LEVEL; 
    length = 0; // read
    if (process_value_of_ccc_uuid((uint16_t *)&handle, (uint8_t *)&uuid, &length, (uint8_t **)&value))
        inf->info |= ((*(uint16_t *)value & 0x01) ? BASS_CCC_MASK : 0);
    // else default setting is off
    
    // Boot report
    handle = 0;
    uuid = ATT_CHAR_BOOT_KB_IN_REPORT; 
    length = 0; // read
    if (process_value_of_ccc_uuid((uint16_t *)&handle, (uint8_t *)&uuid, &length, (uint8_t **)&value))
        inf->info |= ((*(uint16_t *)value & 0x01) ? HID_BOOT_CCC_MASK : 0);
    // else default setting is off
    
    // Normal Reports
    handle = 0;
    for (i = 0; i < 2; i++)
    {
        uuid = ATT_CHAR_REPORT; 
        length = 0;
        if (process_value_of_ccc_uuid((uint16_t *)&handle, (uint8_t *)&uuid, &length, (uint8_t **)&value))
        {
            inf->info |= ((*(uint16_t *)value & 0x01) ? (i == 0 ? HID_REPORT_0_CCC_MASK : HID_REPORT_2_CCC_MASK) : 0);
        }
        // else default setting is off
    }
    
    // Read Control Point
    handle = 0;
    uuid = ATT_CHAR_HID_CTNL_PT; 
    length = 0; // read
    if (process_value_of_uuid((uint16_t *)&handle, (uint8_t *)&uuid, &length, (uint8_t **)&value))
        inf->info |= (*(uint8_t *)value ? HID_CTNL_PT_CCC_MASK : 0);
    // else default setting is 0 ("Suspend")
    
    // Read Service Changed
    handle = 0;
    uuid = ATT_CHAR_SERVICE_CHANGED; 
    length = 0; // read
    if (process_value_of_ccc_uuid((uint16_t *)&handle, (uint8_t *)&uuid, &length, (uint8_t **)&value))
        inf->info |= ( (*(uint16_t *)value) << SVC_CHNG_CCC_POS );
    // else default setting is 0
    
    // There's no affected attribute range at this point, so the rest of the info.info (bytes 1 and 2) is zero.
}


/**
 * @brief       Write bonding information to a specific entry of the EEPROM.
 *
 * @param[in]   entry     The index to to EEPROM bonding entry.
 *
 * @return      void
 *
 */
void write_bonding_info(int entry)
{
    int addr = EEPROM_BOND_DATA_ADDR;
    
    addr += entry * sizeof(struct bonding_info_); // offset
    i2c_eeprom_write_byte((addr + offsetof(struct bonding_info_, env.nvds_tag)), 0); //invalidate
    i2c_eeprom_write_data((uint8_t *)&bond_info, addr, sizeof(struct bonding_info_));
    if (update_usage_count(entry))
    {
        i2c_eeprom_write_data((uint8_t *)&bond_usage, EEPROM_USAGE_ADDR, sizeof(struct usage_array_));
    }
    
    // Update the IRK array
    if (MBOND_LOAD_IRKS_AT_INIT)
    {
//        memcpy(&irk_array.irk[multi_bond_active_peer_pos].key[0], &bond_info.irk.key[0], KEY_LEN);
        irk_array.irk[multi_bond_active_peer_pos] = bond_info.irk;
    }

    // or, update the buffer in RetRAM
    if (MBOND_LOAD_INFO_AT_INIT)
    {
        bond_array[entry] = bond_info;
    }
}


/**
 * @brief       Store the bonding information of the connected Host to the EEPROM.
 *
 * @details     First, it looks for an entry for this Host in the EEPROM. If one is found,
 *              it updates it only if the Host is using a Random address or the write is
 *              forced because of a DB change (bond_info.info).
 *              If no entry is found in the EEPROM for this host then it gets one via
 *              get_entry_to_delete() and writes the data there.
 *              The usage counters are updated in both cases.
 *
 * @param[in]   force       if 1 then a forced update of the entry is done
 *
 * @return      void
 *
 */
void app_alt_pair_store_bond_data(void)
{
    if (HAS_EEPROM)
    {
        if ((current_fsm_state == CONNECTED_PAIRING_ST) || (current_fsm_state == CONNECTED_ST))
        {
            i2c_eeprom_init(I2C_SLAVE_ADDRESS, I2C_SPEED_MODE, I2C_ADDRESS_MODE, I2C_ADRESS_BYTES_CNT);
        
            if (multi_bond_active_peer_pos != MAX_BOND_PEER)  // entry for peer exists. Overwite!
            {
                prepare_bonding_info(&bond_info);
                write_bonding_info(multi_bond_active_peer_pos);
            }
            else        // entry for peer does not exist. Write to an empty or the one that hasn't been used the longest.
            {
                int entry;
            
                ASSERT_WARNING(multi_bond_active_peer_pos == MAX_BOND_PEER);
                
                entry = get_entry_to_delete();
                update_active_peer_pos(entry);
            
                prepare_bonding_info(&bond_info);
                bond_info.env.nvds_tag = 0x50 + entry; // validity flag
                write_bonding_info(entry);
            
                if ( !(multi_bond_status & (1 << entry)) )
                {
                    multi_bond_status |= (1 << entry); // update status
                    i2c_eeprom_write_byte(EEPROM_STATUS_ADDR, multi_bond_status);
                }
            }
        
            i2c_eeprom_release();
        }
        // else ignore any write. It will fail with "Insufficient Authentication" anyway...
    }
    else
        prepare_bonding_info(&bond_info);
}


/**
 * @brief       Updates the next attribute of a specific type in the DataBase based on the bonding information.
 *
 * @details     Updates a specific attribute in the DataBase based on the bonding information,
 *              starting from the current value of the "read" pointer of the DataBase. 
 *
 * @param[in]   inf     The bonding information.
 * @param[in]   uuid    The UUID of the attribute (16-bit only)!
 * @param[in]   length  The size of the attribute's value.
 * @param[in]   mask    Which field of the inf->info is the attribute's value.
 * @param[in]   pos     Shift range of the value to become normal.
 * @param[in]   type    Selects whether a CCC (CCC_TYPE) or an attribute (ATTR_TYPE) is accessed.
 *
 * @return      int
 *
 * @retval      The value of the attribute.
 *
 */
int process_next_attribute(struct bonding_info_ *inf, int uuid, int length, int mask, int pos, int type)
{
    int value;
    uint8_t *p_val = (uint8_t *)&value;
    
    value = (inf->info & mask) >> pos;
    if (type == ATTR_TYPE)
        process_value_of_uuid((uint16_t *)&attribute_handle, (uint8_t *)&uuid, &length, &p_val);
    else
        process_value_of_ccc_uuid((uint16_t *)&attribute_handle, (uint8_t *)&uuid, &length, &p_val);
    
    return value;
}


/**
 * @brief       Updates the first attribute of a specific type in the DataBase based on the bonding information.
 *
 * @details     Updates a specific attribute in the DataBase based on the bonding information,
 *              starting from the beginning of the DataBase. The "read" pointer is reset.
 *
 * @param[in]   inf     The bonding information.
 * @param[in]   uuid    The UUID of the attribute (16-bit only)!
 * @param[in]   length  The size of the attribute's value.
 * @param[in]   mask    Which field of the inf->info is the attribute's value.
 * @param[in]   pos     Shift range of the value to become normal.
 * @param[in]   type    Selects whether a CCC (CCC_TYPE) or an attribute (ATTR_TYPE) is accessed.
 *
 * @return      int
 *
 * @retval      The value of the attribute.
 *
 */
int process_attribute(struct bonding_info_ *inf, int uuid, int length, int mask, int pos, int type)
{
    attribute_handle = 0;
        
    return process_next_attribute(inf, uuid, length, mask, pos, type);
}


/**
 * @brief       Update the DataBase based on the bonding information.
 *
 * @details     Updates the DB with the values of inf. If a modified attribute range
 *              is written in the EEPROM then a Service Changed indication (if enabled)
 *              is sent to the Host.
 *
 * @param[in]   inf     The bonding information
 *
 * @return      void
 *
 */
void updatedb_from_bonding_info(struct bonding_info_ *inf)
{
    int value;
    
    // Update Protocol mode
    value = process_attribute(inf, ATT_CHAR_PROTOCOL_MODE, 1, PROTOCOL_MODE_MASK, PROTOCOL_MODE_POS, ATTR_TYPE);
    if (value)
        hogpd_env.proto_mode[0] = HOGP_REPORT_PROTOCOL_MODE;
    else
        hogpd_env.proto_mode[0] = HOGP_BOOT_PROTOCOL_MODE;
    
    // Update Notifications' status
    // Update Battery Level
    value = process_attribute(inf, ATT_CHAR_BATTERY_LEVEL, 2, BASS_CCC_MASK, BASS_CCC_POS, CCC_TYPE);
    if (value)
        bass_env.features[0] |= BASS_FLAG_NTF_CFG_BIT;
    
    // Update Boot report
    value = process_attribute(inf, ATT_CHAR_BOOT_KB_IN_REPORT, 2, HID_BOOT_CCC_MASK, HID_BOOT_CCC_POS, CCC_TYPE);
    if (value)
        hogpd_env.features[0].svc_features |= HOGPD_REPORT_NTF_CFG_MASK;
    
    // Update Normal Reports
    // 1st report
    value = process_attribute(inf, ATT_CHAR_REPORT, 2, HID_REPORT_0_CCC_MASK, HID_REPORT_0_CCC_POS, CCC_TYPE);
    if (value)
        hogpd_env.features[0].report_char_cfg[0] |= HOGPD_REPORT_NTF_CFG_MASK;
    // next report
    value = process_next_attribute(inf, ATT_CHAR_REPORT, 2, HID_REPORT_2_CCC_MASK, HID_REPORT_2_CCC_POS, CCC_TYPE);
    if (value)
        hogpd_env.features[0].report_char_cfg[2] |= HOGPD_REPORT_NTF_CFG_MASK;
    
    // Update Control Point
    process_attribute(inf, ATT_CHAR_HID_CTNL_PT, 1, HID_CTNL_PT_CCC_MASK, HID_CTNL_PT_CCC_POS, ATTR_TYPE);
    
    // Update Service Changed
    process_attribute(inf, ATT_CHAR_SERVICE_CHANGED, 2, SVC_CHNG_CCC_MASK, SVC_CHNG_CCC_POS, CCC_TYPE);
    
    // Check if there's any affected attribute range at this point.
    if (inf->info & 0xFFFF00)
    {
        struct gattc_send_svc_changed_cmd *req = KE_MSG_ALLOC(GATTC_SEND_SVC_CHANGED_CMD,
                                                      KE_BUILD_ID(TASK_GATTC, app_env.conidx), TASK_APP,
                                                      gattc_send_svc_changed_cmd);
        req->svc_shdl = (inf->info & 0xFF00) >> 8;
        req->svc_ehdl = (inf->info & 0xFF0000) >> 16;
        
        // Send the event
        ke_msg_send(req);
    }
}


/**
 * @brief       Check if bond info must be stored in the EEPROM due to a DataBase change.
 *
 * @param[in]   mask        The bitfield mask in the info
 * @param[in]   pos         The offest of the field in the info
 * @param[in]   value       The value of this field in the DataBase.
 *
 * @return      void
 *
 */
void app_alt_pair_update_bond_data(int mask, int pos, int value)
{
    if ( ((bond_info.info & mask) >> pos) != value)
    {
        app_alt_pair_store_bond_data();
    }
}


/**
 * @brief       Find the keys for the connecting host, if available.
 *
 * @details     Search if there's an entry in the EEPROM for the connecting host. It is 
 *              called upon the reception of an LL_ENC_REQ message from the host.
 *
 * @param[in]   rand_nb     The RAND_NB value of the LL_ENC_REQ message.
 * @param[in]   ediv        The EDIV value of the LL_ENC_REQ message.
 *
 * @return      int
 *
 * @retval      Status of operation.
 *              <ul>
 *                  <li> 0 if there's no entry in the EEPROM for this host
 *                  <li> 1 if the new host is different from the last host we connected to
 *                  <li> 2 if the new host is the same with the last host we connected to
 *              </ul>
 *
 */
int app_alt_pair_load_bond_data(struct rand_nb *rand_nb, uint16_t ediv)
{
    if (HAS_EEPROM)
    {
        uint32_t i;
        int retval = 0;
        
        if (MBOND_LOAD_INFO_AT_INIT)
        {
            //Read buffer in RetRAM
            
            for (i = 0; i < MAX_BOND_PEER; i++)
            {
                if ( ( (bond_array[i].env.nvds_tag >> 4) == 0x5) && (bond_array[i].env.ediv == ediv) && (!memcmp(rand_nb, &bond_array[i].env.rand_nb, RAND_NB_LEN))
                        && (bond_array[i].env.auth & GAP_AUTH_BOND))
                {
                    if ( (bond_info.env.ediv == ediv) && (!memcmp(rand_nb, &bond_info.env.rand_nb, RAND_NB_LEN))
                          && (bond_info.env.auth & GAP_AUTH_BOND) )
                        retval = 2;
                    else
                        retval = 1;
                    
                    bond_info = bond_array[i];  // use memcpy instead???
                    update_active_peer_pos(i);
                    if (update_usage_count(i))
                    {
                        i2c_eeprom_write_data((uint8_t *)&bond_usage, EEPROM_USAGE_ADDR, sizeof(struct usage_array_));
                    }
                    updatedb_from_bonding_info(&bond_info);
                    break; 
                }
            }

            return retval;
        }
        else
        {
            // Read EEPROM
            
            struct bonding_info_ info;
            int addr = EEPROM_BOND_DATA_ADDR;

            i2c_eeprom_init(I2C_SLAVE_ADDRESS, I2C_SPEED_MODE, I2C_ADDRESS_MODE, I2C_ADRESS_BYTES_CNT);
            
            for (i = 0; i < MAX_BOND_PEER; i++)
            {
                i2c_eeprom_read_data( (uint8_t *) &info, addr, sizeof(struct bonding_info_));
                
                if ( ( (info.env.nvds_tag >> 4) == 0x5) && (info.env.ediv == ediv) && (!memcmp(rand_nb, &info.env.rand_nb, RAND_NB_LEN))
                        && (info.env.auth & GAP_AUTH_BOND))
                {
                    if ( (bond_info.env.ediv == ediv) && (!memcmp(rand_nb, &bond_info.env.rand_nb, RAND_NB_LEN))
                          && (bond_info.env.auth & GAP_AUTH_BOND) )
                        retval = 2;
                    else
                        retval = 1;
                    
                    bond_info = info;
                    update_active_peer_pos(i);
                    if (update_usage_count(i))
                    {
                        i2c_eeprom_write_data((uint8_t *)&bond_usage, EEPROM_USAGE_ADDR, sizeof(struct usage_array_));
                    }
                    updatedb_from_bonding_info(&bond_info);
                    break; 
                }
                
                addr += sizeof(struct bonding_info_);
            }
            
            i2c_eeprom_release();

            return retval;
        }
    }
    else
        return 0;
}


/**
 * @brief       Find the first or next entry in the EEPROM with valid keys.
 *
 * @details     If init is true then find the first valid entry starting from the beginning.
 *              Used when no connection exists and advertising is about to start!
 *              If init is false then find the next valid entry (if any) starting just 
 *              after the last position used. This can be the position of the current bonded 
 *              host or, if directed advertising has started, the position of the last host 
 *              we did directed advertising to. Used when connected and intend to start 
 *              directed advertising to other hosts for whom bonded data exist or while a 
 *              cycle of directed advertising to all known bonded hosts, until a connection 
 *              is established, is being executed.
 *
 * @warning     Active and Next pointers will be reset when init is true.
 *
 * @param[in]   init    Controls which entry to find; first, if true or next, if false.
 *
 * @return      bool
 *
 * @retval      Status of operation.
 *              <ul>
 *                  <li> true if a valid entry was found
 *                  <li> false if no valid entry was found. bond_info is reset.
 *              </ul>
 *
 */
bool app_alt_pair_get_next_bond_data(bool init)
{
    if (HAS_EEPROM)
    {
        uint32_t i;
        bool status = false;
        
        if (MBOND_LOAD_INFO_AT_INIT)
        {
            // Read buffer in RetRAM
            
            // after reset 'multi_bond_next_peer_pos' is 0. if it's not 0 then this is the position we should use
            i = multi_bond_next_peer_pos;
            
            if (init) 
                multi_bond_active_peer_pos = MAX_BOND_PEER;

            do
            {
                if (i == MAX_BOND_PEER) 
                    i = 0;
                    
                if ( ( (bond_array[i].env.nvds_tag >> 4) == 0x5) && (bond_array[i].env.peer_addr_type == ADDR_PUBLIC) && (bond_array[i].env.auth & GAP_AUTH_BOND) ) //used entry (public address)
                {
                    bond_info = bond_array[i];
                    updatedb_from_bonding_info(&bond_info);
                    update_active_peer_pos(i);
                    status = true;
                    break; 
                }
                
                i++;
                
                if (i == multi_bond_active_peer_pos) 
                {
                    if (multi_bond_active_peer_pos == MAX_BOND_PEER) // special case
                        multi_bond_next_peer_pos = 0;
                    else
                        multi_bond_next_peer_pos = i + 1;
                        
                    break;
                }
            } while(1);
            
            if (!status) 
            {
                if (init)
                {
                    bond_info.info = BOND_INFO_DEFAULT_VAL;
                    updatedb_from_bonding_info(&bond_info);
                    if (HAS_MITM)
                    {
                        bond_info.env.auth = GAP_AUTH_REQ_MITM_BOND;
                    }
                    else
                    {
                        bond_info.env.auth = GAP_AUTH_REQ_NO_MITM_BOND;
                    }
                }
                else
                {
                    bond_info = bond_array[multi_bond_active_peer_pos];
    //                updatedb_from_bonding_info(&info);
                }
            }

            return status;
        }
        else
        {
            // Read EEPROM
            
            struct bonding_info_ info;
            int addr;

            i2c_eeprom_init(I2C_SLAVE_ADDRESS, I2C_SPEED_MODE, I2C_ADDRESS_MODE, I2C_ADRESS_BYTES_CNT);
            
            // after reset 'multi_bond_next_peer_pos' is 0. if it's not 0 then this is the position we should use
            i = multi_bond_next_peer_pos;
            
            if (init) 
                multi_bond_active_peer_pos = MAX_BOND_PEER;

            do
            {
                if (i == MAX_BOND_PEER) 
                    i = 0;
                    
                addr = (i * sizeof(struct bonding_info_)) + EEPROM_BOND_DATA_ADDR;

                i2c_eeprom_read_data((uint8_t *) &info, addr, sizeof(struct bonding_info_));
                
                if ( ( (info.env.nvds_tag >> 4) == 0x5) && (info.env.peer_addr_type == ADDR_PUBLIC) && (info.env.auth & GAP_AUTH_BOND) ) //used entry (public address)
                {
                    bond_info = info;
                    updatedb_from_bonding_info(&info);
                    update_active_peer_pos(i);
                    status = true;
                    break; 
                }
                
                i++;
                
                if (i == multi_bond_active_peer_pos) 
                {
                    if (multi_bond_active_peer_pos == MAX_BOND_PEER) // special case
                        multi_bond_next_peer_pos = 0;
                    else
                        multi_bond_next_peer_pos = i + 1;
                        
                    break;
                }
                    
            } while(1);
            
            if (!status) 
            {
                if (init)
                {
                    bond_info.info = BOND_INFO_DEFAULT_VAL;
                    updatedb_from_bonding_info(&bond_info);
                    if (HAS_MITM)
                    {
                        bond_info.env.auth = GAP_AUTH_REQ_MITM_BOND;
                    }
                    else
                    {
                        bond_info.env.auth = GAP_AUTH_REQ_NO_MITM_BOND;
                    }
                }
                else
                {
                    addr = (multi_bond_active_peer_pos * sizeof(struct bonding_info_)) + EEPROM_BOND_DATA_ADDR;
                    i2c_eeprom_read_data( (uint8_t *) &info, addr, sizeof(struct bonding_info_));
                    bond_info = info;
    //                updatedb_from_bonding_info(&info);
                }
            }
                
            i2c_eeprom_release();

            return status;
        }
    }
    else
        return false;
}


/**
 * @brief       Find the last used entry in the EEPROM and load keys.
 *
 * @details     Find the last used entry in the EEPROM and load the bonding info.
 *
 * @param       void
 *
 * @return      bool
 *
 * @retval      Status of operation.
 *              <ul>
 *                  <li> true if a valid entry was found
 *                  <li> false if the EEPROM is empty. bond_info is left unchanged.
 *              </ul>
 *
 */
bool app_alt_pair_load_last_used(void)
{
    bool status = false;
    uint32_t i;
    
    if (HAS_EEPROM)
    {
        if (MBOND_LOAD_INFO_AT_INIT)
        {
            // Read buffer in RetRAM
            
            i = get_last_used_entry();
            
            if (i != MAX_BOND_PEER)
            {
                ASSERT_WARNING((bond_array[i].env.nvds_tag >> 4) == 0x5);
                if ( (bond_array[i].env.nvds_tag >> 4) == 0x5 )
                {
                    bond_info = bond_array[i];
                    updatedb_from_bonding_info(&bond_info);
                    update_active_peer_pos(i);
                    status = true;
                }
                else
                {
                    // Data are corrupted! bond_usage or multi_bond_status reported as used an invalid entry!
                    ASSERT_ERROR(0);
                }
            }
        }
        else
        {
            // Read EEPROM
            
            struct bonding_info_ info;
            int addr;

            i = get_last_used_entry();
            
            if (i != MAX_BOND_PEER)
            {
                i2c_eeprom_init(I2C_SLAVE_ADDRESS, I2C_SPEED_MODE, I2C_ADDRESS_MODE, I2C_ADRESS_BYTES_CNT);
                
                addr = (i * sizeof(struct bonding_info_)) + EEPROM_BOND_DATA_ADDR;

                i2c_eeprom_read_data((uint8_t *) &info, addr, sizeof(struct bonding_info_));
                ASSERT_WARNING((info.env.nvds_tag >> 4) == 0x5);
                if ( (info.env.nvds_tag >> 4) == 0x5 )
                {
                    bond_info = info;
                    updatedb_from_bonding_info(&info);
                    update_active_peer_pos(i);
                    status = true;
                }
                else
                {
                    // EEPROM is corrupted! bond_usage or multi_bond_status reported as used an invalid entry!
                    clear_eeprom();
                }
                
                i2c_eeprom_release();
            }
        }
    }
        
    return status;
}


/**
 * @brief       Load the keys from a specific entry in the EEPROM.
 *
 * @details     Gets the bonding info from a specific entry in the EEPROM and, if it's valid,
 *              it copies it to the bond_info and updates the DataBase according to it.
 *
 * @param       int     The index to the entry in the EEPROM.
 *
 * @return      bool
 *
 * @retval      Status of operation.
 *              <ul>
 *                  <li> true if a valid entry was found
 *                  <li> false if there is no valid info at "entry". bond_info is left unchanged.
 *              </ul>
 *
 */
bool app_alt_pair_load_entry(int entry)
{
    bool status = false;
    
    ASSERT_WARNING( ((entry >= 0) && (entry < MAX_BOND_PEER)) );
    ASSERT_WARNING(entry != multi_bond_active_peer_pos);
    
    if (HAS_EEPROM)
    {
        if (MBOND_LOAD_INFO_AT_INIT)
        {
            // Read buffer in RetRAM
            
            if ( (bond_array[entry].env.nvds_tag >> 4) == 0x5 )
            {
                bond_info = bond_array[entry];
                updatedb_from_bonding_info(&bond_info);
                update_active_peer_pos(entry);
                if (update_usage_count(entry))
                {
                    i2c_eeprom_write_data((uint8_t *)&bond_usage, EEPROM_USAGE_ADDR, sizeof(struct usage_array_));
                }
                status = true;
            }
            else
            {
                // The entry is not valid!
            }
        }
        else
        {
            // Read EEPROM
            
            struct bonding_info_ info;
            int addr;
            
            i2c_eeprom_init(I2C_SLAVE_ADDRESS, I2C_SPEED_MODE, I2C_ADDRESS_MODE, I2C_ADRESS_BYTES_CNT);
            
            addr = (entry * sizeof(struct bonding_info_)) + EEPROM_BOND_DATA_ADDR;

            i2c_eeprom_read_data((uint8_t *) &info, addr, sizeof(struct bonding_info_));
            if ( (info.env.nvds_tag >> 4) == 0x5 )
            {
                bond_info = info;
                updatedb_from_bonding_info(&info);
                update_active_peer_pos(entry);
                if (update_usage_count(entry))
                {
                    i2c_eeprom_write_data((uint8_t *)&bond_usage, EEPROM_USAGE_ADDR, sizeof(struct usage_array_));
                }
                status = true;
            }
            else
            {
                // The entry is not valid!
            }
            
            i2c_eeprom_release();
        }
    }
        
    return status;
}


/**
 * @brief       Delete a specific entry in the EEPROM.
 *
 * @param       int     The index to the entry in the EEPROM to be deleted.
 *
 * @return      void
 *
 */
void app_alt_pair_delete_entry(int entry)
{
    ASSERT_WARNING( ((entry >= 0) && (entry < MAX_BOND_PEER)) );
    
    if (HAS_EEPROM)
    {
        int addr;
        
        // Update usage counters
        if (HAS_MULTI_BOND)
        {
            if (bond_usage.pos[entry] != 0)
            {
                int i;
                
                for (i = 0; i < MAX_BOND_PEER; i++)
                {
                    if (bond_usage.pos[i] > bond_usage.pos[entry])
                        bond_usage.pos[i]--;
                }
                bond_usage.pos[entry] = 0;
            }
        }
        
        i2c_eeprom_init(I2C_SLAVE_ADDRESS, I2C_SPEED_MODE, I2C_ADDRESS_MODE, I2C_ADRESS_BYTES_CNT);
        
        addr = (entry * sizeof(struct bonding_info_)) + EEPROM_BOND_DATA_ADDR;
        
        if (MBOND_LOAD_IRKS_AT_INIT)
        {
            struct bonding_info_ info;
            
            i2c_eeprom_read_data((uint8_t *) &info, addr, sizeof(struct bonding_info_));
            
            if ( (info.env.nvds_tag >> 4) == 0x5 )
            {
                if (HAS_WHITE_LIST || HAS_VIRTUAL_WHITE_LIST)
                {
                    remove_host_from_white_list(info.env.peer_addr_type, &info.env.peer_addr, entry);
                }
            }
        }
        
        // Delete the entry
        i2c_eeprom_write_byte((addr + offsetof(struct bonding_info_, env.nvds_tag)), 0); //invalidate
        
        // Write the usage counters
        i2c_eeprom_write_data((uint8_t *)&bond_usage, EEPROM_USAGE_ADDR, sizeof(struct usage_array_));
        
        // Update the multi_bond_status
        multi_bond_status &= ~(1 << entry);
        i2c_eeprom_write_byte(EEPROM_STATUS_ADDR, multi_bond_status);

        i2c_eeprom_release();

        // Update the IRK array
        if (MBOND_LOAD_IRKS_AT_INIT)
        {
            memset(&irk_array.irk[entry].key[0], 0, KEY_LEN);
        }
        
        // or, update the buffer in RetRAM
        if (MBOND_LOAD_INFO_AT_INIT)
        {
            if (HAS_WHITE_LIST || HAS_VIRTUAL_WHITE_LIST)
            {
                remove_host_from_white_list(bond_array[entry].env.peer_addr_type, &bond_array[entry].env.peer_addr, entry);
            }
            memset(&bond_array[entry], 0, sizeof(struct bonding_info_));
        }
    }
}


/**
 * @brief       Clear the EEPROM.
 *
 * @warning     i2c_eeprom_init() must be called before calling this function.  
 *              i2c_eeprom_release() must be called after this function exits.
 *
 * @param       void
 *
 * @return      void
 *
 */
void clear_eeprom(void)
{
    int i;
    int magic;
    int security;
    
    uint8_t zero_data[32] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                             0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                             0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                             0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
    uint8_t addr = EEPROM_BASE_ADDR;
       
                                
    for (i = 0; i < (EEPROM_BOND_SIZE / 32); i++)
    {
        addr += i * 32;
        i2c_eeprom_write_data( zero_data, (uint8_t) addr, 32);
    }
    multi_bond_status = 0;
    
    if (HAS_MULTI_BOND)
    {
        for (i = 0; i < MAX_BOND_PEER; i++)
            bond_usage.pos[i] = 0;
    }

    if (DEVELOPMENT_DEBUG)
    {
        uint8_t addr;
        int i, j;
        uint8_t read_data[4];

        for (i = 0; i < 64; i++)
        {
            addr = i * 4;
            
            for (j = 0; j < 4; j++)
                read_data[j] = 0xFF;
            
            i2c_eeprom_read_data(read_data, addr, 4);
            
            if (memcmp(zero_data, read_data, 4))
                __asm("BKPT #0\n"); 
        }
    }
    
#if (DEVELOPMENT_DEBUG)
    security = EEPROM_GUARD_NUMBER;
    i2c_eeprom_write_data((uint8_t *)&security, EEPROM_BASE_ADDR, sizeof(int));        
#endif        
    magic = EEPROM_MAGIC_NUMBER;
    i2c_eeprom_write_data((uint8_t *)&magic, EEPROM_MAGIC_ADDR, sizeof(int));
    memset(&bond_usage, 0, sizeof(struct usage_array_));

    if (MBOND_LOAD_IRKS_AT_INIT)
    {
        memset(&irk_array, 0, sizeof(struct irk_array_));
    }
    
    if (MBOND_LOAD_INFO_AT_INIT)
    {
        memset(bond_array, 0, (MAX_BOND_PEER * sizeof(struct bonding_info_)));
    }
    
    if (HAS_WHITE_LIST || HAS_VIRTUAL_WHITE_LIST)
    {
        clear_white_list();
    }
}


/**
 * @brief       Clear the EEPROM.
 *
 * @param       void
 *
 * @return      void
 *
 */
void app_alt_pair_clear_all_bond_data(void)
{
    if (HAS_EEPROM)
    {
        i2c_eeprom_init(I2C_SLAVE_ADDRESS, I2C_SPEED_MODE, I2C_ADDRESS_MODE, I2C_ADRESS_BYTES_CNT);
                                    
        clear_eeprom();
        
        i2c_eeprom_release();
    }
}


/**
 * @brief       Initialize the index to the last (or current) used EEPROM entry.
 *
 * @param       void
 *
 * @return      void
 *
 */
void reset_active_peer_pos(void)
{
    multi_bond_active_peer_pos = MAX_BOND_PEER;
    multi_bond_next_peer_pos = 0;
}


/**
 * @brief       Update the index to the last (or current) used EEPROM entry.
 *
 * @param       void
 *
 * @return      void
 *
 */
void update_active_peer_pos(int index)
{
    ASSERT_WARNING((index >= 0) && (index < MAX_BOND_PEER));
    
    multi_bond_active_peer_pos = index;
    multi_bond_next_peer_pos = ((index + 1) % MAX_BOND_PEER);
}


/**
 * @brief       Get the index to the current (used) EEPROM entry.
 *
 * @param       void
 *
 * @return      int
 *
 * @retval      The index to the EEPROM entry being used. If no entry has been loaded, MAX_BOND_PEER is returned.
 *
 */
int app_alt_peer_get_active_index(void)
{
    return multi_bond_active_peer_pos;
}


#endif //(BLE_APP_PRESENT)
/// @} APP
