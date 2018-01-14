/**
 ****************************************************************************************
 *
 * @file rwip_config.h
 *
 * @brief Configuration of the RW IP SW
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */

#ifndef RWIP_CONFIG_H_
#define RWIP_CONFIG_H_
/**
 ****************************************************************************************
 * @addtogroup ROOT
 * @{
 *
 *  Information about RW SW IP options and flags
 *
 *        BT_DUAL_MODE             BT/BLE Dual Mode
 *        BT_STD_MODE              BT Only
 *        BLE_STD_MODE             BLE Only
 *
 *        RW_DM_SUPPORT            Dual mode is supported
 *        RW_BLE_SUPPORT           Configured as BLE only
 *
 *        BT_EMB_PRESENT           BT controller exists
 *        BLE_EMB_PRESENT          BLE controller exists
 *        BLE_HOST_PRESENT         BLE host exists
 *
 * @name RW Stack Configuration
 * @{
 ****************************************************************************************
 */

/*
 * DEFINES
 ****************************************************************************************
 */

/******************************************************************************************/
/* --------------------------   GENERAL SETUP       --------------------------------------*/
/******************************************************************************************/

/// Flag indicating if stack is compiled in dual or single mode
#if defined(CFG_BT)
    #define BLE_STD_MODE                     0
    #if defined(CFG_BLE)
        #define BT_DUAL_MODE                 1
        #define BT_STD_MODE                  0
    #else // CFG_BLE
        #define BT_DUAL_MODE                 0
        #define BT_STD_MODE                  1
    #endif // CFG_BLE
#elif defined(CFG_BLE)
    #define BT_DUAL_MODE                     0
    #define BT_STD_MODE                      0
    #define BLE_STD_MODE                     1
#endif // CFG_BT

/// Flag indicating if Dual mode is supported
#define RW_DM_SUPPORT         BT_DUAL_MODE

/// Flag indicating if BLE handles main parts of the stack
#define RW_BLE_SUPPORT        BLE_STD_MODE

/// Flag indicating if stack is compiled for BLE1.0 HW or later
#if defined (CFG_BLECORE_11)
    #define BLE11_HW                    1
    #define BLE12_HW                    0
#else // defined (CFG_BLECORE_11)
    #define BLE11_HW                    0
    #define BLE12_HW                    1
#endif // defined (CFG_BLECORE_11)
/******************************************************************************************/
/* -------------------------   STACK PARTITIONING      -----------------------------------*/
/******************************************************************************************/

#if (BT_DUAL_MODE)
    #define BT_EMB_PRESENT              1
    #define BLE_EMB_PRESENT             1
    #define BLE_HOST_PRESENT            0
    #define BLE_APP_PRESENT             0
#elif (BT_STD_MODE)
    #define BT_EMB_PRESENT              1
    #define BLE_EMB_PRESENT             0
    #define BLE_HOST_PRESENT            0
    #define BLE_APP_PRESENT             0
#elif (BLE_STD_MODE)
    #define BT_EMB_PRESENT              0
    #if defined(CFG_EMB)
        #define BLE_EMB_PRESENT         1
    #else
        #define BLE_EMB_PRESENT         0
    #endif //CFG_EMB
    #if defined(CFG_HOST)
        #define BLE_HOST_PRESENT        1
    #else
        #define BLE_HOST_PRESENT        0
    #endif //CFG_HOST
    #if defined(CFG_APP)
        #define BLE_APP_PRESENT         1
    #else
        #define BLE_APP_PRESENT         0
    #endif //CFG_APP
#endif // BT_DUAL_MODE / BT_STD_MODE / BLE_STD_MODE

/******************************************************************************************/
/* -------------------------   INTERFACES DEFINITIONS      -------------------------------*/
/******************************************************************************************/

/// Generic Transport Layer
#if defined(CFG_GTL)
#define GTL_ITF           1
#else // defined(CFG_GTL)
#define GTL_ITF           0
#endif // defined(CFG_GTL)
/// Host Controller Interface (Controller side)
#define HCIC_ITF        (!BLE_HOST_PRESENT)
/// Host Controller Interface (Host side)
#define HCIH_ITF        (BLE_HOST_PRESENT && !BLE_EMB_PRESENT)

#define TL_TASK_SIZE      GTL_ITF + HCIC_ITF + HCIH_ITF


/******************************************************************************************/
/* --------------------------   BLE COMMON DEFINITIONS      ------------------------------*/
/******************************************************************************************/
/// Kernel Heap memory sized reserved for allocate dynamically connection environment
#define KE_HEAP_MEM_RESERVED        (4)

#if defined(CFG_BLE)
/// Application role definitions

