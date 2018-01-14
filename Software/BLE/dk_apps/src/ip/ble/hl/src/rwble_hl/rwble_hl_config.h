/**
 ****************************************************************************************
 *
 * @file rwble_hl_config.h
 *
 * @brief Configuration of the BLE protocol stack (max number of supported connections,
 * type of partitioning, etc.)
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */

#ifndef RWBLE_HL_CONFIG_H_
#define RWBLE_HL_CONFIG_H_

/**
 ****************************************************************************************
 * @addtogroup ROOT
 * @{
 * @name BLE stack configuration
 * @{
 ****************************************************************************************
 */

/******************************************************************************************/
/* -------------------------   BLE PARTITIONING      -------------------------------------*/
/******************************************************************************************/


/******************************************************************************************/
/* --------------------------   INTERFACES        ----------------------------------------*/
/******************************************************************************************/

#if BLE_EMB_PRESENT
#define EMB_LLM_TASK      TASK_LLM
#define EMB_LLC_TASK      KE_BUILD_ID(TASK_LLC, idx)
#else // BLE_EMB_PRESENT
#define EMB_LLM_TASK      TASK_HCIH
#define EMB_LLC_TASK      TASK_HCIH
#endif // BLE_EMB_PRESENT

#if BLE_APP_PRESENT
#include "arch.h"
#define APP_MAIN_TASK       jump_table_struct[FLAG_FULLEMB_POS]//TASK_APP
#else // BLE_APP_PRESENT
#define APP_MAIN_TASK       jump_table_struct[FLAG_FULLEMB_POS]//TASK_GTL
#endif // BLE_APP_PRESENT

// Host Controller Interface (Host side)
#define BLEHL_HCIH_ITF            HCIH_ITF

/******************************************************************************************/
/* --------------------------   COEX SETUP        ----------------------------------------*/
/******************************************************************************************/

///WLAN coex
#define BLEHL_WLAN_COEX          RW_WLAN_COEX
///WLAN test mode
#define BLEHL_WLAN_COEX_TEST     RW_WLAN_COEX_TEST

/******************************************************************************************/
/* --------------------------   HOST MODULES      ----------------------------------------*/
/******************************************************************************************/

#define BLE_GAPM                    1
#if (BLE_CENTRAL || BLE_PERIPHERAL)
#define BLE_GAPC                    1
#define BLE_GAPC_HEAP_ENV_SIZE      (sizeof(struct gapc_env_tag)  + KE_HEAP_MEM_RESERVED)
#else //(BLE_CENTRAL || BLE_PERIPHERAL)
#define BLE_GAPC                    0
#define BLE_GAPC_HEAP_ENV_SIZE      0
#endif //(BLE_CENTRAL || BLE_PERIPHERAL)

#if (BLE_CENTRAL || BLE_PERIPHERAL)
#define BLE_L2CM      1
#define BLE_L2CC      1
#define BLE_ATTM      1
#define BLE_GATTM     1
#define BLE_GATTC     1
#define BLE_GATTC_HEAP_ENV_SIZE     (sizeof(struct gattc_env_tag)  + KE_HEAP_MEM_RESERVED)
#define BLE_L2CC_HEAP_ENV_SIZE      (sizeof(struct l2cc_env_tag)   + KE_HEAP_MEM_RESERVED)
#else //(BLE_CENTRAL || BLE_PERIPHERAL)
#define BLE_L2CM      0
#define BLE_L2CC      0
#define BLE_ATTC                    0
#define BLE_ATTS                    0
#define BLE_ATTM      0
#define BLE_GATTM     0
#define BLE_GATTC     0
#define BLE_GATTC_HEAP_ENV_SIZE     0
#define BLE_L2CC_HEAP_ENV_SIZE      0
#endif //(BLE_CENTRAL || BLE_PERIPHERAL)

#if RW_BLE_USE_CRYPT
#define BLE_SMPM      1
#if (BLE_CENTRAL || BLE_PERIPHERAL)
#define BLE_SMPC      1
#define BLE_SMPC_HEAP_ENV_SIZE      (sizeof(struct smpc_env_tag) + KE_HEAP_MEM_RESERVED)
#else //(BLE_CENTRAL || BLE_PERIPHERAL)
#define BLE_SMPC      0
#define BLE_SMPC_HEAP_ENV_SIZE      0
#endif //(BLE_CENTRAL || BLE_PERIPHERAL)
#else //RW_BLE_USE_CRYPT
#define BLE_SMPM      0
#define BLE_SMPC      0
#endif //RW_BLE_USE_CRYPT


