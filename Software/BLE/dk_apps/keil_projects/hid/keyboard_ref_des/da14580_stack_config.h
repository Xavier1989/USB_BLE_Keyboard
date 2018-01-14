/**
 ****************************************************************************************
 *
 * @file da14580_stack_config.h
 *
 * @brief RW stack configuration file.
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

#ifndef DA14580_STACK_CONFIG_H_
#define DA14580_STACK_CONFIG_H_

//CFG_ES4 CFG_LUT_PATCH CFG_EXT_SLEEP nCFG_WDOG CFG_NVDS CFG_EMB CFG_HOST CFG_APP CFG_GTL CFG_BLE CFG_HCI_UART CFG_EXT_DB 
//nCFG_DBG_MEM nCFG_RF_RIPPLE nCFG_DBG_FLASH nCFG_DBG_NVDS nCFG_DBG_STACK_PROF CFG_RF_RIPPLE nCFG_PERIPHERAL CFG_ALLROLES 
//CFG_CON=6 CFG_SECURITY_ON  CFG_ATTC CFG_ATTS CFG_PRF_BASS nCFG_PRF_PXPM CFG_PRF_PXPR nCFG_PRF_FMPL nCFG_PRF_FMPT nCFG_PRF_HTPT
//nCFG_PRF_HTPC  nCFG_PRF_DISC CFG_PRF_DISSnCFG_PRF_BLPC  nCFG_PRF_BLPS  nCFG_PRF_HRPC nCFG_PRF_HRPS nCFG_PRF_ACCEL  
//POWER_OFF_SLEEP CFG_BLECORE_11 RADIO_580 __NO_EMBEDDED_ASM CFG_APP_SEC



/*****************************************
 * Do not alter
 *****************************************/


/////////////////////////////////////////////////////////////
#define CFG_ES4
#define CFG_EMB
#define CFG_HOST
#define CFG_GTL
#define CFG_BLE
#define CFG_HCI_UART
#define CFG_EXT_DB
#define nCFG_DBG_MEM
#define nCFG_DBG_FLASH
#define nCFG_DBG_NVDS
#define nCFG_DBG_STACK_PROF
#define CFG_RF_RIPPLE
#define nCFG_PERIPHERAL
#define CFG_ALLROLES    1
#ifndef __DA14581__
#define CFG_CON             6  
#else
#define CFG_CON             8  
#endif
#define CFG_SECURITY_ON 1
#define CFG_ATTC
#define CFG_ATTS
#define CFG_DBG_NVDS
#define CFG_DBG_MEM
#define CFG_BLECORE_11
#define POWER_OFF_SLEEP
#define CFG_SLEEP
/////////////////////////////////////////////////////////////

/*FPGA*/
#define nFPGA_USED

/*Radio interface*/

#ifdef FPGA_USED
#define RADIO_RIPPLE    1
#define RIPPLE_ID       66
#else
#define RADIO_580       1
#endif

/*Misc*/
#define __NO_EMBEDDED_ASM


/*Scatterfile: Memory maps*/
#define DEEP_SLEEP_SETUP    1
#define EXT_SLEEP_SETUP     2

#endif // DA14580_STACK_CONFIG_H_