#define BLE_BROADCASTER   ((CFG_BROADCASTER) || (CFG_ALLROLES))
#define BLE_OBSERVER      ((CFG_OBSERVER)    || (CFG_ALLROLES))
#define BLE_PERIPHERAL    ((CFG_PERIPHERAL)  || (CFG_ALLROLES))
#define BLE_CENTRAL       ((CFG_CENTRAL)     || (CFG_ALLROLES))

#if (!BLE_BROADCASTER) && (!BLE_OBSERVER) && (!BLE_PERIPHERAL) && (!BLE_CENTRAL)
    #error "No application role defined"
#endif /* #if (!BLE_BROADCASTER) && (!BLE_OBSERVER) && (!BLE_PERIPHERAL) && (!BLE_CENTRAL) */


/// Maximum number of simultaneous connections
#if (BLE_CENTRAL)
    #define BLE_CONNECTION_MAX          CFG_CON
#elif (BLE_PERIPHERAL)
    #define BLE_CONNECTION_MAX          1
#else
    #define BLE_CONNECTION_MAX          1
#endif /* #if (BLE_CENTRAL) */


/// Number of tx data buffers
#if ( BLE_CONNECTION_MAX == 1)
#if (BLE_CENTRAL || BLE_PERIPHERAL)
#define BLE_TX_BUFFER_DATA      5
#else
#define BLE_TX_BUFFER_DATA      0
#endif // (BLE_CENTRAL || BLE_PERIPHERAL)
#else
#define BLE_TX_BUFFER_DATA     (BLE_CONNECTION_MAX * 3)
#endif //  BLE_CONNECTION_MAX == 1

#if (BLE_CENTRAL || BLE_PERIPHERAL)
/// Number of tx advertising buffers
#define BLE_TX_BUFFER_ADV       3
/// Number of tx control buffers
#define BLE_TX_BUFFER_CNTL      BLE_CONNECTION_MAX
#else
#if (BLE_BROADCASTER)
/// Number of tx advertising buffers
#define BLE_TX_BUFFER_ADV       2
/// Number of tx control buffers
#define BLE_TX_BUFFER_CNTL      0
#else
/// Number of tx advertising buffers
#define BLE_TX_BUFFER_ADV       1
/// Number of tx control buffers
#define BLE_TX_BUFFER_CNTL      0
#endif // BLE_BROADCASTER
#endif // (BLE_CENTRAL || BLE_PERIPHERAL)

/// Total number of elements in the TX buffer pool
#define BLE_TX_BUFFER_CNT      (BLE_TX_BUFFER_DATA + BLE_TX_BUFFER_CNTL + BLE_TX_BUFFER_ADV)

/// Number of receive buffers in the RX ring. This number defines the interrupt
/// rate during a connection event. An interrupt is asserted every BLE_RX_BUFFER_CNT/2
/// reception. This number has an impact on the size of the exchange memory. This number
/// may have to be increased when CPU is very slow to free the received data, in order not
/// to overflow the RX ring of buffers.
#if (BLE_CENTRAL || BLE_PERIPHERAL)
    #define BLE_RX_BUFFER_CNT           8
#elif (BLE_BROADCASTER)
    #define BLE_RX_BUFFER_CNT           1
#else
    #define BLE_RX_BUFFER_CNT           4
#endif // BLE_CENTRAL || BLE_PERIPHERAL


/// Max advertising reports before sending the info to the host
#define BLE_ADV_REPORTS_MAX             1

/// Use of security manager block
#define RW_BLE_USE_CRYPT  CFG_SECURITY_ON // TODO [modularity] [KE]
#endif //defined(CFG_BLE)


/******************************************************************************************/
/* --------------------------      WDOG SETUP        -------------------------------------*/
/******************************************************************************************/

/// Watchdog enable/disable
#if defined(CFG_WDOG)
#define USE_WDOG            1
#else
#define USE_WDOG            0
#endif //CFG_WDOG


/******************************************************************************************/
/* -------------------------   BLE APPLICATION SETTINGS      -----------------------------*/
/******************************************************************************************/

/// Security Application
#if defined(CFG_APP_SEC)
#define BLE_APP_SEC          1
#else // defined(CFG_APP_SEC)
#define BLE_APP_SEC          0
#endif // defined(CFG_APP_SEC)

/// Health Thermometer Application
#if defined(CFG_APP_HT)
#define BLE_APP_HT           1
#else // defined(CFG_APP_HT)
#define BLE_APP_HT           0
#endif // defined(CFG_APP_HT)