/******************************************************************************************/
/* --------------------------   ATT DB            ----------------------------------------*/
/******************************************************************************************/

//ATT DB,Testing and Qualification related flags
#if (BLE_CENTRAL || BLE_PERIPHERAL)
    /// Support of External DB Management
    #if defined(CFG_EXT_DB)
        #define BLE_EXT_ATT_DB         1
    #else
        #define BLE_EXT_ATT_DB         0
    #endif // defined(CFG_EXT_DB)

/******************************************************************************************/
/* --------------------------   PROFILES          ----------------------------------------*/
/******************************************************************************************/

    /// Proximity Profile Monitor Role
    #if defined(CFG_PRF_PXPM)
        #define BLE_PROX_MONITOR        1
    #else
        #define BLE_PROX_MONITOR        0
    #endif // defined(CFG_PRF_PXPM)

    /// Proximity Profile Reporter Role
    #if defined(CFG_PRF_PXPR)
        #define BLE_PROX_REPORTER       1
    #else
        #define BLE_PROX_REPORTER       0
    #endif // defined(CFG_PRF_PXPR)
    #if defined (CFG_PRF_SPOTAR)
        #define BLE_SPOTA_RECEIVER	1
    #else
        #define BLE_SPOTA_RECEIVER	0
    #endif
    #if defined (CFG_PRF_STREAMDATAD)
        #define BLE_STREAMDATA_DEVICE	1
    #else
        #define BLE_STREAMDATA_DEVICE	0
    #endif
    #if defined (CFG_PRF_STREAMDATAH)		
        #define BLE_STREAMDATA_HOST		1
    #else
        #define BLE_STREAMDATA_HOST		0
    #endif
    
    #if defined (CFG_PRF_SAMPLE128)		
        #define BLE_SAMPLE128   		1
    #else
        #define BLE_SAMPLE128	    	0
    #endif
    
    ///Find Me Profile Locator role
    #if defined(CFG_PRF_FMPL)
        #define BLE_FINDME_LOCATOR      1
    #else
        #define BLE_FINDME_LOCATOR      0
    #endif // defined(CFG_PRF_FMPL)

    ///Find Me Profile Target role
    #if defined(CFG_PRF_FMPT)
        #define BLE_FINDME_TARGET       1
    #else
        #define BLE_FINDME_TARGET       0
    #endif // defined(CFG_PRF_FMPT)

    ///Health Thermometer Profile Collector Role
    #if defined(CFG_PRF_HTPC)
        #define BLE_HT_COLLECTOR        1
    #else
        #define BLE_HT_COLLECTOR        0
    #endif // defined(CFG_PRF_HTPC)

    ///Health Thermometer Profile Thermometer Role
    #if defined(CFG_PRF_HTPT)
        #define BLE_HT_THERMOM          1
    #else
        #define BLE_HT_THERMOM          0
    #endif // defined(CFG_PRF_HTPT)

    ///Device Information Service Client Role
    #if defined(CFG_PRF_DISC)
        #define BLE_DIS_CLIENT          1
    #else
        #define BLE_DIS_CLIENT          0
    #endif // defined(CFG_PRF_DISC)

    ///Device Information Service Server Role
    #if defined(CFG_PRF_DISS)
        #define BLE_DIS_SERVER          1
    #else
        #define BLE_DIS_SERVER          0
    #endif // defined(CFG_PRF_DISS)

    ///Blood Pressure Profile Collector Role
    #if defined(CFG_PRF_BLPC)
        #define BLE_BP_COLLECTOR        1
    #else
        #define BLE_BP_COLLECTOR        0
    #endif // defined(CFG_PRF_BLPC)

    ///Blood Pressure Profile Sensor Role
    #if defined(CFG_PRF_BLPS)
        #define BLE_BP_SENSOR           1
    #else
        #define BLE_BP_SENSOR           0
    #endif // defined(CFG_PRF_BLPS)

    ///Time Profile Client Role
    #if defined(CFG_PRF_TIPC)
        #define BLE_TIP_CLIENT          1
    #else
        #define BLE_TIP_CLIENT          0
    #endif // defined(CFG_PRF_TIPC)

    ///Time Profile Server Role
    #if defined(CFG_PRF_TIPS)
        #define BLE_TIP_SERVER          1
    #else
        #define BLE_TIP_SERVER          0
    #endif // defined(CFG_PRF_TIPS)

    ///Heart Rate Profile Collector Role
    #if defined(CFG_PRF_HRPC)
        #define BLE_HR_COLLECTOR        1
    #else
        #define BLE_HR_COLLECTOR        0
    #endif // defined(CFG_PRF_HRPC)

    ///Heart Rate Profile Sensor Role
    #if defined(CFG_PRF_HRPS)
        #define BLE_HR_SENSOR           1
    #else
        #define BLE_HR_SENSOR           0
    #endif // defined(CFG_PRF_HRPS)

    ///Scan Parameter Profile Client Role
    #if defined(CFG_PRF_SCPPC)
        #define BLE_SP_CLIENT           1
    #else
        #define BLE_SP_CLIENT           0
    #endif // defined(CFG_PRF_SCPPC)

    ///Scan Parameter Profile Server Role
    #if defined(CFG_PRF_SCPPS)
        #define BLE_SP_SERVER           1
    #else
        #define BLE_SP_SERVER           0
    #endif // defined(CFG_PRF_SCPPS)

    ///Battery Service Client Role
    #if defined(CFG_PRF_BASC)
        #define BLE_BATT_CLIENT         1
    #else
        #define BLE_BATT_CLIENT         0
    #endif // defined(CFG_PRF_BASC)

    ///Battery Service Server Role
    #if defined(CFG_PRF_BASS)
        #define BLE_BATT_SERVER         1
    #else
        #define BLE_BATT_SERVER         0
    #endif // defined(CFG_PRF_BASS)

    ///HID Device Role
    #if defined(CFG_PRF_HOGPD)
        #define BLE_HID_DEVICE          1
    #else
        #define BLE_HID_DEVICE          0
    #endif // defined(CFG_PRF_HOGPD)

    ///HID Boot Host Role
    #if defined(CFG_PRF_HOGPBH)
        #define BLE_HID_BOOT_HOST       1
    #else
        #define BLE_HID_BOOT_HOST       0
    #endif // defined(CFG_PRF_HOGPBH)

    ///HID Report Host Role
    #if defined(CFG_PRF_HOGPRH)
        #define BLE_HID_REPORT_HOST     1
    #else
        #define BLE_HID_REPORT_HOST     0
    #endif // defined(CFG_PRF_HOGPRH)

    /// Glucose Profile Collector Role
    #if defined(CFG_PRF_GLPC)
        #define BLE_GL_COLLECTOR        1
    #else
        #define BLE_GL_COLLECTOR        0
    #endif // defined(CFG_PRF_GLPC)

    /// Glucose Profile Sensor Role
    #if defined(CFG_PRF_GLPS)
        #define BLE_GL_SENSOR           1
    #else
        #define BLE_GL_SENSOR           0
    #endif // defined(CFG_PRF_GLPS)

    /// Nebulizer Profile Server Role
    #if defined(CFG_PRF_NBPS)
        #define BLE_NEB_SERVER          1
    #else
        #define BLE_NEB_SERVER          0
    #endif // defined(CFG_PRF_NBPS)

    /// Nebulizer Profile Client Role
    #if defined(CFG_PRF_NBPC)
        #define BLE_NEB_CLIENT          1
    #else
        #define BLE_NEB_CLIENT          0
    #endif // defined(CFG_PRF_NBPC)

    /// Running Speed and Cadence Profile Collector Role
    #if defined(CFG_PRF_RSCPC)
        #define BLE_RSC_COLLECTOR        1
    #else
        #define BLE_RSC_COLLECTOR        0
    #endif // defined(CFG_PRF_RSCPC)

    /// Running Speed and Cadence Profile Server Role
    #if defined(CFG_PRF_RSCPS)
        #define BLE_RSC_SENSOR           1
    #else
        #define BLE_RSC_SENSOR           0
    #endif // defined(CFG_PRF_RSCPS)

    /// Cycling Speed and Cadence Profile Collector Role
    #if defined(CFG_PRF_CSCPC)
        #define BLE_CSC_COLLECTOR        1
    #else
        #define BLE_CSC_COLLECTOR        0
    #endif // defined(CFG_PRF_CSCPC)

    /// Cycling Speed and Cadence Profile Server Role
    #if defined(CFG_PRF_CSCPS)
        #define BLE_CSC_SENSOR           1
    #else
        #define BLE_CSC_SENSOR           0
    #endif // defined(CFG_PRF_CSCPS)

    /// Cycling Power Profile Collector Role
    #if defined(CFG_PRF_CPPC)
        #define BLE_CP_COLLECTOR        1
    #else
        #define BLE_CP_COLLECTOR        0
    #endif // defined (CFG_PRF_CPPC)

    /// Cycling Power Profile Server Role
    #if defined(CFG_PRF_CPPS)
        #define BLE_CP_SENSOR            1
    #else
        #define BLE_CP_SENSOR            0
    #endif // defined (CFG_PRF_CPPS)

    /// Location and Navigation Profile Collector Role
    #if defined(CFG_PRF_LANC)
        #define BLE_LN_COLLECTOR        1
    #else
        #define BLE_LN_COLLECTOR        0
    #endif // defined (CFG_PRF_LANC)

    /// Location and Navigation Profile Server Role
    #if defined(CFG_PRF_LANS)
        #define BLE_LN_SENSOR            1
    #else
        #define BLE_LN_SENSOR            0
    #endif // defined (CFG_PRF_LANS)

    /// Alert Notification Profile Client Role
    #if defined(CFG_PRF_ANPC)
        #define BLE_AN_CLIENT           1
    #else
        #define BLE_AN_CLIENT           0
    #endif // defined(CFG_PRF_ANPC)

    /// Alert Notification Profile Server Role
    #if defined(CFG_PRF_ANPS)
        #define BLE_AN_SERVER           1
    #else
        #define BLE_AN_SERVER           0
    #endif // defined(CFG_PRF_ANPS)

    /// ANCS Profile Client Role
    #if defined(CFG_PRF_ANCC)
        #define BLE_ANC_CLIENT           1
    #else
        #define BLE_ANC_CLIENT           0
    #endif // defined(CFG_PRF_ANCC)
	
    /// Phone Alert Status Profile Client Role
    #if defined(CFG_PRF_PASPC)
        #define BLE_PAS_CLIENT           1
    #else
        #define BLE_PAS_CLIENT           0
    #endif // defined(CFG_PRF_PASPC)

    /// Phone Alert Status Profile Server Role
    #if defined(CFG_PRF_PASPS)
        #define BLE_PAS_SERVER           1
    #else
        #define BLE_PAS_SERVER           0
    #endif // defined(CFG_PRF_PASPS)

    /// Accelerometer Profile
    #if defined(CFG_PRF_ACCEL)
        #define BLE_ACCEL               1
    #else
        #define BLE_ACCEL               0
    #endif /* defined(CFG_PRF_ACCEL)) */
    
    #if defined(CFG_PRF_IEU)   
        #define BLE_IEU                1
    #else
        #define BLE_IEU                0
    #endif // (CFG_PRF_IEU)

    #if defined(CFG_PRF_MPU)    
        #define BLE_MPU                 1
    #else
        #define BLE_MPU                 0
    #endif // (CFG_PRF_MPU)

	#if defined(CFG_PRF_WPTC)
        #define BLE_WPT_CLIENT       1
    #else
        #define BLE_WPT_CLIENT       0
    #endif // defined(CFG_PRF_WPTC)
	
	#if defined(CFG_PRF_WPTS)
        #define BLE_WPTS       1
    #else
        #define BLE_WPTS       0
    #endif // defined(CFG_PRF_WPTS)

    /// User Data Service Server
    #if defined(CFG_PRF_UDSS)
        #define BLE_UDS_SERVER          1
    #else
        #define BLE_UDS_SERVER          0
    #endif /* defined(CFG_PRF_UDSS)) */

    /// Weight Scale Service Server
    #if defined(CFG_PRF_WSSS)
        #define BLE_WSS_SERVER          1
    #else
        #define BLE_WSS_SERVER          0
    #endif /* defined(CFG_PRF_WSSS)) */
    
    
    /// Serial Port Service Server
    #if defined(CFG_PRF_SPS_SERVER)
        #define BLE_SPS_SERVER          1
    #else
        #define BLE_SPS_SERVER          0
    #endif /* defined(CFG_PRF_WSSS)) */
    
    /// Serial Port Service Client
    #if defined(CFG_PRF_SPS_CLIENT)
        #define BLE_SPS_CLIENT          1
    #else
        #define BLE_SPS_CLIENT          0
    #endif /* defined(CFG_PRF_WSSS)) */
    
       /// Sample Profile sending nofication with ADC values
    #if defined(CFG_PRF_ADC_NOTIFY)
        #define BLE_ADC_NOTIFY          1
    #else
        #define BLE_ADC_NOTIFY          0
    #endif /* defined(CFG_PRF_ADC_NOTIFY)) */
    
    /// Sample Profile for Device configuration
    #if defined(CFG_PRF_DEVICE_CONFIG)
        #define BLE_DEVICE_CONFIG    1
    #else
        #define BLE_DEVICE_CONFIG    0
    #endif /* defined(CFG_PRF_DEVICE_CONFIG)) */
	
