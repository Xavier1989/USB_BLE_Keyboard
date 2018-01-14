/**
****************************************************************************************
*
* @file app_white_list.c
*
* @brief White List management API.
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

#include "rwip_config.h"
#include "arch.h"
#include "app.h" 
#include "app_task.h"
#include "app_api.h"
#include "gapm_task.h"
#include "gapm_util.h"
#include "gattc_task.h"
#include "gapc.h"
#include "llc_util.h"
#include "rwble_hl_config.h"

#include "app_kbd.h"
#include "app_kbd_key_matrix.h"
#include "app_white_list.h"

#if (BLE_APP_PRESENT)

/*
 * Retained variables
 ****************************************************************************************
 */

uint8_t white_list_written                                  __attribute__((section("retention_mem_area0"), zero_init));
struct virtual_wl_entry virtual_white_list[MAX_BOND_PEER]   __attribute__((section("retention_mem_area0"), zero_init));
/***
 * NOTE
 * ----
 * virtual_white_list has a 1-1 correspondence with the indexes to the volatile storage bond info of the 
 * entries that have been entered into it. Since it is initialized to 0, the value of 0 is used to mark
 * "unused" or "empty". A "used" virtual white list entry has a value of (index + 1).
 */
enum adv_filter_policy virtual_wlist_policy __attribute__((section("retention_mem_area0"), zero_init));
/***
 * NOTE
 * ----
 * The Virtual White List does not support Scan policies. So, below are listed the possible values of 
 * virtual_wlist_policy and those that are not supported are marked explicitly.
 *
 * ADV_ALLOW_SCAN_ANY_CON_ANY   : Allow both scan and connection requests from anyone. (default setting)
 * ADV_ALLOW_SCAN_WLST_CON_ANY  : Allow scan req from Virtual White List devices only and connection req from anyone. (NOT SUPPORTED)
 * ADV_ALLOW_SCAN_ANY_CON_WLST  : Allow scan req from anyone and connection req from Virtual White List devices only.
 * ADV_ALLOW_SCAN_WLST_CON_WLST : Allow scan and connection requests from Virtual White List devices only. (NOT SUPPORTED)
 */



/*
 * Function definitions
 ****************************************************************************************
 */

/**
 * @brief       Send a GAPM_WHITE_LIST_MGT_CMD to the stack.
 *
 * @details     Sends a GAPM_WHITE_LIST_MGT_CMD to the stack to add or remove a host
 *              from the White List.
 *
 * @param[in]   add              Controls whether it will be an add or remove operation; true for add, false for remove.
 * @param[in]   peer_addr_type   The BD_ADDR type of the host.
 * @param[in]   peer_addr        The BD_ADDR of the host.
 *
 * @return      void
 *
 */
void send_white_list_mgt_cmd(bool add, uint8_t peer_addr_type, struct bd_addr const *peer_addr)
{
    struct gapm_white_list_mgt_cmd * req = KE_MSG_ALLOC_DYN(GAPM_WHITE_LIST_MGT_CMD, 
                                                            TASK_GAPM, TASK_APP, 
                                                            gapm_white_list_mgt_cmd, 
                                                            sizeof(struct gap_bdaddr));

    // Fill in the parameter structure
    if (add)
        req->operation = GAPM_ADD_DEV_IN_WLIST;
    else
        req->operation = GAPM_RMV_DEV_FRM_WLIST;
        
    req->nb = 1;
    req->devices[0].addr_type = peer_addr_type;
    memcpy(req->devices[0].addr.addr, peer_addr->addr, BD_ADDR_LEN);
    
    ke_msg_send(req);
}


/**
 * @brief       Adds a host to the normal or virtual White List.
 *
 * @details     Adds a host to the White List. If the host has Public Address,
 *              it is added to the normal White List. If it has a Resolvable
 *              Random Address, it is added to the virtual White List.
 *
 * @param[in]   peer_addr_type   The BD_ADDR type of the host.
 * @param[in]   peer_addr        The BD_ADDR of the host.
 * @param[in]   bond_info_entry  The index to the storage area for this bonding info.
 *
 * @return      bool
 *
 * @retval      Status of operation.
 *              <ul>
 *                  <li> true if no error occurred; white_list_written will be increased by 1 upon successful completion
 *                  <li> false if the host could not be added for some reason
 *              </ul>
 *
 */