/// Nebulizer Application
#if defined(CFG_APP_NEB)
#define BLE_APP_NEB          1
#else // defined(CFG_APP_NEB)
#define BLE_APP_NEB          0
#endif // defined(CFG_APP_NEB)

/// Keyboard Application
#if defined(CFG_APP_KEYBOARD)
#define BLE_APP_KEYBOARD          1
#else // defined(CFG_APP_NEB)
#define BLE_APP_KEYBOARD          0
#endif // defined(CFG_APP_NEB)

/// Keyboard Test Application
#if defined(CFG_APP_KEYBOARD_TESTER)
#define BLE_APP_KEYBOARD_TESTER   1
#else // defined(CFG_APP_NEB)
#define BLE_APP_KEYBOARD_TESTER   0
#endif // defined(CFG_APP_NEB)

/// Keyboard Test Application
#if defined(CFG_APP_PROXR)
#define BLE_APP_PROXR   1
#else // defined(CFG_APP_PROXR)
#define BLE_APP_PROXR   0
#endif // defined(CFG_APP_PROXR)

/// SPOTA Receiver Test application
#if defined(CFG_APP_SPOTAR)
#define BLE_APP_SPOTAR   1
#else // defined(CFG_APP_SPOTAR)
#define BLE_APP_SPOTAR   0
#endif // defined(CFG_APP_SPOTAR)

/// Audio Stream Test Application
#if defined(CFG_APP_STREAM)
#define BLE_APP_STREAM   1
#else // defined(CFG_APP_STREAM)
#define BLE_APP_STREAM   0
#endif // defined(CFG_APP_STREAM)

/// Smart Tag Application
#if defined(CFG_APP_SMARTTAG)
#define BLE_APP_SMARTTAG   1
#else // defined(CFG_APP_SMARTTAG)
#define BLE_APP_SMARTTAG   0
#endif // defined(CFG_APP_SMARTTAG)

/// Dialog Beacon Application
#if defined(CFG_APP_DBEACON)
#define BLE_APP_DBEACON   1
#else // defined(CFG_APP_DBEACON)
#define BLE_APP_DBEACON   0
#endif // defined(CFG_APP_DBEACON)

/// Max throughput evaluation app - Central Role
#if defined(CFG_APP_THROUGHPUT_CENTRAL)
#define BLE_APP_THROUGHPUT_CENTRAL   1
#else // defined(CFG_APP_THROUGHPUT_CENTRAL)
#define BLE_APP_THROUGHPUT_CENTRAL   0
#endif // defined(CFG_APP_THROUGHPUT_CENTRAL)

/// Max throughput evaluation app -Peripheral Role
#if defined(CFG_APP_THROUGHPUT_PERIPHERAL)
#define BLE_APP_THROUGHPUT_PERIPHERAL   1
#else // defined(CFG_APP_THROUGHPUT_CENTRAL)
#define BLE_APP_THROUGHPUT_PERIPHERAL   0
#endif // defined(CFG_APP_THROUGHPUT_CENTRAL)

// A4WP PTU FH application
#if defined(CFG_APP_PTU_FH)
    #define BLE_APP_PTU         1
#else
    #define BLE_APP_PTU         0
#endif

// A4WP PRU FH application
#if defined(CFG_APP_PRU_FH)
    #define BLE_APP_PRU         1
#else
    #define BLE_APP_PRU         0
#endif

#if defined(CFG_APP_SENSORTAG) 
#define BLE_APP_SENSORTAG   1
#else // defined(CFG_APP_SENSORTAG)
#define BLE_APP_SENSORTAG   0
#endif // defined(CFG_APP_SENSORTAG)

/// Alternate pairing mechanism
#if defined(CFG_MULTI_BOND)
#define HAS_MULTI_BOND          1
#else // defined(CFG_APP_NEB)
#define HAS_MULTI_BOND          0
#endif // defined(CFG_MULTI_BOND)

/// Inegrated processor host application with GTL iface
#if defined(CFG_INTEGRATED_HOST_GTL)
#define BLE_INTEGRATED_HOST_GTL          1
#else // defined(CFG_INTEGRATED_HOST_GTL)
#define BLE_INTEGRATED_HOST_GTL          0
#endif // defined(CFG_INTEGRATED_HOST_GTL)

/// Accelerometer Application
#define BLE_APP_ACCEL        0

#if defined(CFG_PRF_ACCEL) || defined(CFG_PRF_PXPR) || defined(CFG_APP_KEYBOARD) || defined(CFG_PRF_STREAMDATAH) || defined(CFG_PRF_SPOTAR)
#define BLE_APP_SLAVE         1
#else
#define BLE_APP_SLAVE         0
#endif
/// Accelerometer Application
#ifndef __DA14581__
#define BLE_APP_TASK_SIZE    BLE_APP_PRESENT + BLE_APP_SEC + BLE_APP_HT + BLE_APP_NEB +\
                             BLE_APP_HID + BLE_APP_DIS + BLE_APP_BATT + BLE_APP_ACCEL