#elif (BLE_OBSERVER || BLE_BROADCASTER)
    /// Proximity Profile Monitor Role
    #define BLE_PROX_MONITOR            0
    /// Proximity Profile Reporter Role
    #define BLE_PROX_REPORTER           0
    #define BLE_STREAMDATA_DEVICE       0
    #define BLE_STREAMDATA_HOST         0
    ///Find Me Profile Locator role
    #define BLE_FINDME_LOCATOR          0
    ///Find Me Profile Target role
    #define BLE_FINDME_TARGET           0
    ///Health Thermometer Profile Collector Role
    #define BLE_HT_COLLECTOR            0
    ///Health Thermometer Profile Thermometer Role
    #define BLE_HT_THERMOM              0
    ///Blood Pressure Profile Collector Role
    #define BLE_BP_COLLECTOR            0
    ///Blood Pressure Profile Sensor Role
    #define BLE_BP_SENSOR               0
    ///Heart Rate Profile Collector Role
    #define BLE_HR_COLLECTOR            0
    ///Heart Rate Profile Sensor Role
    #define BLE_HR_SENSOR               0
    ///Time Profile Client Role
    #define BLE_TIP_CLIENT              0
    ///Time Profile Server Role
    #define BLE_TIP_SERVER              0
    /// Accelerometer Profile
    #define BLE_ACCEL                   0
    /// Device Information Service Client Role
    #define BLE_DIS_CLIENT              0
    /// Device Information Service Server Role
    #define BLE_DIS_SERVER              0
    /// Scan Parameter Profile Client Role
    #define BLE_SP_CLIENT               0
    /// Scan Parameter Profile Server Role
    #define BLE_SP_SERVER               0
    /// Battery Service Client Role
    #define BLE_BATT_CLIENT             0
    /// Battery Service Server Role
    #define BLE_BATT_SERVER             0
    /// HID Device Role
    #define BLE_HID_DEVICE              0
    /// HID Boot Host Role
    #define BLE_HID_BOOT_HOST           0
    /// HID Report Host Role
    #define BLE_HID_REPORT_HOST         0
    /// Glucose Profile Collector Role
    #define BLE_GL_COLLECTOR            0
    /// Glucose Profile Sensor Role
    #define BLE_GL_SENSOR               0
    /// Nebulizer Profile Server Role
    #define BLE_NEB_SERVER              0
    /// Nebulizer Profile Client Role
    #define BLE_NEB_CLIENT              0
    /// Running Speed and Cadence Collector Role
    #define BLE_RSC_COLLECTOR           0
    /// Running Speed and Cadence Server Role
    #define BLE_RSC_SENSOR              0
    /// Cycling Speed and Cadence Collector Role
    #define BLE_CSC_COLLECTOR           0
    /// Cycling Speed and Cadence Server Role
    #define BLE_CSC_SENSOR              0
    /// Cycling Power Collector Role
    #define BLE_CP_COLLECTOR            0
    /// Cycling Power Server Role
    #define BLE_CP_SENSOR               0
    /// Location and Navigation Collector Role
    #define BLE_LN_COLLECTOR            0
    /// Location and Navigation Server Role
    #define BLE_LN_SENSOR               0
    /// Alert Notification Client Role
    #define BLE_AN_CLIENT               0
    /// Alert Notification Server Role
    #define BLE_AN_SERVER               0
    /// Phone Alert Status Client Role
    #define BLE_PAS_CLIENT              0
    /// Phone Alert Status Server Role
    #define BLE_PAS_SERVER              0
    /// ANCS Client Role
    #define BLE_ANC_CLIENT              0

    //Force ATT parts to 0
    /// External database management
    #define BLE_EXT_ATTS_DB             0
    /// Profile Server
    #define BLE_SERVER_PRF              0
    /// Profile Client
    #define BLE_CLIENT_PRF              0
    /// Attribute Client
    #define BLE_ATTC                    0
    /// Attribute Server
    #define BLE_ATTS                    0