bool add_host_in_white_list(uint8_t peer_addr_type, struct bd_addr const *peer_addr, uint8_t bond_info_entry)
{
    int i;
    bool ret = false;
    
    if (HAS_WHITE_LIST)
    {
        if (ADDR_PUBLIC == peer_addr_type)
        {
            send_white_list_mgt_cmd(true, peer_addr_type, peer_addr);
            ret = true;
        }
    }
    else if (HAS_VIRTUAL_WHITE_LIST)
    {
        // Public or Static Private addresses
        if (   (ADDR_PUBLIC == peer_addr_type)
            || ( (peer_addr_type == ADDR_RAND) && ((peer_addr->addr[5] & SMPM_ADDR_TYPE_STATIC) == SMPM_ADDR_TYPE_STATIC) ) )
        {
            for (i = 0; i < MAX_BOND_PEER; i++)
                if ( (virtual_white_list[i].status == USED_ADDR_PUBLIC_or_PRIVATE) && (!memcmp(&virtual_white_list[i].info.addr, peer_addr, BD_ADDR_LEN)) )
                    break;  // already in there...
                
            if (i == MAX_BOND_PEER)
            {
                for (i = 0; i < MAX_BOND_PEER; i++)
                    if (virtual_white_list[i].status == UNUSED)
                    {
                        memcpy(&virtual_white_list[i].info.addr, peer_addr, BD_ADDR_LEN);    // add entry
                        virtual_white_list[i].status = USED_ADDR_PUBLIC_or_PRIVATE;
                        white_list_written++;
                        ret = true;
                        break;
                    }
            }
        }
        else if ( (peer_addr_type == ADDR_RAND) && ((peer_addr->addr[BD_ADDR_LEN - 1] & 0xC0) == SMPM_ADDR_TYPE_PRIV_RESOLV) )
        {
            // Resolvable Random addresses
            for (i = 0; i < MAX_BOND_PEER; i++)
                if ( (virtual_white_list[i].status == USED_ADDR_RAND) && (virtual_white_list[i].info.entry == (bond_info_entry + 1)) )
                    break;  // already in there...
                
            if (i == MAX_BOND_PEER)
            {
                for (i = 0; i < MAX_BOND_PEER; i++)
                    if (virtual_white_list[i].status == UNUSED)
                    {
                        virtual_white_list[i].info.entry = bond_info_entry + 1;    // add entry
                        virtual_white_list[i].status = USED_ADDR_RAND;
                        white_list_written++;
                        ret = true;
                        break;
                    }
            }
        }
    }
        
    return ret;
}


/**
 * @brief       Removes a host to the normal or virtual White List.
 *
 * @details     Removes a host from the White List. If the host has Public Address,
 *              it is removed from the normal White List. If it has a Resolvable
 *              Random Address, it is removed from the virtual White List.
 *
 * @param[in]   peer_addr_type   The BD_ADDR type of the host.
 * @param[in]   peer_addr        The BD_ADDR of the host.
 * @param[in]   bond_info_entry  The index to the storage area for this bonding info.
 *
 * @return      bool
 *
 * @retval      Status of operation.
 *              <ul>
 *                  <li> true if no error occurred; white_list_written will be decreased by 1 upon successful completion
 *                  <li> false if the host could not be removed for some reason
 *              </ul>
 *
 */
bool remove_host_from_white_list(uint8_t peer_addr_type, struct bd_addr const *peer_addr, uint8_t bond_info_entry)
{
    int i;
    bool ret = false;
    
    if (HAS_WHITE_LIST)
    {
        if (ADDR_PUBLIC == peer_addr_type)
        {
            send_white_list_mgt_cmd(false, peer_addr_type, peer_addr);
            ret = true;
        }
    }
    else if (HAS_VIRTUAL_WHITE_LIST)
    {
        // Public or Static Private addresses
        if (   (ADDR_PUBLIC == peer_addr_type)
            || ( (peer_addr_type == ADDR_RAND) && ((peer_addr->addr[5] & SMPM_ADDR_TYPE_STATIC) == SMPM_ADDR_TYPE_STATIC) ) )
        {
            for (i = 0; i < MAX_BOND_PEER; i++)
                if ( (virtual_white_list[i].status == USED_ADDR_PUBLIC_or_PRIVATE) && (!memcmp(&virtual_white_list[i].info.addr, peer_addr, BD_ADDR_LEN)) )
                {
                    virtual_white_list[i].status = UNUSED;      // remove entry
                    white_list_written--;
                    ret = true;
                    break;
                }
        }
        else if ( (peer_addr_type == ADDR_RAND) && ((peer_addr->addr[BD_ADDR_LEN - 1] & 0xC0) == SMPM_ADDR_TYPE_PRIV_RESOLV) )
        {
            // Resolvable Random addresses
            for (i = 0; i < MAX_BOND_PEER; i++)
                if ( (virtual_white_list[i].status == USED_ADDR_RAND) && (virtual_white_list[i].info.entry == (bond_info_entry + 1)) )
                {
                    virtual_white_list[i].status = UNUSED;      // remove entry
                    white_list_written--;
                    ret = true;
                    break;
                }
        }
    }
        
    return ret;
}