#else
#define BLE_APP_TASK_SIZE    BLE_APP_PRESENT
#endif

#if defined(CFG_APP_ANC)
#define BLE_APP_ANC 1
#else // defined(CFG_APP_ANC)
#define BLE_APP_ANC 0
#endif // defined(CFG_APP_ANC)

/******************************************************************************************/
/* --------------------------   DISPLAY SETUP        -------------------------------------*/
/******************************************************************************************/

/// Number of tasks for DISPLAY module
#define DISPLAY_TASK_SIZE    1

/// Display controller enable/disable
#if defined(CFG_DISPLAY)
#define DISPLAY_SUPPORT      1
#else
#define DISPLAY_SUPPORT      0
#endif //CFG_DISPLAY


/******************************************************************************************/
/* --------------------------      RTC SETUP         -------------------------------------*/
/******************************************************************************************/

/// RTC enable/disable
#if defined(CFG_RTC)
#define RTC_SUPPORT      1
#else
#define RTC_SUPPORT      0
#endif //CFG_DISPLAY


/******************************************************************************************/
/* -------------------------   DEEP SLEEP SETUP      -------------------------------------*/
/******************************************************************************************/

/// DEEP SLEEP enable
#define DEEP_SLEEP                              1
/// Use 32K Hz Clock if set to 1 else 32,768k is used
#define HZ32000                                     0


/******************************************************************************************/
/* -------------------------    PROCESSOR SETUP      -------------------------------------*/
/******************************************************************************************/

/// 8 BIT processor
#define PROC_8BITS                        0

/******************************************************************************************/
/* --------------------------   RADIO SETUP       ----------------------------------------*/
/******************************************************************************************/

/// Power control features
#define RF_TXPWR                            1
/// Class of device
#define RF_CLASS1                           0

/******************************************************************************************/
/* -------------------------   COEXISTENCE SETUP      ------------------------------------*/
/******************************************************************************************/

/// WLAN Coexistence
#define RW_WLAN_COEX                     (defined(CFG_WLAN_COEX))
///WLAN test mode
#if defined(CFG_WLAN_COEX)
    #define RW_WLAN_COEX_TEST            (defined(CFG_WLAN_COEX_TEST))
#else
    #define RW_WLAN_COEX_TEST            0
#endif // defined(CFG_WLAN_COEX)

/******************************************************************************************/
/* -------------------------   CHANNEL ASSESSMENT SETUP      -----------------------------*/
/******************************************************************************************/

/// Channel Assessment
#if (defined(CFG_CHNL_ASSESS) && BLE_CENTRAL)
    #define BLE_CHNL_ASSESS        (1)
#else
    #define BLE_CHNL_ASSESS        (0)
#endif //(defined(CFG_CHNL_ASSESS) && BLE_CENTRAL)

/******************************************************************************************/
/* --------------------------   DEBUG SETUP       ----------------------------------------*/
/******************************************************************************************/

/// Number of tasks for DEBUG module
#define DEBUG_TASK_SIZE                 1

/// Flag indicating if tester emulator is available or not
#if defined(CFG_TESTER)
    #define RW_TESTER                   1
#else
    #define RW_TESTER                   0
#endif // defined (CFG_TESTER)

/// Flag indicating if debug mode is activated or not
#if defined(CFG_DBG)
    #define RW_DEBUG                        1
#if (BLE_EMB_PRESENT || BT_EMB_PRESENT)
    #define RW_SWDIAG                       1
#else
    #define RW_SWDIAG                       0
#endif
    #define KE_PROFILING                    1
#else
    #define RW_DEBUG                        0
    #define RW_SWDIAG                       0
    #define KE_PROFILING                    0
#endif /* CFG_DBG */

/// Flag indicating if Read/Write memory commands are supported or not
#if defined(CFG_DBG_MEM)
    #define RW_DEBUG_MEM               1
#else //CFG_DBG_MEM
    #define RW_DEBUG_MEM               0
#endif //CFG_DBG_MEM

/// Flag indicating if Flash debug commands are supported or not
#if defined(CFG_DBG_FLASH)
    #define RW_DEBUG_FLASH                  1
#else //CFG_DBG_FLASH
    #define RW_DEBUG_FLASH                  0
#endif //CFG_DBG_FLASH