#endif //(BLE_OBSERVER || BLE_BROADCASTER)

/// BLE_CLIENT_PRF indicates if at least one client profile is present
#if (BLE_PROX_MONITOR || BLE_STREAMDATA_HOST || BLE_FINDME_LOCATOR || BLE_HT_COLLECTOR || BLE_BP_COLLECTOR \
    || BLE_HR_COLLECTOR || BLE_DIS_CLIENT || BLE_TIP_CLIENT || BLE_SP_CLIENT \
    || BLE_BATT_CLIENT || BLE_GL_COLLECTOR || BLE_HID_BOOT_HOST || BLE_HID_REPORT_HOST || BLE_NEB_CLIENT \
    || BLE_RSC_COLLECTOR || BLE_CSC_COLLECTOR || BLE_CP_COLLECTOR || BLE_LN_COLLECTOR || BLE_AN_CLIENT \
    || BLE_PAS_CLIENT || BLE_STREAMDATA_HOST || BLE_WPT_CLIENT || BLE_ANC_CLIENT || BLE_SPS_CLIENT)
    #define BLE_CLIENT_PRF          1
#else
    #define BLE_CLIENT_PRF          0
#endif //(BLE_PROX_MONITOR || BLE_FINDME_LOCATOR ...)

/// BLE_SERVER_PRF indicates if at least one server profile is present
#if (BLE_PROX_REPORTER || BLE_STREAMDATA_DEVICE || BLE_FINDME_TARGET || BLE_HT_THERMOM || BLE_BP_SENSOR \
    || BLE_TIP_SERVER || BLE_HR_SENSOR || BLE_DIS_SERVER || BLE_ACCEL || BLE_SP_SERVER \
    || BLE_BATT_SERVER || BLE_HID_DEVICE || BLE_GL_SENSOR || BLE_NEB_SERVER \
    || BLE_RSC_SENSOR || BLE_CSC_SENSOR || BLE_CP_SENSOR || BLE_LN_SENSOR || BLE_AN_SERVER \
    || BLE_PAS_SERVER || BLE_SAMPLE128 || BLE_WPTS ||  BLE_UDS_SERVER || BLE_WSS_SERVER ||BLE_SPS_SERVER || BLE_ADC_NOTIFY || BLE_DEVICE_CONFIG || BLE_SPOTA_RECEIVER)
    #define BLE_SERVER_PRF          1
