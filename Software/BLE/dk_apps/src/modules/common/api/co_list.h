/**
 ****************************************************************************************
 *
 * @file co_list.h
 *
 * @brief Common list structures definitions
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */

#ifndef _CO_LIST_H_
#define _CO_LIST_H_

/**
 *****************************************************************************************
 * @defgroup CO_LIST List management
 * @ingroup COMMON
 *
 * @brief  List management.
 *
 * This module contains the list structures and handling functions.
 * @{
 *****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>         // standard definition
#include <stdbool.h>        // boolean definition
#include <stddef.h>         // for NULL and size_t
#include "rwip_config.h"    // stack configuration
#include "compiler.h"       // for __INLINE


/*
 * DEFINES
 ****************************************************************************************
 */
///list type
enum
{
    POOL_LINKED_LIST    = 0x00,
    RING_LINKED_LIST,
    LINK_TYPE_END
};

/// structure of a list element header
struct co_list_hdr
{
    /// Pointer to next co_list_hdr
    struct co_list_hdr *next;
};

/// structure of a list
struct co_list
{
    /// pointer to first element of the list
    struct co_list_hdr *first;
    /// pointer to the last element
    struct co_list_hdr *last;

    #if (KE_PROFILING)
    /// number of element in the list
    uint32_t cnt;
    /// max number of element in the list
    uint32_t maxcnt;
    /// min number of element in the list
    uint32_t mincnt;
    #endif //KE_PROFILING
   uint32_t tx_data_packet;
};


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Initialize a list to defaults values.
 *
 * @param list           Pointer to the list structure.
 ****************************************************************************************
 */
void co_list_init(struct co_list *list);

/**
 ****************************************************************************************
 * @brief Initialize a pool to default values, and initialize the relative free list.
 *
 * @param list           Pointer to the list structure
 * @param pool           Pointer to the pool to be initialized
 * @param elmt_size      Size of one element of the pool
 * @param elmt_cnt       Nb of elements available in the pool
 * @param default_value  Pointer to the default value of each element (may be NULL)
 * @param list_type      Determine if the it is a ring list or not
 *
 ****************************************************************************************
 */
void co_list_pool_init(struct co_list *list,
                       void *pool,
                       size_t elmt_size,
                       uint32_t elmt_cnt,
                       void *default_value,
                       uint8_t list_type);

/**
 ****************************************************************************************
 * @brief Add an element as last on the list.
 *
 * @param list           Pointer to the list structure
 * @param list_hdr       Pointer to the header to add at the end of the list
 *
 ****************************************************************************************
 */
void co_list_push_back(struct co_list *list, struct co_list_hdr *list_hdr);

/**
 ****************************************************************************************
 * @brief Add an element as first on the list.
 *
 * @param list           Pointer to the list structure
 * @param list_hdr       Pointer to the header to add at the beginning of the list
 ****************************************************************************************
 */
void co_list_push_front(struct co_list *list, struct co_list_hdr *list_hdr);

/**
 ****************************************************************************************
 * @brief Extract the first element of the list.
 * @param list           Pointer to the list structure
 * @return The pointer to the element extracted, and NULL if the list is empty.
 ****************************************************************************************
 */
struct co_list_hdr *co_list_pop_front(struct co_list *list);

/**
 ****************************************************************************************
 * @brief Search for a given element in the list, and extract it if found.
 *
 * @param list           Pointer to the list structure
 * @param list_hdr       Pointer to the searched element
 *
 * @return CO_EMPTY if the list is empty, CO_FAIL if the element not found in the list,
 * CO_OK else.
 ****************************************************************************************
 */
void co_list_extract(struct co_list *list, struct co_list_hdr *list_hdr);

/**
 ****************************************************************************************
 * @brief Searched a given element in the list.
 *
 * @param list           Pointer to the list structure
 * @param list_hdr       Pointer to the searched element
 *
 * @return true if the element is found in the list, false otherwise
 ****************************************************************************************
 */
bool co_list_find(struct co_list *list, struct co_list_hdr *list_hdr);

/**
 ****************************************************************************************
 * @brief Merge two lists in a single one.
 *
 * This function appends the list pointed by list2 to the list pointed by list1. Once the
 * merge is done, it empties list2.
 *
 * @param list1    Pointer to the destination list
 * @param list2    Pointer to the list to append to list1
 ****************************************************************************************
 */
void co_list_merge(struct co_list *list1, struct co_list *list2);

/**
 ****************************************************************************************
 * @brief Test if the list is empty.
 * @param list           Pointer to the list structure.
 * @return true if the list is empty, false else otherwise.
 ****************************************************************************************
 */
__INLINE bool co_list_is_empty(const struct co_list *const list)
{
    bool listempty;
    listempty = (list->first == NULL);
    return (listempty);
}

/**
 ****************************************************************************************
 * @brief Pick the first element from the list without removing it.
 *
 * @param list           Pointer to the list structure.
 *
 * @return First element address. Returns NULL pointer if the list is empty.
 ****************************************************************************************
 */
__INLINE struct co_list_hdr *co_list_pick(const struct co_list *const list)
{
    return(list->first);
}


/**
 ****************************************************************************************
 * @brief Return following element of a list element.
 *
 * @param list_hdr     Pointer to the list element.
 *
 * @return The pointer to the next element.
 ****************************************************************************************
 */
__INLINE struct co_list_hdr *co_list_next(const struct co_list_hdr *const list_hdr)
{
    return(list_hdr->next);
}

/// @} CO_LIST
#endif // _CO_LIST_H_