/// Flag indicating if NVDS feature is supported or not
#if defined(CFG_DBG_NVDS)
    #define RW_DEBUG_NVDS                   1
#else //CFG_DBG_NVDS
    #define RW_DEBUG_NVDS                   0
#endif //CFG_DBG_NVDS

/// Flag indicating if CPU stack profiling commands are supported or not
#if defined(CFG_DBG_STACK_PROF)
    #define RW_DEBUG_STACK_PROF             1
#else
    #define RW_DEBUG_STACK_PROF             0
#endif // defined (CFG_DBG_STACK_PROF)

/// Debug printing
#if (RW_DEBUG)
    #define WARNING(P)                      dbg_warning P
#else
    #define WARNING(P)
#endif //RW_DEBUG

/// Modem back to back setup
#define MODEM2MODEM                          0
/// Special clock testing
#define CLK_WRAPPING                         0

/******************************************************************************************/
/* --------------------------      NVDS SETUP       --------------------------------------*/
/******************************************************************************************/

/// Flag indicating if NVDS feature is supported or not
#if defined(CFG_NVDS)
    #define NVDS_SUPPORT                    1
#else //CFG_DBG_NVDS
    #define NVDS_SUPPORT                    0
#endif //CFG_DBG_NVDS

/******************************************************************************************/
/* --------------------------      MISC SETUP       --------------------------------------*/
/******************************************************************************************/
/// Manufacturer: RivieraWaves SAS
#define RW_COMP_ID                           0x0060


/******************************************************************************************/
/* -------------------------   BT / BLE / BLE HL CONFIG    -------------------------------*/
/******************************************************************************************/

#if (BT_EMB_PRESENT)
#include "rwbt_config.h"    // bt stack configuration
#endif //BT_EMB_PRESENT

#if (BLE_EMB_PRESENT) || (BLE_HOST_PRESENT)
#include "rwble_config.h"   // ble stack configuration
#endif //BLE_EMB_PRESENT

#if (BLE_HOST_PRESENT)
#include "rwble_hl_config.h"  // ble Host stack configuration
#endif //BLE_HOST_PRESENT



/******************************************************************************************/
/* -------------------------   KERNEL SETUP          -------------------------------------*/
/******************************************************************************************/

/// Flag indicating Kernel is supported
#define KE_SUPPORT  (BLE_EMB_PRESENT || BT_EMB_PRESENT || BLE_HOST_PRESENT || BLE_APP_PRESENT)


/// Event types definition
enum KE_EVENT_TYPE
{
    #if DISPLAY_SUPPORT
    KE_EVENT_DISPLAY         ,
    #endif //DISPLAY_SUPPORT

    #if RTC_SUPPORT
    KE_EVENT_RTC_1S_TICK     ,
    #endif //RTC_SUPPORT

    #if BLE_EMB_PRESENT
    KE_EVENT_BLE_CRYPT       ,
    #endif //BLE_EMB_PRESENT

    KE_EVENT_KE_MESSAGE      ,
    KE_EVENT_KE_TIMER        ,

    #if (GTL_ITF)
    KE_EVENT_GTL_TX_DONE     ,
    #endif //(GTL_ITF)

    #if HCIC_ITF || HCIH_ITF
    KE_EVENT_HCI_TX_DONE     ,
    #endif //HCIC_ITF || HCIH_ITF


    #if BLE_EMB_PRESENT
    KE_EVENT_BLE_EVT_END     ,
    KE_EVENT_BLE_RX          ,
    KE_EVENT_BLE_EVT_START   ,
    #endif //BLE_EMB_PRESENT

    KE_EVENT_MAX             ,
};

/// Tasks types definition
enum KE_TASK_TYPE
{
    TASK_NONE = 0xFF,

    // Link Layer Tasks
    TASK_LLM          = 0   ,
    TASK_LLC          = 1   ,
    TASK_LLD          = 2   ,
    TASK_DBG          = 3   ,

    TASK_L2CM         = 4   ,
    TASK_L2CC         = 5   ,
    TASK_SMPM         = 6   ,
    TASK_SMPC         = 7   ,
    TASK_ATTM         = 8   ,   // Attribute Protocol Manager Task
    TASK_ATTC         = 9   ,   // Attribute Protocol Client Task
		
    TASK_ATTS         = 10  ,   // Attribute Protocol Server Task
    TASK_GATTM        = 11  ,   // Generic Attribute Profile Manager Task
    TASK_GATTC        = 12  ,   // Generic Attribute Profile Controller Task
    TASK_GAPM         = 13  ,   // Generic Access Profile Manager
    TASK_GAPC         = 14  ,   // Generic Access Profile Controller
    
