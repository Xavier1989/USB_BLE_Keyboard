/**
 ****************************************************************************************
 *
 * @file boot.h
 *
 * @brief This file contains the declarations of the boot related variables.
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 * $Rev: 7039 $
 *
 ****************************************************************************************
 */

#ifndef _BOOT_H_
#define _BOOT_H_

#if 0 //@wik, link errors if not using  .sct file but if using .sct file the program start incorrect (no sp ??)
/// Length of the code
extern const uint32_t Image$$EXEC_RAM_TEXT$$Length[];
#define CODE_LENGTH        ((uint32_t)Image$$EXEC_RAM_TEXT$$Length)

/// Length of the RW data
extern const uint32_t Image$$RAM_DATA$$Length[];
#define DATA_LENGTH        ((uint32_t)Image$$RAM_DATA$$Length)

extern const uint32_t Image$$EXT_FLASH$$Base[];
#define EXT_FLASH_BASE      ((uint32_t)Image$$EXT_FLASH$$Base)
/// Unloaded RAM area base address
extern const uint32_t Image$$RAM_UNLOADED$$Base[];
#define RAM_UNLOADED_BASE   ((uint32_t)Image$$RAM_UNLOADED$$Base)

/// Stack base address
extern const uint32_t Image$$RAM_STACK_UNUSED$$Base[];
#define STACK_BASE_UNUSED   ((uint32_t)Image$$RAM_STACK_UNUSED$$Base)
extern const uint32_t Image$$RAM_STACK_UNUSED$$ZI$$Length[];
#define STACK_LEN_UNUSED   ((uint32_t)Image$$RAM_STACK_UNUSED$$ZI$$Length)

extern const uint32_t Image$$RAM_STACK_SVC$$Base[] ;
#define STACK_BASE_SVC   ((uint32_t)Image$$RAM_STACK_SVC$$Base)
extern const uint32_t Image$$RAM_STACK_SVC$$ZI$$Length[];
#define STACK_LEN_SVC  ((uint32_t)Image$$RAM_STACK_SVC$$ZI$$Length)

extern const uint32_t Image$$RAM_STACK_IRQ$$Base[];
#define STACK_BASE_IRQ  ((uint32_t)Image$$RAM_STACK_IRQ$$Base)
extern const uint32_t Image$$RAM_STACK_IRQ$$ZI$$Length[];
#define STACK_LEN_IRQ   ((uint32_t)Image$$RAM_STACK_IRQ$$ZI$$Length)

extern const uint32_t Image$$RAM_STACK_FIQ$$Base[];
#define STACK_BASE_FIQ   ((uint32_t)Image$$RAM_STACK_FIQ$$Base)
extern const uint32_t Image$$RAM_STACK_FIQ$$ZI$$Length[];
#define STACK_LEN_FIQ   ((uint32_t)Image$$RAM_STACK_FIQ$$ZI$$Length)

#define BOOT_PATTERN_UNUSED   0xAA      // Pattern to fill UNUSED stack
#define BOOT_PATTERN_SVC      0xBB      // Pattern to fill SVC stack
#define BOOT_PATTERN_IRQ      0xCC      // Pattern to fill IRQ stack
#define BOOT_PATTERN_FIQ      0xDD      // Pattern to fill FIQ stack
#endif //@wik, 0

#endif // _BOOT_H_
