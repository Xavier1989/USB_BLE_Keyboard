/**
 ****************************************************************************************
 *
 * @file da14580_config.h
 *
 * @brief HID Keyboard compile configuration file.
 *
 * Copyright (C) 2014. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

#ifndef DA14580_CONFIG_H_
#define DA14580_CONFIG_H_

#include "da14580_stack_config.h"


/**
 ******************************************************************************
 * Profiles - Stack configuration section
 ******************************************************************************
 */
 
/* FullEmbedded - FullHosted */
#define CFG_APP  

/* Profiles */
#define CFG_PRF_BASS
#define CFG_PRF_DISS
#define CFG_PRF_HOGPD
#define CFG_PRF_SPOTAR
#ifdef CFG_PRF_SPOTAR
    #define CFG_SPOTAR_SPI_DISABLE                  //SPI support is disabled
    #define CFG_SPOTAR_UPDATE_DISABLE               //SUOTA is not included
    #define CFG_APP_SPOTAR      1
    #define SPOTAR_PATCH_AREA   1                   //Placed in the RetRAM when SPOTAR_PATCH_AREA is 0 and in SYSRAM when 1
#endif
        
/* Misc */
#define CFG_NVDS 
#define CFG_APP_SEC
#define CFG_CHNL_ASSESS
#define CFG_LUT_PATCH
#define CFG_APP_KEYBOARD 

/* Maximum user connections */
#define BLE_CONNECTION_MAX_USER 1



/**
 ******************************************************************************
 * Clock & Boot section
 ******************************************************************************
 */
 
/* Low power clock selection */
#define CFG_LP_CLK              0x00                //0x00: XTAL32, 0xAA: RCX20, 0xFF: Select from OTP Header

/* Fab Calibration - Must be defined for calibrated devices */
#define CFG_CALIBRATED_AT_FAB

/* Application boot from OTP memory - Bootloader copies OTP Header to sysRAM */
#undef APP_BOOT_FROM_OTP

/* NVDS struture is padded with 0 - NVDS struture data must be written in OTP in production procedure */
#undef READ_NVDS_STRUCT_FROM_OTP


/**
 ******************************************************************************
 * Debug - Log section
 ******************************************************************************
 */
 
/* Build for OTP or JTAG */
#define DEVELOPMENT_DEBUG     	0//1                   //0: code at OTP, 1: code via JTAG

/* Log HEAP usage */
#define LOG_MEM_USAGE           0                   //0: no logging, 1: logging is active

/* Debug output in Production mode (DEVELOPMENT_DEBUG == 0) */
#define nPRODUCTION_DEBUG_OUTPUT

/* UART Tx pin to be used for Debug output in Production mode */
#define PRODUCTION_DEBUG_PORT   UART_TX_GPIO_PORT
#define PRODUCTION_DEBUG_PIN    UART_TX_GPIO_PIN

/* UART pins (set to -1, if not used) */
#define UART_RX_PORT            0
#define UART_RX_PIN             5
#define UART_TX_PORT            0
#define UART_TX_PIN             4
/* UART GPIO definitions (Must correspond to the above definitions. Remove if not used!) */
#define UART_RX_GPIO_PORT       GPIO_PORT_0
#define UART_RX_GPIO_PIN        GPIO_PIN_5
#define UART_TX_GPIO_PORT       GPIO_PORT_0
#define UART_TX_GPIO_PIN        GPIO_PIN_4


#define UART2_RX_GPIO_PORT       GPIO_PORT_0
#define UART2_RX_GPIO_PIN        GPIO_PIN_6
#define UART2_TX_GPIO_PORT       GPIO_PORT_0
#define UART2_TX_GPIO_PIN        GPIO_PIN_7
 
/* arch_printf() and arch_puts() */
#define nCFG_PRINTF
#define CFG_PRINTF

 

#ifdef CFG_PRINTF
#define HAS_PRINTF              1
#else
#define HAS_PRINTF              0
#endif

#define COMMUNICATE_UART2 

/**
 ******************************************************************************
 * Sleep & Watchdog section
 ******************************************************************************
 */
 