/**
 * @brief       Looks up a host with Resolvable Random address in virtual White List.
 *
 * @details     Looks for a host with Resolvable Random address into the virtual White List.
 *
 * @param       bond_info_entry  The index to the storage area for this bonding info.
 *
 * @return      bool
 *
 * @retval      Status of operation.
 *              <ul>
 *                  <li> true if the host has been found or the policy is ADV_ALLOW_SCAN_ANY_CON_ANY
 *                  <li> false if the host wasn't found
 *              </ul>
 *
 * @warning     The caller should only call this function for Resolvable Random addresses.
 *
 */
bool lookup_rand_in_virtual_white_list(uint8_t bond_info_entry)
{
    bool ret = false;
    int i;
    
    if (HAS_VIRTUAL_WHITE_LIST)
    {
        if (virtual_wlist_policy == ADV_ALLOW_SCAN_ANY_CON_ANY)
            ret = true;
        else
        {
            for (i = 0; i < MAX_BOND_PEER; i++)
                if ( (virtual_white_list[i].status == USED_ADDR_RAND) && (virtual_white_list[i].info.entry == (bond_info_entry + 1)) )
                {
                    ret = true;
                    break;
                }
        }
    }
    
    return ret;
}


/**
 * @brief       Looks up a host with Public or Static Random address in virtual White List.
 *
 * @details     Looks for a host with Public or Static Random address into the virtual White List.
 *
 * @param       bond_info_entry  The index to the storage area for this bonding info.
 *
 * @return      bool
 *
 * @retval      Status of operation.
 *              <ul>
 *                  <li> true if the host has been found or the policy is ADV_ALLOW_SCAN_ANY_CON_ANY
 *                  <li> false if the host wasn't found
 *              </ul>
 *
 * @warning     The caller should only call this function for Public or Static Random addresses.
 *
 */
bool lookup_public_in_virtual_white_list(uint8_t peer_addr_type, struct bd_addr const *peer_addr)
{
    bool ret = false;
    int i;
    
    if (HAS_VIRTUAL_WHITE_LIST)
    {
        if (virtual_wlist_policy == ADV_ALLOW_SCAN_ANY_CON_ANY)
            ret = true;
        else
        {
            for (i = 0; i < MAX_BOND_PEER; i++)
                if ( (virtual_white_list[i].status == USED_ADDR_PUBLIC_or_PRIVATE) && (!memcmp(&virtual_white_list[i].info.addr, peer_addr, BD_ADDR_LEN)) )
                {
                    ret = true;
                    break;
                }
        }
    }
    
    return ret;
}


/**
 * @brief       Clears both the normal and the virtual White Lists.
 *
 * @param       void
 *
 * @return      void
 *
 */
void clear_white_list(void)
{
    int i;

    if (HAS_WHITE_LIST)
    {
        struct gapm_white_list_mgt_cmd * req = KE_MSG_ALLOC(GAPM_WHITE_LIST_MGT_CMD, 
                                                            TASK_GAPM, TASK_APP, 
                                                            gapm_white_list_mgt_cmd);

        // Fill in the parameter structure
        req->operation = GAPM_CLEAR_WLIST;
        req->nb = 0;
        ke_msg_send(req);
    }
    else if (HAS_VIRTUAL_WHITE_LIST)
    {
        for (i = 0; i < MAX_BOND_PEER; i++)
            virtual_white_list[i].status = UNUSED;
        virtual_wlist_policy = ADV_ALLOW_SCAN_ANY_CON_ANY;
    }
    
    white_list_written = 0;
}

#endif //(BLE_APP_PRESENT)
/// @} APP