#else
    #define BLE_SERVER_PRF          0
#endif //(BLE_PROX_REPORTER || BLE_FINDME_TARGET ...)

//Force ATT parts depending on profile roles or compile options
/// Attribute Client
#if (BLE_CLIENT_PRF || BLE_CENTRAL || defined(CFG_ATTC))
    #define BLE_ATTC                    1
#else
    #define BLE_ATTC                    0
#endif //(BLE_CLIENT_PRF || defined(CFG_ATTC))

/// Attribute Server
#if (BLE_SERVER_PRF || BLE_PERIPHERAL || defined(CFG_ATTS))
    #define BLE_ATTS                    1
    #define BLE_ATTS_HEAP_ENV_SIZE      (sizeof(struct atts_env_tag) + KE_HEAP_MEM_RESERVED)
#else
    #define BLE_ATTS                    0
    #define BLE_ATTS_HEAP_ENV_SIZE      0
#endif //(BLE_SERVER_PRF || defined(CFG_ATTS))





/// Size of the heap
#if (BLE_CENTRAL || BLE_PERIPHERAL)
    /// some heap must be reserved for attribute database
    #if (BLE_ATTS || BLE_ATTC)
        #define BLEHL_HEAP_DB_SIZE                 (3072)
    #else
        #define BLEHL_HEAP_DB_SIZE                 (0)
    #endif /* (BLE_ATTS || BLE_ATTC) */

    #define BLEHL_HEAP_MSG_SIZE                    (256 + 256 * BLE_CONNECTION_MAX)