/* Sleep modes */
#undef CFG_EXT_SLEEP  
//#define CFG_DEEP_SLEEP  

/* Watchdog */
#define nCFG_WDOG 



/**
 ******************************************************************************
 * Multibond section
 ******************************************************************************
 */
 
/* Bonding to multiple devices */
//#define CFG_MULTI_BOND

/* Load all bond info into RetRAM at power-up to eliminate subsequent Reads and reduce power consuptiom */
#define MBOND_LOAD_INFO_AT_INIT 0                   // 0, do not load - 1, do load



/**
 ******************************************************************************
 * Battery configuration section
 ******************************************************************************
 */
 
/* Battery Type (if any) */
#define USED_BATTERY_TYPE                   BATT_AAA
#define BATTERY_LEVEL_POLLING_PERIOD        (10000)
#define BATTERY_ALERT_AT_PERCENTAGE_LEFT    (25)    // Will set custom battery alert level and also enable on/off behaviour with hysteresis
#define CUSTOM_BATTERY_LEVEL_ALERT_LED_HANDLING




/**
 ******************************************************************************
 * GPIO Drv configuration section
 ******************************************************************************
 */
 
/* GPIO driver - Pin allocation checks */
#define GPIO_DRV_PIN_ALLOC_MON_DISABLED             //Checks are off

/* GPIO driver - GPIO_INTx handling */
#define GPIO_DRV_IRQ_HANDLING_DISABLED              //IRQs are off



/**
 ******************************************************************************
 * Misc settings section
 ******************************************************************************
 */
 
/* HIDS instances: 1 */
#define USE_ONE_HIDS_INSTANCE

/* BASS instances: 1 */
#define USE_ONE_BAS_INSTANCE



/**
 ******************************************************************************
 * RW data placement configuration section
 ******************************************************************************
 */
 
/*
 * Set the arrays used by the application to be placed in .constdata section.
 * If not defined, the arrays will be put in the ".data" section which can optionally be
 * compressed and placed elsewhere.
 * WARNING: If not defined then the following options are available:
 *     - uncompress data in RetRAM once at startup
 *     - uncompress data in SysRAM once at startup and use Extended Sleep (done automatically if EXT_SLEEP_SETUP is used)
 *     - uncompress data in SysRAM at startup and after every Deep Sleep wakeup
 */
#define ARRAYS_AS_CONST_DATA

#if defined(ARRAYS_AS_CONST_DATA)
# define KBD_TYPE_QUALIFIER  const
# define KBD_ARRAY_ATTRIBUTE  
#else
# define KBD_TYPE_QUALIFIER  
# define KBD_ARRAY_ATTRIBUTE __attribute__((section(".data")))
#endif

/* Select where the .data section will be placed and whether it will be compressed */
#define DATA_UNCOMPRESSED       1
#define DATA_COMPRESSED_SYSRAM  2       // only in Extended Sleep
#define DATA_COMPRESSED_RETRAM  3       // re-init will be enforced

#if defined(ARRAYS_AS_CONST_DATA)
#define DATA_SECTION_SEL        DATA_UNCOMPRESSED
#else
#define DATA_SECTION_SEL        DATA_COMPRESSED_RETRAM
#endif



/**
 ******************************************************************************
 * Memory maps section (Scatterfile)
 ******************************************************************************
 */

/* Memory map */
#define USE_MEMORY_MAP          DEEP_SLEEP_SETUP

/* Descriptors' reinitilization */
#if (defined(CFG_DEEP_SLEEP) && (DATA_SECTION_SEL == DATA_COMPRESSED_RETRAM) )
# define REINIT_DESCRIPT_BUF    1                   //0: keep in RetRAM, 1: re-init is required (set to 0 when Extended Sleep is used)
#else
# define REINIT_DESCRIPT_BUF    0
#endif

/* HEAP sizes */
#define ENV_HEAP_SZ             352

#if !defined(CFG_PRF_SPOTAR)
# define DB_HEAP_SZ             1800
#else
# define DB_HEAP_SZ             (2232)
#endif

#define MSG_HEAP_SZ             524
#define NON_RET_HEAP_SZ         1024



#endif // DA14580_CONFIG_H_