    TASK_PROXM        = 15  ,   // Proximity Monitor Task
    TASK_PROXR        = 16  ,   // Proximity Reporter Task
    TASK_FINDL        = 17  ,   // Find Me Locator Task
    TASK_FINDT        = 18  ,   // Find Me Target Task
    TASK_HTPC         = 19  ,   // Health Thermometer Collector Task
    TASK_HTPT         = 20  ,   // Health Thermometer Sensor Task
    TASK_ACCEL        = 21  ,   // Accelerometer Sensor Task
    TASK_BLPS         = 22  ,   // Blood Pressure Sensor Task
    TASK_BLPC         = 23  ,   // Blood Pressure Collector Task
    TASK_HRPS         = 24  ,   // Heart Rate Sensor Task
    TASK_HRPC         = 25  ,   // Heart Rate Collector Task
    TASK_TIPS         = 26  ,   // Time Server Task
    TASK_TIPC         = 27  ,   // Time Client Task
    TASK_DISS         = 28  ,   // Device Information Service Server Task
    TASK_DISC         = 29  ,   // Device Information Service Client Task
    TASK_SCPPS        = 30  ,   // Scan Parameter Profile Server Task
    TASK_SCPPC        = 31  ,   // Scan Parameter Profile Client Task
    TASK_BASS         = 32  ,   // Battery Service Server Task
    TASK_BASC         = 33  ,   // Battery Service Client Task
    TASK_HOGPD        = 34  ,   // HID Device Task
    TASK_HOGPBH       = 35  ,   // HID Boot Host Task
    TASK_HOGPRH       = 36  ,   // HID Report Host Task
    TASK_GLPS         = 37  ,   // Glucose Profile Sensor Task
    TASK_GLPC         = 38  ,   // Glucose Profile Collector Task
    TASK_NBPS         = 39  ,   // Nebulizer Profile Server Task
    TASK_NBPC         = 40  ,   // Nebulizer Profile Client Task
    TASK_RSCPS        = 41  ,   // Running Speed and Cadence Profile Server Task
    TASK_RSCPC        = 42  ,   // Running Speed and Cadence Profile Collector Task
    TASK_CSCPS        = 43  ,   // Cycling Speed and Cadence Profile Server Task
    TASK_CSCPC        = 44  ,   // Cycling Speed and Cadence Profile Client Task
    TASK_ANPS         = 45  ,   // Alert Notification Profile Server Task
    TASK_ANPC         = 46  ,   // Alert Notification Profile Client Task
    TASK_PASPS        = 47  ,   // Phone Alert Status Profile Server Task
    TASK_PASPC        = 48  ,   // Phone Alert Status Profile Client Task

    TASK_LANS         = 49  ,   // Location and Navigation Profile Server Task
    TASK_APP          = 50  ,   // Do not Alter. 

    TASK_LANC         = 51  ,   // Location and Navigation Profile Client Task

    TASK_CPPS         = 52  ,   // Cycling Power Profile Server Task
    TASK_CPPC         = 53  ,   // Cycling Power Profile Client Task
    
    // Start of conditionally assigned task types
    
#if (BLE_SAMPLE128)
    TASK_SAMPLE128    ,   // Sample128 Task
#endif

#if (BLE_SPOTA_RECEIVER)
    TASK_SPOTAR       ,   // SPOTA Receiver task
#endif

#if (BLE_STREAMDATA_DEVICE)
    TASK_STREAMDATAD  ,   // Stream Data Device Server task
#endif

#if (BLE_STREAMDATA_HOST)
    TASK_STREAMDATAH  ,   // Stream Data Device Server task
#endif

#if (BLE_ANC_CLIENT)
    TASK_ANCC         ,   // ANCS Client Task
#endif

#if (BLE_WPT_CLIENT)
    TASK_WPTC         ,   // A4WP Wireless Power Transfer Client Profile Task
#endif

#if (BLE_WPTS)
    TASK_WPTS         ,   // A4WP Wireless Power Transfer Server Profile Task
#endif

#if (BLE_APP_PTU)
     TASK_APP_PTU     ,   // A4WP Wireless Power Transfer Client App Task
#endif

#if (BLE_IEU)
    TASK_IEU          ,   // Integrated Environmantal Unit Task
#endif

#if (BLE_MPU)
    TASK_MPU          ,   // Motion Processing Unit Task
#endif

#if (BLE_WSS_SERVER)
    TASK_WSSS          ,   // Weight Scale Server Task
#endif

#if (BLE_UDS_SERVER)
    TASK_UDSS          ,   // User Data Server Task
#endif

#if (BLE_SPS_SERVER)
    TASK_SPS_SERVER    ,   // Serial Proert Service Server Task
#endif

#if (BLE_SPS_CLIENT)
    TASK_SPS_CLIENT    ,   // Serial Proert Service Server Task
#endif

#if (BLE_ADC_NOTIFY)
    TASK_ADC_NOTIFY    ,   // Serial Proert Service Server Task
#endif

#if (BLE_DEVICE_CONFIG)
    TASK_DEVICE_CONFIG    ,   // Serial Proert Service Server Task
#endif