#else
    #define BLEHL_HEAP_MSG_SIZE                    (256)
    #define BLEHL_HEAP_DB_SIZE                     (0)
#endif /* #if (BLE_CENTRAL || BLE_PERIPHERAL) */


/// Number of BLE Host stack tasks
#define BLE_HOST_TASK_SIZE  ( BLE_GAPM       +  \
                              BLE_GAPC       +  \
                              BLE_L2CM       +  \
                              BLE_L2CC       +  \
                              BLE_SMPM       +  \
                              BLE_SMPC       +  \
                              BLE_ATTM       +  \
                              BLE_ATTC       +  \
                              BLE_ATTS       +  \
                              BLE_GATTM      +  \
                              BLE_GATTC      )


/// Number of BLE profiles tasks
#define BLE_PRF_TASK_SIZE   ( BLE_PROX_MONITOR       +  \
                              BLE_PROX_REPORTER      +  \
                              BLE_STREAMDATA_DEVICE  +  \
                              BLE_STREAMDATA_HOST    +  \
                              BLE_FINDME_LOCATOR     +  \
                              BLE_FINDME_TARGET      +  \
                              BLE_HT_COLLECTOR       +  \
                              BLE_HT_THERMOM         +  \
                              BLE_DIS_CLIENT         +  \
                              BLE_DIS_SERVER         +  \
                              BLE_BP_COLLECTOR       +  \
                              BLE_BP_SENSOR          +  \
                              BLE_TIP_CLIENT         +  \
                              BLE_TIP_SERVER         +  \
                              BLE_HR_COLLECTOR       +  \
                              BLE_HR_SENSOR          +  \
                              BLE_SP_CLIENT          +  \
                              BLE_SP_SERVER          +  \
                              BLE_BATT_CLIENT        +  \
                              BLE_BATT_SERVER        +  \
                              BLE_HID_DEVICE         +  \
                              BLE_HID_BOOT_HOST      +  \
                              BLE_HID_REPORT_HOST    +  \
                              BLE_GL_COLLECTOR       +  \
                              BLE_GL_SENSOR          +  \
                              BLE_NEB_SERVER         +  \
                              BLE_NEB_CLIENT         +  \
                              BLE_RSC_COLLECTOR      +  \
                              BLE_RSC_SENSOR         +  \
                              BLE_CSC_COLLECTOR      +  \
                              BLE_CSC_SENSOR         +  \
                              BLE_CP_COLLECTOR       +  \
                              BLE_CP_SENSOR          +  \
                              BLE_LN_COLLECTOR       +  \
                              BLE_LN_SENSOR          +  \
                              BLE_AN_CLIENT          +  \
                              BLE_AN_SERVER          +  \
							  BLE_ANC_CLIENT         +  \
                              BLE_PAS_CLIENT         +  \
                              BLE_PAS_SERVER         +  \
                              BLE_ACCEL              +  \
                              BLE_WPT_CLIENT         +  \
                              BLE_UDS_SERVER         +  \
                              BLE_WSS_SERVER)

/// Number of BLE HL tasks
#define BLEHL_TASK_SIZE     BLE_HOST_TASK_SIZE + BLE_PRF_TASK_SIZE
#if (!BLE_HOST_PRESENT)
#define BLEHL_HEAP_ENV_SIZE (10)
#else
/// Size of environment variable needed on BLE Host Stack for one link
#define BLEHL_HEAP_ENV_SIZE ( BLE_GAPC_HEAP_ENV_SIZE       +  \
                              BLE_SMPC_HEAP_ENV_SIZE       +  \
                              BLE_ATTS_HEAP_ENV_SIZE       +  \
                              BLE_GATTC_HEAP_ENV_SIZE      +  \
                              BLE_L2CC_HEAP_ENV_SIZE)
#endif
/// @} BLE stack configuration
/// @} ROOT

#endif // RWBLE_HL_CONFIG_H_
