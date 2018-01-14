/**
 ****************************************************************************************
 *
 * @file nvds.c
 *
 * @brief Non Volatile Data Storage (NVDS) driver
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup NVDS
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <string.h>      // string definitions
#include <stddef.h>      // standard definitions
#include "nvds.h"        // nvds definitions
#include "arch.h"        // main
#include "co_math.h"     // math operations
#include "co_bt.h"

#include "rwip_config.h"

extern struct nvds_data_struct *nvds_data_ptr __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY

#ifdef READ_NVDS_STRUCT_FROM_OTP 
const struct nvds_data_struct nvds_data_storage __attribute__((section("nvds_data_storage_area")));
#else
const struct nvds_data_struct nvds_data_storage __attribute__((section("nvds_data_storage_area"))) =	
{
    .NVDS_VALIDATION_FLAG                   = (   BD_ADDRESS_VALID | DEVICE_NAME_VALID | LPCLK_DRIFT_VALID | APP_BLE_ADV_DATA_VALID \
                                                | APP_BLE_SCAN_RESP_DATA_VALID | UART_BAUDRATE_VALID | SLEEP_ENABLE_VALID | EXT_WAKEUP_ENABLE_VALID \
                                                | DIAG_BLE_HW_VALID | DIAG_SW_VALID | SECURITY_ENABLE_VALID | NEB_ID_VALID \
                                                | NVDS_BLE_CA_TIMER_DUR_VALID | NVDS_BLE_CRA_TIMER_DUR_VALID | NVDS_BLE_CA_MIN_RSSI_VALID | NVDS_BLE_CA_NB_PKT_VALID \
                                                | NVDS_BLE_CA_NB_BAD_PKT_VALID),
    .NVDS_TAG_UART_BAUDRATE                 = 115200,
    .NVDS_TAG_DIAG_SW                       = 0,
    .NVDS_TAG_DIAG_BLE_HW                   = 0,
    .NVDS_TAG_NEB_ID                        = 0,
    .NVDS_TAG_LPCLK_DRIFT                   = DRIFT_BLE_DFT,
    .NVDS_TAG_SLEEP_ENABLE                  = 1,
    .NVDS_TAG_EXT_WAKEUP_ENABLE             = 0,
    .NVDS_TAG_SECURITY_ENABLE               = 1,
    .NVDS_TAG_APP_BLE_ADV_DATA              = {0x1A,                                                                                                 // Size of all the frame
                                              0xFF,                                                                                                  // Manufacturer Specific Data
                                              0xD2, 0x00,                                                                                            //Company ID 
                                              0x02,                                                                                                  // Data type: BEACON: It is 0x02
                                              0x15,                                                                                                  // Data length (21 bytes)
                                              0x58, 0x5C, 0xDE, 0x93, 0x1B, 0x01, 0x42, 0xCC, 0x9A, 0x13, 0x25, 0x00, 0x9B, 0xED, 0xC6, 0x5E,        // UUID
                                              0x00, 0x01,                                                                                            //Major
                                              0x00, 0x02,                                                                                            //Minor
                                              0xC5},                                                                                                 // Output power @ 1 meter
    .NVDS_TAG_APP_BLE_SCAN_RESP_DATA        = "\x09\xFF\x00\x60\x52\x57\x2D\x42\x4C\x45",
    .NVDS_TAG_DEVICE_NAME                   = "KB_BLE",
    .NVDS_TAG_BD_ADDRESS                    = {0x12, 0x34, 0x56, 0xCA, 0xEA, 0x80},
    .ADV_DATA_TAG_LEN                       = 27,
    .SCAN_RESP_DATA_TAG_LEN                 = 10,	
    .DEVICE_NAME_TAG_LEN                    = 6,
    /// Default Channel Assessment Timer duration (20s - Multiple of 10ms)
    .NVDS_TAG_BLE_CA_TIMER_DUR              = 2000,
    /// Default Channel Reassessment Timer duration (Multiple of Channel Assessment Timer duration)
    .NVDS_TAG_BLE_CRA_TIMER_DUR             = 6,
    /// Default Minimal RSSI Threshold - -48dBm
    .NVDS_TAG_BLE_CA_MIN_RSSI               = 0x40,     //0xD0,
    /// Default number of packets to receive for statistics
    .NVDS_TAG_BLE_CA_NB_PKT                 = 100,
    /// Default number of bad packets needed to remove a channel
    .NVDS_TAG_BLE_CA_NB_BAD_PKT             = 50,
};
#endif

#define BDADDR_FROM_OTP 0x7fd4     //OTP address offset with BDADDR

/// NULL BD address
extern const struct bd_addr co_null_bdaddr;
/// Device BD address
struct bd_addr dev_bdaddr __attribute__((section("retention_mem_area0"), zero_init));

uint8_t custom_nvds_get_func(uint8_t tag, nvds_tag_len_t * lengthPtr, uint8_t *buf)
{
    // declaration of ROM function nvds_get_func()
    uint8_t nvds_get_func(uint8_t tag, nvds_tag_len_t * lengthPtr, uint8_t *buf);
    
    if (tag == NVDS_TAG_BD_ADDRESS)
    {
#ifdef BDADDR_FROM_OTP   //check if dev_bdaddr is not zero
        if(memcmp(&dev_bdaddr, &co_null_bdaddr, NVDS_LEN_BD_ADDRESS))
        {
            memcpy(buf,&dev_bdaddr,NVDS_LEN_BD_ADDRESS);
            *lengthPtr = NVDS_LEN_BD_ADDRESS;
            return NVDS_OK;
        }
#endif
    }

    return nvds_get_func(tag, lengthPtr, buf);
}

void nvds_read_bdaddr_from_otp()
{
#ifdef BDADDR_FROM_OTP
# ifndef APP_BOOT_FROM_OTP    
    int cnt = 100000;
#define XPMC_MODE_MREAD   0x1
    uint8_t *otp_bdaddr = (uint8_t *)0x40000 + BDADDR_FROM_OTP;   //where in OTP header is BDADDR
    
    SetBits16(CLK_AMBA_REG, OTP_ENABLE, 1);		// enable OTP clock	
    while ((GetWord16(ANA_STATUS_REG) & LDO_OTP_OK) != LDO_OTP_OK && cnt--)
        /* Just wait */;
        
    // set OTP in read mode 
    SetWord32(OTPC_MODE_REG, XPMC_MODE_MREAD);
# else
    uint8_t *otp_bdaddr = (uint8_t *)0x20000000 + BDADDR_FROM_OTP;   //where in OTP header is BDADDR
# endif //APP_BOOT_FROM_OTP
    
    memcpy(&dev_bdaddr, otp_bdaddr, sizeof(dev_bdaddr));
    SetBits16(CLK_AMBA_REG, OTP_ENABLE, 0);     //disable OTP clock    
# ifdef SUPPORT_1_8_V
        SetBits16(DCDC_CTRL2_REG, DCDC_VBAT3V_LEV, 0x0);   //Support 1.8V boot
# endif //SUPPORT_1_8_V
#endif //BDADDR_FROM_OTP
}
/// @} NVDS