    // End of conditionally assigned task types

    TASK_HCI          = 60  ,
    TASK_HCIH         = 61  ,

    TASK_GTL          = 63  ,

    TASK_MAX          = 64,  //MAX is 64. Do  not exceed. 
};
/// Kernel memory heaps types.
enum
{
    /// Memory allocated for environment variables
    KE_MEM_ENV,
    #if 1//(BLE_HOST_PRESENT)
    /// Memory allocated for Attribute database
    KE_MEM_ATT_DB,
    #endif // (BLE_HOST_PRESENT)
    /// Memory allocated for kernel messages
    KE_MEM_KE_MSG,
    /// Non Retention memory block
    KE_MEM_NON_RETENTION,
    KE_MEM_BLOCK_MAX,
};


#if (BT_EMB_PRESENT)
#define BT_TASK_SIZE_        BT_TASK_SIZE
#define BT_HEAP_MSG_SIZE_      BT_HEAP_MSG_SIZE
#define BT_HEAP_ENV_SIZE_      0
#else
#define BT_TASK_SIZE_        0
#define BT_HEAP_MSG_SIZE_      0
#define BT_HEAP_ENV_SIZE_      0
#endif //BT_EMB_PRESENT

#if (BLE_EMB_PRESENT)
#define BLE_TASK_SIZE_       BLE_TASK_SIZE
#define BLE_HEAP_MSG_SIZE_     BLE_HEAP_MSG_SIZE
#define BLE_HEAP_ENV_SIZE_     BLE_HEAP_ENV_SIZE
#else
#define BLE_TASK_SIZE_       0
#define BLE_HEAP_MSG_SIZE_     0
#define BLE_HEAP_ENV_SIZE_     0
#endif //BLE_EMB_PRESENT

#if (BLE_HOST_PRESENT)
#define BLEHL_TASK_SIZE_     BLEHL_TASK_SIZE
#define BLEHL_HEAP_MSG_SIZE_   BLEHL_HEAP_MSG_SIZE
#define BLEHL_HEAP_ENV_SIZE_   BLEHL_HEAP_ENV_SIZE
#define BLEHL_HEAP_DB_SIZE_    BLEHL_HEAP_DB_SIZE
#else
#define BLEHL_TASK_SIZE_     0
#define BLEHL_HEAP_MSG_SIZE_   0
#define BLEHL_HEAP_ENV_SIZE_   0
#define BLEHL_HEAP_DB_SIZE_    0
#endif //BLE_HOST_PRESENT

/// Number of Kernel tasks
#define KE_TASK_SIZE         (  DISPLAY_TASK_SIZE     + \
                                DEBUG_TASK_SIZE       + \
                                TL_TASK_SIZE          + \
                                BLE_APP_TASK_SIZE     + \
                                BT_TASK_SIZE_         + \
                                BLE_TASK_SIZE_        + \
                                BLEHL_TASK_SIZE_         )


/// Kernel Message Heap
#define RWIP_HEAP_MSG_SIZE         (  BT_HEAP_MSG_SIZE_      + \
                                    BLE_HEAP_MSG_SIZE_     + \
                                    BLEHL_HEAP_MSG_SIZE_      )

/// Number of link in kernel environment TODO [FBE] add a define in scons build
#define KE_NB_LINK_IN_HEAP_ENV   4



/// Size of Environment heap
#define RWIP_HEAP_ENV_SIZE         (( BT_HEAP_ENV_SIZE_         + \
                                    BLE_HEAP_ENV_SIZE_        + \
                                    BLEHL_HEAP_ENV_SIZE_       )\
                                    * KE_NB_LINK_IN_HEAP_ENV)

/// Size of Attribute database heap
#define RWIP_HEAP_DB_SIZE         (  BLEHL_HEAP_DB_SIZE  ) //3072, TO BE TUNES

/// Size of non retention heap
//#define RWIP_HEAP_NON_RET_SIZE    (  2048  ) 
#define RWIP_HEAP_NON_RET_SIZE    (  1024*BLE_CONNECTION_MAX  ) 

// Heap header size is 12 bytes
 #define RWIP_HEAP_HEADER             (12 / sizeof(uint32_t)) //header size in uint32_t
 // ceil(len/sizeof(uint32_t)) + RWIP_HEAP_HEADER
 #define RWIP_CALC_HEAP_LEN(len)      ((((len) + (sizeof(uint32_t) - 1)) / sizeof(uint32_t)) + RWIP_HEAP_HEADER)

#define HEAP_HDR_LEN  (12) // in bytes


/// @} BT Stack Configuration
/// @} ROOT

#define  FLAG_FULLEMB_POS               0
#define  main_pos                       1
#define  rf_init_pos                    2
#define  prf_init_pos                   3
#define  calibrate_rc32KHz_pos          4
#define  calibrate_RF_pos               5
#define  uart_init_pos                  6
#define  uart_flow_on_pos               7
#define  uart_flow_off_pos              8
#define  uart_finish_transfers_pos      9
#define  uart_read_pos                  10
#define  uart_write_pos                 11
#define  UART_Handler_pos               12
#define  lld_sleep_compensate_pos       13
#define  lld_sleep_init_pos             14
#define  lld_sleep_us_2_lpcycles_pos    15
#define  lld_sleep_lpcycles_2_us_pos    16
#define  hci_tx_done_pos                17
#define  hci_enter_sleep_pos            18
#define  app_sec_task_pos           	19
#define  rwip_heap_non_ret_pos          20
#define  rwip_heap_non_ret_size         21
#define  rwip_heap_env_pos				22
#define  rwip_heap_env_size				23
#define  rwip_heap_db_pos				24
#define  rwip_heap_db_size				25
#define  rwip_heap_msg_pos				26
#define  rwip_heap_msg_size				27
#define  offset_em_et                   28 		         
#define  offset_em_ft                   29 	         
#define  offset_em_enc_plain            30 	
#define  offset_em_enc_cipher           31 	 
#define  offset_em_cs                   32 	 
#define  offset_em_wpb                  33 	        
#define  offset_em_wpv                  34 	
#define  offset_em_cnxadd               35 	
#define  offset_em_txe                  36 
#define  offset_em_tx                   37 
#define  offset_em_rx                   38 
#define  nb_links_user					39
#define  sleep_wake_up_delay_pos		40
#define  lld_assessment_stat_pos		41
#define  lld_evt_init_pos				42
#define  gtl_eif_init_pos				43
#define  ke_task_init_pos				44
#define  ke_timer_init_pos				45
#define  llm_encryption_done_pos		46
#define  nvds_get_pos					47
#define  lld_evt_prog_latency_pos		48
#define  rwip_eif_get_pos				49
#define  prf_cleanup_pos				50
#define lld_rx_irq_thres				51
#define lld_init_pos					52
#define enable_BLE_core_irq_pos			53
#define platform_reset_pos				54
#define gtl_hci_rx_header_func_pos		55
#define	gtl_hci_rx_payload_func_pos		56
#define	gtl_hci_tx_evt_func_pos			57

#define	GPADC_init_func_pos				58
#define	meas_precharge_freq_func_pos	59
#define	lut_cfg_pos						60
#define	check_pll_lock_func_pos			61	
#define	update_calcap_min_channel_func_pos	62
#define	update_calcap_max_channel_func_pos	63
#define	write_one_SW_LUT_entry_func_pos		64
#define	write_HW_LUT_func_pos				65
#define	clear_HW_LUT_func_pos				66
#define	update_LUT_func_pos					67
#define	save_configure_restore_func_pos		68
#define	update_calcap_ranges_func_pos		69
#define	find_initial_calcap_ranges_func_pos	70
#define	pll_vcocal_LUT_InitUpdate_func_pos	71
#define	set_rf_cal_cap_func_pos				72		
#define	enable_rf_diag_irq_func_pos			73
#define	modulationGainCalibration_func_pos	74
#define	DCoffsetCalibration_func_pos		75	
#define	IffCalibration_func_pos				76
#define	rf_calibration_func_pos				77
#define	get_rc16m_count_func_pos			78
#define set_gauss_modgain_func_pos			79
#define	lld_test_stop_func_pos				80	
#define	lld_test_mode_tx_func_pos			81	
#define	lld_test_mode_rx_func_pos			82			
#define	ret_rxwinsize_pos					83			
#define max_sleep_duration_periodic_wakeup_pos 84
#define max_sleep_duration_external_wakeup_pos 85
#ifdef __DA14581__
#define host_tester_task_pos					86
#define host_gap_task_pos						87
#define ke_task_size_pos						88


#define KE_USER_TASK_SIZE 5
#endif

#endif //RWIP_CONFIG_H_
