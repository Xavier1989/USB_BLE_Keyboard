#ifndef _REG_BLECORE_H_
#define _REG_BLECORE_H_

#include <stdint.h>
#include "_reg_blecore.h"
#include "compiler.h"
#include "arch.h"
#include "reg_access.h"

#define REG_BLECORE_COUNT 69

#define REG_BLECORE_DECODING_MASK 0x000001FF

/**
 * @brief RWBTLECNTL register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31      MASTER_SOFT_RST   0
 *     30    MASTER_TGSOFT_RST   0
 *     29         REG_SOFT_RST   0
 *     26         RFTEST_ABORT   0
 *     25         ADVERT_ABORT   0
 *     24           SCAN_ABORT   0
 *     22               MD_DSB   0
 *     21               SN_DSB   0
 *     20             NESN_DSB   0
 *     19            CRYPT_DSB   0
 *     18             WHIT_DSB   0
 *     17              CRC_DSB   0
 *     16        HOP_REMAP_DSB   0
 *  15:14            CORR_MODE   0x0
 *     12          TXWINOFFSEL   0
 *     11         RXDESCPTRSEL   0
 *     10        ADVERRFILT_EN   0
 *     09            WLSYNC_EN   0
 *     08             RWBLE_EN   0
 *  07:04           RXWINSZDEF   0x0
 *  02:00              SYNCERR   0x0
 * </pre>
 */
#define BLE_RWBTLECNTL_ADDR   0x40000000
#define BLE_RWBTLECNTL_OFFSET 0x00000000
#define BLE_RWBTLECNTL_INDEX  0x00000000
#define BLE_RWBTLECNTL_RESET  0x00000000

__INLINE uint32_t ble_rwbtlecntl_get(void)
{
    return REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
}

__INLINE void ble_rwbtlecntl_set(uint32_t value)
{
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, value);
}

// field definitions
#define BLE_MASTER_SOFT_RST_BIT      ((uint32_t)0x80000000)
#define BLE_MASTER_SOFT_RST_POS      31
#define BLE_MASTER_TGSOFT_RST_BIT    ((uint32_t)0x40000000)
#define BLE_MASTER_TGSOFT_RST_POS    30
#define BLE_REG_SOFT_RST_BIT         ((uint32_t)0x20000000)
#define BLE_REG_SOFT_RST_POS         29
#define BLE_RFTEST_ABORT_BIT         ((uint32_t)0x04000000)
#define BLE_RFTEST_ABORT_POS         26
#define BLE_ADVERT_ABORT_BIT         ((uint32_t)0x02000000)
#define BLE_ADVERT_ABORT_POS         25
#define BLE_SCAN_ABORT_BIT           ((uint32_t)0x01000000)
#define BLE_SCAN_ABORT_POS           24
#define BLE_MD_DSB_BIT               ((uint32_t)0x00400000)
#define BLE_MD_DSB_POS               22
#define BLE_SN_DSB_BIT               ((uint32_t)0x00200000)
#define BLE_SN_DSB_POS               21
#define BLE_NESN_DSB_BIT             ((uint32_t)0x00100000)
#define BLE_NESN_DSB_POS             20
#define BLE_CRYPT_DSB_BIT            ((uint32_t)0x00080000)
#define BLE_CRYPT_DSB_POS            19
#define BLE_WHIT_DSB_BIT             ((uint32_t)0x00040000)
#define BLE_WHIT_DSB_POS             18
#define BLE_CRC_DSB_BIT              ((uint32_t)0x00020000)
#define BLE_CRC_DSB_POS              17
#define BLE_HOP_REMAP_DSB_BIT        ((uint32_t)0x00010000)
#define BLE_HOP_REMAP_DSB_POS        16
#define BLE_CORR_MODE_MASK           ((uint32_t)0x0000C000)
#define BLE_CORR_MODE_LSB            14
#define BLE_CORR_MODE_WIDTH          ((uint32_t)0x00000002)
#define BLE_TXWINOFFSEL_BIT          ((uint32_t)0x00001000)
#define BLE_TXWINOFFSEL_POS          12
#define BLE_RXDESCPTRSEL_BIT         ((uint32_t)0x00000800)
#define BLE_RXDESCPTRSEL_POS         11
#define BLE_ADVERRFILT_EN_BIT        ((uint32_t)0x00000400)
#define BLE_ADVERRFILT_EN_POS        10
#define BLE_WLSYNC_EN_BIT            ((uint32_t)0x00000200)
#define BLE_WLSYNC_EN_POS            9
#define BLE_RWBLE_EN_BIT             ((uint32_t)0x00000100)
#define BLE_RWBLE_EN_POS             8
#define BLE_RXWINSZDEF_MASK          ((uint32_t)0x000000F0)
#define BLE_RXWINSZDEF_LSB           4
#define BLE_RXWINSZDEF_WIDTH         ((uint32_t)0x00000004)
#define BLE_SYNCERR_MASK             ((uint32_t)0x00000007)
#define BLE_SYNCERR_LSB              0
#define BLE_SYNCERR_WIDTH            ((uint32_t)0x00000003)

#define BLE_MASTER_SOFT_RST_RST      0x0
#define BLE_MASTER_TGSOFT_RST_RST    0x0
#define BLE_REG_SOFT_RST_RST         0x0
#define BLE_RFTEST_ABORT_RST         0x0
#define BLE_ADVERT_ABORT_RST         0x0
#define BLE_SCAN_ABORT_RST           0x0
#define BLE_MD_DSB_RST               0x0
#define BLE_SN_DSB_RST               0x0
#define BLE_NESN_DSB_RST             0x0
#define BLE_CRYPT_DSB_RST            0x0
#define BLE_WHIT_DSB_RST             0x0
#define BLE_CRC_DSB_RST              0x0
#define BLE_HOP_REMAP_DSB_RST        0x0
#define BLE_CORR_MODE_RST            0x0
#define BLE_TXWINOFFSEL_RST          0x0
#define BLE_RXDESCPTRSEL_RST         0x0
#define BLE_ADVERRFILT_EN_RST        0x0
#define BLE_WLSYNC_EN_RST            0x0
#define BLE_RWBLE_EN_RST             0x0
#define BLE_RXWINSZDEF_RST           0x0
#define BLE_SYNCERR_RST              0x0

__INLINE void ble_rwbtlecntl_pack(uint8_t mastersoftrst, uint8_t mastertgsoftrst, uint8_t regsoftrst, uint8_t rftestabort, uint8_t advertabort, uint8_t scanabort, uint8_t mddsb, uint8_t sndsb, uint8_t nesndsb, uint8_t cryptdsb, uint8_t whitdsb, uint8_t crcdsb, uint8_t hopremapdsb, uint8_t corrmode, uint8_t txwinoffsel, uint8_t rxdescptrsel, uint8_t adverrfilten, uint8_t wlsyncen, uint8_t rwbleen, uint8_t rxwinszdef, uint8_t syncerr)
{
    ASSERT_ERR((((uint32_t)mastersoftrst << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)mastertgsoftrst << 30) & ~((uint32_t)0x40000000)) == 0);
    ASSERT_ERR((((uint32_t)regsoftrst << 29) & ~((uint32_t)0x20000000)) == 0);
    ASSERT_ERR((((uint32_t)rftestabort << 26) & ~((uint32_t)0x04000000)) == 0);
    ASSERT_ERR((((uint32_t)advertabort << 25) & ~((uint32_t)0x02000000)) == 0);
    ASSERT_ERR((((uint32_t)scanabort << 24) & ~((uint32_t)0x01000000)) == 0);
    ASSERT_ERR((((uint32_t)mddsb << 22) & ~((uint32_t)0x00400000)) == 0);
    ASSERT_ERR((((uint32_t)sndsb << 21) & ~((uint32_t)0x00200000)) == 0);
    ASSERT_ERR((((uint32_t)nesndsb << 20) & ~((uint32_t)0x00100000)) == 0);
    ASSERT_ERR((((uint32_t)cryptdsb << 19) & ~((uint32_t)0x00080000)) == 0);
    ASSERT_ERR((((uint32_t)whitdsb << 18) & ~((uint32_t)0x00040000)) == 0);
    ASSERT_ERR((((uint32_t)crcdsb << 17) & ~((uint32_t)0x00020000)) == 0);
    ASSERT_ERR((((uint32_t)hopremapdsb << 16) & ~((uint32_t)0x00010000)) == 0);
    ASSERT_ERR((((uint32_t)corrmode << 14) & ~((uint32_t)0x0000C000)) == 0);
    ASSERT_ERR((((uint32_t)txwinoffsel << 12) & ~((uint32_t)0x00001000)) == 0);
    ASSERT_ERR((((uint32_t)rxdescptrsel << 11) & ~((uint32_t)0x00000800)) == 0);
    ASSERT_ERR((((uint32_t)adverrfilten << 10) & ~((uint32_t)0x00000400)) == 0);
    ASSERT_ERR((((uint32_t)wlsyncen << 9) & ~((uint32_t)0x00000200)) == 0);
    ASSERT_ERR((((uint32_t)rwbleen << 8) & ~((uint32_t)0x00000100)) == 0);
    ASSERT_ERR((((uint32_t)rxwinszdef << 4) & ~((uint32_t)0x000000F0)) == 0);
    ASSERT_ERR((((uint32_t)syncerr << 0) & ~((uint32_t)0x00000007)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR,  ((uint32_t)mastersoftrst << 31) | ((uint32_t)mastertgsoftrst << 30) | ((uint32_t)regsoftrst << 29) | ((uint32_t)rftestabort << 26) | ((uint32_t)advertabort << 25) | ((uint32_t)scanabort << 24) | ((uint32_t)mddsb << 22) | ((uint32_t)sndsb << 21) | ((uint32_t)nesndsb << 20) | ((uint32_t)cryptdsb << 19) | ((uint32_t)whitdsb << 18) | ((uint32_t)crcdsb << 17) | ((uint32_t)hopremapdsb << 16) | ((uint32_t)corrmode << 14) | ((uint32_t)txwinoffsel << 12) | ((uint32_t)rxdescptrsel << 11) | ((uint32_t)adverrfilten << 10) | ((uint32_t)wlsyncen << 9) | ((uint32_t)rwbleen << 8) | ((uint32_t)rxwinszdef << 4) | ((uint32_t)syncerr << 0));
}

__INLINE void ble_rwbtlecntl_unpack(uint8_t* mastersoftrst, uint8_t* mastertgsoftrst, uint8_t* regsoftrst, uint8_t* rftestabort, uint8_t* advertabort, uint8_t* scanabort, uint8_t* mddsb, uint8_t* sndsb, uint8_t* nesndsb, uint8_t* cryptdsb, uint8_t* whitdsb, uint8_t* crcdsb, uint8_t* hopremapdsb, uint8_t* corrmode, uint8_t* txwinoffsel, uint8_t* rxdescptrsel, uint8_t* adverrfilten, uint8_t* wlsyncen, uint8_t* rwbleen, uint8_t* rxwinszdef, uint8_t* syncerr)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);

    *mastersoftrst = (localVal & ((uint32_t)0x80000000)) >> 31;
    *mastertgsoftrst = (localVal & ((uint32_t)0x40000000)) >> 30;
    *regsoftrst = (localVal & ((uint32_t)0x20000000)) >> 29;
    *rftestabort = (localVal & ((uint32_t)0x04000000)) >> 26;
    *advertabort = (localVal & ((uint32_t)0x02000000)) >> 25;
    *scanabort = (localVal & ((uint32_t)0x01000000)) >> 24;
    *mddsb = (localVal & ((uint32_t)0x00400000)) >> 22;
    *sndsb = (localVal & ((uint32_t)0x00200000)) >> 21;
    *nesndsb = (localVal & ((uint32_t)0x00100000)) >> 20;
    *cryptdsb = (localVal & ((uint32_t)0x00080000)) >> 19;
    *whitdsb = (localVal & ((uint32_t)0x00040000)) >> 18;
    *crcdsb = (localVal & ((uint32_t)0x00020000)) >> 17;
    *hopremapdsb = (localVal & ((uint32_t)0x00010000)) >> 16;
    *corrmode = (localVal & ((uint32_t)0x0000C000)) >> 14;
    *txwinoffsel = (localVal & ((uint32_t)0x00001000)) >> 12;
    *rxdescptrsel = (localVal & ((uint32_t)0x00000800)) >> 11;
    *adverrfilten = (localVal & ((uint32_t)0x00000400)) >> 10;
    *wlsyncen = (localVal & ((uint32_t)0x00000200)) >> 9;
    *rwbleen = (localVal & ((uint32_t)0x00000100)) >> 8;
    *rxwinszdef = (localVal & ((uint32_t)0x000000F0)) >> 4;
    *syncerr = (localVal & ((uint32_t)0x00000007)) >> 0;
}

__INLINE uint8_t ble_master_soft_rst_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

__INLINE void ble_master_soft_rst_setf(uint8_t mastersoftrst)
{
    ASSERT_ERR((((uint32_t)mastersoftrst << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x80000000)) | ((uint32_t)mastersoftrst << 31));
}

__INLINE uint8_t ble_master_tgsoft_rst_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x40000000)) >> 30);
}

__INLINE void ble_master_tgsoft_rst_setf(uint8_t mastertgsoftrst)
{
    ASSERT_ERR((((uint32_t)mastertgsoftrst << 30) & ~((uint32_t)0x40000000)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x40000000)) | ((uint32_t)mastertgsoftrst << 30));
}

__INLINE uint8_t ble_reg_soft_rst_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x20000000)) >> 29);
}

__INLINE void ble_reg_soft_rst_setf(uint8_t regsoftrst)
{
    ASSERT_ERR((((uint32_t)regsoftrst << 29) & ~((uint32_t)0x20000000)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x20000000)) | ((uint32_t)regsoftrst << 29));
}

__INLINE uint8_t ble_rftest_abort_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x04000000)) >> 26);
}

__INLINE void ble_rftest_abort_setf(uint8_t rftestabort)
{
    ASSERT_ERR((((uint32_t)rftestabort << 26) & ~((uint32_t)0x04000000)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x04000000)) | ((uint32_t)rftestabort << 26));
}

__INLINE uint8_t ble_advert_abort_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x02000000)) >> 25);
}

__INLINE void ble_advert_abort_setf(uint8_t advertabort)
{
    ASSERT_ERR((((uint32_t)advertabort << 25) & ~((uint32_t)0x02000000)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x02000000)) | ((uint32_t)advertabort << 25));
}

__INLINE uint8_t ble_scan_abort_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x01000000)) >> 24);
}

__INLINE void ble_scan_abort_setf(uint8_t scanabort)
{
    ASSERT_ERR((((uint32_t)scanabort << 24) & ~((uint32_t)0x01000000)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x01000000)) | ((uint32_t)scanabort << 24));
}

__INLINE uint8_t ble_md_dsb_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x00400000)) >> 22);
}

__INLINE void ble_md_dsb_setf(uint8_t mddsb)
{
    ASSERT_ERR((((uint32_t)mddsb << 22) & ~((uint32_t)0x00400000)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x00400000)) | ((uint32_t)mddsb << 22));
}

__INLINE uint8_t ble_sn_dsb_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x00200000)) >> 21);
}

__INLINE void ble_sn_dsb_setf(uint8_t sndsb)
{
    ASSERT_ERR((((uint32_t)sndsb << 21) & ~((uint32_t)0x00200000)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x00200000)) | ((uint32_t)sndsb << 21));
}

__INLINE uint8_t ble_nesn_dsb_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x00100000)) >> 20);
}

__INLINE void ble_nesn_dsb_setf(uint8_t nesndsb)
{
    ASSERT_ERR((((uint32_t)nesndsb << 20) & ~((uint32_t)0x00100000)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x00100000)) | ((uint32_t)nesndsb << 20));
}

__INLINE uint8_t ble_crypt_dsb_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x00080000)) >> 19);
}

__INLINE void ble_crypt_dsb_setf(uint8_t cryptdsb)
{
    ASSERT_ERR((((uint32_t)cryptdsb << 19) & ~((uint32_t)0x00080000)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x00080000)) | ((uint32_t)cryptdsb << 19));
}

__INLINE uint8_t ble_whit_dsb_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x00040000)) >> 18);
}

__INLINE void ble_whit_dsb_setf(uint8_t whitdsb)
{
    ASSERT_ERR((((uint32_t)whitdsb << 18) & ~((uint32_t)0x00040000)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x00040000)) | ((uint32_t)whitdsb << 18));
}

__INLINE uint8_t ble_crc_dsb_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x00020000)) >> 17);
}

__INLINE void ble_crc_dsb_setf(uint8_t crcdsb)
{
    ASSERT_ERR((((uint32_t)crcdsb << 17) & ~((uint32_t)0x00020000)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x00020000)) | ((uint32_t)crcdsb << 17));
}

__INLINE uint8_t ble_hop_remap_dsb_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

__INLINE void ble_hop_remap_dsb_setf(uint8_t hopremapdsb)
{
    ASSERT_ERR((((uint32_t)hopremapdsb << 16) & ~((uint32_t)0x00010000)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x00010000)) | ((uint32_t)hopremapdsb << 16));
}

__INLINE uint8_t ble_corr_mode_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x0000C000)) >> 14);
}

__INLINE void ble_corr_mode_setf(uint8_t corrmode)
{
    ASSERT_ERR((((uint32_t)corrmode << 14) & ~((uint32_t)0x0000C000)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x0000C000)) | ((uint32_t)corrmode << 14));
}

__INLINE uint8_t ble_txwinoffsel_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x00001000)) >> 12);
}

__INLINE void ble_txwinoffsel_setf(uint8_t txwinoffsel)
{
    ASSERT_ERR((((uint32_t)txwinoffsel << 12) & ~((uint32_t)0x00001000)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x00001000)) | ((uint32_t)txwinoffsel << 12));
}

__INLINE uint8_t ble_rxdescptrsel_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000800)) >> 11);
}

__INLINE void ble_rxdescptrsel_setf(uint8_t rxdescptrsel)
{
    ASSERT_ERR((((uint32_t)rxdescptrsel << 11) & ~((uint32_t)0x00000800)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x00000800)) | ((uint32_t)rxdescptrsel << 11));
}

__INLINE uint8_t ble_adverrfilt_en_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000400)) >> 10);
}

__INLINE void ble_adverrfilt_en_setf(uint8_t adverrfilten)
{
    ASSERT_ERR((((uint32_t)adverrfilten << 10) & ~((uint32_t)0x00000400)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x00000400)) | ((uint32_t)adverrfilten << 10));
}

__INLINE uint8_t ble_wlsync_en_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000200)) >> 9);
}

__INLINE void ble_wlsync_en_setf(uint8_t wlsyncen)
{
    ASSERT_ERR((((uint32_t)wlsyncen << 9) & ~((uint32_t)0x00000200)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x00000200)) | ((uint32_t)wlsyncen << 9));
}

__INLINE uint8_t ble_rwble_en_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

__INLINE void ble_rwble_en_setf(uint8_t rwbleen)
{
    ASSERT_ERR((((uint32_t)rwbleen << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x00000100)) | ((uint32_t)rwbleen << 8));
}

__INLINE uint8_t ble_rxwinszdef_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x000000F0)) >> 4);
}

__INLINE void ble_rxwinszdef_setf(uint8_t rxwinszdef)
{
    ASSERT_ERR((((uint32_t)rxwinszdef << 4) & ~((uint32_t)0x000000F0)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x000000F0)) | ((uint32_t)rxwinszdef << 4));
}

__INLINE uint8_t ble_syncerr_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000007)) >> 0);
}

__INLINE void ble_syncerr_setf(uint8_t syncerr)
{
    ASSERT_ERR((((uint32_t)syncerr << 0) & ~((uint32_t)0x00000007)) == 0);
    REG_BLE_WR(BLE_RWBTLECNTL_ADDR, (REG_BLE_RD(BLE_RWBTLECNTL_ADDR) & ~((uint32_t)0x00000007)) | ((uint32_t)syncerr << 0));
}

/**
 * @brief VERSION register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24                  TYP   0x6
 *  23:16                  REL   0x1
 *  15:08                  UPG   0xC
 *  07:00                BUILD   0x0
 * </pre>
 */
#define BLE_VERSION_ADDR   0x40000004
#define BLE_VERSION_OFFSET 0x00000004
#define BLE_VERSION_INDEX  0x00000001
#define BLE_VERSION_RESET  0x06010C00

__INLINE uint32_t ble_version_get(void)
{
    return REG_BLE_RD(BLE_VERSION_ADDR);
}

// field definitions
#define BLE_TYP_MASK     ((uint32_t)0xFF000000)
#define BLE_TYP_LSB      24
#define BLE_TYP_WIDTH    ((uint32_t)0x00000008)
#define BLE_REL_MASK     ((uint32_t)0x00FF0000)
#define BLE_REL_LSB      16
#define BLE_REL_WIDTH    ((uint32_t)0x00000008)
#define BLE_UPG_MASK     ((uint32_t)0x0000FF00)
#define BLE_UPG_LSB      8
#define BLE_UPG_WIDTH    ((uint32_t)0x00000008)
#define BLE_BUILD_MASK   ((uint32_t)0x000000FF)
#define BLE_BUILD_LSB    0
#define BLE_BUILD_WIDTH  ((uint32_t)0x00000008)

#define BLE_TYP_RST      0x6
#define BLE_REL_RST      0x1
#define BLE_UPG_RST      0xC
#define BLE_BUILD_RST    0x0

__INLINE void ble_version_unpack(uint8_t* typ, uint8_t* rel, uint8_t* upg, uint8_t* build)
{
    uint32_t localVal = REG_BLE_RD(BLE_VERSION_ADDR);

    *typ = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *rel = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *upg = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *build = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

__INLINE uint8_t ble_typ_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_VERSION_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

__INLINE uint8_t ble_rel_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_VERSION_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

__INLINE uint8_t ble_upg_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_VERSION_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

__INLINE uint8_t ble_build_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_VERSION_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief RWBTLECONF register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24            ADD_WIDTH   0xA
 *  22:16                 RFIF   0x1
 *  13:08              CLK_SEL   0xD
 *     05               DMMODE   0
 *     04              INTMODE   1
 *     03                 WLAN   1
 *     02               USEDBG   1
 *     01             USECRYPT   1
 *     00             BUSWIDTH   0
 * </pre>
 */
#define BLE_RWBTLECONF_ADDR   0x40000008
#define BLE_RWBTLECONF_OFFSET 0x00000008
#define BLE_RWBTLECONF_INDEX  0x00000002
#define BLE_RWBTLECONF_RESET  0x0A010D1E

__INLINE uint32_t ble_rwbtleconf_get(void)
{
    return REG_BLE_RD(BLE_RWBTLECONF_ADDR);
}

// field definitions
#define BLE_ADD_WIDTH_MASK   ((uint32_t)0x3F000000)
#define BLE_ADD_WIDTH_LSB    24
#define BLE_ADD_WIDTH_WIDTH  ((uint32_t)0x00000006)
#define BLE_RFIF_MASK        ((uint32_t)0x007F0000)
#define BLE_RFIF_LSB         16
#define BLE_RFIF_WIDTH       ((uint32_t)0x00000007)
#define BLE_CLK_SEL_MASK     ((uint32_t)0x00003F00)
#define BLE_CLK_SEL_LSB      8
#define BLE_CLK_SEL_WIDTH    ((uint32_t)0x00000006)
#define BLE_DMMODE_BIT       ((uint32_t)0x00000020)
#define BLE_DMMODE_POS       5
#define BLE_INTMODE_BIT      ((uint32_t)0x00000010)
#define BLE_INTMODE_POS      4
#define BLE_WLAN_BIT         ((uint32_t)0x00000008)
#define BLE_WLAN_POS         3
#define BLE_USEDBG_BIT       ((uint32_t)0x00000004)
#define BLE_USEDBG_POS       2
#define BLE_USECRYPT_BIT     ((uint32_t)0x00000002)
#define BLE_USECRYPT_POS     1
#define BLE_BUSWIDTH_BIT     ((uint32_t)0x00000001)
#define BLE_BUSWIDTH_POS     0

#define BLE_ADD_WIDTH_RST    0xA
#define BLE_RFIF_RST         0x1
#define BLE_CLK_SEL_RST      0xD
#define BLE_DMMODE_RST       0x0
#define BLE_INTMODE_RST      0x1
#define BLE_WLAN_RST         0x1
#define BLE_USEDBG_RST       0x1
#define BLE_USECRYPT_RST     0x1
#define BLE_BUSWIDTH_RST     0x0

__INLINE void ble_rwbtleconf_unpack(uint8_t* addwidth, uint8_t* rfif, uint8_t* clksel, uint8_t* dmmode, uint8_t* intmode, uint8_t* wlan, uint8_t* usedbg, uint8_t* usecrypt, uint8_t* buswidth)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECONF_ADDR);

    *addwidth = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *rfif = (localVal & ((uint32_t)0x007F0000)) >> 16;
    *clksel = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *dmmode = (localVal & ((uint32_t)0x00000020)) >> 5;
    *intmode = (localVal & ((uint32_t)0x00000010)) >> 4;
    *wlan = (localVal & ((uint32_t)0x00000008)) >> 3;
    *usedbg = (localVal & ((uint32_t)0x00000004)) >> 2;
    *usecrypt = (localVal & ((uint32_t)0x00000002)) >> 1;
    *buswidth = (localVal & ((uint32_t)0x00000001)) >> 0;
}

__INLINE uint8_t ble_add_width_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECONF_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

__INLINE uint8_t ble_rfif_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECONF_ADDR);
    return ((localVal & ((uint32_t)0x007F0000)) >> 16);
}

__INLINE uint8_t ble_clk_sel_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECONF_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

__INLINE uint8_t ble_dmmode_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECONF_ADDR);
    return ((localVal & ((uint32_t)0x00000020)) >> 5);
}

__INLINE uint8_t ble_intmode_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECONF_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}

__INLINE uint8_t ble_wlan_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECONF_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}

__INLINE uint8_t ble_usedbg_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECONF_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}

__INLINE uint8_t ble_usecrypt_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECONF_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

__INLINE uint8_t ble_buswidth_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RWBTLECONF_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/**
 * @brief INTCNTL register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:16             INCSCNTL   0xFFFF
 *     15          CSCNTDEVMSK   1
 *     08      RADIOCNTLINTMSK   0
 *     07      FINETGTIMINTMSK   0
 *     06     GROSSTGTIMINTMSK   0
 *     05          ERRORINTMSK   0
 *     04          CRYPTINTMSK   1
 *     03          EVENTINTMSK   1
 *     02            SLPINTMSK   1
 *     01             RXINTMSK   1
 *     00          CSCNTINTMSK   1
 * </pre>
 */
#define BLE_INTCNTL_ADDR   0x4000000C
#define BLE_INTCNTL_OFFSET 0x0000000C
#define BLE_INTCNTL_INDEX  0x00000003
#define BLE_INTCNTL_RESET  0xFFFF801F

__INLINE uint32_t ble_intcntl_get(void)
{
    return REG_BLE_RD(BLE_INTCNTL_ADDR);
}

__INLINE void ble_intcntl_set(uint32_t value)
{
    REG_BLE_WR(BLE_INTCNTL_ADDR, value);
}

// field definitions
#define BLE_INCSCNTL_MASK           ((uint32_t)0xFFFF0000)
#define BLE_INCSCNTL_LSB            16
#define BLE_INCSCNTL_WIDTH          ((uint32_t)0x00000010)
#define BLE_CSCNTDEVMSK_BIT         ((uint32_t)0x00008000)
#define BLE_CSCNTDEVMSK_POS         15
#define BLE_RADIOCNTLINTMSK_BIT     ((uint32_t)0x00000100)
#define BLE_RADIOCNTLINTMSK_POS     8
#define BLE_FINETGTIMINTMSK_BIT     ((uint32_t)0x00000080)
#define BLE_FINETGTIMINTMSK_POS     7
#define BLE_GROSSTGTIMINTMSK_BIT    ((uint32_t)0x00000040)
#define BLE_GROSSTGTIMINTMSK_POS    6
#define BLE_ERRORINTMSK_BIT         ((uint32_t)0x00000020)
#define BLE_ERRORINTMSK_POS         5
#define BLE_CRYPTINTMSK_BIT         ((uint32_t)0x00000010)
#define BLE_CRYPTINTMSK_POS         4
#define BLE_EVENTINTMSK_BIT         ((uint32_t)0x00000008)
#define BLE_EVENTINTMSK_POS         3
#define BLE_SLPINTMSK_BIT           ((uint32_t)0x00000004)
#define BLE_SLPINTMSK_POS           2
#define BLE_RXINTMSK_BIT            ((uint32_t)0x00000002)
#define BLE_RXINTMSK_POS            1
#define BLE_CSCNTINTMSK_BIT         ((uint32_t)0x00000001)
#define BLE_CSCNTINTMSK_POS         0

#define BLE_INCSCNTL_RST            0xFFFF
#define BLE_CSCNTDEVMSK_RST         0x1
#define BLE_RADIOCNTLINTMSK_RST     0x0
#define BLE_FINETGTIMINTMSK_RST     0x0
#define BLE_GROSSTGTIMINTMSK_RST    0x0
#define BLE_ERRORINTMSK_RST         0x0
#define BLE_CRYPTINTMSK_RST         0x1
#define BLE_EVENTINTMSK_RST         0x1
#define BLE_SLPINTMSK_RST           0x1
#define BLE_RXINTMSK_RST            0x1
#define BLE_CSCNTINTMSK_RST         0x1

__INLINE void ble_intcntl_pack(uint16_t incscntl, uint8_t cscntdevmsk, uint8_t radiocntlintmsk, uint8_t finetgtimintmsk, uint8_t grosstgtimintmsk, uint8_t errorintmsk, uint8_t cryptintmsk, uint8_t eventintmsk, uint8_t slpintmsk, uint8_t rxintmsk, uint8_t cscntintmsk)
{
    ASSERT_ERR((((uint32_t)incscntl << 16) & ~((uint32_t)0xFFFF0000)) == 0);
    ASSERT_ERR((((uint32_t)cscntdevmsk << 15) & ~((uint32_t)0x00008000)) == 0);
    ASSERT_ERR((((uint32_t)radiocntlintmsk << 8) & ~((uint32_t)0x00000100)) == 0);
    ASSERT_ERR((((uint32_t)finetgtimintmsk << 7) & ~((uint32_t)0x00000080)) == 0);
    ASSERT_ERR((((uint32_t)grosstgtimintmsk << 6) & ~((uint32_t)0x00000040)) == 0);
    ASSERT_ERR((((uint32_t)errorintmsk << 5) & ~((uint32_t)0x00000020)) == 0);
    ASSERT_ERR((((uint32_t)cryptintmsk << 4) & ~((uint32_t)0x00000010)) == 0);
    ASSERT_ERR((((uint32_t)eventintmsk << 3) & ~((uint32_t)0x00000008)) == 0);
    ASSERT_ERR((((uint32_t)slpintmsk << 2) & ~((uint32_t)0x00000004)) == 0);
    ASSERT_ERR((((uint32_t)rxintmsk << 1) & ~((uint32_t)0x00000002)) == 0);
    ASSERT_ERR((((uint32_t)cscntintmsk << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_BLE_WR(BLE_INTCNTL_ADDR,  ((uint32_t)incscntl << 16) | ((uint32_t)cscntdevmsk << 15) | ((uint32_t)radiocntlintmsk << 8) | ((uint32_t)finetgtimintmsk << 7) | ((uint32_t)grosstgtimintmsk << 6) | ((uint32_t)errorintmsk << 5) | ((uint32_t)cryptintmsk << 4) | ((uint32_t)eventintmsk << 3) | ((uint32_t)slpintmsk << 2) | ((uint32_t)rxintmsk << 1) | ((uint32_t)cscntintmsk << 0));
}

__INLINE void ble_intcntl_unpack(uint16_t* incscntl, uint8_t* cscntdevmsk, uint8_t* radiocntlintmsk, uint8_t* finetgtimintmsk, uint8_t* grosstgtimintmsk, uint8_t* errorintmsk, uint8_t* cryptintmsk, uint8_t* eventintmsk, uint8_t* slpintmsk, uint8_t* rxintmsk, uint8_t* cscntintmsk)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTCNTL_ADDR);

    *incscntl = (localVal & ((uint32_t)0xFFFF0000)) >> 16;
    *cscntdevmsk = (localVal & ((uint32_t)0x00008000)) >> 15;
    *radiocntlintmsk = (localVal & ((uint32_t)0x00000100)) >> 8;
    *finetgtimintmsk = (localVal & ((uint32_t)0x00000080)) >> 7;
    *grosstgtimintmsk = (localVal & ((uint32_t)0x00000040)) >> 6;
    *errorintmsk = (localVal & ((uint32_t)0x00000020)) >> 5;
    *cryptintmsk = (localVal & ((uint32_t)0x00000010)) >> 4;
    *eventintmsk = (localVal & ((uint32_t)0x00000008)) >> 3;
    *slpintmsk = (localVal & ((uint32_t)0x00000004)) >> 2;
    *rxintmsk = (localVal & ((uint32_t)0x00000002)) >> 1;
    *cscntintmsk = (localVal & ((uint32_t)0x00000001)) >> 0;
}

__INLINE uint16_t ble_incscntl_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTCNTL_ADDR);
    return ((localVal & ((uint32_t)0xFFFF0000)) >> 16);
}

__INLINE void ble_incscntl_setf(uint16_t incscntl)
{
    ASSERT_ERR((((uint32_t)incscntl << 16) & ~((uint32_t)0xFFFF0000)) == 0);
    REG_BLE_WR(BLE_INTCNTL_ADDR, (REG_BLE_RD(BLE_INTCNTL_ADDR) & ~((uint32_t)0xFFFF0000)) | ((uint32_t)incscntl << 16));
}

__INLINE uint8_t ble_cscntdevmsk_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00008000)) >> 15);
}

__INLINE void ble_cscntdevmsk_setf(uint8_t cscntdevmsk)
{
    ASSERT_ERR((((uint32_t)cscntdevmsk << 15) & ~((uint32_t)0x00008000)) == 0);
    REG_BLE_WR(BLE_INTCNTL_ADDR, (REG_BLE_RD(BLE_INTCNTL_ADDR) & ~((uint32_t)0x00008000)) | ((uint32_t)cscntdevmsk << 15));
}

__INLINE uint8_t ble_radiocntlintmsk_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

__INLINE void ble_radiocntlintmsk_setf(uint8_t radiocntlintmsk)
{
    ASSERT_ERR((((uint32_t)radiocntlintmsk << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_BLE_WR(BLE_INTCNTL_ADDR, (REG_BLE_RD(BLE_INTCNTL_ADDR) & ~((uint32_t)0x00000100)) | ((uint32_t)radiocntlintmsk << 8));
}

__INLINE uint8_t ble_finetgtimintmsk_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000080)) >> 7);
}

__INLINE void ble_finetgtimintmsk_setf(uint8_t finetgtimintmsk)
{
    ASSERT_ERR((((uint32_t)finetgtimintmsk << 7) & ~((uint32_t)0x00000080)) == 0);
    REG_BLE_WR(BLE_INTCNTL_ADDR, (REG_BLE_RD(BLE_INTCNTL_ADDR) & ~((uint32_t)0x00000080)) | ((uint32_t)finetgtimintmsk << 7));
}

__INLINE uint8_t ble_grosstgtimintmsk_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000040)) >> 6);
}

__INLINE void ble_grosstgtimintmsk_setf(uint8_t grosstgtimintmsk)
{
    ASSERT_ERR((((uint32_t)grosstgtimintmsk << 6) & ~((uint32_t)0x00000040)) == 0);
    REG_BLE_WR(BLE_INTCNTL_ADDR, (REG_BLE_RD(BLE_INTCNTL_ADDR) & ~((uint32_t)0x00000040)) | ((uint32_t)grosstgtimintmsk << 6));
}

__INLINE uint8_t ble_errorintmsk_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000020)) >> 5);
}

__INLINE void ble_errorintmsk_setf(uint8_t errorintmsk)
{
    ASSERT_ERR((((uint32_t)errorintmsk << 5) & ~((uint32_t)0x00000020)) == 0);
    REG_BLE_WR(BLE_INTCNTL_ADDR, (REG_BLE_RD(BLE_INTCNTL_ADDR) & ~((uint32_t)0x00000020)) | ((uint32_t)errorintmsk << 5));
}

__INLINE uint8_t ble_cryptintmsk_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}

__INLINE void ble_cryptintmsk_setf(uint8_t cryptintmsk)
{
    ASSERT_ERR((((uint32_t)cryptintmsk << 4) & ~((uint32_t)0x00000010)) == 0);
    REG_BLE_WR(BLE_INTCNTL_ADDR, (REG_BLE_RD(BLE_INTCNTL_ADDR) & ~((uint32_t)0x00000010)) | ((uint32_t)cryptintmsk << 4));
}

__INLINE uint8_t ble_eventintmsk_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}

__INLINE void ble_eventintmsk_setf(uint8_t eventintmsk)
{
    ASSERT_ERR((((uint32_t)eventintmsk << 3) & ~((uint32_t)0x00000008)) == 0);
    REG_BLE_WR(BLE_INTCNTL_ADDR, (REG_BLE_RD(BLE_INTCNTL_ADDR) & ~((uint32_t)0x00000008)) | ((uint32_t)eventintmsk << 3));
}

__INLINE uint8_t ble_slpintmsk_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}

__INLINE void ble_slpintmsk_setf(uint8_t slpintmsk)
{
    ASSERT_ERR((((uint32_t)slpintmsk << 2) & ~((uint32_t)0x00000004)) == 0);
    REG_BLE_WR(BLE_INTCNTL_ADDR, (REG_BLE_RD(BLE_INTCNTL_ADDR) & ~((uint32_t)0x00000004)) | ((uint32_t)slpintmsk << 2));
}

__INLINE uint8_t ble_rxintmsk_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

__INLINE void ble_rxintmsk_setf(uint8_t rxintmsk)
{
    ASSERT_ERR((((uint32_t)rxintmsk << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_BLE_WR(BLE_INTCNTL_ADDR, (REG_BLE_RD(BLE_INTCNTL_ADDR) & ~((uint32_t)0x00000002)) | ((uint32_t)rxintmsk << 1));
}

__INLINE uint8_t ble_cscntintmsk_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

__INLINE void ble_cscntintmsk_setf(uint8_t cscntintmsk)
{
    ASSERT_ERR((((uint32_t)cscntintmsk << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_BLE_WR(BLE_INTCNTL_ADDR, (REG_BLE_RD(BLE_INTCNTL_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)cscntintmsk << 0));
}

/**
 * @brief INTSTAT register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     08     RADIOCNTLINTSTAT   0
 *     07     FINETGTIMINTSTAT   0
 *     06    GROSSTGTIMINTSTAT   0
 *     05         ERRORINTSTAT   0
 *     04         CRYPTINTSTAT   0
 *     03         EVENTINTSTAT   0
 *     02           SLPINTSTAT   0
 *     01            RXINTSTAT   0
 *     00         CSCNTINTSTAT   0
 * </pre>
 */
#define BLE_INTSTAT_ADDR   0x40000010
#define BLE_INTSTAT_OFFSET 0x00000010
#define BLE_INTSTAT_INDEX  0x00000004
#define BLE_INTSTAT_RESET  0x00000000

__INLINE uint32_t ble_intstat_get(void)
{
    return REG_BLE_RD(BLE_INTSTAT_ADDR);
}

// field definitions
#define BLE_RADIOCNTLINTSTAT_BIT     ((uint32_t)0x00000100)
#define BLE_RADIOCNTLINTSTAT_POS     8
#define BLE_FINETGTIMINTSTAT_BIT     ((uint32_t)0x00000080)
#define BLE_FINETGTIMINTSTAT_POS     7
#define BLE_GROSSTGTIMINTSTAT_BIT    ((uint32_t)0x00000040)
#define BLE_GROSSTGTIMINTSTAT_POS    6
#define BLE_ERRORINTSTAT_BIT         ((uint32_t)0x00000020)
#define BLE_ERRORINTSTAT_POS         5
#define BLE_CRYPTINTSTAT_BIT         ((uint32_t)0x00000010)
#define BLE_CRYPTINTSTAT_POS         4
#define BLE_EVENTINTSTAT_BIT         ((uint32_t)0x00000008)
#define BLE_EVENTINTSTAT_POS         3
#define BLE_SLPINTSTAT_BIT           ((uint32_t)0x00000004)
#define BLE_SLPINTSTAT_POS           2
#define BLE_RXINTSTAT_BIT            ((uint32_t)0x00000002)
#define BLE_RXINTSTAT_POS            1
#define BLE_CSCNTINTSTAT_BIT         ((uint32_t)0x00000001)
#define BLE_CSCNTINTSTAT_POS         0

#define BLE_RADIOCNTLINTSTAT_RST     0x0
#define BLE_FINETGTIMINTSTAT_RST     0x0
#define BLE_GROSSTGTIMINTSTAT_RST    0x0
#define BLE_ERRORINTSTAT_RST         0x0
#define BLE_CRYPTINTSTAT_RST         0x0
#define BLE_EVENTINTSTAT_RST         0x0
#define BLE_SLPINTSTAT_RST           0x0
#define BLE_RXINTSTAT_RST            0x0
#define BLE_CSCNTINTSTAT_RST         0x0

__INLINE void ble_intstat_unpack(uint8_t* radiocntlintstat, uint8_t* finetgtimintstat, uint8_t* grosstgtimintstat, uint8_t* errorintstat, uint8_t* cryptintstat, uint8_t* eventintstat, uint8_t* slpintstat, uint8_t* rxintstat, uint8_t* cscntintstat)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTSTAT_ADDR);

    *radiocntlintstat = (localVal & ((uint32_t)0x00000100)) >> 8;
    *finetgtimintstat = (localVal & ((uint32_t)0x00000080)) >> 7;
    *grosstgtimintstat = (localVal & ((uint32_t)0x00000040)) >> 6;
    *errorintstat = (localVal & ((uint32_t)0x00000020)) >> 5;
    *cryptintstat = (localVal & ((uint32_t)0x00000010)) >> 4;
    *eventintstat = (localVal & ((uint32_t)0x00000008)) >> 3;
    *slpintstat = (localVal & ((uint32_t)0x00000004)) >> 2;
    *rxintstat = (localVal & ((uint32_t)0x00000002)) >> 1;
    *cscntintstat = (localVal & ((uint32_t)0x00000001)) >> 0;
}

__INLINE uint8_t ble_radiocntlintstat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

__INLINE uint8_t ble_finetgtimintstat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000080)) >> 7);
}

__INLINE uint8_t ble_grosstgtimintstat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000040)) >> 6);
}

__INLINE uint8_t ble_errorintstat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000020)) >> 5);
}

__INLINE uint8_t ble_cryptintstat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}

__INLINE uint8_t ble_eventintstat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}

__INLINE uint8_t ble_slpintstat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}

__INLINE uint8_t ble_rxintstat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

__INLINE uint8_t ble_cscntintstat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/**
 * @brief INTRAWSTAT register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     08   RADIOCNTLINTRAWSTAT   0
 *     07   FINETGTIMINTRAWSTAT   0
 *     06   GROSSTGTIMINTRAWSTAT   0
 *     05      ERRORINTRAWSTAT   0
 *     04      CRYPTINTRAWSTAT   0
 *     03      EVENTINTRAWSTAT   0
 *     02        SLPINTRAWSTAT   0
 *     01         RXINTRAWSTAT   0
 *     00      CSCNTINTRAWSTAT   0
 * </pre>
 */
#define BLE_INTRAWSTAT_ADDR   0x40000014
#define BLE_INTRAWSTAT_OFFSET 0x00000014
#define BLE_INTRAWSTAT_INDEX  0x00000005
#define BLE_INTRAWSTAT_RESET  0x00000000

__INLINE uint32_t ble_intrawstat_get(void)
{
    return REG_BLE_RD(BLE_INTRAWSTAT_ADDR);
}

// field definitions
#define BLE_RADIOCNTLINTRAWSTAT_BIT     ((uint32_t)0x00000100)
#define BLE_RADIOCNTLINTRAWSTAT_POS     8
#define BLE_FINETGTIMINTRAWSTAT_BIT     ((uint32_t)0x00000080)
#define BLE_FINETGTIMINTRAWSTAT_POS     7
#define BLE_GROSSTGTIMINTRAWSTAT_BIT    ((uint32_t)0x00000040)
#define BLE_GROSSTGTIMINTRAWSTAT_POS    6
#define BLE_ERRORINTRAWSTAT_BIT         ((uint32_t)0x00000020)
#define BLE_ERRORINTRAWSTAT_POS         5
#define BLE_CRYPTINTRAWSTAT_BIT         ((uint32_t)0x00000010)
#define BLE_CRYPTINTRAWSTAT_POS         4
#define BLE_EVENTINTRAWSTAT_BIT         ((uint32_t)0x00000008)
#define BLE_EVENTINTRAWSTAT_POS         3
#define BLE_SLPINTRAWSTAT_BIT           ((uint32_t)0x00000004)
#define BLE_SLPINTRAWSTAT_POS           2
#define BLE_RXINTRAWSTAT_BIT            ((uint32_t)0x00000002)
#define BLE_RXINTRAWSTAT_POS            1
#define BLE_CSCNTINTRAWSTAT_BIT         ((uint32_t)0x00000001)
#define BLE_CSCNTINTRAWSTAT_POS         0

#define BLE_RADIOCNTLINTRAWSTAT_RST     0x0
#define BLE_FINETGTIMINTRAWSTAT_RST     0x0
#define BLE_GROSSTGTIMINTRAWSTAT_RST    0x0
#define BLE_ERRORINTRAWSTAT_RST         0x0
#define BLE_CRYPTINTRAWSTAT_RST         0x0
#define BLE_EVENTINTRAWSTAT_RST         0x0
#define BLE_SLPINTRAWSTAT_RST           0x0
#define BLE_RXINTRAWSTAT_RST            0x0
#define BLE_CSCNTINTRAWSTAT_RST         0x0

__INLINE void ble_intrawstat_unpack(uint8_t* radiocntlintrawstat, uint8_t* finetgtimintrawstat, uint8_t* grosstgtimintrawstat, uint8_t* errorintrawstat, uint8_t* cryptintrawstat, uint8_t* eventintrawstat, uint8_t* slpintrawstat, uint8_t* rxintrawstat, uint8_t* cscntintrawstat)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTRAWSTAT_ADDR);

    *radiocntlintrawstat = (localVal & ((uint32_t)0x00000100)) >> 8;
    *finetgtimintrawstat = (localVal & ((uint32_t)0x00000080)) >> 7;
    *grosstgtimintrawstat = (localVal & ((uint32_t)0x00000040)) >> 6;
    *errorintrawstat = (localVal & ((uint32_t)0x00000020)) >> 5;
    *cryptintrawstat = (localVal & ((uint32_t)0x00000010)) >> 4;
    *eventintrawstat = (localVal & ((uint32_t)0x00000008)) >> 3;
    *slpintrawstat = (localVal & ((uint32_t)0x00000004)) >> 2;
    *rxintrawstat = (localVal & ((uint32_t)0x00000002)) >> 1;
    *cscntintrawstat = (localVal & ((uint32_t)0x00000001)) >> 0;
}

__INLINE uint8_t ble_radiocntlintrawstat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTRAWSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

__INLINE uint8_t ble_finetgtimintrawstat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTRAWSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000080)) >> 7);
}

__INLINE uint8_t ble_grosstgtimintrawstat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTRAWSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000040)) >> 6);
}

__INLINE uint8_t ble_errorintrawstat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTRAWSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000020)) >> 5);
}

__INLINE uint8_t ble_cryptintrawstat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTRAWSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}

__INLINE uint8_t ble_eventintrawstat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTRAWSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}

__INLINE uint8_t ble_slpintrawstat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTRAWSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}

__INLINE uint8_t ble_rxintrawstat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTRAWSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

__INLINE uint8_t ble_cscntintrawstat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTRAWSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/**
 * @brief INTACK register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     08      RADIOCNTLINTACK   0
 *     07      FINETGTIMINTACK   0
 *     06     GROSSTGTIMINTACK   0
 *     05          ERRORINTACK   0
 *     04          CRYPTINTACK   0
 *     03          EVENTINTACK   0
 *     02            SLPINTACK   0
 *     01             RXINTACK   0
 *     00          CSCNTINTACK   0
 * </pre>
 */
#define BLE_INTACK_ADDR   0x40000018
#define BLE_INTACK_OFFSET 0x00000018
#define BLE_INTACK_INDEX  0x00000006
#define BLE_INTACK_RESET  0x00000000

__INLINE uint32_t ble_intack_get(void)
{
    return REG_BLE_RD(BLE_INTACK_ADDR);
}

__INLINE void ble_intack_clear(uint32_t value)
{
    REG_BLE_WR(BLE_INTACK_ADDR, value);
}

// field definitions
#define BLE_RADIOCNTLINTACK_BIT     ((uint32_t)0x00000100)
#define BLE_RADIOCNTLINTACK_POS     8
#define BLE_FINETGTIMINTACK_BIT     ((uint32_t)0x00000080)
#define BLE_FINETGTIMINTACK_POS     7
#define BLE_GROSSTGTIMINTACK_BIT    ((uint32_t)0x00000040)
#define BLE_GROSSTGTIMINTACK_POS    6
#define BLE_ERRORINTACK_BIT         ((uint32_t)0x00000020)
#define BLE_ERRORINTACK_POS         5
#define BLE_CRYPTINTACK_BIT         ((uint32_t)0x00000010)
#define BLE_CRYPTINTACK_POS         4
#define BLE_EVENTINTACK_BIT         ((uint32_t)0x00000008)
#define BLE_EVENTINTACK_POS         3
#define BLE_SLPINTACK_BIT           ((uint32_t)0x00000004)
#define BLE_SLPINTACK_POS           2
#define BLE_RXINTACK_BIT            ((uint32_t)0x00000002)
#define BLE_RXINTACK_POS            1
#define BLE_CSCNTINTACK_BIT         ((uint32_t)0x00000001)
#define BLE_CSCNTINTACK_POS         0

#define BLE_RADIOCNTLINTACK_RST     0x0
#define BLE_FINETGTIMINTACK_RST     0x0
#define BLE_GROSSTGTIMINTACK_RST    0x0
#define BLE_ERRORINTACK_RST         0x0
#define BLE_CRYPTINTACK_RST         0x0
#define BLE_EVENTINTACK_RST         0x0
#define BLE_SLPINTACK_RST           0x0
#define BLE_RXINTACK_RST            0x0
#define BLE_CSCNTINTACK_RST         0x0

__INLINE void ble_intack_pack(uint8_t radiocntlintack, uint8_t finetgtimintack, uint8_t grosstgtimintack, uint8_t errorintack, uint8_t cryptintack, uint8_t eventintack, uint8_t slpintack, uint8_t rxintack, uint8_t cscntintack)
{
    ASSERT_ERR((((uint32_t)radiocntlintack << 8) & ~((uint32_t)0x00000100)) == 0);
    ASSERT_ERR((((uint32_t)finetgtimintack << 7) & ~((uint32_t)0x00000080)) == 0);
    ASSERT_ERR((((uint32_t)grosstgtimintack << 6) & ~((uint32_t)0x00000040)) == 0);
    ASSERT_ERR((((uint32_t)errorintack << 5) & ~((uint32_t)0x00000020)) == 0);
    ASSERT_ERR((((uint32_t)cryptintack << 4) & ~((uint32_t)0x00000010)) == 0);
    ASSERT_ERR((((uint32_t)eventintack << 3) & ~((uint32_t)0x00000008)) == 0);
    ASSERT_ERR((((uint32_t)slpintack << 2) & ~((uint32_t)0x00000004)) == 0);
    ASSERT_ERR((((uint32_t)rxintack << 1) & ~((uint32_t)0x00000002)) == 0);
    ASSERT_ERR((((uint32_t)cscntintack << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_BLE_WR(BLE_INTACK_ADDR,  ((uint32_t)radiocntlintack << 8) | ((uint32_t)finetgtimintack << 7) | ((uint32_t)grosstgtimintack << 6) | ((uint32_t)errorintack << 5) | ((uint32_t)cryptintack << 4) | ((uint32_t)eventintack << 3) | ((uint32_t)slpintack << 2) | ((uint32_t)rxintack << 1) | ((uint32_t)cscntintack << 0));
}

__INLINE void ble_intack_unpack(uint8_t* radiocntlintack, uint8_t* finetgtimintack, uint8_t* grosstgtimintack, uint8_t* errorintack, uint8_t* cryptintack, uint8_t* eventintack, uint8_t* slpintack, uint8_t* rxintack, uint8_t* cscntintack)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTACK_ADDR);

    *radiocntlintack = (localVal & ((uint32_t)0x00000100)) >> 8;
    *finetgtimintack = (localVal & ((uint32_t)0x00000080)) >> 7;
    *grosstgtimintack = (localVal & ((uint32_t)0x00000040)) >> 6;
    *errorintack = (localVal & ((uint32_t)0x00000020)) >> 5;
    *cryptintack = (localVal & ((uint32_t)0x00000010)) >> 4;
    *eventintack = (localVal & ((uint32_t)0x00000008)) >> 3;
    *slpintack = (localVal & ((uint32_t)0x00000004)) >> 2;
    *rxintack = (localVal & ((uint32_t)0x00000002)) >> 1;
    *cscntintack = (localVal & ((uint32_t)0x00000001)) >> 0;
}

__INLINE uint8_t ble_radiocntlintack_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTACK_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

__INLINE void ble_radiocntlintack_clearf(uint8_t radiocntlintack)
{
    ASSERT_ERR((((uint32_t)radiocntlintack << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_BLE_WR(BLE_INTACK_ADDR, (uint32_t)radiocntlintack << 8);
}

__INLINE uint8_t ble_finetgtimintack_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTACK_ADDR);
    return ((localVal & ((uint32_t)0x00000080)) >> 7);
}

__INLINE void ble_finetgtimintack_clearf(uint8_t finetgtimintack)
{
    ASSERT_ERR((((uint32_t)finetgtimintack << 7) & ~((uint32_t)0x00000080)) == 0);
    REG_BLE_WR(BLE_INTACK_ADDR, (uint32_t)finetgtimintack << 7);
}

__INLINE uint8_t ble_grosstgtimintack_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTACK_ADDR);
    return ((localVal & ((uint32_t)0x00000040)) >> 6);
}

__INLINE void ble_grosstgtimintack_clearf(uint8_t grosstgtimintack)
{
    ASSERT_ERR((((uint32_t)grosstgtimintack << 6) & ~((uint32_t)0x00000040)) == 0);
    REG_BLE_WR(BLE_INTACK_ADDR, (uint32_t)grosstgtimintack << 6);
}

__INLINE uint8_t ble_errorintack_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTACK_ADDR);
    return ((localVal & ((uint32_t)0x00000020)) >> 5);
}

__INLINE void ble_errorintack_clearf(uint8_t errorintack)
{
    ASSERT_ERR((((uint32_t)errorintack << 5) & ~((uint32_t)0x00000020)) == 0);
    REG_BLE_WR(BLE_INTACK_ADDR, (uint32_t)errorintack << 5);
}

__INLINE uint8_t ble_cryptintack_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTACK_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}

__INLINE void ble_cryptintack_clearf(uint8_t cryptintack)
{
    ASSERT_ERR((((uint32_t)cryptintack << 4) & ~((uint32_t)0x00000010)) == 0);
    REG_BLE_WR(BLE_INTACK_ADDR, (uint32_t)cryptintack << 4);
}

__INLINE uint8_t ble_eventintack_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTACK_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}

__INLINE void ble_eventintack_clearf(uint8_t eventintack)
{
    ASSERT_ERR((((uint32_t)eventintack << 3) & ~((uint32_t)0x00000008)) == 0);
    REG_BLE_WR(BLE_INTACK_ADDR, (uint32_t)eventintack << 3);
}

__INLINE uint8_t ble_slpintack_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTACK_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}

__INLINE void ble_slpintack_clearf(uint8_t slpintack)
{
    ASSERT_ERR((((uint32_t)slpintack << 2) & ~((uint32_t)0x00000004)) == 0);
    REG_BLE_WR(BLE_INTACK_ADDR, (uint32_t)slpintack << 2);
}

__INLINE uint8_t ble_rxintack_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTACK_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

__INLINE void ble_rxintack_clearf(uint8_t rxintack)
{
    ASSERT_ERR((((uint32_t)rxintack << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_BLE_WR(BLE_INTACK_ADDR, (uint32_t)rxintack << 1);
}

__INLINE uint8_t ble_cscntintack_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_INTACK_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

__INLINE void ble_cscntintack_clearf(uint8_t cscntintack)
{
    ASSERT_ERR((((uint32_t)cscntintack << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_BLE_WR(BLE_INTACK_ADDR, (uint32_t)cscntintack << 0);
}

/**
 * @brief BASETIMECNT register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  26:00          BASETIMECNT   0x0
 * </pre>
 */
#define BLE_BASETIMECNT_ADDR   0x4000001C
#define BLE_BASETIMECNT_OFFSET 0x0000001C
#define BLE_BASETIMECNT_INDEX  0x00000007
#define BLE_BASETIMECNT_RESET  0x00000000

__INLINE uint32_t ble_basetimecnt_get(void)
{
    return REG_BLE_RD(BLE_BASETIMECNT_ADDR);
}

// field definitions
#define BLE_BASETIMECNT_MASK   ((uint32_t)0x07FFFFFF)
#define BLE_BASETIMECNT_LSB    0
#define BLE_BASETIMECNT_WIDTH  ((uint32_t)0x0000001B)

#define BLE_BASETIMECNT_RST    0x0

__INLINE uint32_t ble_basetimecnt_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_BASETIMECNT_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x07FFFFFF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief FINETIMECNT register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  09:00              FINECNT   0x0
 * </pre>
 */
#define BLE_FINETIMECNT_ADDR   0x40000020
#define BLE_FINETIMECNT_OFFSET 0x00000020
#define BLE_FINETIMECNT_INDEX  0x00000008
#define BLE_FINETIMECNT_RESET  0x00000000

__INLINE uint32_t ble_finetimecnt_get(void)
{
    return REG_BLE_RD(BLE_FINETIMECNT_ADDR);
}

// field definitions
#define BLE_FINECNT_MASK   ((uint32_t)0x000003FF)
#define BLE_FINECNT_LSB    0
#define BLE_FINECNT_WIDTH  ((uint32_t)0x0000000A)

#define BLE_FINECNT_RST    0x0

__INLINE uint16_t ble_finecnt_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_FINETIMECNT_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000003FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief BDADDRL register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:00              BDADDRL   0x0
 * </pre>
 */
#define BLE_BDADDRL_ADDR   0x40000024
#define BLE_BDADDRL_OFFSET 0x00000024
#define BLE_BDADDRL_INDEX  0x00000009
#define BLE_BDADDRL_RESET  0x00000000

__INLINE uint32_t ble_bdaddrl_get(void)
{
    return REG_BLE_RD(BLE_BDADDRL_ADDR);
}

__INLINE void ble_bdaddrl_set(uint32_t value)
{
    REG_BLE_WR(BLE_BDADDRL_ADDR, value);
}

// field definitions
#define BLE_BDADDRL_MASK   ((uint32_t)0xFFFFFFFF)
#define BLE_BDADDRL_LSB    0
#define BLE_BDADDRL_WIDTH  ((uint32_t)0x00000020)

#define BLE_BDADDRL_RST    0x0

__INLINE uint32_t ble_bdaddrl_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_BDADDRL_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0xFFFFFFFF)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_bdaddrl_setf(uint32_t bdaddrl)
{
    ASSERT_ERR((((uint32_t)bdaddrl << 0) & ~((uint32_t)0xFFFFFFFF)) == 0);
    REG_BLE_WR(BLE_BDADDRL_ADDR, (uint32_t)bdaddrl << 0);
}

/**
 * @brief BDADDRU register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     16            PRIV_NPUB   0
 *  15:00              BDADDRU   0x0
 * </pre>
 */
#define BLE_BDADDRU_ADDR   0x40000028
#define BLE_BDADDRU_OFFSET 0x00000028
#define BLE_BDADDRU_INDEX  0x0000000A
#define BLE_BDADDRU_RESET  0x00000000

__INLINE uint32_t ble_bdaddru_get(void)
{
    return REG_BLE_RD(BLE_BDADDRU_ADDR);
}

__INLINE void ble_bdaddru_set(uint32_t value)
{
    REG_BLE_WR(BLE_BDADDRU_ADDR, value);
}

// field definitions
#define BLE_PRIV_NPUB_BIT    ((uint32_t)0x00010000)
#define BLE_PRIV_NPUB_POS    16
#define BLE_BDADDRU_MASK     ((uint32_t)0x0000FFFF)
#define BLE_BDADDRU_LSB      0
#define BLE_BDADDRU_WIDTH    ((uint32_t)0x00000010)

#define BLE_PRIV_NPUB_RST    0x0
#define BLE_BDADDRU_RST      0x0

__INLINE void ble_bdaddru_pack(uint8_t privnpub, uint16_t bdaddru)
{
    ASSERT_ERR((((uint32_t)privnpub << 16) & ~((uint32_t)0x00010000)) == 0);
    ASSERT_ERR((((uint32_t)bdaddru << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_BLE_WR(BLE_BDADDRU_ADDR,  ((uint32_t)privnpub << 16) | ((uint32_t)bdaddru << 0));
}

__INLINE void ble_bdaddru_unpack(uint8_t* privnpub, uint16_t* bdaddru)
{
    uint32_t localVal = REG_BLE_RD(BLE_BDADDRU_ADDR);

    *privnpub = (localVal & ((uint32_t)0x00010000)) >> 16;
    *bdaddru = (localVal & ((uint32_t)0x0000FFFF)) >> 0;
}

__INLINE uint8_t ble_priv_npub_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_BDADDRU_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

__INLINE void ble_priv_npub_setf(uint8_t privnpub)
{
    ASSERT_ERR((((uint32_t)privnpub << 16) & ~((uint32_t)0x00010000)) == 0);
    REG_BLE_WR(BLE_BDADDRU_ADDR, (REG_BLE_RD(BLE_BDADDRU_ADDR) & ~((uint32_t)0x00010000)) | ((uint32_t)privnpub << 16));
}

__INLINE uint16_t ble_bdaddru_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_BDADDRU_ADDR);
    return ((localVal & ((uint32_t)0x0000FFFF)) >> 0);
}

__INLINE void ble_bdaddru_setf(uint16_t bdaddru)
{
    ASSERT_ERR((((uint32_t)bdaddru << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_BLE_WR(BLE_BDADDRU_ADDR, (REG_BLE_RD(BLE_BDADDRU_ADDR) & ~((uint32_t)0x0000FFFF)) | ((uint32_t)bdaddru << 0));
}

/**
 * @brief CURRENTRXDESCPTR register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  13:00     CURRENTRXDESCPTR   0x0
 * </pre>
 */
#define BLE_CURRENTRXDESCPTR_ADDR   0x4000002C
#define BLE_CURRENTRXDESCPTR_OFFSET 0x0000002C
#define BLE_CURRENTRXDESCPTR_INDEX  0x0000000B
#define BLE_CURRENTRXDESCPTR_RESET  0x00000000

__INLINE uint32_t ble_currentrxdescptr_get(void)
{
    return REG_BLE_RD(BLE_CURRENTRXDESCPTR_ADDR);
}

__INLINE void ble_currentrxdescptr_set(uint32_t value)
{
    REG_BLE_WR(BLE_CURRENTRXDESCPTR_ADDR, value);
}

// field definitions
#define BLE_CURRENTRXDESCPTR_MASK   ((uint32_t)0x00003FFF)
#define BLE_CURRENTRXDESCPTR_LSB    0
#define BLE_CURRENTRXDESCPTR_WIDTH  ((uint32_t)0x0000000E)

#define BLE_CURRENTRXDESCPTR_RST    0x0

__INLINE uint16_t ble_currentrxdescptr_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_CURRENTRXDESCPTR_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x00003FFF)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_currentrxdescptr_setf(uint16_t currentrxdescptr)
{
    ASSERT_ERR((((uint32_t)currentrxdescptr << 0) & ~((uint32_t)0x00003FFF)) == 0);
    REG_BLE_WR(BLE_CURRENTRXDESCPTR_ADDR, (uint32_t)currentrxdescptr << 0);
}

#if RW_BLE_SUPPORT
/**
 * @brief DEEPSLCNTL register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31           EXTWKUPDSB   0
 *     15      DEEP_SLEEP_STAT   0
 *     04      SOFT_WAKEUP_REQ   0
 *     03   DEEP_SLEEP_CORR_EN   0
 *     02        DEEP_SLEEP_ON   0
 *     01       RADIO_SLEEP_EN   0
 *     00         OSC_SLEEP_EN   0
 * </pre>
 */
#define BLE_DEEPSLCNTL_ADDR   0x40000030
#define BLE_DEEPSLCNTL_OFFSET 0x00000030
#define BLE_DEEPSLCNTL_INDEX  0x0000000C
#define BLE_DEEPSLCNTL_RESET  0x00000000

__INLINE uint32_t ble_deepslcntl_get(void)
{
    return REG_BLE_RD(BLE_DEEPSLCNTL_ADDR);
}

__INLINE void ble_deepslcntl_set(uint32_t value)
{
    REG_BLE_WR(BLE_DEEPSLCNTL_ADDR, value);
}

// field definitions
#define BLE_EXTWKUPDSB_BIT            ((uint32_t)0x80000000)
#define BLE_EXTWKUPDSB_POS            31
#define BLE_DEEP_SLEEP_STAT_BIT       ((uint32_t)0x00008000)
#define BLE_DEEP_SLEEP_STAT_POS       15
#define BLE_SOFT_WAKEUP_REQ_BIT       ((uint32_t)0x00000010)
#define BLE_SOFT_WAKEUP_REQ_POS       4
#define BLE_DEEP_SLEEP_CORR_EN_BIT    ((uint32_t)0x00000008)
#define BLE_DEEP_SLEEP_CORR_EN_POS    3
#define BLE_DEEP_SLEEP_ON_BIT         ((uint32_t)0x00000004)
#define BLE_DEEP_SLEEP_ON_POS         2
#define BLE_RADIO_SLEEP_EN_BIT        ((uint32_t)0x00000002)
#define BLE_RADIO_SLEEP_EN_POS        1
#define BLE_OSC_SLEEP_EN_BIT          ((uint32_t)0x00000001)
#define BLE_OSC_SLEEP_EN_POS          0

#define BLE_EXTWKUPDSB_RST            0x0
#define BLE_DEEP_SLEEP_STAT_RST       0x0
#define BLE_SOFT_WAKEUP_REQ_RST       0x0
#define BLE_DEEP_SLEEP_CORR_EN_RST    0x0
#define BLE_DEEP_SLEEP_ON_RST         0x0
#define BLE_RADIO_SLEEP_EN_RST        0x0
#define BLE_OSC_SLEEP_EN_RST          0x0

__INLINE void ble_deepslcntl_pack(uint8_t extwkupdsb, uint8_t softwakeupreq, uint8_t deepsleepcorren, uint8_t deepsleepon, uint8_t radiosleepen, uint8_t oscsleepen)
{
    ASSERT_ERR((((uint32_t)extwkupdsb << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)softwakeupreq << 4) & ~((uint32_t)0x00000010)) == 0);
    ASSERT_ERR((((uint32_t)deepsleepcorren << 3) & ~((uint32_t)0x00000008)) == 0);
    ASSERT_ERR((((uint32_t)deepsleepon << 2) & ~((uint32_t)0x00000004)) == 0);
    ASSERT_ERR((((uint32_t)radiosleepen << 1) & ~((uint32_t)0x00000002)) == 0);
    ASSERT_ERR((((uint32_t)oscsleepen << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_BLE_WR(BLE_DEEPSLCNTL_ADDR,  ((uint32_t)extwkupdsb << 31) | ((uint32_t)softwakeupreq << 4) | ((uint32_t)deepsleepcorren << 3) | ((uint32_t)deepsleepon << 2) | ((uint32_t)radiosleepen << 1) | ((uint32_t)oscsleepen << 0));
}

__INLINE void ble_deepslcntl_unpack(uint8_t* extwkupdsb, uint8_t* deepsleepstat, uint8_t* softwakeupreq, uint8_t* deepsleepcorren, uint8_t* deepsleepon, uint8_t* radiosleepen, uint8_t* oscsleepen)
{
    uint32_t localVal = REG_BLE_RD(BLE_DEEPSLCNTL_ADDR);

    *extwkupdsb = (localVal & ((uint32_t)0x80000000)) >> 31;
    *deepsleepstat = (localVal & ((uint32_t)0x00008000)) >> 15;
    *softwakeupreq = (localVal & ((uint32_t)0x00000010)) >> 4;
    *deepsleepcorren = (localVal & ((uint32_t)0x00000008)) >> 3;
    *deepsleepon = (localVal & ((uint32_t)0x00000004)) >> 2;
    *radiosleepen = (localVal & ((uint32_t)0x00000002)) >> 1;
    *oscsleepen = (localVal & ((uint32_t)0x00000001)) >> 0;
}

__INLINE uint8_t ble_extwkupdsb_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DEEPSLCNTL_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

__INLINE void ble_extwkupdsb_setf(uint8_t extwkupdsb)
{
    ASSERT_ERR((((uint32_t)extwkupdsb << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_BLE_WR(BLE_DEEPSLCNTL_ADDR, (REG_BLE_RD(BLE_DEEPSLCNTL_ADDR) & ~((uint32_t)0x80000000)) | ((uint32_t)extwkupdsb << 31));
}

__INLINE uint8_t ble_deep_sleep_stat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DEEPSLCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00008000)) >> 15);
}

__INLINE uint8_t ble_soft_wakeup_req_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DEEPSLCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}

__INLINE void ble_soft_wakeup_req_setf(uint8_t softwakeupreq)
{
    ASSERT_ERR((((uint32_t)softwakeupreq << 4) & ~((uint32_t)0x00000010)) == 0);
    REG_BLE_WR(BLE_DEEPSLCNTL_ADDR, (REG_BLE_RD(BLE_DEEPSLCNTL_ADDR) & ~((uint32_t)0x00000010)) | ((uint32_t)softwakeupreq << 4));
}

__INLINE uint8_t ble_deep_sleep_corr_en_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DEEPSLCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}

__INLINE void ble_deep_sleep_corr_en_setf(uint8_t deepsleepcorren)
{
    ASSERT_ERR((((uint32_t)deepsleepcorren << 3) & ~((uint32_t)0x00000008)) == 0);
    REG_BLE_WR(BLE_DEEPSLCNTL_ADDR, (REG_BLE_RD(BLE_DEEPSLCNTL_ADDR) & ~((uint32_t)0x00000008)) | ((uint32_t)deepsleepcorren << 3));
}

__INLINE uint8_t ble_deep_sleep_on_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DEEPSLCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}

__INLINE void ble_deep_sleep_on_setf(uint8_t deepsleepon)
{
    ASSERT_ERR((((uint32_t)deepsleepon << 2) & ~((uint32_t)0x00000004)) == 0);
    REG_BLE_WR(BLE_DEEPSLCNTL_ADDR, (REG_BLE_RD(BLE_DEEPSLCNTL_ADDR) & ~((uint32_t)0x00000004)) | ((uint32_t)deepsleepon << 2));
}

__INLINE uint8_t ble_radio_sleep_en_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DEEPSLCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

__INLINE void ble_radio_sleep_en_setf(uint8_t radiosleepen)
{
    ASSERT_ERR((((uint32_t)radiosleepen << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_BLE_WR(BLE_DEEPSLCNTL_ADDR, (REG_BLE_RD(BLE_DEEPSLCNTL_ADDR) & ~((uint32_t)0x00000002)) | ((uint32_t)radiosleepen << 1));
}

__INLINE uint8_t ble_osc_sleep_en_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DEEPSLCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

__INLINE void ble_osc_sleep_en_setf(uint8_t oscsleepen)
{
    ASSERT_ERR((((uint32_t)oscsleepen << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_BLE_WR(BLE_DEEPSLCNTL_ADDR, (REG_BLE_RD(BLE_DEEPSLCNTL_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)oscsleepen << 0));
}

#endif // RW_BLE_SUPPORT
#if RW_BLE_SUPPORT
/**
 * @brief DEEPSLWKUP register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:00           DEEPSLTIME   0x0
 * </pre>
 */
#define BLE_DEEPSLWKUP_ADDR   0x40000034
#define BLE_DEEPSLWKUP_OFFSET 0x00000034
#define BLE_DEEPSLWKUP_INDEX  0x0000000D
#define BLE_DEEPSLWKUP_RESET  0x00000000

__INLINE uint32_t ble_deepslwkup_get(void)
{
    return REG_BLE_RD(BLE_DEEPSLWKUP_ADDR);
}

__INLINE void ble_deepslwkup_set(uint32_t value)
{
    REG_BLE_WR(BLE_DEEPSLWKUP_ADDR, value);
}

// field definitions
#define BLE_DEEPSLTIME_MASK   ((uint32_t)0xFFFFFFFF)
#define BLE_DEEPSLTIME_LSB    0
#define BLE_DEEPSLTIME_WIDTH  ((uint32_t)0x00000020)

#define BLE_DEEPSLTIME_RST    0x0

__INLINE uint32_t ble_deepsltime_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DEEPSLWKUP_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0xFFFFFFFF)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_deepsltime_setf(uint32_t deepsltime)
{
    ASSERT_ERR((((uint32_t)deepsltime << 0) & ~((uint32_t)0xFFFFFFFF)) == 0);
    REG_BLE_WR(BLE_DEEPSLWKUP_ADDR, (uint32_t)deepsltime << 0);
}

#endif // RW_BLE_SUPPORT
#if RW_BLE_SUPPORT
/**
 * @brief DEEPSLSTAT register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:00            DEEPSLDUR   0x0
 * </pre>
 */
#define BLE_DEEPSLSTAT_ADDR   0x40000038
#define BLE_DEEPSLSTAT_OFFSET 0x00000038
#define BLE_DEEPSLSTAT_INDEX  0x0000000E
#define BLE_DEEPSLSTAT_RESET  0x00000000

__INLINE uint32_t ble_deepslstat_get(void)
{
    return REG_BLE_RD(BLE_DEEPSLSTAT_ADDR);
}

// field definitions
#define BLE_DEEPSLDUR_MASK   ((uint32_t)0xFFFFFFFF)
#define BLE_DEEPSLDUR_LSB    0
#define BLE_DEEPSLDUR_WIDTH  ((uint32_t)0x00000020)

#define BLE_DEEPSLDUR_RST    0x0

__INLINE uint32_t ble_deepsldur_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DEEPSLSTAT_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0xFFFFFFFF)) == 0);
    return (localVal >> 0);
}

#endif // RW_BLE_SUPPORT
#if RW_BLE_SUPPORT
/**
 * @brief ENBPRESET register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:21                TWEXT   0x0
 *  20:10                TWOSC   0x0
 *  09:00                 TWRM   0x0
 * </pre>
 */
#define BLE_ENBPRESET_ADDR   0x4000003C
#define BLE_ENBPRESET_OFFSET 0x0000003C
#define BLE_ENBPRESET_INDEX  0x0000000F
#define BLE_ENBPRESET_RESET  0x00000000

__INLINE uint32_t ble_enbpreset_get(void)
{
    return REG_BLE_RD(BLE_ENBPRESET_ADDR);
}

__INLINE void ble_enbpreset_set(uint32_t value)
{
    REG_BLE_WR(BLE_ENBPRESET_ADDR, value);
}

// field definitions
#define BLE_TWEXT_MASK   ((uint32_t)0xFFE00000)
#define BLE_TWEXT_LSB    21
#define BLE_TWEXT_WIDTH  ((uint32_t)0x0000000B)
#define BLE_TWOSC_MASK   ((uint32_t)0x001FFC00)
#define BLE_TWOSC_LSB    10
#define BLE_TWOSC_WIDTH  ((uint32_t)0x0000000B)
#define BLE_TWRM_MASK    ((uint32_t)0x000003FF)
#define BLE_TWRM_LSB     0
#define BLE_TWRM_WIDTH   ((uint32_t)0x0000000A)

#define BLE_TWEXT_RST    0x0
#define BLE_TWOSC_RST    0x0
#define BLE_TWRM_RST     0x0

__INLINE void ble_enbpreset_pack(uint16_t twext, uint16_t twosc, uint16_t twrm)
{
    ASSERT_ERR((((uint32_t)twext << 21) & ~((uint32_t)0xFFE00000)) == 0);
    ASSERT_ERR((((uint32_t)twosc << 10) & ~((uint32_t)0x001FFC00)) == 0);
    ASSERT_ERR((((uint32_t)twrm << 0) & ~((uint32_t)0x000003FF)) == 0);
    REG_BLE_WR(BLE_ENBPRESET_ADDR,  ((uint32_t)twext << 21) | ((uint32_t)twosc << 10) | ((uint32_t)twrm << 0));
}

__INLINE void ble_enbpreset_unpack(uint16_t* twext, uint16_t* twosc, uint16_t* twrm)
{
    uint32_t localVal = REG_BLE_RD(BLE_ENBPRESET_ADDR);

    *twext = (localVal & ((uint32_t)0xFFE00000)) >> 21;
    *twosc = (localVal & ((uint32_t)0x001FFC00)) >> 10;
    *twrm = (localVal & ((uint32_t)0x000003FF)) >> 0;
}

__INLINE uint16_t ble_twext_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_ENBPRESET_ADDR);
    return ((localVal & ((uint32_t)0xFFE00000)) >> 21);
}

__INLINE void ble_twext_setf(uint16_t twext)
{
    ASSERT_ERR((((uint32_t)twext << 21) & ~((uint32_t)0xFFE00000)) == 0);
    REG_BLE_WR(BLE_ENBPRESET_ADDR, (REG_BLE_RD(BLE_ENBPRESET_ADDR) & ~((uint32_t)0xFFE00000)) | ((uint32_t)twext << 21));
}

__INLINE uint16_t ble_twosc_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_ENBPRESET_ADDR);
    return ((localVal & ((uint32_t)0x001FFC00)) >> 10);
}

__INLINE void ble_twosc_setf(uint16_t twosc)
{
    ASSERT_ERR((((uint32_t)twosc << 10) & ~((uint32_t)0x001FFC00)) == 0);
    REG_BLE_WR(BLE_ENBPRESET_ADDR, (REG_BLE_RD(BLE_ENBPRESET_ADDR) & ~((uint32_t)0x001FFC00)) | ((uint32_t)twosc << 10));
}

__INLINE uint16_t ble_twrm_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_ENBPRESET_ADDR);
    return ((localVal & ((uint32_t)0x000003FF)) >> 0);
}

__INLINE void ble_twrm_setf(uint16_t twrm)
{
    ASSERT_ERR((((uint32_t)twrm << 0) & ~((uint32_t)0x000003FF)) == 0);
    REG_BLE_WR(BLE_ENBPRESET_ADDR, (REG_BLE_RD(BLE_ENBPRESET_ADDR) & ~((uint32_t)0x000003FF)) | ((uint32_t)twrm << 0));
}

#endif // RW_BLE_SUPPORT
#if RW_BLE_SUPPORT
/**
 * @brief FINECNTCORR register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  09:00          FINECNTCORR   0x0
 * </pre>
 */
#define BLE_FINECNTCORR_ADDR   0x40000040
#define BLE_FINECNTCORR_OFFSET 0x00000040
#define BLE_FINECNTCORR_INDEX  0x00000010
#define BLE_FINECNTCORR_RESET  0x00000000

__INLINE uint32_t ble_finecntcorr_get(void)
{
    return REG_BLE_RD(BLE_FINECNTCORR_ADDR);
}

__INLINE void ble_finecntcorr_set(uint32_t value)
{
    REG_BLE_WR(BLE_FINECNTCORR_ADDR, value);
}

// field definitions
#define BLE_FINECNTCORR_MASK   ((uint32_t)0x000003FF)
#define BLE_FINECNTCORR_LSB    0
#define BLE_FINECNTCORR_WIDTH  ((uint32_t)0x0000000A)

#define BLE_FINECNTCORR_RST    0x0

__INLINE uint16_t ble_finecntcorr_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_FINECNTCORR_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000003FF)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_finecntcorr_setf(uint16_t finecntcorr)
{
    ASSERT_ERR((((uint32_t)finecntcorr << 0) & ~((uint32_t)0x000003FF)) == 0);
    REG_BLE_WR(BLE_FINECNTCORR_ADDR, (uint32_t)finecntcorr << 0);
}

#endif // RW_BLE_SUPPORT
#if RW_BLE_SUPPORT
/**
 * @brief BASETIMECNTCORR register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  26:00      BASETIMECNTCORR   0x0
 * </pre>
 */
#define BLE_BASETIMECNTCORR_ADDR   0x40000044
#define BLE_BASETIMECNTCORR_OFFSET 0x00000044
#define BLE_BASETIMECNTCORR_INDEX  0x00000011
#define BLE_BASETIMECNTCORR_RESET  0x00000000

__INLINE uint32_t ble_basetimecntcorr_get(void)
{
    return REG_BLE_RD(BLE_BASETIMECNTCORR_ADDR);
}

__INLINE void ble_basetimecntcorr_set(uint32_t value)
{
    REG_BLE_WR(BLE_BASETIMECNTCORR_ADDR, value);
}

// field definitions
#define BLE_BASETIMECNTCORR_MASK   ((uint32_t)0x07FFFFFF)
#define BLE_BASETIMECNTCORR_LSB    0
#define BLE_BASETIMECNTCORR_WIDTH  ((uint32_t)0x0000001B)

#define BLE_BASETIMECNTCORR_RST    0x0

__INLINE uint32_t ble_basetimecntcorr_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_BASETIMECNTCORR_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x07FFFFFF)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_basetimecntcorr_setf(uint32_t basetimecntcorr)
{
    ASSERT_ERR((((uint32_t)basetimecntcorr << 0) & ~((uint32_t)0x07FFFFFF)) == 0);
    REG_BLE_WR(BLE_BASETIMECNTCORR_ADDR, (uint32_t)basetimecntcorr << 0);
}

#endif // RW_BLE_SUPPORT
/**
 * @brief DIAGCNTL register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31             DIAG3_EN   0
 *  29:24                DIAG3   0x0
 *     23             DIAG2_EN   0
 *  21:16                DIAG2   0x0
 *     15             DIAG1_EN   0
 *  13:08                DIAG1   0x0
 *     07             DIAG0_EN   0
 *  05:00                DIAG0   0x0
 * </pre>
 */
#define BLE_DIAGCNTL_ADDR   0x40000050
#define BLE_DIAGCNTL_OFFSET 0x00000050
#define BLE_DIAGCNTL_INDEX  0x00000014
#define BLE_DIAGCNTL_RESET  0x00000000

__INLINE uint32_t ble_diagcntl_get(void)
{
    return REG_BLE_RD(BLE_DIAGCNTL_ADDR);
}

__INLINE void ble_diagcntl_set(uint32_t value)
{
    REG_BLE_WR(BLE_DIAGCNTL_ADDR, value);
}

// field definitions
#define BLE_DIAG3_EN_BIT    ((uint32_t)0x80000000)
#define BLE_DIAG3_EN_POS    31
#define BLE_DIAG3_MASK      ((uint32_t)0x3F000000)
#define BLE_DIAG3_LSB       24
#define BLE_DIAG3_WIDTH     ((uint32_t)0x00000006)
#define BLE_DIAG2_EN_BIT    ((uint32_t)0x00800000)
#define BLE_DIAG2_EN_POS    23
#define BLE_DIAG2_MASK      ((uint32_t)0x003F0000)
#define BLE_DIAG2_LSB       16
#define BLE_DIAG2_WIDTH     ((uint32_t)0x00000006)
#define BLE_DIAG1_EN_BIT    ((uint32_t)0x00008000)
#define BLE_DIAG1_EN_POS    15
#define BLE_DIAG1_MASK      ((uint32_t)0x00003F00)
#define BLE_DIAG1_LSB       8
#define BLE_DIAG1_WIDTH     ((uint32_t)0x00000006)
#define BLE_DIAG0_EN_BIT    ((uint32_t)0x00000080)
#define BLE_DIAG0_EN_POS    7
#define BLE_DIAG0_MASK      ((uint32_t)0x0000003F)
#define BLE_DIAG0_LSB       0
#define BLE_DIAG0_WIDTH     ((uint32_t)0x00000006)

#define BLE_DIAG3_EN_RST    0x0
#define BLE_DIAG3_RST       0x0
#define BLE_DIAG2_EN_RST    0x0
#define BLE_DIAG2_RST       0x0
#define BLE_DIAG1_EN_RST    0x0
#define BLE_DIAG1_RST       0x0
#define BLE_DIAG0_EN_RST    0x0
#define BLE_DIAG0_RST       0x0

__INLINE void ble_diagcntl_pack(uint8_t diag3en, uint8_t diag3, uint8_t diag2en, uint8_t diag2, uint8_t diag1en, uint8_t diag1, uint8_t diag0en, uint8_t diag0)
{
    ASSERT_ERR((((uint32_t)diag3en << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)diag3 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)diag2en << 23) & ~((uint32_t)0x00800000)) == 0);
    ASSERT_ERR((((uint32_t)diag2 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)diag1en << 15) & ~((uint32_t)0x00008000)) == 0);
    ASSERT_ERR((((uint32_t)diag1 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)diag0en << 7) & ~((uint32_t)0x00000080)) == 0);
    ASSERT_ERR((((uint32_t)diag0 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_BLE_WR(BLE_DIAGCNTL_ADDR,  ((uint32_t)diag3en << 31) | ((uint32_t)diag3 << 24) | ((uint32_t)diag2en << 23) | ((uint32_t)diag2 << 16) | ((uint32_t)diag1en << 15) | ((uint32_t)diag1 << 8) | ((uint32_t)diag0en << 7) | ((uint32_t)diag0 << 0));
}

__INLINE void ble_diagcntl_unpack(uint8_t* diag3en, uint8_t* diag3, uint8_t* diag2en, uint8_t* diag2, uint8_t* diag1en, uint8_t* diag1, uint8_t* diag0en, uint8_t* diag0)
{
    uint32_t localVal = REG_BLE_RD(BLE_DIAGCNTL_ADDR);

    *diag3en = (localVal & ((uint32_t)0x80000000)) >> 31;
    *diag3 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *diag2en = (localVal & ((uint32_t)0x00800000)) >> 23;
    *diag2 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *diag1en = (localVal & ((uint32_t)0x00008000)) >> 15;
    *diag1 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *diag0en = (localVal & ((uint32_t)0x00000080)) >> 7;
    *diag0 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

__INLINE uint8_t ble_diag3_en_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DIAGCNTL_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

__INLINE void ble_diag3_en_setf(uint8_t diag3en)
{
    ASSERT_ERR((((uint32_t)diag3en << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_BLE_WR(BLE_DIAGCNTL_ADDR, (REG_BLE_RD(BLE_DIAGCNTL_ADDR) & ~((uint32_t)0x80000000)) | ((uint32_t)diag3en << 31));
}

__INLINE uint8_t ble_diag3_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DIAGCNTL_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

__INLINE void ble_diag3_setf(uint8_t diag3)
{
    ASSERT_ERR((((uint32_t)diag3 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_BLE_WR(BLE_DIAGCNTL_ADDR, (REG_BLE_RD(BLE_DIAGCNTL_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)diag3 << 24));
}

__INLINE uint8_t ble_diag2_en_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DIAGCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00800000)) >> 23);
}

__INLINE void ble_diag2_en_setf(uint8_t diag2en)
{
    ASSERT_ERR((((uint32_t)diag2en << 23) & ~((uint32_t)0x00800000)) == 0);
    REG_BLE_WR(BLE_DIAGCNTL_ADDR, (REG_BLE_RD(BLE_DIAGCNTL_ADDR) & ~((uint32_t)0x00800000)) | ((uint32_t)diag2en << 23));
}

__INLINE uint8_t ble_diag2_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DIAGCNTL_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

__INLINE void ble_diag2_setf(uint8_t diag2)
{
    ASSERT_ERR((((uint32_t)diag2 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_BLE_WR(BLE_DIAGCNTL_ADDR, (REG_BLE_RD(BLE_DIAGCNTL_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)diag2 << 16));
}

__INLINE uint8_t ble_diag1_en_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DIAGCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00008000)) >> 15);
}

__INLINE void ble_diag1_en_setf(uint8_t diag1en)
{
    ASSERT_ERR((((uint32_t)diag1en << 15) & ~((uint32_t)0x00008000)) == 0);
    REG_BLE_WR(BLE_DIAGCNTL_ADDR, (REG_BLE_RD(BLE_DIAGCNTL_ADDR) & ~((uint32_t)0x00008000)) | ((uint32_t)diag1en << 15));
}

__INLINE uint8_t ble_diag1_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DIAGCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

__INLINE void ble_diag1_setf(uint8_t diag1)
{
    ASSERT_ERR((((uint32_t)diag1 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_BLE_WR(BLE_DIAGCNTL_ADDR, (REG_BLE_RD(BLE_DIAGCNTL_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)diag1 << 8));
}

__INLINE uint8_t ble_diag0_en_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DIAGCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000080)) >> 7);
}

__INLINE void ble_diag0_en_setf(uint8_t diag0en)
{
    ASSERT_ERR((((uint32_t)diag0en << 7) & ~((uint32_t)0x00000080)) == 0);
    REG_BLE_WR(BLE_DIAGCNTL_ADDR, (REG_BLE_RD(BLE_DIAGCNTL_ADDR) & ~((uint32_t)0x00000080)) | ((uint32_t)diag0en << 7));
}

__INLINE uint8_t ble_diag0_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DIAGCNTL_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

__INLINE void ble_diag0_setf(uint8_t diag0)
{
    ASSERT_ERR((((uint32_t)diag0 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_BLE_WR(BLE_DIAGCNTL_ADDR, (REG_BLE_RD(BLE_DIAGCNTL_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)diag0 << 0));
}

/**
 * @brief DIAGSTAT register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24            DIAG3STAT   0x0
 *  23:16            DIAG2STAT   0x0
 *  15:08            DIAG1STAT   0x0
 *  07:00            DIAG0STAT   0x0
 * </pre>
 */
#define BLE_DIAGSTAT_ADDR   0x40000054
#define BLE_DIAGSTAT_OFFSET 0x00000054
#define BLE_DIAGSTAT_INDEX  0x00000015
#define BLE_DIAGSTAT_RESET  0x00000000

__INLINE uint32_t ble_diagstat_get(void)
{
    return REG_BLE_RD(BLE_DIAGSTAT_ADDR);
}

// field definitions
#define BLE_DIAG3STAT_MASK   ((uint32_t)0xFF000000)
#define BLE_DIAG3STAT_LSB    24
#define BLE_DIAG3STAT_WIDTH  ((uint32_t)0x00000008)
#define BLE_DIAG2STAT_MASK   ((uint32_t)0x00FF0000)
#define BLE_DIAG2STAT_LSB    16
#define BLE_DIAG2STAT_WIDTH  ((uint32_t)0x00000008)
#define BLE_DIAG1STAT_MASK   ((uint32_t)0x0000FF00)
#define BLE_DIAG1STAT_LSB    8
#define BLE_DIAG1STAT_WIDTH  ((uint32_t)0x00000008)
#define BLE_DIAG0STAT_MASK   ((uint32_t)0x000000FF)
#define BLE_DIAG0STAT_LSB    0
#define BLE_DIAG0STAT_WIDTH  ((uint32_t)0x00000008)

#define BLE_DIAG3STAT_RST    0x0
#define BLE_DIAG2STAT_RST    0x0
#define BLE_DIAG1STAT_RST    0x0
#define BLE_DIAG0STAT_RST    0x0

__INLINE void ble_diagstat_unpack(uint8_t* diag3stat, uint8_t* diag2stat, uint8_t* diag1stat, uint8_t* diag0stat)
{
    uint32_t localVal = REG_BLE_RD(BLE_DIAGSTAT_ADDR);

    *diag3stat = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *diag2stat = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *diag1stat = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *diag0stat = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

__INLINE uint8_t ble_diag3stat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DIAGSTAT_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

__INLINE uint8_t ble_diag2stat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DIAGSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

__INLINE uint8_t ble_diag1stat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DIAGSTAT_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

__INLINE uint8_t ble_diag0stat_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DIAGSTAT_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief DEBUGADDMAX register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  15:00               ADDMAX   0x0
 * </pre>
 */
#define BLE_DEBUGADDMAX_ADDR   0x40000058
#define BLE_DEBUGADDMAX_OFFSET 0x00000058
#define BLE_DEBUGADDMAX_INDEX  0x00000016
#define BLE_DEBUGADDMAX_RESET  0x00000000

__INLINE uint32_t ble_debugaddmax_get(void)
{
    return REG_BLE_RD(BLE_DEBUGADDMAX_ADDR);
}

__INLINE void ble_debugaddmax_set(uint32_t value)
{
    REG_BLE_WR(BLE_DEBUGADDMAX_ADDR, value);
}

// field definitions
#define BLE_ADDMAX_MASK   ((uint32_t)0x0000FFFF)
#define BLE_ADDMAX_LSB    0
#define BLE_ADDMAX_WIDTH  ((uint32_t)0x00000010)

#define BLE_ADDMAX_RST    0x0

__INLINE uint16_t ble_addmax_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DEBUGADDMAX_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0000FFFF)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_addmax_setf(uint16_t addmax)
{
    ASSERT_ERR((((uint32_t)addmax << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_BLE_WR(BLE_DEBUGADDMAX_ADDR, (uint32_t)addmax << 0);
}

/**
 * @brief DEBUGADDMIN register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  15:00               ADDMIN   0x0
 * </pre>
 */
#define BLE_DEBUGADDMIN_ADDR   0x4000005C
#define BLE_DEBUGADDMIN_OFFSET 0x0000005C
#define BLE_DEBUGADDMIN_INDEX  0x00000017
#define BLE_DEBUGADDMIN_RESET  0x00000000

__INLINE uint32_t ble_debugaddmin_get(void)
{
    return REG_BLE_RD(BLE_DEBUGADDMIN_ADDR);
}

__INLINE void ble_debugaddmin_set(uint32_t value)
{
    REG_BLE_WR(BLE_DEBUGADDMIN_ADDR, value);
}

// field definitions
#define BLE_ADDMIN_MASK   ((uint32_t)0x0000FFFF)
#define BLE_ADDMIN_LSB    0
#define BLE_ADDMIN_WIDTH  ((uint32_t)0x00000010)

#define BLE_ADDMIN_RST    0x0

__INLINE uint16_t ble_addmin_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_DEBUGADDMIN_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0000FFFF)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_addmin_setf(uint16_t addmin)
{
    ASSERT_ERR((((uint32_t)addmin << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_BLE_WR(BLE_DEBUGADDMIN_ADDR, (uint32_t)addmin << 0);
}

/**
 * @brief ERRORTYPESTAT register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     11       CSFORMAT_ERROR   0
 *     10        CSTXPTR_ERROR   0
 *     08    RADIO_EMACC_ERROR   0
 *     07        LLCHMAP_ERROR   0
 *     06         IFS_UNDERRUN   0
 *     05        RXCRYPT_ERROR   0
 *     04      WHITELIST_ERROR   0
 *     03           APFM_ERROR   0
 *     02         TXDESC_ERROR   0
 *     01   PKTCNTL_EMACC_ERROR   0
 *     00        TXCRYPT_ERROR   0
 * </pre>
 */
#define BLE_ERRORTYPESTAT_ADDR   0x40000060
#define BLE_ERRORTYPESTAT_OFFSET 0x00000060
#define BLE_ERRORTYPESTAT_INDEX  0x00000018
#define BLE_ERRORTYPESTAT_RESET  0x00000000

__INLINE uint32_t ble_errortypestat_get(void)
{
    return REG_BLE_RD(BLE_ERRORTYPESTAT_ADDR);
}

// field definitions
#define BLE_CSFORMAT_ERROR_BIT         ((uint32_t)0x00000800)
#define BLE_CSFORMAT_ERROR_POS         11
#define BLE_CSTXPTR_ERROR_BIT          ((uint32_t)0x00000400)
#define BLE_CSTXPTR_ERROR_POS          10
#define BLE_RADIO_EMACC_ERROR_BIT      ((uint32_t)0x00000100)
#define BLE_RADIO_EMACC_ERROR_POS      8
#define BLE_LLCHMAP_ERROR_BIT          ((uint32_t)0x00000080)
#define BLE_LLCHMAP_ERROR_POS          7
#define BLE_IFS_UNDERRUN_BIT           ((uint32_t)0x00000040)
#define BLE_IFS_UNDERRUN_POS           6
#define BLE_RXCRYPT_ERROR_BIT          ((uint32_t)0x00000020)
#define BLE_RXCRYPT_ERROR_POS          5
#define BLE_WHITELIST_ERROR_BIT        ((uint32_t)0x00000010)
#define BLE_WHITELIST_ERROR_POS        4
#define BLE_APFM_ERROR_BIT             ((uint32_t)0x00000008)
#define BLE_APFM_ERROR_POS             3
#define BLE_TXDESC_ERROR_BIT           ((uint32_t)0x00000004)
#define BLE_TXDESC_ERROR_POS           2
#define BLE_PKTCNTL_EMACC_ERROR_BIT    ((uint32_t)0x00000002)
#define BLE_PKTCNTL_EMACC_ERROR_POS    1
#define BLE_TXCRYPT_ERROR_BIT          ((uint32_t)0x00000001)
#define BLE_TXCRYPT_ERROR_POS          0

#define BLE_CSFORMAT_ERROR_RST         0x0
#define BLE_CSTXPTR_ERROR_RST          0x0
#define BLE_RADIO_EMACC_ERROR_RST      0x0
#define BLE_LLCHMAP_ERROR_RST          0x0
#define BLE_IFS_UNDERRUN_RST           0x0
#define BLE_RXCRYPT_ERROR_RST          0x0
#define BLE_WHITELIST_ERROR_RST        0x0
#define BLE_APFM_ERROR_RST             0x0
#define BLE_TXDESC_ERROR_RST           0x0
#define BLE_PKTCNTL_EMACC_ERROR_RST    0x0
#define BLE_TXCRYPT_ERROR_RST          0x0

__INLINE void ble_errortypestat_unpack(uint8_t* csformaterror, uint8_t* cstxptrerror, uint8_t* radioemaccerror, uint8_t* llchmaperror, uint8_t* ifsunderrun, uint8_t* rxcrypterror, uint8_t* whitelisterror, uint8_t* apfmerror, uint8_t* txdescerror, uint8_t* pktcntlemaccerror, uint8_t* txcrypterror)
{
    uint32_t localVal = REG_BLE_RD(BLE_ERRORTYPESTAT_ADDR);

    *csformaterror = (localVal & ((uint32_t)0x00000800)) >> 11;
    *cstxptrerror = (localVal & ((uint32_t)0x00000400)) >> 10;
    *radioemaccerror = (localVal & ((uint32_t)0x00000100)) >> 8;
    *llchmaperror = (localVal & ((uint32_t)0x00000080)) >> 7;
    *ifsunderrun = (localVal & ((uint32_t)0x00000040)) >> 6;
    *rxcrypterror = (localVal & ((uint32_t)0x00000020)) >> 5;
    *whitelisterror = (localVal & ((uint32_t)0x00000010)) >> 4;
    *apfmerror = (localVal & ((uint32_t)0x00000008)) >> 3;
    *txdescerror = (localVal & ((uint32_t)0x00000004)) >> 2;
    *pktcntlemaccerror = (localVal & ((uint32_t)0x00000002)) >> 1;
    *txcrypterror = (localVal & ((uint32_t)0x00000001)) >> 0;
}

__INLINE uint8_t ble_csformat_error_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_ERRORTYPESTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000800)) >> 11);
}

__INLINE uint8_t ble_cstxptr_error_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_ERRORTYPESTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000400)) >> 10);
}

__INLINE uint8_t ble_radio_emacc_error_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_ERRORTYPESTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

__INLINE uint8_t ble_llchmap_error_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_ERRORTYPESTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000080)) >> 7);
}

__INLINE uint8_t ble_ifs_underrun_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_ERRORTYPESTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000040)) >> 6);
}

__INLINE uint8_t ble_rxcrypt_error_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_ERRORTYPESTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000020)) >> 5);
}

__INLINE uint8_t ble_whitelist_error_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_ERRORTYPESTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}

__INLINE uint8_t ble_apfm_error_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_ERRORTYPESTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}

__INLINE uint8_t ble_txdesc_error_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_ERRORTYPESTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}

__INLINE uint8_t ble_pktcntl_emacc_error_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_ERRORTYPESTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

__INLINE uint8_t ble_txcrypt_error_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_ERRORTYPESTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/**
 * @brief SWPROFILING register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31             SWPROF31   0
 *     30             SWPROF30   0
 *     29             SWPROF29   0
 *     28             SWPROF28   0
 *     27             SWPROF27   0
 *     26             SWPROF26   0
 *     25             SWPROF25   0
 *     24             SWPROF24   0
 *     23             SWPROF23   0
 *     22             SWPROF22   0
 *     21             SWPROF21   0
 *     20             SWPROF20   0
 *     19             SWPROF19   0
 *     18             SWPROF18   0
 *     17             SWPROF17   0
 *     16             SWPROF16   0
 *     15             SWPROF15   0
 *     14             SWPROF14   0
 *     13             SWPROF13   0
 *     12             SWPROF12   0
 *     11             SWPROF11   0
 *     10             SWPROF10   0
 *     09              SWPROF9   0
 *     08              SWPROF8   0
 *     07              SWPROF7   0
 *     06              SWPROF6   0
 *     05              SWPROF5   0
 *     04              SWPROF4   0
 *     03              SWPROF3   0
 *     02              SWPROF2   0
 *     01              SWPROF1   0
 *     00              SWPROF0   0
 * </pre>
 */
#define BLE_SWPROFILING_ADDR   0x40000064
#define BLE_SWPROFILING_OFFSET 0x00000064
#define BLE_SWPROFILING_INDEX  0x00000019
#define BLE_SWPROFILING_RESET  0x00000000

__INLINE uint32_t ble_swprofiling_get(void)
{
    return REG_BLE_RD(BLE_SWPROFILING_ADDR);
}

__INLINE void ble_swprofiling_set(uint32_t value)
{
    REG_BLE_WR(BLE_SWPROFILING_ADDR, value);
}

// field definitions
#define BLE_SWPROF31_BIT    ((uint32_t)0x80000000)
#define BLE_SWPROF31_POS    31
#define BLE_SWPROF30_BIT    ((uint32_t)0x40000000)
#define BLE_SWPROF30_POS    30
#define BLE_SWPROF29_BIT    ((uint32_t)0x20000000)
#define BLE_SWPROF29_POS    29
#define BLE_SWPROF28_BIT    ((uint32_t)0x10000000)
#define BLE_SWPROF28_POS    28
#define BLE_SWPROF27_BIT    ((uint32_t)0x08000000)
#define BLE_SWPROF27_POS    27
#define BLE_SWPROF26_BIT    ((uint32_t)0x04000000)
#define BLE_SWPROF26_POS    26
#define BLE_SWPROF25_BIT    ((uint32_t)0x02000000)
#define BLE_SWPROF25_POS    25
#define BLE_SWPROF24_BIT    ((uint32_t)0x01000000)
#define BLE_SWPROF24_POS    24
#define BLE_SWPROF23_BIT    ((uint32_t)0x00800000)
#define BLE_SWPROF23_POS    23
#define BLE_SWPROF22_BIT    ((uint32_t)0x00400000)
#define BLE_SWPROF22_POS    22
#define BLE_SWPROF21_BIT    ((uint32_t)0x00200000)
#define BLE_SWPROF21_POS    21
#define BLE_SWPROF20_BIT    ((uint32_t)0x00100000)
#define BLE_SWPROF20_POS    20
#define BLE_SWPROF19_BIT    ((uint32_t)0x00080000)
#define BLE_SWPROF19_POS    19
#define BLE_SWPROF18_BIT    ((uint32_t)0x00040000)
#define BLE_SWPROF18_POS    18
#define BLE_SWPROF17_BIT    ((uint32_t)0x00020000)
#define BLE_SWPROF17_POS    17
#define BLE_SWPROF16_BIT    ((uint32_t)0x00010000)
#define BLE_SWPROF16_POS    16
#define BLE_SWPROF15_BIT    ((uint32_t)0x00008000)
#define BLE_SWPROF15_POS    15
#define BLE_SWPROF14_BIT    ((uint32_t)0x00004000)
#define BLE_SWPROF14_POS    14
#define BLE_SWPROF13_BIT    ((uint32_t)0x00002000)
#define BLE_SWPROF13_POS    13
#define BLE_SWPROF12_BIT    ((uint32_t)0x00001000)
#define BLE_SWPROF12_POS    12
#define BLE_SWPROF11_BIT    ((uint32_t)0x00000800)
#define BLE_SWPROF11_POS    11
#define BLE_SWPROF10_BIT    ((uint32_t)0x00000400)
#define BLE_SWPROF10_POS    10
#define BLE_SWPROF9_BIT     ((uint32_t)0x00000200)
#define BLE_SWPROF9_POS     9
#define BLE_SWPROF8_BIT     ((uint32_t)0x00000100)
#define BLE_SWPROF8_POS     8
#define BLE_SWPROF7_BIT     ((uint32_t)0x00000080)
#define BLE_SWPROF7_POS     7
#define BLE_SWPROF6_BIT     ((uint32_t)0x00000040)
#define BLE_SWPROF6_POS     6
#define BLE_SWPROF5_BIT     ((uint32_t)0x00000020)
#define BLE_SWPROF5_POS     5
#define BLE_SWPROF4_BIT     ((uint32_t)0x00000010)
#define BLE_SWPROF4_POS     4
#define BLE_SWPROF3_BIT     ((uint32_t)0x00000008)
#define BLE_SWPROF3_POS     3
#define BLE_SWPROF2_BIT     ((uint32_t)0x00000004)
#define BLE_SWPROF2_POS     2
#define BLE_SWPROF1_BIT     ((uint32_t)0x00000002)
#define BLE_SWPROF1_POS     1
#define BLE_SWPROF0_BIT     ((uint32_t)0x00000001)
#define BLE_SWPROF0_POS     0

#define BLE_SWPROF31_RST    0x0
#define BLE_SWPROF30_RST    0x0
#define BLE_SWPROF29_RST    0x0
#define BLE_SWPROF28_RST    0x0
#define BLE_SWPROF27_RST    0x0
#define BLE_SWPROF26_RST    0x0
#define BLE_SWPROF25_RST    0x0
#define BLE_SWPROF24_RST    0x0
#define BLE_SWPROF23_RST    0x0
#define BLE_SWPROF22_RST    0x0
#define BLE_SWPROF21_RST    0x0
#define BLE_SWPROF20_RST    0x0
#define BLE_SWPROF19_RST    0x0
#define BLE_SWPROF18_RST    0x0
#define BLE_SWPROF17_RST    0x0
#define BLE_SWPROF16_RST    0x0
#define BLE_SWPROF15_RST    0x0
#define BLE_SWPROF14_RST    0x0
#define BLE_SWPROF13_RST    0x0
#define BLE_SWPROF12_RST    0x0
#define BLE_SWPROF11_RST    0x0
#define BLE_SWPROF10_RST    0x0
#define BLE_SWPROF9_RST     0x0
#define BLE_SWPROF8_RST     0x0
#define BLE_SWPROF7_RST     0x0
#define BLE_SWPROF6_RST     0x0
#define BLE_SWPROF5_RST     0x0
#define BLE_SWPROF4_RST     0x0
#define BLE_SWPROF3_RST     0x0
#define BLE_SWPROF2_RST     0x0
#define BLE_SWPROF1_RST     0x0
#define BLE_SWPROF0_RST     0x0

__INLINE void ble_swprofiling_pack(uint8_t swprof31, uint8_t swprof30, uint8_t swprof29, uint8_t swprof28, uint8_t swprof27, uint8_t swprof26, uint8_t swprof25, uint8_t swprof24, uint8_t swprof23, uint8_t swprof22, uint8_t swprof21, uint8_t swprof20, uint8_t swprof19, uint8_t swprof18, uint8_t swprof17, uint8_t swprof16, uint8_t swprof15, uint8_t swprof14, uint8_t swprof13, uint8_t swprof12, uint8_t swprof11, uint8_t swprof10, uint8_t swprof9, uint8_t swprof8, uint8_t swprof7, uint8_t swprof6, uint8_t swprof5, uint8_t swprof4, uint8_t swprof3, uint8_t swprof2, uint8_t swprof1, uint8_t swprof0)
{
    ASSERT_ERR((((uint32_t)swprof31 << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)swprof30 << 30) & ~((uint32_t)0x40000000)) == 0);
    ASSERT_ERR((((uint32_t)swprof29 << 29) & ~((uint32_t)0x20000000)) == 0);
    ASSERT_ERR((((uint32_t)swprof28 << 28) & ~((uint32_t)0x10000000)) == 0);
    ASSERT_ERR((((uint32_t)swprof27 << 27) & ~((uint32_t)0x08000000)) == 0);
    ASSERT_ERR((((uint32_t)swprof26 << 26) & ~((uint32_t)0x04000000)) == 0);
    ASSERT_ERR((((uint32_t)swprof25 << 25) & ~((uint32_t)0x02000000)) == 0);
    ASSERT_ERR((((uint32_t)swprof24 << 24) & ~((uint32_t)0x01000000)) == 0);
    ASSERT_ERR((((uint32_t)swprof23 << 23) & ~((uint32_t)0x00800000)) == 0);
    ASSERT_ERR((((uint32_t)swprof22 << 22) & ~((uint32_t)0x00400000)) == 0);
    ASSERT_ERR((((uint32_t)swprof21 << 21) & ~((uint32_t)0x00200000)) == 0);
    ASSERT_ERR((((uint32_t)swprof20 << 20) & ~((uint32_t)0x00100000)) == 0);
    ASSERT_ERR((((uint32_t)swprof19 << 19) & ~((uint32_t)0x00080000)) == 0);
    ASSERT_ERR((((uint32_t)swprof18 << 18) & ~((uint32_t)0x00040000)) == 0);
    ASSERT_ERR((((uint32_t)swprof17 << 17) & ~((uint32_t)0x00020000)) == 0);
    ASSERT_ERR((((uint32_t)swprof16 << 16) & ~((uint32_t)0x00010000)) == 0);
    ASSERT_ERR((((uint32_t)swprof15 << 15) & ~((uint32_t)0x00008000)) == 0);
    ASSERT_ERR((((uint32_t)swprof14 << 14) & ~((uint32_t)0x00004000)) == 0);
    ASSERT_ERR((((uint32_t)swprof13 << 13) & ~((uint32_t)0x00002000)) == 0);
    ASSERT_ERR((((uint32_t)swprof12 << 12) & ~((uint32_t)0x00001000)) == 0);
    ASSERT_ERR((((uint32_t)swprof11 << 11) & ~((uint32_t)0x00000800)) == 0);
    ASSERT_ERR((((uint32_t)swprof10 << 10) & ~((uint32_t)0x00000400)) == 0);
    ASSERT_ERR((((uint32_t)swprof9 << 9) & ~((uint32_t)0x00000200)) == 0);
    ASSERT_ERR((((uint32_t)swprof8 << 8) & ~((uint32_t)0x00000100)) == 0);
    ASSERT_ERR((((uint32_t)swprof7 << 7) & ~((uint32_t)0x00000080)) == 0);
    ASSERT_ERR((((uint32_t)swprof6 << 6) & ~((uint32_t)0x00000040)) == 0);
    ASSERT_ERR((((uint32_t)swprof5 << 5) & ~((uint32_t)0x00000020)) == 0);
    ASSERT_ERR((((uint32_t)swprof4 << 4) & ~((uint32_t)0x00000010)) == 0);
    ASSERT_ERR((((uint32_t)swprof3 << 3) & ~((uint32_t)0x00000008)) == 0);
    ASSERT_ERR((((uint32_t)swprof2 << 2) & ~((uint32_t)0x00000004)) == 0);
    ASSERT_ERR((((uint32_t)swprof1 << 1) & ~((uint32_t)0x00000002)) == 0);
    ASSERT_ERR((((uint32_t)swprof0 << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR,  ((uint32_t)swprof31 << 31) | ((uint32_t)swprof30 << 30) | ((uint32_t)swprof29 << 29) | ((uint32_t)swprof28 << 28) | ((uint32_t)swprof27 << 27) | ((uint32_t)swprof26 << 26) | ((uint32_t)swprof25 << 25) | ((uint32_t)swprof24 << 24) | ((uint32_t)swprof23 << 23) | ((uint32_t)swprof22 << 22) | ((uint32_t)swprof21 << 21) | ((uint32_t)swprof20 << 20) | ((uint32_t)swprof19 << 19) | ((uint32_t)swprof18 << 18) | ((uint32_t)swprof17 << 17) | ((uint32_t)swprof16 << 16) | ((uint32_t)swprof15 << 15) | ((uint32_t)swprof14 << 14) | ((uint32_t)swprof13 << 13) | ((uint32_t)swprof12 << 12) | ((uint32_t)swprof11 << 11) | ((uint32_t)swprof10 << 10) | ((uint32_t)swprof9 << 9) | ((uint32_t)swprof8 << 8) | ((uint32_t)swprof7 << 7) | ((uint32_t)swprof6 << 6) | ((uint32_t)swprof5 << 5) | ((uint32_t)swprof4 << 4) | ((uint32_t)swprof3 << 3) | ((uint32_t)swprof2 << 2) | ((uint32_t)swprof1 << 1) | ((uint32_t)swprof0 << 0));
}

__INLINE void ble_swprofiling_unpack(uint8_t* swprof31, uint8_t* swprof30, uint8_t* swprof29, uint8_t* swprof28, uint8_t* swprof27, uint8_t* swprof26, uint8_t* swprof25, uint8_t* swprof24, uint8_t* swprof23, uint8_t* swprof22, uint8_t* swprof21, uint8_t* swprof20, uint8_t* swprof19, uint8_t* swprof18, uint8_t* swprof17, uint8_t* swprof16, uint8_t* swprof15, uint8_t* swprof14, uint8_t* swprof13, uint8_t* swprof12, uint8_t* swprof11, uint8_t* swprof10, uint8_t* swprof9, uint8_t* swprof8, uint8_t* swprof7, uint8_t* swprof6, uint8_t* swprof5, uint8_t* swprof4, uint8_t* swprof3, uint8_t* swprof2, uint8_t* swprof1, uint8_t* swprof0)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);

    *swprof31 = (localVal & ((uint32_t)0x80000000)) >> 31;
    *swprof30 = (localVal & ((uint32_t)0x40000000)) >> 30;
    *swprof29 = (localVal & ((uint32_t)0x20000000)) >> 29;
    *swprof28 = (localVal & ((uint32_t)0x10000000)) >> 28;
    *swprof27 = (localVal & ((uint32_t)0x08000000)) >> 27;
    *swprof26 = (localVal & ((uint32_t)0x04000000)) >> 26;
    *swprof25 = (localVal & ((uint32_t)0x02000000)) >> 25;
    *swprof24 = (localVal & ((uint32_t)0x01000000)) >> 24;
    *swprof23 = (localVal & ((uint32_t)0x00800000)) >> 23;
    *swprof22 = (localVal & ((uint32_t)0x00400000)) >> 22;
    *swprof21 = (localVal & ((uint32_t)0x00200000)) >> 21;
    *swprof20 = (localVal & ((uint32_t)0x00100000)) >> 20;
    *swprof19 = (localVal & ((uint32_t)0x00080000)) >> 19;
    *swprof18 = (localVal & ((uint32_t)0x00040000)) >> 18;
    *swprof17 = (localVal & ((uint32_t)0x00020000)) >> 17;
    *swprof16 = (localVal & ((uint32_t)0x00010000)) >> 16;
    *swprof15 = (localVal & ((uint32_t)0x00008000)) >> 15;
    *swprof14 = (localVal & ((uint32_t)0x00004000)) >> 14;
    *swprof13 = (localVal & ((uint32_t)0x00002000)) >> 13;
    *swprof12 = (localVal & ((uint32_t)0x00001000)) >> 12;
    *swprof11 = (localVal & ((uint32_t)0x00000800)) >> 11;
    *swprof10 = (localVal & ((uint32_t)0x00000400)) >> 10;
    *swprof9 = (localVal & ((uint32_t)0x00000200)) >> 9;
    *swprof8 = (localVal & ((uint32_t)0x00000100)) >> 8;
    *swprof7 = (localVal & ((uint32_t)0x00000080)) >> 7;
    *swprof6 = (localVal & ((uint32_t)0x00000040)) >> 6;
    *swprof5 = (localVal & ((uint32_t)0x00000020)) >> 5;
    *swprof4 = (localVal & ((uint32_t)0x00000010)) >> 4;
    *swprof3 = (localVal & ((uint32_t)0x00000008)) >> 3;
    *swprof2 = (localVal & ((uint32_t)0x00000004)) >> 2;
    *swprof1 = (localVal & ((uint32_t)0x00000002)) >> 1;
    *swprof0 = (localVal & ((uint32_t)0x00000001)) >> 0;
}

__INLINE uint8_t ble_swprof31_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

__INLINE void ble_swprof31_setf(uint8_t swprof31)
{
    ASSERT_ERR((((uint32_t)swprof31 << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x80000000)) | ((uint32_t)swprof31 << 31));
}

__INLINE uint8_t ble_swprof30_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x40000000)) >> 30);
}

__INLINE void ble_swprof30_setf(uint8_t swprof30)
{
    ASSERT_ERR((((uint32_t)swprof30 << 30) & ~((uint32_t)0x40000000)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x40000000)) | ((uint32_t)swprof30 << 30));
}

__INLINE uint8_t ble_swprof29_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x20000000)) >> 29);
}

__INLINE void ble_swprof29_setf(uint8_t swprof29)
{
    ASSERT_ERR((((uint32_t)swprof29 << 29) & ~((uint32_t)0x20000000)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x20000000)) | ((uint32_t)swprof29 << 29));
}

__INLINE uint8_t ble_swprof28_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x10000000)) >> 28);
}

__INLINE void ble_swprof28_setf(uint8_t swprof28)
{
    ASSERT_ERR((((uint32_t)swprof28 << 28) & ~((uint32_t)0x10000000)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x10000000)) | ((uint32_t)swprof28 << 28));
}

__INLINE uint8_t ble_swprof27_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x08000000)) >> 27);
}

__INLINE void ble_swprof27_setf(uint8_t swprof27)
{
    ASSERT_ERR((((uint32_t)swprof27 << 27) & ~((uint32_t)0x08000000)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x08000000)) | ((uint32_t)swprof27 << 27));
}

__INLINE uint8_t ble_swprof26_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x04000000)) >> 26);
}

__INLINE void ble_swprof26_setf(uint8_t swprof26)
{
    ASSERT_ERR((((uint32_t)swprof26 << 26) & ~((uint32_t)0x04000000)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x04000000)) | ((uint32_t)swprof26 << 26));
}

__INLINE uint8_t ble_swprof25_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x02000000)) >> 25);
}

__INLINE void ble_swprof25_setf(uint8_t swprof25)
{
    ASSERT_ERR((((uint32_t)swprof25 << 25) & ~((uint32_t)0x02000000)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x02000000)) | ((uint32_t)swprof25 << 25));
}

__INLINE uint8_t ble_swprof24_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x01000000)) >> 24);
}

__INLINE void ble_swprof24_setf(uint8_t swprof24)
{
    ASSERT_ERR((((uint32_t)swprof24 << 24) & ~((uint32_t)0x01000000)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x01000000)) | ((uint32_t)swprof24 << 24));
}

__INLINE uint8_t ble_swprof23_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00800000)) >> 23);
}

__INLINE void ble_swprof23_setf(uint8_t swprof23)
{
    ASSERT_ERR((((uint32_t)swprof23 << 23) & ~((uint32_t)0x00800000)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00800000)) | ((uint32_t)swprof23 << 23));
}

__INLINE uint8_t ble_swprof22_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00400000)) >> 22);
}

__INLINE void ble_swprof22_setf(uint8_t swprof22)
{
    ASSERT_ERR((((uint32_t)swprof22 << 22) & ~((uint32_t)0x00400000)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00400000)) | ((uint32_t)swprof22 << 22));
}

__INLINE uint8_t ble_swprof21_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00200000)) >> 21);
}

__INLINE void ble_swprof21_setf(uint8_t swprof21)
{
    ASSERT_ERR((((uint32_t)swprof21 << 21) & ~((uint32_t)0x00200000)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00200000)) | ((uint32_t)swprof21 << 21));
}

__INLINE uint8_t ble_swprof20_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00100000)) >> 20);
}

__INLINE void ble_swprof20_setf(uint8_t swprof20)
{
    ASSERT_ERR((((uint32_t)swprof20 << 20) & ~((uint32_t)0x00100000)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00100000)) | ((uint32_t)swprof20 << 20));
}

__INLINE uint8_t ble_swprof19_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00080000)) >> 19);
}

__INLINE void ble_swprof19_setf(uint8_t swprof19)
{
    ASSERT_ERR((((uint32_t)swprof19 << 19) & ~((uint32_t)0x00080000)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00080000)) | ((uint32_t)swprof19 << 19));
}

__INLINE uint8_t ble_swprof18_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00040000)) >> 18);
}

__INLINE void ble_swprof18_setf(uint8_t swprof18)
{
    ASSERT_ERR((((uint32_t)swprof18 << 18) & ~((uint32_t)0x00040000)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00040000)) | ((uint32_t)swprof18 << 18));
}

__INLINE uint8_t ble_swprof17_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00020000)) >> 17);
}

__INLINE void ble_swprof17_setf(uint8_t swprof17)
{
    ASSERT_ERR((((uint32_t)swprof17 << 17) & ~((uint32_t)0x00020000)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00020000)) | ((uint32_t)swprof17 << 17));
}

__INLINE uint8_t ble_swprof16_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

__INLINE void ble_swprof16_setf(uint8_t swprof16)
{
    ASSERT_ERR((((uint32_t)swprof16 << 16) & ~((uint32_t)0x00010000)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00010000)) | ((uint32_t)swprof16 << 16));
}

__INLINE uint8_t ble_swprof15_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00008000)) >> 15);
}

__INLINE void ble_swprof15_setf(uint8_t swprof15)
{
    ASSERT_ERR((((uint32_t)swprof15 << 15) & ~((uint32_t)0x00008000)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00008000)) | ((uint32_t)swprof15 << 15));
}

__INLINE uint8_t ble_swprof14_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00004000)) >> 14);
}

__INLINE void ble_swprof14_setf(uint8_t swprof14)
{
    ASSERT_ERR((((uint32_t)swprof14 << 14) & ~((uint32_t)0x00004000)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00004000)) | ((uint32_t)swprof14 << 14));
}

__INLINE uint8_t ble_swprof13_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00002000)) >> 13);
}

__INLINE void ble_swprof13_setf(uint8_t swprof13)
{
    ASSERT_ERR((((uint32_t)swprof13 << 13) & ~((uint32_t)0x00002000)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00002000)) | ((uint32_t)swprof13 << 13));
}

__INLINE uint8_t ble_swprof12_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00001000)) >> 12);
}

__INLINE void ble_swprof12_setf(uint8_t swprof12)
{
    ASSERT_ERR((((uint32_t)swprof12 << 12) & ~((uint32_t)0x00001000)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00001000)) | ((uint32_t)swprof12 << 12));
}

__INLINE uint8_t ble_swprof11_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00000800)) >> 11);
}

__INLINE void ble_swprof11_setf(uint8_t swprof11)
{
    ASSERT_ERR((((uint32_t)swprof11 << 11) & ~((uint32_t)0x00000800)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00000800)) | ((uint32_t)swprof11 << 11));
}

__INLINE uint8_t ble_swprof10_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00000400)) >> 10);
}

__INLINE void ble_swprof10_setf(uint8_t swprof10)
{
    ASSERT_ERR((((uint32_t)swprof10 << 10) & ~((uint32_t)0x00000400)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00000400)) | ((uint32_t)swprof10 << 10));
}

__INLINE uint8_t ble_swprof9_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00000200)) >> 9);
}

__INLINE void ble_swprof9_setf(uint8_t swprof9)
{
    ASSERT_ERR((((uint32_t)swprof9 << 9) & ~((uint32_t)0x00000200)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00000200)) | ((uint32_t)swprof9 << 9));
}

__INLINE uint8_t ble_swprof8_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

__INLINE void ble_swprof8_setf(uint8_t swprof8)
{
    ASSERT_ERR((((uint32_t)swprof8 << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00000100)) | ((uint32_t)swprof8 << 8));
}

__INLINE uint8_t ble_swprof7_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00000080)) >> 7);
}

__INLINE void ble_swprof7_setf(uint8_t swprof7)
{
    ASSERT_ERR((((uint32_t)swprof7 << 7) & ~((uint32_t)0x00000080)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00000080)) | ((uint32_t)swprof7 << 7));
}

__INLINE uint8_t ble_swprof6_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00000040)) >> 6);
}

__INLINE void ble_swprof6_setf(uint8_t swprof6)
{
    ASSERT_ERR((((uint32_t)swprof6 << 6) & ~((uint32_t)0x00000040)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00000040)) | ((uint32_t)swprof6 << 6));
}

__INLINE uint8_t ble_swprof5_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00000020)) >> 5);
}

__INLINE void ble_swprof5_setf(uint8_t swprof5)
{
    ASSERT_ERR((((uint32_t)swprof5 << 5) & ~((uint32_t)0x00000020)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00000020)) | ((uint32_t)swprof5 << 5));
}

__INLINE uint8_t ble_swprof4_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}

__INLINE void ble_swprof4_setf(uint8_t swprof4)
{
    ASSERT_ERR((((uint32_t)swprof4 << 4) & ~((uint32_t)0x00000010)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00000010)) | ((uint32_t)swprof4 << 4));
}

__INLINE uint8_t ble_swprof3_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}

__INLINE void ble_swprof3_setf(uint8_t swprof3)
{
    ASSERT_ERR((((uint32_t)swprof3 << 3) & ~((uint32_t)0x00000008)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00000008)) | ((uint32_t)swprof3 << 3));
}

__INLINE uint8_t ble_swprof2_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}

__INLINE void ble_swprof2_setf(uint8_t swprof2)
{
    ASSERT_ERR((((uint32_t)swprof2 << 2) & ~((uint32_t)0x00000004)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00000004)) | ((uint32_t)swprof2 << 2));
}

__INLINE uint8_t ble_swprof1_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

__INLINE void ble_swprof1_setf(uint8_t swprof1)
{
    ASSERT_ERR((((uint32_t)swprof1 << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00000002)) | ((uint32_t)swprof1 << 1));
}

__INLINE uint8_t ble_swprof0_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SWPROFILING_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

__INLINE void ble_swprof0_setf(uint8_t swprof0)
{
    ASSERT_ERR((((uint32_t)swprof0 << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_BLE_WR(BLE_SWPROFILING_ADDR, (REG_BLE_RD(BLE_SWPROFILING_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)swprof0 << 0));
}

/**
 * @brief RADIOCNTL0 register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:26               VGACNT   0x0
 *  25:24               LNACNT   0x0
 *     23        AGCFORCE_MODE   0
 *     22            DPCORR_EN   0
 *     21        SPISWACCDBGEN   0
 *     20     CORR_WIN_MSK_DSB   0
 *     19          FORCEAGC_EN   0
 *     18           FORCEBLEIQ   0
 *  13:12              RXLOWIF   0x2
 *     09             BIDIRCLK   0
 *     08            BIDIRDATA   1
 *     07               SPIPOL   0
 *  06:05              SPIFREQ   0x0
 *     04              SPIMODE   0
 *     01              SPICOMP   1
 *     00                SPIGO   0
 * </pre>
 */
#define BLE_RADIOCNTL0_ADDR   0x40000070
#define BLE_RADIOCNTL0_OFFSET 0x00000070
#define BLE_RADIOCNTL0_INDEX  0x0000001C
#define BLE_RADIOCNTL0_RESET  0x00002102

__INLINE uint32_t ble_radiocntl0_get(void)
{
    return REG_BLE_RD(BLE_RADIOCNTL0_ADDR);
}

__INLINE void ble_radiocntl0_set(uint32_t value)
{
    REG_BLE_WR(BLE_RADIOCNTL0_ADDR, value);
}

// field definitions
#define BLE_VGACNT_MASK             ((uint32_t)0xFC000000)
#define BLE_VGACNT_LSB              26
#define BLE_VGACNT_WIDTH            ((uint32_t)0x00000006)
#define BLE_LNACNT_MASK             ((uint32_t)0x03000000)
#define BLE_LNACNT_LSB              24
#define BLE_LNACNT_WIDTH            ((uint32_t)0x00000002)
#define BLE_AGCFORCE_MODE_BIT       ((uint32_t)0x00800000)
#define BLE_AGCFORCE_MODE_POS       23
#define BLE_DPCORR_EN_BIT           ((uint32_t)0x00400000)
#define BLE_DPCORR_EN_POS           22
#define BLE_SPISWACCDBGEN_BIT       ((uint32_t)0x00200000)
#define BLE_SPISWACCDBGEN_POS       21
#define BLE_CORR_WIN_MSK_DSB_BIT    ((uint32_t)0x00100000)
#define BLE_CORR_WIN_MSK_DSB_POS    20
#define BLE_FORCEAGC_EN_BIT         ((uint32_t)0x00080000)
#define BLE_FORCEAGC_EN_POS         19
#define BLE_FORCEBLEIQ_BIT          ((uint32_t)0x00040000)
#define BLE_FORCEBLEIQ_POS          18
#define BLE_RXLOWIF_MASK            ((uint32_t)0x00003000)
#define BLE_RXLOWIF_LSB             12
#define BLE_RXLOWIF_WIDTH           ((uint32_t)0x00000002)
#define BLE_BIDIRCLK_BIT            ((uint32_t)0x00000200)
#define BLE_BIDIRCLK_POS            9
#define BLE_BIDIRDATA_BIT           ((uint32_t)0x00000100)
#define BLE_BIDIRDATA_POS           8
#define BLE_SPIPOL_BIT              ((uint32_t)0x00000080)
#define BLE_SPIPOL_POS              7
#define BLE_SPIFREQ_MASK            ((uint32_t)0x00000060)
#define BLE_SPIFREQ_LSB             5
#define BLE_SPIFREQ_WIDTH           ((uint32_t)0x00000002)
#define BLE_SPIMODE_BIT             ((uint32_t)0x00000010)
#define BLE_SPIMODE_POS             4
#define BLE_SPICOMP_BIT             ((uint32_t)0x00000002)
#define BLE_SPICOMP_POS             1
#define BLE_SPIGO_BIT               ((uint32_t)0x00000001)
#define BLE_SPIGO_POS               0

#define BLE_VGACNT_RST              0x0
#define BLE_LNACNT_RST              0x0
#define BLE_AGCFORCE_MODE_RST       0x0
#define BLE_DPCORR_EN_RST           0x0
#define BLE_SPISWACCDBGEN_RST       0x0
#define BLE_CORR_WIN_MSK_DSB_RST    0x0
#define BLE_FORCEAGC_EN_RST         0x0
#define BLE_FORCEBLEIQ_RST          0x0
#define BLE_RXLOWIF_RST             0x2
#define BLE_BIDIRCLK_RST            0x0
#define BLE_BIDIRDATA_RST           0x1
#define BLE_SPIPOL_RST              0x0
#define BLE_SPIFREQ_RST             0x0
#define BLE_SPIMODE_RST             0x0
#define BLE_SPICOMP_RST             0x1
#define BLE_SPIGO_RST               0x0

__INLINE void ble_radiocntl0_pack(uint8_t vgacnt, uint8_t lnacnt, uint8_t agcforcemode, uint8_t dpcorren, uint8_t spiswaccdbgen, uint8_t corrwinmskdsb, uint8_t forceagcen, uint8_t forcebleiq, uint8_t rxlowif, uint8_t bidirclk, uint8_t bidirdata, uint8_t spipol, uint8_t spifreq, uint8_t spimode, uint8_t spigo)
{
    ASSERT_ERR((((uint32_t)vgacnt << 26) & ~((uint32_t)0xFC000000)) == 0);
    ASSERT_ERR((((uint32_t)lnacnt << 24) & ~((uint32_t)0x03000000)) == 0);
    ASSERT_ERR((((uint32_t)agcforcemode << 23) & ~((uint32_t)0x00800000)) == 0);
    ASSERT_ERR((((uint32_t)dpcorren << 22) & ~((uint32_t)0x00400000)) == 0);
    ASSERT_ERR((((uint32_t)spiswaccdbgen << 21) & ~((uint32_t)0x00200000)) == 0);
    ASSERT_ERR((((uint32_t)corrwinmskdsb << 20) & ~((uint32_t)0x00100000)) == 0);
    ASSERT_ERR((((uint32_t)forceagcen << 19) & ~((uint32_t)0x00080000)) == 0);
    ASSERT_ERR((((uint32_t)forcebleiq << 18) & ~((uint32_t)0x00040000)) == 0);
    ASSERT_ERR((((uint32_t)rxlowif << 12) & ~((uint32_t)0x00003000)) == 0);
    ASSERT_ERR((((uint32_t)bidirclk << 9) & ~((uint32_t)0x00000200)) == 0);
    ASSERT_ERR((((uint32_t)bidirdata << 8) & ~((uint32_t)0x00000100)) == 0);
    ASSERT_ERR((((uint32_t)spipol << 7) & ~((uint32_t)0x00000080)) == 0);
    ASSERT_ERR((((uint32_t)spifreq << 5) & ~((uint32_t)0x00000060)) == 0);
    ASSERT_ERR((((uint32_t)spimode << 4) & ~((uint32_t)0x00000010)) == 0);
    ASSERT_ERR((((uint32_t)spigo << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL0_ADDR,  ((uint32_t)vgacnt << 26) | ((uint32_t)lnacnt << 24) | ((uint32_t)agcforcemode << 23) | ((uint32_t)dpcorren << 22) | ((uint32_t)spiswaccdbgen << 21) | ((uint32_t)corrwinmskdsb << 20) | ((uint32_t)forceagcen << 19) | ((uint32_t)forcebleiq << 18) | ((uint32_t)rxlowif << 12) | ((uint32_t)bidirclk << 9) | ((uint32_t)bidirdata << 8) | ((uint32_t)spipol << 7) | ((uint32_t)spifreq << 5) | ((uint32_t)spimode << 4) | ((uint32_t)spigo << 0));
}

__INLINE void ble_radiocntl0_unpack(uint8_t* vgacnt, uint8_t* lnacnt, uint8_t* agcforcemode, uint8_t* dpcorren, uint8_t* spiswaccdbgen, uint8_t* corrwinmskdsb, uint8_t* forceagcen, uint8_t* forcebleiq, uint8_t* rxlowif, uint8_t* bidirclk, uint8_t* bidirdata, uint8_t* spipol, uint8_t* spifreq, uint8_t* spimode, uint8_t* spicomp, uint8_t* spigo)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL0_ADDR);

    *vgacnt = (localVal & ((uint32_t)0xFC000000)) >> 26;
    *lnacnt = (localVal & ((uint32_t)0x03000000)) >> 24;
    *agcforcemode = (localVal & ((uint32_t)0x00800000)) >> 23;
    *dpcorren = (localVal & ((uint32_t)0x00400000)) >> 22;
    *spiswaccdbgen = (localVal & ((uint32_t)0x00200000)) >> 21;
    *corrwinmskdsb = (localVal & ((uint32_t)0x00100000)) >> 20;
    *forceagcen = (localVal & ((uint32_t)0x00080000)) >> 19;
    *forcebleiq = (localVal & ((uint32_t)0x00040000)) >> 18;
    *rxlowif = (localVal & ((uint32_t)0x00003000)) >> 12;
    *bidirclk = (localVal & ((uint32_t)0x00000200)) >> 9;
    *bidirdata = (localVal & ((uint32_t)0x00000100)) >> 8;
    *spipol = (localVal & ((uint32_t)0x00000080)) >> 7;
    *spifreq = (localVal & ((uint32_t)0x00000060)) >> 5;
    *spimode = (localVal & ((uint32_t)0x00000010)) >> 4;
    *spicomp = (localVal & ((uint32_t)0x00000002)) >> 1;
    *spigo = (localVal & ((uint32_t)0x00000001)) >> 0;
}

__INLINE uint8_t ble_vgacnt_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL0_ADDR);
    return ((localVal & ((uint32_t)0xFC000000)) >> 26);
}

__INLINE void ble_vgacnt_setf(uint8_t vgacnt)
{
    ASSERT_ERR((((uint32_t)vgacnt << 26) & ~((uint32_t)0xFC000000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL0_ADDR, (REG_BLE_RD(BLE_RADIOCNTL0_ADDR) & ~((uint32_t)0xFC000000)) | ((uint32_t)vgacnt << 26));
}

__INLINE uint8_t ble_lnacnt_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL0_ADDR);
    return ((localVal & ((uint32_t)0x03000000)) >> 24);
}

__INLINE void ble_lnacnt_setf(uint8_t lnacnt)
{
    ASSERT_ERR((((uint32_t)lnacnt << 24) & ~((uint32_t)0x03000000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL0_ADDR, (REG_BLE_RD(BLE_RADIOCNTL0_ADDR) & ~((uint32_t)0x03000000)) | ((uint32_t)lnacnt << 24));
}

__INLINE uint8_t ble_agcforce_mode_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL0_ADDR);
    return ((localVal & ((uint32_t)0x00800000)) >> 23);
}

__INLINE void ble_agcforce_mode_setf(uint8_t agcforcemode)
{
    ASSERT_ERR((((uint32_t)agcforcemode << 23) & ~((uint32_t)0x00800000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL0_ADDR, (REG_BLE_RD(BLE_RADIOCNTL0_ADDR) & ~((uint32_t)0x00800000)) | ((uint32_t)agcforcemode << 23));
}

__INLINE uint8_t ble_dpcorr_en_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL0_ADDR);
    return ((localVal & ((uint32_t)0x00400000)) >> 22);
}

__INLINE void ble_dpcorr_en_setf(uint8_t dpcorren)
{
    ASSERT_ERR((((uint32_t)dpcorren << 22) & ~((uint32_t)0x00400000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL0_ADDR, (REG_BLE_RD(BLE_RADIOCNTL0_ADDR) & ~((uint32_t)0x00400000)) | ((uint32_t)dpcorren << 22));
}

__INLINE uint8_t ble_spiswaccdbgen_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL0_ADDR);
    return ((localVal & ((uint32_t)0x00200000)) >> 21);
}

__INLINE void ble_spiswaccdbgen_setf(uint8_t spiswaccdbgen)
{
    ASSERT_ERR((((uint32_t)spiswaccdbgen << 21) & ~((uint32_t)0x00200000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL0_ADDR, (REG_BLE_RD(BLE_RADIOCNTL0_ADDR) & ~((uint32_t)0x00200000)) | ((uint32_t)spiswaccdbgen << 21));
}

__INLINE uint8_t ble_corr_win_msk_dsb_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL0_ADDR);
    return ((localVal & ((uint32_t)0x00100000)) >> 20);
}

__INLINE void ble_corr_win_msk_dsb_setf(uint8_t corrwinmskdsb)
{
    ASSERT_ERR((((uint32_t)corrwinmskdsb << 20) & ~((uint32_t)0x00100000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL0_ADDR, (REG_BLE_RD(BLE_RADIOCNTL0_ADDR) & ~((uint32_t)0x00100000)) | ((uint32_t)corrwinmskdsb << 20));
}

__INLINE uint8_t ble_forceagc_en_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL0_ADDR);
    return ((localVal & ((uint32_t)0x00080000)) >> 19);
}

__INLINE void ble_forceagc_en_setf(uint8_t forceagcen)
{
    ASSERT_ERR((((uint32_t)forceagcen << 19) & ~((uint32_t)0x00080000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL0_ADDR, (REG_BLE_RD(BLE_RADIOCNTL0_ADDR) & ~((uint32_t)0x00080000)) | ((uint32_t)forceagcen << 19));
}

__INLINE uint8_t ble_forcebleiq_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL0_ADDR);
    return ((localVal & ((uint32_t)0x00040000)) >> 18);
}

__INLINE void ble_forcebleiq_setf(uint8_t forcebleiq)
{
    ASSERT_ERR((((uint32_t)forcebleiq << 18) & ~((uint32_t)0x00040000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL0_ADDR, (REG_BLE_RD(BLE_RADIOCNTL0_ADDR) & ~((uint32_t)0x00040000)) | ((uint32_t)forcebleiq << 18));
}

__INLINE uint8_t ble_rxlowif_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL0_ADDR);
    return ((localVal & ((uint32_t)0x00003000)) >> 12);
}

__INLINE void ble_rxlowif_setf(uint8_t rxlowif)
{
    ASSERT_ERR((((uint32_t)rxlowif << 12) & ~((uint32_t)0x00003000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL0_ADDR, (REG_BLE_RD(BLE_RADIOCNTL0_ADDR) & ~((uint32_t)0x00003000)) | ((uint32_t)rxlowif << 12));
}

__INLINE uint8_t ble_bidirclk_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL0_ADDR);
    return ((localVal & ((uint32_t)0x00000200)) >> 9);
}

__INLINE void ble_bidirclk_setf(uint8_t bidirclk)
{
    ASSERT_ERR((((uint32_t)bidirclk << 9) & ~((uint32_t)0x00000200)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL0_ADDR, (REG_BLE_RD(BLE_RADIOCNTL0_ADDR) & ~((uint32_t)0x00000200)) | ((uint32_t)bidirclk << 9));
}

__INLINE uint8_t ble_bidirdata_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL0_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

__INLINE void ble_bidirdata_setf(uint8_t bidirdata)
{
    ASSERT_ERR((((uint32_t)bidirdata << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL0_ADDR, (REG_BLE_RD(BLE_RADIOCNTL0_ADDR) & ~((uint32_t)0x00000100)) | ((uint32_t)bidirdata << 8));
}

__INLINE uint8_t ble_spipol_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL0_ADDR);
    return ((localVal & ((uint32_t)0x00000080)) >> 7);
}

__INLINE void ble_spipol_setf(uint8_t spipol)
{
    ASSERT_ERR((((uint32_t)spipol << 7) & ~((uint32_t)0x00000080)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL0_ADDR, (REG_BLE_RD(BLE_RADIOCNTL0_ADDR) & ~((uint32_t)0x00000080)) | ((uint32_t)spipol << 7));
}

__INLINE uint8_t ble_spifreq_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL0_ADDR);
    return ((localVal & ((uint32_t)0x00000060)) >> 5);
}

__INLINE void ble_spifreq_setf(uint8_t spifreq)
{
    ASSERT_ERR((((uint32_t)spifreq << 5) & ~((uint32_t)0x00000060)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL0_ADDR, (REG_BLE_RD(BLE_RADIOCNTL0_ADDR) & ~((uint32_t)0x00000060)) | ((uint32_t)spifreq << 5));
}

__INLINE uint8_t ble_spimode_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL0_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}

__INLINE void ble_spimode_setf(uint8_t spimode)
{
    ASSERT_ERR((((uint32_t)spimode << 4) & ~((uint32_t)0x00000010)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL0_ADDR, (REG_BLE_RD(BLE_RADIOCNTL0_ADDR) & ~((uint32_t)0x00000010)) | ((uint32_t)spimode << 4));
}

__INLINE uint8_t ble_spicomp_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL0_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

__INLINE uint8_t ble_spigo_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL0_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

__INLINE void ble_spigo_setf(uint8_t spigo)
{
    ASSERT_ERR((((uint32_t)spigo << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL0_ADDR, (REG_BLE_RD(BLE_RADIOCNTL0_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)spigo << 0));
}

/**
 * @brief RADIOCNTL1 register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     28            RAMP_TYPE   0
 *  27:24             RAMPSTEP   0x0
 *  20:16               XRFSEL   0x0
 *  15:00               SPIPTR   0x0
 * </pre>
 */
#define BLE_RADIOCNTL1_ADDR   0x40000074
#define BLE_RADIOCNTL1_OFFSET 0x00000074
#define BLE_RADIOCNTL1_INDEX  0x0000001D
#define BLE_RADIOCNTL1_RESET  0x00000000

__INLINE uint32_t ble_radiocntl1_get(void)
{
    return REG_BLE_RD(BLE_RADIOCNTL1_ADDR);
}

__INLINE void ble_radiocntl1_set(uint32_t value)
{
    REG_BLE_WR(BLE_RADIOCNTL1_ADDR, value);
}

// field definitions
#define BLE_RAMP_TYPE_BIT    ((uint32_t)0x10000000)
#define BLE_RAMP_TYPE_POS    28
#define BLE_RAMPSTEP_MASK    ((uint32_t)0x0F000000)
#define BLE_RAMPSTEP_LSB     24
#define BLE_RAMPSTEP_WIDTH   ((uint32_t)0x00000004)
#define BLE_XRFSEL_MASK      ((uint32_t)0x001F0000)
#define BLE_XRFSEL_LSB       16
#define BLE_XRFSEL_WIDTH     ((uint32_t)0x00000005)
#define BLE_SPIPTR_MASK      ((uint32_t)0x0000FFFF)
#define BLE_SPIPTR_LSB       0
#define BLE_SPIPTR_WIDTH     ((uint32_t)0x00000010)

#define BLE_RAMP_TYPE_RST    0x0
#define BLE_RAMPSTEP_RST     0x0
#define BLE_XRFSEL_RST       0x0
#define BLE_SPIPTR_RST       0x0

__INLINE void ble_radiocntl1_pack(uint8_t ramptype, uint8_t rampstep, uint8_t xrfsel, uint16_t spiptr)
{
    ASSERT_ERR((((uint32_t)ramptype << 28) & ~((uint32_t)0x10000000)) == 0);
    ASSERT_ERR((((uint32_t)rampstep << 24) & ~((uint32_t)0x0F000000)) == 0);
    ASSERT_ERR((((uint32_t)xrfsel << 16) & ~((uint32_t)0x001F0000)) == 0);
    ASSERT_ERR((((uint32_t)spiptr << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL1_ADDR,  ((uint32_t)ramptype << 28) | ((uint32_t)rampstep << 24) | ((uint32_t)xrfsel << 16) | ((uint32_t)spiptr << 0));
}

__INLINE void ble_radiocntl1_unpack(uint8_t* ramptype, uint8_t* rampstep, uint8_t* xrfsel, uint16_t* spiptr)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL1_ADDR);

    *ramptype = (localVal & ((uint32_t)0x10000000)) >> 28;
    *rampstep = (localVal & ((uint32_t)0x0F000000)) >> 24;
    *xrfsel = (localVal & ((uint32_t)0x001F0000)) >> 16;
    *spiptr = (localVal & ((uint32_t)0x0000FFFF)) >> 0;
}

__INLINE uint8_t ble_ramp_type_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL1_ADDR);
    return ((localVal & ((uint32_t)0x10000000)) >> 28);
}

__INLINE void ble_ramp_type_setf(uint8_t ramptype)
{
    ASSERT_ERR((((uint32_t)ramptype << 28) & ~((uint32_t)0x10000000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL1_ADDR, (REG_BLE_RD(BLE_RADIOCNTL1_ADDR) & ~((uint32_t)0x10000000)) | ((uint32_t)ramptype << 28));
}

__INLINE uint8_t ble_rampstep_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL1_ADDR);
    return ((localVal & ((uint32_t)0x0F000000)) >> 24);
}

__INLINE void ble_rampstep_setf(uint8_t rampstep)
{
    ASSERT_ERR((((uint32_t)rampstep << 24) & ~((uint32_t)0x0F000000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL1_ADDR, (REG_BLE_RD(BLE_RADIOCNTL1_ADDR) & ~((uint32_t)0x0F000000)) | ((uint32_t)rampstep << 24));
}

__INLINE uint8_t ble_xrfsel_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL1_ADDR);
    return ((localVal & ((uint32_t)0x001F0000)) >> 16);
}

__INLINE void ble_xrfsel_setf(uint8_t xrfsel)
{
    ASSERT_ERR((((uint32_t)xrfsel << 16) & ~((uint32_t)0x001F0000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL1_ADDR, (REG_BLE_RD(BLE_RADIOCNTL1_ADDR) & ~((uint32_t)0x001F0000)) | ((uint32_t)xrfsel << 16));
}

__INLINE uint16_t ble_spiptr_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL1_ADDR);
    return ((localVal & ((uint32_t)0x0000FFFF)) >> 0);
}

__INLINE void ble_spiptr_setf(uint16_t spiptr)
{
    ASSERT_ERR((((uint32_t)spiptr << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL1_ADDR, (REG_BLE_RD(BLE_RADIOCNTL1_ADDR) & ~((uint32_t)0x0000FFFF)) | ((uint32_t)spiptr << 0));
}

#if RW_BLE_SUPPORT
/**
 * @brief RADIOCNTL2 register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24          REGUP_DELAY   0x0
 *  23:18           RXDN_DELAY   0x0
 *  17:12           RXUP_DELAY   0x0
 *  11:06           TXDN_DELAY   0x0
 *  05:00           TXUP_DELAY   0x0
 * </pre>
 */
#define BLE_RADIOCNTL2_ADDR   0x40000078
#define BLE_RADIOCNTL2_OFFSET 0x00000078
#define BLE_RADIOCNTL2_INDEX  0x0000001E
#define BLE_RADIOCNTL2_RESET  0x00000000

__INLINE uint32_t ble_radiocntl2_get(void)
{
    return REG_BLE_RD(BLE_RADIOCNTL2_ADDR);
}

__INLINE void ble_radiocntl2_set(uint32_t value)
{
    REG_BLE_WR(BLE_RADIOCNTL2_ADDR, value);
}

// field definitions
#define BLE_REGUP_DELAY_MASK   ((uint32_t)0x3F000000)
#define BLE_REGUP_DELAY_LSB    24
#define BLE_REGUP_DELAY_WIDTH  ((uint32_t)0x00000006)
#define BLE_RXDN_DELAY_MASK    ((uint32_t)0x00FC0000)
#define BLE_RXDN_DELAY_LSB     18
#define BLE_RXDN_DELAY_WIDTH   ((uint32_t)0x00000006)
#define BLE_RXUP_DELAY_MASK    ((uint32_t)0x0003F000)
#define BLE_RXUP_DELAY_LSB     12
#define BLE_RXUP_DELAY_WIDTH   ((uint32_t)0x00000006)
#define BLE_TXDN_DELAY_MASK    ((uint32_t)0x00000FC0)
#define BLE_TXDN_DELAY_LSB     6
#define BLE_TXDN_DELAY_WIDTH   ((uint32_t)0x00000006)
#define BLE_TXUP_DELAY_MASK    ((uint32_t)0x0000003F)
#define BLE_TXUP_DELAY_LSB     0
#define BLE_TXUP_DELAY_WIDTH   ((uint32_t)0x00000006)

#define BLE_REGUP_DELAY_RST    0x0
#define BLE_RXDN_DELAY_RST     0x0
#define BLE_RXUP_DELAY_RST     0x0
#define BLE_TXDN_DELAY_RST     0x0
#define BLE_TXUP_DELAY_RST     0x0

__INLINE void ble_radiocntl2_pack(uint8_t regupdelay, uint8_t rxdndelay, uint8_t rxupdelay, uint8_t txdndelay, uint8_t txupdelay)
{
    ASSERT_ERR((((uint32_t)regupdelay << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)rxdndelay << 18) & ~((uint32_t)0x00FC0000)) == 0);
    ASSERT_ERR((((uint32_t)rxupdelay << 12) & ~((uint32_t)0x0003F000)) == 0);
    ASSERT_ERR((((uint32_t)txdndelay << 6) & ~((uint32_t)0x00000FC0)) == 0);
    ASSERT_ERR((((uint32_t)txupdelay << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL2_ADDR,  ((uint32_t)regupdelay << 24) | ((uint32_t)rxdndelay << 18) | ((uint32_t)rxupdelay << 12) | ((uint32_t)txdndelay << 6) | ((uint32_t)txupdelay << 0));
}

__INLINE void ble_radiocntl2_unpack(uint8_t* regupdelay, uint8_t* rxdndelay, uint8_t* rxupdelay, uint8_t* txdndelay, uint8_t* txupdelay)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL2_ADDR);

    *regupdelay = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *rxdndelay = (localVal & ((uint32_t)0x00FC0000)) >> 18;
    *rxupdelay = (localVal & ((uint32_t)0x0003F000)) >> 12;
    *txdndelay = (localVal & ((uint32_t)0x00000FC0)) >> 6;
    *txupdelay = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

__INLINE uint8_t ble_regup_delay_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL2_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

__INLINE void ble_regup_delay_setf(uint8_t regupdelay)
{
    ASSERT_ERR((((uint32_t)regupdelay << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL2_ADDR, (REG_BLE_RD(BLE_RADIOCNTL2_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)regupdelay << 24));
}

__INLINE uint8_t ble_rxdn_delay_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL2_ADDR);
    return ((localVal & ((uint32_t)0x00FC0000)) >> 18);
}

__INLINE void ble_rxdn_delay_setf(uint8_t rxdndelay)
{
    ASSERT_ERR((((uint32_t)rxdndelay << 18) & ~((uint32_t)0x00FC0000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL2_ADDR, (REG_BLE_RD(BLE_RADIOCNTL2_ADDR) & ~((uint32_t)0x00FC0000)) | ((uint32_t)rxdndelay << 18));
}

__INLINE uint8_t ble_rxup_delay_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL2_ADDR);
    return ((localVal & ((uint32_t)0x0003F000)) >> 12);
}

__INLINE void ble_rxup_delay_setf(uint8_t rxupdelay)
{
    ASSERT_ERR((((uint32_t)rxupdelay << 12) & ~((uint32_t)0x0003F000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL2_ADDR, (REG_BLE_RD(BLE_RADIOCNTL2_ADDR) & ~((uint32_t)0x0003F000)) | ((uint32_t)rxupdelay << 12));
}

__INLINE uint8_t ble_txdn_delay_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL2_ADDR);
    return ((localVal & ((uint32_t)0x00000FC0)) >> 6);
}

__INLINE void ble_txdn_delay_setf(uint8_t txdndelay)
{
    ASSERT_ERR((((uint32_t)txdndelay << 6) & ~((uint32_t)0x00000FC0)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL2_ADDR, (REG_BLE_RD(BLE_RADIOCNTL2_ADDR) & ~((uint32_t)0x00000FC0)) | ((uint32_t)txdndelay << 6));
}

__INLINE uint8_t ble_txup_delay_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL2_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

__INLINE void ble_txup_delay_setf(uint8_t txupdelay)
{
    ASSERT_ERR((((uint32_t)txupdelay << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL2_ADDR, (REG_BLE_RD(BLE_RADIOCNTL2_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)txupdelay << 0));
}

#endif // RW_BLE_SUPPORT
#if RW_BLE_CUST1_INST
/**
 * @brief RADIOCNTL3 register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24           PBUS_VALUE   0x0
 *     18           PSTB_VALUE   0
 *  17:16           PSEL_VALUE   0x0
 *     15            RX_ON_POL   0
 *     14            TX_EN_POL   0
 *     13            PA_ON_POL   0
 *     12           REG_ON_POL   0
 *     11          RX_ON_VALUE   0
 *     10          TX_EN_VALUE   0
 *     09          PA_ON_VALUE   0
 *     08         REG_ON_VALUE   0
 *     04           FORCE_PBUS   0
 *     03          FORCE_RX_ON   0
 *     02          FORCE_TX_EN   0
 *     01          FORCE_PA_ON   0
 *     00         FORCE_REG_ON   0
 * </pre>
 */
#define BLE_RADIOCNTL3_ADDR   0x4000007C
#define BLE_RADIOCNTL3_OFFSET 0x0000007C
#define BLE_RADIOCNTL3_INDEX  0x0000001F
#define BLE_RADIOCNTL3_RESET  0x00000000

__INLINE uint32_t ble_radiocntl3_get(void)
{
    return REG_BLE_RD(BLE_RADIOCNTL3_ADDR);
}

__INLINE void ble_radiocntl3_set(uint32_t value)
{
    REG_BLE_WR(BLE_RADIOCNTL3_ADDR, value);
}

// field definitions
#define BLE_PBUS_VALUE_MASK     ((uint32_t)0xFF000000)
#define BLE_PBUS_VALUE_LSB      24
#define BLE_PBUS_VALUE_WIDTH    ((uint32_t)0x00000008)
#define BLE_PSTB_VALUE_BIT      ((uint32_t)0x00040000)
#define BLE_PSTB_VALUE_POS      18
#define BLE_PSEL_VALUE_MASK     ((uint32_t)0x00030000)
#define BLE_PSEL_VALUE_LSB      16
#define BLE_PSEL_VALUE_WIDTH    ((uint32_t)0x00000002)
#define BLE_RX_ON_POL_BIT       ((uint32_t)0x00008000)
#define BLE_RX_ON_POL_POS       15
#define BLE_TX_EN_POL_BIT       ((uint32_t)0x00004000)
#define BLE_TX_EN_POL_POS       14
#define BLE_PA_ON_POL_BIT       ((uint32_t)0x00002000)
#define BLE_PA_ON_POL_POS       13
#define BLE_REG_ON_POL_BIT      ((uint32_t)0x00001000)
#define BLE_REG_ON_POL_POS      12
#define BLE_RX_ON_VALUE_BIT     ((uint32_t)0x00000800)
#define BLE_RX_ON_VALUE_POS     11
#define BLE_TX_EN_VALUE_BIT     ((uint32_t)0x00000400)
#define BLE_TX_EN_VALUE_POS     10
#define BLE_PA_ON_VALUE_BIT     ((uint32_t)0x00000200)
#define BLE_PA_ON_VALUE_POS     9
#define BLE_REG_ON_VALUE_BIT    ((uint32_t)0x00000100)
#define BLE_REG_ON_VALUE_POS    8
#define BLE_FORCE_PBUS_BIT      ((uint32_t)0x00000010)
#define BLE_FORCE_PBUS_POS      4
#define BLE_FORCE_RX_ON_BIT     ((uint32_t)0x00000008)
#define BLE_FORCE_RX_ON_POS     3
#define BLE_FORCE_TX_EN_BIT     ((uint32_t)0x00000004)
#define BLE_FORCE_TX_EN_POS     2
#define BLE_FORCE_PA_ON_BIT     ((uint32_t)0x00000002)
#define BLE_FORCE_PA_ON_POS     1
#define BLE_FORCE_REG_ON_BIT    ((uint32_t)0x00000001)
#define BLE_FORCE_REG_ON_POS    0

#define BLE_PBUS_VALUE_RST      0x0
#define BLE_PSTB_VALUE_RST      0x0
#define BLE_PSEL_VALUE_RST      0x0
#define BLE_RX_ON_POL_RST       0x0
#define BLE_TX_EN_POL_RST       0x0
#define BLE_PA_ON_POL_RST       0x0
#define BLE_REG_ON_POL_RST      0x0
#define BLE_RX_ON_VALUE_RST     0x0
#define BLE_TX_EN_VALUE_RST     0x0
#define BLE_PA_ON_VALUE_RST     0x0
#define BLE_REG_ON_VALUE_RST    0x0
#define BLE_FORCE_PBUS_RST      0x0
#define BLE_FORCE_RX_ON_RST     0x0
#define BLE_FORCE_TX_EN_RST     0x0
#define BLE_FORCE_PA_ON_RST     0x0
#define BLE_FORCE_REG_ON_RST    0x0

__INLINE void ble_radiocntl3_pack(uint8_t pbusvalue, uint8_t pstbvalue, uint8_t pselvalue, uint8_t rxonpol, uint8_t txenpol, uint8_t paonpol, uint8_t regonpol, uint8_t rxonvalue, uint8_t txenvalue, uint8_t paonvalue, uint8_t regonvalue, uint8_t forcepbus, uint8_t forcerxon, uint8_t forcetxen, uint8_t forcepaon, uint8_t forceregon)
{
    ASSERT_ERR((((uint32_t)pbusvalue << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)pstbvalue << 18) & ~((uint32_t)0x00040000)) == 0);
    ASSERT_ERR((((uint32_t)pselvalue << 16) & ~((uint32_t)0x00030000)) == 0);
    ASSERT_ERR((((uint32_t)rxonpol << 15) & ~((uint32_t)0x00008000)) == 0);
    ASSERT_ERR((((uint32_t)txenpol << 14) & ~((uint32_t)0x00004000)) == 0);
    ASSERT_ERR((((uint32_t)paonpol << 13) & ~((uint32_t)0x00002000)) == 0);
    ASSERT_ERR((((uint32_t)regonpol << 12) & ~((uint32_t)0x00001000)) == 0);
    ASSERT_ERR((((uint32_t)rxonvalue << 11) & ~((uint32_t)0x00000800)) == 0);
    ASSERT_ERR((((uint32_t)txenvalue << 10) & ~((uint32_t)0x00000400)) == 0);
    ASSERT_ERR((((uint32_t)paonvalue << 9) & ~((uint32_t)0x00000200)) == 0);
    ASSERT_ERR((((uint32_t)regonvalue << 8) & ~((uint32_t)0x00000100)) == 0);
    ASSERT_ERR((((uint32_t)forcepbus << 4) & ~((uint32_t)0x00000010)) == 0);
    ASSERT_ERR((((uint32_t)forcerxon << 3) & ~((uint32_t)0x00000008)) == 0);
    ASSERT_ERR((((uint32_t)forcetxen << 2) & ~((uint32_t)0x00000004)) == 0);
    ASSERT_ERR((((uint32_t)forcepaon << 1) & ~((uint32_t)0x00000002)) == 0);
    ASSERT_ERR((((uint32_t)forceregon << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL3_ADDR,  ((uint32_t)pbusvalue << 24) | ((uint32_t)pstbvalue << 18) | ((uint32_t)pselvalue << 16) | ((uint32_t)rxonpol << 15) | ((uint32_t)txenpol << 14) | ((uint32_t)paonpol << 13) | ((uint32_t)regonpol << 12) | ((uint32_t)rxonvalue << 11) | ((uint32_t)txenvalue << 10) | ((uint32_t)paonvalue << 9) | ((uint32_t)regonvalue << 8) | ((uint32_t)forcepbus << 4) | ((uint32_t)forcerxon << 3) | ((uint32_t)forcetxen << 2) | ((uint32_t)forcepaon << 1) | ((uint32_t)forceregon << 0));
}

__INLINE void ble_radiocntl3_unpack(uint8_t* pbusvalue, uint8_t* pstbvalue, uint8_t* pselvalue, uint8_t* rxonpol, uint8_t* txenpol, uint8_t* paonpol, uint8_t* regonpol, uint8_t* rxonvalue, uint8_t* txenvalue, uint8_t* paonvalue, uint8_t* regonvalue, uint8_t* forcepbus, uint8_t* forcerxon, uint8_t* forcetxen, uint8_t* forcepaon, uint8_t* forceregon)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL3_ADDR);

    *pbusvalue = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *pstbvalue = (localVal & ((uint32_t)0x00040000)) >> 18;
    *pselvalue = (localVal & ((uint32_t)0x00030000)) >> 16;
    *rxonpol = (localVal & ((uint32_t)0x00008000)) >> 15;
    *txenpol = (localVal & ((uint32_t)0x00004000)) >> 14;
    *paonpol = (localVal & ((uint32_t)0x00002000)) >> 13;
    *regonpol = (localVal & ((uint32_t)0x00001000)) >> 12;
    *rxonvalue = (localVal & ((uint32_t)0x00000800)) >> 11;
    *txenvalue = (localVal & ((uint32_t)0x00000400)) >> 10;
    *paonvalue = (localVal & ((uint32_t)0x00000200)) >> 9;
    *regonvalue = (localVal & ((uint32_t)0x00000100)) >> 8;
    *forcepbus = (localVal & ((uint32_t)0x00000010)) >> 4;
    *forcerxon = (localVal & ((uint32_t)0x00000008)) >> 3;
    *forcetxen = (localVal & ((uint32_t)0x00000004)) >> 2;
    *forcepaon = (localVal & ((uint32_t)0x00000002)) >> 1;
    *forceregon = (localVal & ((uint32_t)0x00000001)) >> 0;
}

__INLINE uint8_t ble_pbus_value_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL3_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

__INLINE void ble_pbus_value_setf(uint8_t pbusvalue)
{
    ASSERT_ERR((((uint32_t)pbusvalue << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL3_ADDR, (REG_BLE_RD(BLE_RADIOCNTL3_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)pbusvalue << 24));
}

__INLINE uint8_t ble_pstb_value_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL3_ADDR);
    return ((localVal & ((uint32_t)0x00040000)) >> 18);
}

__INLINE void ble_pstb_value_setf(uint8_t pstbvalue)
{
    ASSERT_ERR((((uint32_t)pstbvalue << 18) & ~((uint32_t)0x00040000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL3_ADDR, (REG_BLE_RD(BLE_RADIOCNTL3_ADDR) & ~((uint32_t)0x00040000)) | ((uint32_t)pstbvalue << 18));
}

__INLINE uint8_t ble_psel_value_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL3_ADDR);
    return ((localVal & ((uint32_t)0x00030000)) >> 16);
}

__INLINE void ble_psel_value_setf(uint8_t pselvalue)
{
    ASSERT_ERR((((uint32_t)pselvalue << 16) & ~((uint32_t)0x00030000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL3_ADDR, (REG_BLE_RD(BLE_RADIOCNTL3_ADDR) & ~((uint32_t)0x00030000)) | ((uint32_t)pselvalue << 16));
}

__INLINE uint8_t ble_rx_on_pol_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL3_ADDR);
    return ((localVal & ((uint32_t)0x00008000)) >> 15);
}

__INLINE void ble_rx_on_pol_setf(uint8_t rxonpol)
{
    ASSERT_ERR((((uint32_t)rxonpol << 15) & ~((uint32_t)0x00008000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL3_ADDR, (REG_BLE_RD(BLE_RADIOCNTL3_ADDR) & ~((uint32_t)0x00008000)) | ((uint32_t)rxonpol << 15));
}

__INLINE uint8_t ble_tx_en_pol_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL3_ADDR);
    return ((localVal & ((uint32_t)0x00004000)) >> 14);
}

__INLINE void ble_tx_en_pol_setf(uint8_t txenpol)
{
    ASSERT_ERR((((uint32_t)txenpol << 14) & ~((uint32_t)0x00004000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL3_ADDR, (REG_BLE_RD(BLE_RADIOCNTL3_ADDR) & ~((uint32_t)0x00004000)) | ((uint32_t)txenpol << 14));
}

__INLINE uint8_t ble_pa_on_pol_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL3_ADDR);
    return ((localVal & ((uint32_t)0x00002000)) >> 13);
}

__INLINE void ble_pa_on_pol_setf(uint8_t paonpol)
{
    ASSERT_ERR((((uint32_t)paonpol << 13) & ~((uint32_t)0x00002000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL3_ADDR, (REG_BLE_RD(BLE_RADIOCNTL3_ADDR) & ~((uint32_t)0x00002000)) | ((uint32_t)paonpol << 13));
}

__INLINE uint8_t ble_reg_on_pol_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL3_ADDR);
    return ((localVal & ((uint32_t)0x00001000)) >> 12);
}

__INLINE void ble_reg_on_pol_setf(uint8_t regonpol)
{
    ASSERT_ERR((((uint32_t)regonpol << 12) & ~((uint32_t)0x00001000)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL3_ADDR, (REG_BLE_RD(BLE_RADIOCNTL3_ADDR) & ~((uint32_t)0x00001000)) | ((uint32_t)regonpol << 12));
}

__INLINE uint8_t ble_rx_on_value_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL3_ADDR);
    return ((localVal & ((uint32_t)0x00000800)) >> 11);
}

__INLINE void ble_rx_on_value_setf(uint8_t rxonvalue)
{
    ASSERT_ERR((((uint32_t)rxonvalue << 11) & ~((uint32_t)0x00000800)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL3_ADDR, (REG_BLE_RD(BLE_RADIOCNTL3_ADDR) & ~((uint32_t)0x00000800)) | ((uint32_t)rxonvalue << 11));
}

__INLINE uint8_t ble_tx_en_value_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL3_ADDR);
    return ((localVal & ((uint32_t)0x00000400)) >> 10);
}

__INLINE void ble_tx_en_value_setf(uint8_t txenvalue)
{
    ASSERT_ERR((((uint32_t)txenvalue << 10) & ~((uint32_t)0x00000400)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL3_ADDR, (REG_BLE_RD(BLE_RADIOCNTL3_ADDR) & ~((uint32_t)0x00000400)) | ((uint32_t)txenvalue << 10));
}

__INLINE uint8_t ble_pa_on_value_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL3_ADDR);
    return ((localVal & ((uint32_t)0x00000200)) >> 9);
}

__INLINE void ble_pa_on_value_setf(uint8_t paonvalue)
{
    ASSERT_ERR((((uint32_t)paonvalue << 9) & ~((uint32_t)0x00000200)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL3_ADDR, (REG_BLE_RD(BLE_RADIOCNTL3_ADDR) & ~((uint32_t)0x00000200)) | ((uint32_t)paonvalue << 9));
}

__INLINE uint8_t ble_reg_on_value_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL3_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

__INLINE void ble_reg_on_value_setf(uint8_t regonvalue)
{
    ASSERT_ERR((((uint32_t)regonvalue << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL3_ADDR, (REG_BLE_RD(BLE_RADIOCNTL3_ADDR) & ~((uint32_t)0x00000100)) | ((uint32_t)regonvalue << 8));
}

__INLINE uint8_t ble_force_pbus_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL3_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}

__INLINE void ble_force_pbus_setf(uint8_t forcepbus)
{
    ASSERT_ERR((((uint32_t)forcepbus << 4) & ~((uint32_t)0x00000010)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL3_ADDR, (REG_BLE_RD(BLE_RADIOCNTL3_ADDR) & ~((uint32_t)0x00000010)) | ((uint32_t)forcepbus << 4));
}

__INLINE uint8_t ble_force_rx_on_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL3_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}

__INLINE void ble_force_rx_on_setf(uint8_t forcerxon)
{
    ASSERT_ERR((((uint32_t)forcerxon << 3) & ~((uint32_t)0x00000008)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL3_ADDR, (REG_BLE_RD(BLE_RADIOCNTL3_ADDR) & ~((uint32_t)0x00000008)) | ((uint32_t)forcerxon << 3));
}

__INLINE uint8_t ble_force_tx_en_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL3_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}

__INLINE void ble_force_tx_en_setf(uint8_t forcetxen)
{
    ASSERT_ERR((((uint32_t)forcetxen << 2) & ~((uint32_t)0x00000004)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL3_ADDR, (REG_BLE_RD(BLE_RADIOCNTL3_ADDR) & ~((uint32_t)0x00000004)) | ((uint32_t)forcetxen << 2));
}

__INLINE uint8_t ble_force_pa_on_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL3_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

__INLINE void ble_force_pa_on_setf(uint8_t forcepaon)
{
    ASSERT_ERR((((uint32_t)forcepaon << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL3_ADDR, (REG_BLE_RD(BLE_RADIOCNTL3_ADDR) & ~((uint32_t)0x00000002)) | ((uint32_t)forcepaon << 1));
}

__INLINE uint8_t ble_force_reg_on_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL3_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

__INLINE void ble_force_reg_on_setf(uint8_t forceregon)
{
    ASSERT_ERR((((uint32_t)forceregon << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL3_ADDR, (REG_BLE_RD(BLE_RADIOCNTL3_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)forceregon << 0));
}

#endif // RW_BLE_CUST1_INST
/**
 * @brief RADIOPWRUPDN register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  30:24          RTRIP_DELAY   0x0
 *  23:16              RXPWRUP   0xD2
 *  11:08              TXPWRDN   0x3
 *  07:00              TXPWRUP   0xD2
 * </pre>
 */
#define BLE_RADIOPWRUPDN_ADDR   0x40000080
#define BLE_RADIOPWRUPDN_OFFSET 0x00000080
#define BLE_RADIOPWRUPDN_INDEX  0x00000020
#define BLE_RADIOPWRUPDN_RESET  0x00D203D2

__INLINE uint32_t ble_radiopwrupdn_get(void)
{
    return REG_BLE_RD(BLE_RADIOPWRUPDN_ADDR);
}

__INLINE void ble_radiopwrupdn_set(uint32_t value)
{
    REG_BLE_WR(BLE_RADIOPWRUPDN_ADDR, value);
}

// field definitions
#define BLE_RTRIP_DELAY_MASK   ((uint32_t)0x7F000000)
#define BLE_RTRIP_DELAY_LSB    24
#define BLE_RTRIP_DELAY_WIDTH  ((uint32_t)0x00000007)
#define BLE_RXPWRUP_MASK       ((uint32_t)0x00FF0000)
#define BLE_RXPWRUP_LSB        16
#define BLE_RXPWRUP_WIDTH      ((uint32_t)0x00000008)
#define BLE_TXPWRDN_MASK       ((uint32_t)0x00000F00)
#define BLE_TXPWRDN_LSB        8
#define BLE_TXPWRDN_WIDTH      ((uint32_t)0x00000004)
#define BLE_TXPWRUP_MASK       ((uint32_t)0x000000FF)
#define BLE_TXPWRUP_LSB        0
#define BLE_TXPWRUP_WIDTH      ((uint32_t)0x00000008)

#define BLE_RTRIP_DELAY_RST    0x0
#define BLE_RXPWRUP_RST        0xD2
#define BLE_TXPWRDN_RST        0x3
#define BLE_TXPWRUP_RST        0xD2

__INLINE void ble_radiopwrupdn_pack(uint8_t rtripdelay, uint8_t rxpwrup, uint8_t txpwrdn, uint8_t txpwrup)
{
    ASSERT_ERR((((uint32_t)rtripdelay << 24) & ~((uint32_t)0x7F000000)) == 0);
    ASSERT_ERR((((uint32_t)rxpwrup << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)txpwrdn << 8) & ~((uint32_t)0x00000F00)) == 0);
    ASSERT_ERR((((uint32_t)txpwrup << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_BLE_WR(BLE_RADIOPWRUPDN_ADDR,  ((uint32_t)rtripdelay << 24) | ((uint32_t)rxpwrup << 16) | ((uint32_t)txpwrdn << 8) | ((uint32_t)txpwrup << 0));
}

__INLINE void ble_radiopwrupdn_unpack(uint8_t* rtripdelay, uint8_t* rxpwrup, uint8_t* txpwrdn, uint8_t* txpwrup)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOPWRUPDN_ADDR);

    *rtripdelay = (localVal & ((uint32_t)0x7F000000)) >> 24;
    *rxpwrup = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *txpwrdn = (localVal & ((uint32_t)0x00000F00)) >> 8;
    *txpwrup = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

__INLINE uint8_t ble_rtrip_delay_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOPWRUPDN_ADDR);
    return ((localVal & ((uint32_t)0x7F000000)) >> 24);
}

__INLINE void ble_rtrip_delay_setf(uint8_t rtripdelay)
{
    ASSERT_ERR((((uint32_t)rtripdelay << 24) & ~((uint32_t)0x7F000000)) == 0);
    REG_BLE_WR(BLE_RADIOPWRUPDN_ADDR, (REG_BLE_RD(BLE_RADIOPWRUPDN_ADDR) & ~((uint32_t)0x7F000000)) | ((uint32_t)rtripdelay << 24));
}

__INLINE uint8_t ble_rxpwrup_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOPWRUPDN_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

__INLINE void ble_rxpwrup_setf(uint8_t rxpwrup)
{
    ASSERT_ERR((((uint32_t)rxpwrup << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_BLE_WR(BLE_RADIOPWRUPDN_ADDR, (REG_BLE_RD(BLE_RADIOPWRUPDN_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)rxpwrup << 16));
}

__INLINE uint8_t ble_txpwrdn_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOPWRUPDN_ADDR);
    return ((localVal & ((uint32_t)0x00000F00)) >> 8);
}

__INLINE void ble_txpwrdn_setf(uint8_t txpwrdn)
{
    ASSERT_ERR((((uint32_t)txpwrdn << 8) & ~((uint32_t)0x00000F00)) == 0);
    REG_BLE_WR(BLE_RADIOPWRUPDN_ADDR, (REG_BLE_RD(BLE_RADIOPWRUPDN_ADDR) & ~((uint32_t)0x00000F00)) | ((uint32_t)txpwrdn << 8));
}

__INLINE uint8_t ble_txpwrup_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOPWRUPDN_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

__INLINE void ble_txpwrup_setf(uint8_t txpwrup)
{
    ASSERT_ERR((((uint32_t)txpwrup << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_BLE_WR(BLE_RADIOPWRUPDN_ADDR, (REG_BLE_RD(BLE_RADIOPWRUPDN_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)txpwrup << 0));
}

#if RW_BLE_SUPPORT
/**
 * @brief RADIOCNTL4 register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  11:00      FORCEAGC_LENGTH   0x0
 * </pre>
 */
#define BLE_RADIOCNTL4_ADDR   0x40000084
#define BLE_RADIOCNTL4_OFFSET 0x00000084
#define BLE_RADIOCNTL4_INDEX  0x00000021
#define BLE_RADIOCNTL4_RESET  0x00000000

__INLINE uint32_t ble_radiocntl4_get(void)
{
    return REG_BLE_RD(BLE_RADIOCNTL4_ADDR);
}

__INLINE void ble_radiocntl4_set(uint32_t value)
{
    REG_BLE_WR(BLE_RADIOCNTL4_ADDR, value);
}

// field definitions
#define BLE_FORCEAGC_LENGTH_MASK   ((uint32_t)0x00000FFF)
#define BLE_FORCEAGC_LENGTH_LSB    0
#define BLE_FORCEAGC_LENGTH_WIDTH  ((uint32_t)0x0000000C)

#define BLE_FORCEAGC_LENGTH_RST    0x0

__INLINE uint16_t ble_forceagc_length_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RADIOCNTL4_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x00000FFF)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_forceagc_length_setf(uint16_t forceagclength)
{
    ASSERT_ERR((((uint32_t)forceagclength << 0) & ~((uint32_t)0x00000FFF)) == 0);
    REG_BLE_WR(BLE_RADIOCNTL4_ADDR, (uint32_t)forceagclength << 0);
}

#endif // RW_BLE_SUPPORT
/**
 * @brief ADVCHMAP register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  02:00             ADVCHMAP   0x7
 * </pre>
 */
#define BLE_ADVCHMAP_ADDR   0x40000090
#define BLE_ADVCHMAP_OFFSET 0x00000090
#define BLE_ADVCHMAP_INDEX  0x00000024
#define BLE_ADVCHMAP_RESET  0x00000007

__INLINE uint32_t ble_advchmap_get(void)
{
    return REG_BLE_RD(BLE_ADVCHMAP_ADDR);
}

__INLINE void ble_advchmap_set(uint32_t value)
{
    REG_BLE_WR(BLE_ADVCHMAP_ADDR, value);
}

// field definitions
#define BLE_ADVCHMAP_MASK   ((uint32_t)0x00000007)
#define BLE_ADVCHMAP_LSB    0
#define BLE_ADVCHMAP_WIDTH  ((uint32_t)0x00000003)

#define BLE_ADVCHMAP_RST    0x7

__INLINE uint8_t ble_advchmap_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_ADVCHMAP_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x00000007)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_advchmap_setf(uint8_t advchmap)
{
    ASSERT_ERR((((uint32_t)advchmap << 0) & ~((uint32_t)0x00000007)) == 0);
    REG_BLE_WR(BLE_ADVCHMAP_ADDR, (uint32_t)advchmap << 0);
}

/**
 * @brief ADVTIM register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  13:00               ADVINT   0x0
 * </pre>
 */
#define BLE_ADVTIM_ADDR   0x400000A0
#define BLE_ADVTIM_OFFSET 0x000000A0
#define BLE_ADVTIM_INDEX  0x00000028
#define BLE_ADVTIM_RESET  0x00000000

__INLINE uint32_t ble_advtim_get(void)
{
    return REG_BLE_RD(BLE_ADVTIM_ADDR);
}

__INLINE void ble_advtim_set(uint32_t value)
{
    REG_BLE_WR(BLE_ADVTIM_ADDR, value);
}

// field definitions
#define BLE_ADVINT_MASK   ((uint32_t)0x00003FFF)
#define BLE_ADVINT_LSB    0
#define BLE_ADVINT_WIDTH  ((uint32_t)0x0000000E)

#define BLE_ADVINT_RST    0x0

__INLINE uint16_t ble_advint_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_ADVTIM_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x00003FFF)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_advint_setf(uint16_t advint)
{
    ASSERT_ERR((((uint32_t)advint << 0) & ~((uint32_t)0x00003FFF)) == 0);
    REG_BLE_WR(BLE_ADVTIM_ADDR, (uint32_t)advint << 0);
}

/**
 * @brief ACTSCANSTAT register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  24:16              BACKOFF   0x1
 *  08:00           UPPERLIMIT   0x1
 * </pre>
 */
#define BLE_ACTSCANSTAT_ADDR   0x400000A4
#define BLE_ACTSCANSTAT_OFFSET 0x000000A4
#define BLE_ACTSCANSTAT_INDEX  0x00000029
#define BLE_ACTSCANSTAT_RESET  0x00010001

__INLINE uint32_t ble_actscanstat_get(void)
{
    return REG_BLE_RD(BLE_ACTSCANSTAT_ADDR);
}

// field definitions
#define BLE_BACKOFF_MASK      ((uint32_t)0x01FF0000)
#define BLE_BACKOFF_LSB       16
#define BLE_BACKOFF_WIDTH     ((uint32_t)0x00000009)
#define BLE_UPPERLIMIT_MASK   ((uint32_t)0x000001FF)
#define BLE_UPPERLIMIT_LSB    0
#define BLE_UPPERLIMIT_WIDTH  ((uint32_t)0x00000009)

#define BLE_BACKOFF_RST       0x1
#define BLE_UPPERLIMIT_RST    0x1

__INLINE void ble_actscanstat_unpack(uint16_t* backoff, uint16_t* upperlimit)
{
    uint32_t localVal = REG_BLE_RD(BLE_ACTSCANSTAT_ADDR);

    *backoff = (localVal & ((uint32_t)0x01FF0000)) >> 16;
    *upperlimit = (localVal & ((uint32_t)0x000001FF)) >> 0;
}

__INLINE uint16_t ble_backoff_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_ACTSCANSTAT_ADDR);
    return ((localVal & ((uint32_t)0x01FF0000)) >> 16);
}

__INLINE uint16_t ble_upperlimit_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_ACTSCANSTAT_ADDR);
    return ((localVal & ((uint32_t)0x000001FF)) >> 0);
}

/**
 * @brief WLPUBADDPTR register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  15:00          WLPUBADDPTR   0x0
 * </pre>
 */
#define BLE_WLPUBADDPTR_ADDR   0x400000B0
#define BLE_WLPUBADDPTR_OFFSET 0x000000B0
#define BLE_WLPUBADDPTR_INDEX  0x0000002C
#define BLE_WLPUBADDPTR_RESET  0x00000000

__INLINE uint32_t ble_wlpubaddptr_get(void)
{
    return REG_BLE_RD(BLE_WLPUBADDPTR_ADDR);
}

__INLINE void ble_wlpubaddptr_set(uint32_t value)
{
    REG_BLE_WR(BLE_WLPUBADDPTR_ADDR, value);
}

// field definitions
#define BLE_WLPUBADDPTR_MASK   ((uint32_t)0x0000FFFF)
#define BLE_WLPUBADDPTR_LSB    0
#define BLE_WLPUBADDPTR_WIDTH  ((uint32_t)0x00000010)

#define BLE_WLPUBADDPTR_RST    0x0

__INLINE uint16_t ble_wlpubaddptr_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_WLPUBADDPTR_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0000FFFF)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_wlpubaddptr_setf(uint16_t wlpubaddptr)
{
    ASSERT_ERR((((uint32_t)wlpubaddptr << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_BLE_WR(BLE_WLPUBADDPTR_ADDR, (uint32_t)wlpubaddptr << 0);
}

/**
 * @brief WLPRIVADDPTR register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  15:00         WLPRIVADDPTR   0x0
 * </pre>
 */
#define BLE_WLPRIVADDPTR_ADDR   0x400000B4
#define BLE_WLPRIVADDPTR_OFFSET 0x000000B4
#define BLE_WLPRIVADDPTR_INDEX  0x0000002D
#define BLE_WLPRIVADDPTR_RESET  0x00000000

__INLINE uint32_t ble_wlprivaddptr_get(void)
{
    return REG_BLE_RD(BLE_WLPRIVADDPTR_ADDR);
}

__INLINE void ble_wlprivaddptr_set(uint32_t value)
{
    REG_BLE_WR(BLE_WLPRIVADDPTR_ADDR, value);
}

// field definitions
#define BLE_WLPRIVADDPTR_MASK   ((uint32_t)0x0000FFFF)
#define BLE_WLPRIVADDPTR_LSB    0
#define BLE_WLPRIVADDPTR_WIDTH  ((uint32_t)0x00000010)

#define BLE_WLPRIVADDPTR_RST    0x0

__INLINE uint16_t ble_wlprivaddptr_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_WLPRIVADDPTR_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0000FFFF)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_wlprivaddptr_setf(uint16_t wlprivaddptr)
{
    ASSERT_ERR((((uint32_t)wlprivaddptr << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_BLE_WR(BLE_WLPRIVADDPTR_ADDR, (uint32_t)wlprivaddptr << 0);
}

/**
 * @brief WLNBDEV register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  15:08            NBPRIVDEV   0x0
 *  07:00             NBPUBDEV   0x0
 * </pre>
 */
#define BLE_WLNBDEV_ADDR   0x400000B8
#define BLE_WLNBDEV_OFFSET 0x000000B8
#define BLE_WLNBDEV_INDEX  0x0000002E
#define BLE_WLNBDEV_RESET  0x00000000

__INLINE uint32_t ble_wlnbdev_get(void)
{
    return REG_BLE_RD(BLE_WLNBDEV_ADDR);
}

__INLINE void ble_wlnbdev_set(uint32_t value)
{
    REG_BLE_WR(BLE_WLNBDEV_ADDR, value);
}

// field definitions
#define BLE_NBPRIVDEV_MASK   ((uint32_t)0x0000FF00)
#define BLE_NBPRIVDEV_LSB    8
#define BLE_NBPRIVDEV_WIDTH  ((uint32_t)0x00000008)
#define BLE_NBPUBDEV_MASK    ((uint32_t)0x000000FF)
#define BLE_NBPUBDEV_LSB     0
#define BLE_NBPUBDEV_WIDTH   ((uint32_t)0x00000008)

#define BLE_NBPRIVDEV_RST    0x0
#define BLE_NBPUBDEV_RST     0x0

__INLINE void ble_wlnbdev_pack(uint8_t nbprivdev, uint8_t nbpubdev)
{
    ASSERT_ERR((((uint32_t)nbprivdev << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)nbpubdev << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_BLE_WR(BLE_WLNBDEV_ADDR,  ((uint32_t)nbprivdev << 8) | ((uint32_t)nbpubdev << 0));
}

__INLINE void ble_wlnbdev_unpack(uint8_t* nbprivdev, uint8_t* nbpubdev)
{
    uint32_t localVal = REG_BLE_RD(BLE_WLNBDEV_ADDR);

    *nbprivdev = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *nbpubdev = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

__INLINE uint8_t ble_nbprivdev_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_WLNBDEV_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

__INLINE void ble_nbprivdev_setf(uint8_t nbprivdev)
{
    ASSERT_ERR((((uint32_t)nbprivdev << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_BLE_WR(BLE_WLNBDEV_ADDR, (REG_BLE_RD(BLE_WLNBDEV_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)nbprivdev << 8));
}

__INLINE uint8_t ble_nbpubdev_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_WLNBDEV_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

__INLINE void ble_nbpubdev_setf(uint8_t nbpubdev)
{
    ASSERT_ERR((((uint32_t)nbpubdev << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_BLE_WR(BLE_WLNBDEV_ADDR, (REG_BLE_RD(BLE_WLNBDEV_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)nbpubdev << 0));
}

#if RW_BLE_USE_CRYPT
/**
 * @brief AESCNTL register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     00            AES_START   0
 * </pre>
 */
#define BLE_AESCNTL_ADDR   0x400000C0
#define BLE_AESCNTL_OFFSET 0x000000C0
#define BLE_AESCNTL_INDEX  0x00000030
#define BLE_AESCNTL_RESET  0x00000000

__INLINE uint32_t ble_aescntl_get(void)
{
    return REG_BLE_RD(BLE_AESCNTL_ADDR);
}

__INLINE void ble_aescntl_set(uint32_t value)
{
    REG_BLE_WR(BLE_AESCNTL_ADDR, value);
}

// field definitions
#define BLE_AES_START_BIT    ((uint32_t)0x00000001)
#define BLE_AES_START_POS    0

#define BLE_AES_START_RST    0x0

__INLINE uint8_t ble_aes_start_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_AESCNTL_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x00000001)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_aes_start_setf(uint8_t aesstart)
{
    ASSERT_ERR((((uint32_t)aesstart << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_BLE_WR(BLE_AESCNTL_ADDR, (uint32_t)aesstart << 0);
}

#endif // RW_BLE_USE_CRYPT
#if RW_BLE_USE_CRYPT
/**
 * @brief AESKEY31_0 register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:00           AESKEY31_0   0x0
 * </pre>
 */
#define BLE_AESKEY31_0_ADDR   0x400000C4
#define BLE_AESKEY31_0_OFFSET 0x000000C4
#define BLE_AESKEY31_0_INDEX  0x00000031
#define BLE_AESKEY31_0_RESET  0x00000000

__INLINE uint32_t ble_aeskey31_0_get(void)
{
    return REG_BLE_RD(BLE_AESKEY31_0_ADDR);
}

__INLINE void ble_aeskey31_0_set(uint32_t value)
{
    REG_BLE_WR(BLE_AESKEY31_0_ADDR, value);
}

// field definitions
#define BLE_AESKEY31_0_MASK   ((uint32_t)0xFFFFFFFF)
#define BLE_AESKEY31_0_LSB    0
#define BLE_AESKEY31_0_WIDTH  ((uint32_t)0x00000020)

#define BLE_AESKEY31_0_RST    0x0

__INLINE uint32_t ble_aeskey31_0_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_AESKEY31_0_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0xFFFFFFFF)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_aeskey31_0_setf(uint32_t aeskey310)
{
    ASSERT_ERR((((uint32_t)aeskey310 << 0) & ~((uint32_t)0xFFFFFFFF)) == 0);
    REG_BLE_WR(BLE_AESKEY31_0_ADDR, (uint32_t)aeskey310 << 0);
}

#endif // RW_BLE_USE_CRYPT
#if RW_BLE_USE_CRYPT
/**
 * @brief AESKEY63_32 register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:00          AESKEY63_32   0x0
 * </pre>
 */
#define BLE_AESKEY63_32_ADDR   0x400000C8
#define BLE_AESKEY63_32_OFFSET 0x000000C8
#define BLE_AESKEY63_32_INDEX  0x00000032
#define BLE_AESKEY63_32_RESET  0x00000000

__INLINE uint32_t ble_aeskey63_32_get(void)
{
    return REG_BLE_RD(BLE_AESKEY63_32_ADDR);
}

__INLINE void ble_aeskey63_32_set(uint32_t value)
{
    REG_BLE_WR(BLE_AESKEY63_32_ADDR, value);
}

// field definitions
#define BLE_AESKEY63_32_MASK   ((uint32_t)0xFFFFFFFF)
#define BLE_AESKEY63_32_LSB    0
#define BLE_AESKEY63_32_WIDTH  ((uint32_t)0x00000020)

#define BLE_AESKEY63_32_RST    0x0

__INLINE uint32_t ble_aeskey63_32_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_AESKEY63_32_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0xFFFFFFFF)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_aeskey63_32_setf(uint32_t aeskey6332)
{
    ASSERT_ERR((((uint32_t)aeskey6332 << 0) & ~((uint32_t)0xFFFFFFFF)) == 0);
    REG_BLE_WR(BLE_AESKEY63_32_ADDR, (uint32_t)aeskey6332 << 0);
}

#endif // RW_BLE_USE_CRYPT
#if RW_BLE_USE_CRYPT
/**
 * @brief AESKEY95_64 register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:00          AESKEY95_64   0x0
 * </pre>
 */
#define BLE_AESKEY95_64_ADDR   0x400000CC
#define BLE_AESKEY95_64_OFFSET 0x000000CC
#define BLE_AESKEY95_64_INDEX  0x00000033
#define BLE_AESKEY95_64_RESET  0x00000000

__INLINE uint32_t ble_aeskey95_64_get(void)
{
    return REG_BLE_RD(BLE_AESKEY95_64_ADDR);
}

__INLINE void ble_aeskey95_64_set(uint32_t value)
{
    REG_BLE_WR(BLE_AESKEY95_64_ADDR, value);
}

// field definitions
#define BLE_AESKEY95_64_MASK   ((uint32_t)0xFFFFFFFF)
#define BLE_AESKEY95_64_LSB    0
#define BLE_AESKEY95_64_WIDTH  ((uint32_t)0x00000020)

#define BLE_AESKEY95_64_RST    0x0

__INLINE uint32_t ble_aeskey95_64_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_AESKEY95_64_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0xFFFFFFFF)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_aeskey95_64_setf(uint32_t aeskey9564)
{
    ASSERT_ERR((((uint32_t)aeskey9564 << 0) & ~((uint32_t)0xFFFFFFFF)) == 0);
    REG_BLE_WR(BLE_AESKEY95_64_ADDR, (uint32_t)aeskey9564 << 0);
}

#endif // RW_BLE_USE_CRYPT
#if RW_BLE_USE_CRYPT
/**
 * @brief AESKEY127_96 register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:00         AESKEY127_96   0x0
 * </pre>
 */
#define BLE_AESKEY127_96_ADDR   0x400000D0
#define BLE_AESKEY127_96_OFFSET 0x000000D0
#define BLE_AESKEY127_96_INDEX  0x00000034
#define BLE_AESKEY127_96_RESET  0x00000000

__INLINE uint32_t ble_aeskey127_96_get(void)
{
    return REG_BLE_RD(BLE_AESKEY127_96_ADDR);
}

__INLINE void ble_aeskey127_96_set(uint32_t value)
{
    REG_BLE_WR(BLE_AESKEY127_96_ADDR, value);
}

// field definitions
#define BLE_AESKEY127_96_MASK   ((uint32_t)0xFFFFFFFF)
#define BLE_AESKEY127_96_LSB    0
#define BLE_AESKEY127_96_WIDTH  ((uint32_t)0x00000020)

#define BLE_AESKEY127_96_RST    0x0

__INLINE uint32_t ble_aeskey127_96_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_AESKEY127_96_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0xFFFFFFFF)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_aeskey127_96_setf(uint32_t aeskey12796)
{
    ASSERT_ERR((((uint32_t)aeskey12796 << 0) & ~((uint32_t)0xFFFFFFFF)) == 0);
    REG_BLE_WR(BLE_AESKEY127_96_ADDR, (uint32_t)aeskey12796 << 0);
}

#endif // RW_BLE_USE_CRYPT
#if RW_BLE_USE_CRYPT
/**
 * @brief AESPTR register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  15:00               AESPTR   0x0
 * </pre>
 */
#define BLE_AESPTR_ADDR   0x400000D4
#define BLE_AESPTR_OFFSET 0x000000D4
#define BLE_AESPTR_INDEX  0x00000035
#define BLE_AESPTR_RESET  0x00000000

__INLINE uint32_t ble_aesptr_get(void)
{
    return REG_BLE_RD(BLE_AESPTR_ADDR);
}

__INLINE void ble_aesptr_set(uint32_t value)
{
    REG_BLE_WR(BLE_AESPTR_ADDR, value);
}

// field definitions
#define BLE_AESPTR_MASK   ((uint32_t)0x0000FFFF)
#define BLE_AESPTR_LSB    0
#define BLE_AESPTR_WIDTH  ((uint32_t)0x00000010)

#define BLE_AESPTR_RST    0x0

__INLINE uint16_t ble_aesptr_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_AESPTR_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0000FFFF)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_aesptr_setf(uint16_t aesptr)
{
    ASSERT_ERR((((uint32_t)aesptr << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_BLE_WR(BLE_AESPTR_ADDR, (uint32_t)aesptr << 0);
}

#endif // RW_BLE_USE_CRYPT
#if RW_BLE_USE_CRYPT
/**
 * @brief TXMICVAL register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:00             TXMICVAL   0x0
 * </pre>
 */
#define BLE_TXMICVAL_ADDR   0x400000D8
#define BLE_TXMICVAL_OFFSET 0x000000D8
#define BLE_TXMICVAL_INDEX  0x00000036
#define BLE_TXMICVAL_RESET  0x00000000

__INLINE uint32_t ble_txmicval_get(void)
{
    return REG_BLE_RD(BLE_TXMICVAL_ADDR);
}

// field definitions
#define BLE_TXMICVAL_MASK   ((uint32_t)0xFFFFFFFF)
#define BLE_TXMICVAL_LSB    0
#define BLE_TXMICVAL_WIDTH  ((uint32_t)0x00000020)

#define BLE_TXMICVAL_RST    0x0

__INLINE uint32_t ble_txmicval_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_TXMICVAL_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0xFFFFFFFF)) == 0);
    return (localVal >> 0);
}

#endif // RW_BLE_USE_CRYPT
#if RW_BLE_USE_CRYPT
/**
 * @brief RXMICVAL register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:00             RXMICVAL   0x0
 * </pre>
 */
#define BLE_RXMICVAL_ADDR   0x400000DC
#define BLE_RXMICVAL_OFFSET 0x000000DC
#define BLE_RXMICVAL_INDEX  0x00000037
#define BLE_RXMICVAL_RESET  0x00000000

__INLINE uint32_t ble_rxmicval_get(void)
{
    return REG_BLE_RD(BLE_RXMICVAL_ADDR);
}

// field definitions
#define BLE_RXMICVAL_MASK   ((uint32_t)0xFFFFFFFF)
#define BLE_RXMICVAL_LSB    0
#define BLE_RXMICVAL_WIDTH  ((uint32_t)0x00000020)

#define BLE_RXMICVAL_RST    0x0

__INLINE uint32_t ble_rxmicval_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RXMICVAL_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0xFFFFFFFF)) == 0);
    return (localVal >> 0);
}

#endif // RW_BLE_USE_CRYPT
/**
 * @brief RFTESTCNTL register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31           INFINITERX   0
 *     15           INFINITETX   0
 *     14          TXLENGTHSRC   0
 *     13             PRBSTYPE   0
 *     12             TXPLDSRC   0
 *  08:00             TXLENGTH   0x0
 * </pre>
 */
#define BLE_RFTESTCNTL_ADDR   0x400000E0
#define BLE_RFTESTCNTL_OFFSET 0x000000E0
#define BLE_RFTESTCNTL_INDEX  0x00000038
#define BLE_RFTESTCNTL_RESET  0x00000000

__INLINE uint32_t ble_rftestcntl_get(void)
{
    return REG_BLE_RD(BLE_RFTESTCNTL_ADDR);
}

__INLINE void ble_rftestcntl_set(uint32_t value)
{
    REG_BLE_WR(BLE_RFTESTCNTL_ADDR, value);
}

// field definitions
#define BLE_INFINITERX_BIT     ((uint32_t)0x80000000)
#define BLE_INFINITERX_POS     31
#define BLE_INFINITETX_BIT     ((uint32_t)0x00008000)
#define BLE_INFINITETX_POS     15
#define BLE_TXLENGTHSRC_BIT    ((uint32_t)0x00004000)
#define BLE_TXLENGTHSRC_POS    14
#define BLE_PRBSTYPE_BIT       ((uint32_t)0x00002000)
#define BLE_PRBSTYPE_POS       13
#define BLE_TXPLDSRC_BIT       ((uint32_t)0x00001000)
#define BLE_TXPLDSRC_POS       12
#define BLE_TXLENGTH_MASK      ((uint32_t)0x000001FF)
#define BLE_TXLENGTH_LSB       0
#define BLE_TXLENGTH_WIDTH     ((uint32_t)0x00000009)

#define BLE_INFINITERX_RST     0x0
#define BLE_INFINITETX_RST     0x0
#define BLE_TXLENGTHSRC_RST    0x0
#define BLE_PRBSTYPE_RST       0x0
#define BLE_TXPLDSRC_RST       0x0
#define BLE_TXLENGTH_RST       0x0

__INLINE void ble_rftestcntl_pack(uint8_t infiniterx, uint8_t infinitetx, uint8_t txlengthsrc, uint8_t prbstype, uint8_t txpldsrc, uint16_t txlength)
{
    ASSERT_ERR((((uint32_t)infiniterx << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)infinitetx << 15) & ~((uint32_t)0x00008000)) == 0);
    ASSERT_ERR((((uint32_t)txlengthsrc << 14) & ~((uint32_t)0x00004000)) == 0);
    ASSERT_ERR((((uint32_t)prbstype << 13) & ~((uint32_t)0x00002000)) == 0);
    ASSERT_ERR((((uint32_t)txpldsrc << 12) & ~((uint32_t)0x00001000)) == 0);
    ASSERT_ERR((((uint32_t)txlength << 0) & ~((uint32_t)0x000001FF)) == 0);
    REG_BLE_WR(BLE_RFTESTCNTL_ADDR,  ((uint32_t)infiniterx << 31) | ((uint32_t)infinitetx << 15) | ((uint32_t)txlengthsrc << 14) | ((uint32_t)prbstype << 13) | ((uint32_t)txpldsrc << 12) | ((uint32_t)txlength << 0));
}

__INLINE void ble_rftestcntl_unpack(uint8_t* infiniterx, uint8_t* infinitetx, uint8_t* txlengthsrc, uint8_t* prbstype, uint8_t* txpldsrc, uint16_t* txlength)
{
    uint32_t localVal = REG_BLE_RD(BLE_RFTESTCNTL_ADDR);

    *infiniterx = (localVal & ((uint32_t)0x80000000)) >> 31;
    *infinitetx = (localVal & ((uint32_t)0x00008000)) >> 15;
    *txlengthsrc = (localVal & ((uint32_t)0x00004000)) >> 14;
    *prbstype = (localVal & ((uint32_t)0x00002000)) >> 13;
    *txpldsrc = (localVal & ((uint32_t)0x00001000)) >> 12;
    *txlength = (localVal & ((uint32_t)0x000001FF)) >> 0;
}

__INLINE uint8_t ble_infiniterx_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RFTESTCNTL_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

__INLINE void ble_infiniterx_setf(uint8_t infiniterx)
{
    ASSERT_ERR((((uint32_t)infiniterx << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_BLE_WR(BLE_RFTESTCNTL_ADDR, (REG_BLE_RD(BLE_RFTESTCNTL_ADDR) & ~((uint32_t)0x80000000)) | ((uint32_t)infiniterx << 31));
}

__INLINE uint8_t ble_infinitetx_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RFTESTCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00008000)) >> 15);
}

__INLINE void ble_infinitetx_setf(uint8_t infinitetx)
{
    ASSERT_ERR((((uint32_t)infinitetx << 15) & ~((uint32_t)0x00008000)) == 0);
    REG_BLE_WR(BLE_RFTESTCNTL_ADDR, (REG_BLE_RD(BLE_RFTESTCNTL_ADDR) & ~((uint32_t)0x00008000)) | ((uint32_t)infinitetx << 15));
}

__INLINE uint8_t ble_txlengthsrc_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RFTESTCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00004000)) >> 14);
}

__INLINE void ble_txlengthsrc_setf(uint8_t txlengthsrc)
{
    ASSERT_ERR((((uint32_t)txlengthsrc << 14) & ~((uint32_t)0x00004000)) == 0);
    REG_BLE_WR(BLE_RFTESTCNTL_ADDR, (REG_BLE_RD(BLE_RFTESTCNTL_ADDR) & ~((uint32_t)0x00004000)) | ((uint32_t)txlengthsrc << 14));
}

__INLINE uint8_t ble_prbstype_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RFTESTCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00002000)) >> 13);
}

__INLINE void ble_prbstype_setf(uint8_t prbstype)
{
    ASSERT_ERR((((uint32_t)prbstype << 13) & ~((uint32_t)0x00002000)) == 0);
    REG_BLE_WR(BLE_RFTESTCNTL_ADDR, (REG_BLE_RD(BLE_RFTESTCNTL_ADDR) & ~((uint32_t)0x00002000)) | ((uint32_t)prbstype << 13));
}

__INLINE uint8_t ble_txpldsrc_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RFTESTCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00001000)) >> 12);
}

__INLINE void ble_txpldsrc_setf(uint8_t txpldsrc)
{
    ASSERT_ERR((((uint32_t)txpldsrc << 12) & ~((uint32_t)0x00001000)) == 0);
    REG_BLE_WR(BLE_RFTESTCNTL_ADDR, (REG_BLE_RD(BLE_RFTESTCNTL_ADDR) & ~((uint32_t)0x00001000)) | ((uint32_t)txpldsrc << 12));
}

__INLINE uint16_t ble_txlength_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_RFTESTCNTL_ADDR);
    return ((localVal & ((uint32_t)0x000001FF)) >> 0);
}

__INLINE void ble_txlength_setf(uint16_t txlength)
{
    ASSERT_ERR((((uint32_t)txlength << 0) & ~((uint32_t)0x000001FF)) == 0);
    REG_BLE_WR(BLE_RFTESTCNTL_ADDR, (REG_BLE_RD(BLE_RFTESTCNTL_ADDR) & ~((uint32_t)0x000001FF)) | ((uint32_t)txlength << 0));
}

/**
 * @brief TIMGENCNTL register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  08:00        PREFETCH_TIME   0x137
 * </pre>
 */
#define BLE_TIMGENCNTL_ADDR   0x400000F0
#define BLE_TIMGENCNTL_OFFSET 0x000000F0
#define BLE_TIMGENCNTL_INDEX  0x0000003C
#define BLE_TIMGENCNTL_RESET  0x00000137

__INLINE uint32_t ble_timgencntl_get(void)
{
    return REG_BLE_RD(BLE_TIMGENCNTL_ADDR);
}

__INLINE void ble_timgencntl_set(uint32_t value)
{
    REG_BLE_WR(BLE_TIMGENCNTL_ADDR, value);
}

// field definitions
#define BLE_PREFETCH_TIME_MASK   ((uint32_t)0x000001FF)
#define BLE_PREFETCH_TIME_LSB    0
#define BLE_PREFETCH_TIME_WIDTH  ((uint32_t)0x00000009)

#define BLE_PREFETCH_TIME_RST    0x137

__INLINE uint16_t ble_prefetch_time_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_TIMGENCNTL_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000001FF)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_prefetch_time_setf(uint16_t prefetchtime)
{
    ASSERT_ERR((((uint32_t)prefetchtime << 0) & ~((uint32_t)0x000001FF)) == 0);
    REG_BLE_WR(BLE_TIMGENCNTL_ADDR, (uint32_t)prefetchtime << 0);
}

/**
 * @brief GROSSTIMTGT register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  15:00          GROSSTARGET   0x0
 * </pre>
 */
#define BLE_GROSSTIMTGT_ADDR   0x400000F4
#define BLE_GROSSTIMTGT_OFFSET 0x000000F4
#define BLE_GROSSTIMTGT_INDEX  0x0000003D
#define BLE_GROSSTIMTGT_RESET  0x00000000

__INLINE uint32_t ble_grosstimtgt_get(void)
{
    return REG_BLE_RD(BLE_GROSSTIMTGT_ADDR);
}

__INLINE void ble_grosstimtgt_set(uint32_t value)
{
    REG_BLE_WR(BLE_GROSSTIMTGT_ADDR, value);
}

// field definitions
#define BLE_GROSSTARGET_MASK   ((uint32_t)0x0000FFFF)
#define BLE_GROSSTARGET_LSB    0
#define BLE_GROSSTARGET_WIDTH  ((uint32_t)0x00000010)

#define BLE_GROSSTARGET_RST    0x0

__INLINE uint16_t ble_grosstarget_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_GROSSTIMTGT_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0000FFFF)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_grosstarget_setf(uint16_t grosstarget)
{
    ASSERT_ERR((((uint32_t)grosstarget << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_BLE_WR(BLE_GROSSTIMTGT_ADDR, (uint32_t)grosstarget << 0);
}

/**
 * @brief FINETIMTGT register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  26:00           FINETARGET   0x0
 * </pre>
 */
#define BLE_FINETIMTGT_ADDR   0x400000F8
#define BLE_FINETIMTGT_OFFSET 0x000000F8
#define BLE_FINETIMTGT_INDEX  0x0000003E
#define BLE_FINETIMTGT_RESET  0x00000000

__INLINE uint32_t ble_finetimtgt_get(void)
{
    return REG_BLE_RD(BLE_FINETIMTGT_ADDR);
}

__INLINE void ble_finetimtgt_set(uint32_t value)
{
    REG_BLE_WR(BLE_FINETIMTGT_ADDR, value);
}

// field definitions
#define BLE_FINETARGET_MASK   ((uint32_t)0x07FFFFFF)
#define BLE_FINETARGET_LSB    0
#define BLE_FINETARGET_WIDTH  ((uint32_t)0x0000001B)

#define BLE_FINETARGET_RST    0x0

__INLINE uint32_t ble_finetarget_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_FINETIMTGT_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x07FFFFFF)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_finetarget_setf(uint32_t finetarget)
{
    ASSERT_ERR((((uint32_t)finetarget << 0) & ~((uint32_t)0x07FFFFFF)) == 0);
    REG_BLE_WR(BLE_FINETIMTGT_ADDR, (uint32_t)finetarget << 0);
}

/**
 * @brief SAMPLECLK register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     00                 SAMP   0
 * </pre>
 */
#define BLE_SAMPLECLK_ADDR   0x400000FC
#define BLE_SAMPLECLK_OFFSET 0x000000FC
#define BLE_SAMPLECLK_INDEX  0x0000003F
#define BLE_SAMPLECLK_RESET  0x00000000

__INLINE uint32_t ble_sampleclk_get(void)
{
    return REG_BLE_RD(BLE_SAMPLECLK_ADDR);
}

__INLINE void ble_sampleclk_set(uint32_t value)
{
    REG_BLE_WR(BLE_SAMPLECLK_ADDR, value);
}

// field definitions
#define BLE_SAMP_BIT    ((uint32_t)0x00000001)
#define BLE_SAMP_POS    0

#define BLE_SAMP_RST    0x0

__INLINE uint8_t ble_samp_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_SAMPLECLK_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x00000001)) == 0);
    return (localVal >> 0);
}

__INLINE void ble_samp_setf(uint8_t samp)
{
    ASSERT_ERR((((uint32_t)samp << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_BLE_WR(BLE_SAMPLECLK_ADDR, (uint32_t)samp << 0);
}

#if RW_BLE_WLAN_COEX
/**
 * @brief WLANCNTL register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  28:24        WLCPTHRESHOLD   0x0
 *  22:16         WLCPDURATION   0x0
 *  14:08            WLCPDELAY   0x0
 *     04           WLCRXTXPOL   0
 *  03:02          WLCPRIOMODE   0x0
 *  01:00              WLCMODE   0x0
 * </pre>
 */
#define BLE_WLANCNTL_ADDR   0x40000100
#define BLE_WLANCNTL_OFFSET 0x00000100
#define BLE_WLANCNTL_INDEX  0x00000040
#define BLE_WLANCNTL_RESET  0x00000000

__INLINE uint32_t ble_wlancntl_get(void)
{
    return REG_BLE_RD(BLE_WLANCNTL_ADDR);
}

__INLINE void ble_wlancntl_set(uint32_t value)
{
    REG_BLE_WR(BLE_WLANCNTL_ADDR, value);
}

// field definitions
#define BLE_WLCPTHRESHOLD_MASK   ((uint32_t)0x1F000000)
#define BLE_WLCPTHRESHOLD_LSB    24
#define BLE_WLCPTHRESHOLD_WIDTH  ((uint32_t)0x00000005)
#define BLE_WLCPDURATION_MASK    ((uint32_t)0x007F0000)
#define BLE_WLCPDURATION_LSB     16
#define BLE_WLCPDURATION_WIDTH   ((uint32_t)0x00000007)
#define BLE_WLCPDELAY_MASK       ((uint32_t)0x00007F00)
#define BLE_WLCPDELAY_LSB        8
#define BLE_WLCPDELAY_WIDTH      ((uint32_t)0x00000007)
#define BLE_WLCRXTXPOL_BIT       ((uint32_t)0x00000010)
#define BLE_WLCRXTXPOL_POS       4
#define BLE_WLCPRIOMODE_MASK     ((uint32_t)0x0000000C)
#define BLE_WLCPRIOMODE_LSB      2
#define BLE_WLCPRIOMODE_WIDTH    ((uint32_t)0x00000002)
#define BLE_WLCMODE_MASK         ((uint32_t)0x00000003)
#define BLE_WLCMODE_LSB          0
#define BLE_WLCMODE_WIDTH        ((uint32_t)0x00000002)

#define BLE_WLCPTHRESHOLD_RST    0x0
#define BLE_WLCPDURATION_RST     0x0
#define BLE_WLCPDELAY_RST        0x0
#define BLE_WLCRXTXPOL_RST       0x0
#define BLE_WLCPRIOMODE_RST      0x0
#define BLE_WLCMODE_RST          0x0

__INLINE void ble_wlancntl_pack(uint8_t wlcpthreshold, uint8_t wlcpduration, uint8_t wlcpdelay, uint8_t wlcrxtxpol, uint8_t wlcpriomode, uint8_t wlcmode)
{
    ASSERT_ERR((((uint32_t)wlcpthreshold << 24) & ~((uint32_t)0x1F000000)) == 0);
    ASSERT_ERR((((uint32_t)wlcpduration << 16) & ~((uint32_t)0x007F0000)) == 0);
    ASSERT_ERR((((uint32_t)wlcpdelay << 8) & ~((uint32_t)0x00007F00)) == 0);
    ASSERT_ERR((((uint32_t)wlcrxtxpol << 4) & ~((uint32_t)0x00000010)) == 0);
    ASSERT_ERR((((uint32_t)wlcpriomode << 2) & ~((uint32_t)0x0000000C)) == 0);
    ASSERT_ERR((((uint32_t)wlcmode << 0) & ~((uint32_t)0x00000003)) == 0);
    REG_BLE_WR(BLE_WLANCNTL_ADDR,  ((uint32_t)wlcpthreshold << 24) | ((uint32_t)wlcpduration << 16) | ((uint32_t)wlcpdelay << 8) | ((uint32_t)wlcrxtxpol << 4) | ((uint32_t)wlcpriomode << 2) | ((uint32_t)wlcmode << 0));
}

__INLINE void ble_wlancntl_unpack(uint8_t* wlcpthreshold, uint8_t* wlcpduration, uint8_t* wlcpdelay, uint8_t* wlcrxtxpol, uint8_t* wlcpriomode, uint8_t* wlcmode)
{
    uint32_t localVal = REG_BLE_RD(BLE_WLANCNTL_ADDR);

    *wlcpthreshold = (localVal & ((uint32_t)0x1F000000)) >> 24;
    *wlcpduration = (localVal & ((uint32_t)0x007F0000)) >> 16;
    *wlcpdelay = (localVal & ((uint32_t)0x00007F00)) >> 8;
    *wlcrxtxpol = (localVal & ((uint32_t)0x00000010)) >> 4;
    *wlcpriomode = (localVal & ((uint32_t)0x0000000C)) >> 2;
    *wlcmode = (localVal & ((uint32_t)0x00000003)) >> 0;
}

__INLINE uint8_t ble_wlcpthreshold_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_WLANCNTL_ADDR);
    return ((localVal & ((uint32_t)0x1F000000)) >> 24);
}

__INLINE void ble_wlcpthreshold_setf(uint8_t wlcpthreshold)
{
    ASSERT_ERR((((uint32_t)wlcpthreshold << 24) & ~((uint32_t)0x1F000000)) == 0);
    REG_BLE_WR(BLE_WLANCNTL_ADDR, (REG_BLE_RD(BLE_WLANCNTL_ADDR) & ~((uint32_t)0x1F000000)) | ((uint32_t)wlcpthreshold << 24));
}

__INLINE uint8_t ble_wlcpduration_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_WLANCNTL_ADDR);
    return ((localVal & ((uint32_t)0x007F0000)) >> 16);
}

__INLINE void ble_wlcpduration_setf(uint8_t wlcpduration)
{
    ASSERT_ERR((((uint32_t)wlcpduration << 16) & ~((uint32_t)0x007F0000)) == 0);
    REG_BLE_WR(BLE_WLANCNTL_ADDR, (REG_BLE_RD(BLE_WLANCNTL_ADDR) & ~((uint32_t)0x007F0000)) | ((uint32_t)wlcpduration << 16));
}

__INLINE uint8_t ble_wlcpdelay_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_WLANCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00007F00)) >> 8);
}

__INLINE void ble_wlcpdelay_setf(uint8_t wlcpdelay)
{
    ASSERT_ERR((((uint32_t)wlcpdelay << 8) & ~((uint32_t)0x00007F00)) == 0);
    REG_BLE_WR(BLE_WLANCNTL_ADDR, (REG_BLE_RD(BLE_WLANCNTL_ADDR) & ~((uint32_t)0x00007F00)) | ((uint32_t)wlcpdelay << 8));
}

__INLINE uint8_t ble_wlcrxtxpol_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_WLANCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}

__INLINE void ble_wlcrxtxpol_setf(uint8_t wlcrxtxpol)
{
    ASSERT_ERR((((uint32_t)wlcrxtxpol << 4) & ~((uint32_t)0x00000010)) == 0);
    REG_BLE_WR(BLE_WLANCNTL_ADDR, (REG_BLE_RD(BLE_WLANCNTL_ADDR) & ~((uint32_t)0x00000010)) | ((uint32_t)wlcrxtxpol << 4));
}

__INLINE uint8_t ble_wlcpriomode_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_WLANCNTL_ADDR);
    return ((localVal & ((uint32_t)0x0000000C)) >> 2);
}

__INLINE void ble_wlcpriomode_setf(uint8_t wlcpriomode)
{
    ASSERT_ERR((((uint32_t)wlcpriomode << 2) & ~((uint32_t)0x0000000C)) == 0);
    REG_BLE_WR(BLE_WLANCNTL_ADDR, (REG_BLE_RD(BLE_WLANCNTL_ADDR) & ~((uint32_t)0x0000000C)) | ((uint32_t)wlcpriomode << 2));
}

__INLINE uint8_t ble_wlcmode_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_WLANCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000003)) >> 0);
}

__INLINE void ble_wlcmode_setf(uint8_t wlcmode)
{
    ASSERT_ERR((((uint32_t)wlcmode << 0) & ~((uint32_t)0x00000003)) == 0);
    REG_BLE_WR(BLE_WLANCNTL_ADDR, (REG_BLE_RD(BLE_WLANCNTL_ADDR) & ~((uint32_t)0x00000003)) | ((uint32_t)wlcmode << 0));
}

#endif // RW_BLE_WLAN_COEX
#if RW_BLE_WLAN_COEX
/**
 * @brief BLEMPRIO0 register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:28                BLEM7   0x3
 *  27:24                BLEM6   0x4
 *  23:20                BLEM5   0x8
 *  19:16                BLEM4   0x9
 *  15:12                BLEM3   0xA
 *  11:08                BLEM2   0xD
 *  07:04                BLEM1   0xE
 *  03:00                BLEM0   0xF
 * </pre>
 */
#define BLE_BLEMPRIO0_ADDR   0x40000104
#define BLE_BLEMPRIO0_OFFSET 0x00000104
#define BLE_BLEMPRIO0_INDEX  0x00000041
#define BLE_BLEMPRIO0_RESET  0x3489ADEF

__INLINE uint32_t ble_blemprio0_get(void)
{
    return REG_BLE_RD(BLE_BLEMPRIO0_ADDR);
}

__INLINE void ble_blemprio0_set(uint32_t value)
{
    REG_BLE_WR(BLE_BLEMPRIO0_ADDR, value);
}

// field definitions
#define BLE_BLEM7_MASK   ((uint32_t)0xF0000000)
#define BLE_BLEM7_LSB    28
#define BLE_BLEM7_WIDTH  ((uint32_t)0x00000004)
#define BLE_BLEM6_MASK   ((uint32_t)0x0F000000)
#define BLE_BLEM6_LSB    24
#define BLE_BLEM6_WIDTH  ((uint32_t)0x00000004)
#define BLE_BLEM5_MASK   ((uint32_t)0x00F00000)
#define BLE_BLEM5_LSB    20
#define BLE_BLEM5_WIDTH  ((uint32_t)0x00000004)
#define BLE_BLEM4_MASK   ((uint32_t)0x000F0000)
#define BLE_BLEM4_LSB    16
#define BLE_BLEM4_WIDTH  ((uint32_t)0x00000004)
#define BLE_BLEM3_MASK   ((uint32_t)0x0000F000)
#define BLE_BLEM3_LSB    12
#define BLE_BLEM3_WIDTH  ((uint32_t)0x00000004)
#define BLE_BLEM2_MASK   ((uint32_t)0x00000F00)
#define BLE_BLEM2_LSB    8
#define BLE_BLEM2_WIDTH  ((uint32_t)0x00000004)
#define BLE_BLEM1_MASK   ((uint32_t)0x000000F0)
#define BLE_BLEM1_LSB    4
#define BLE_BLEM1_WIDTH  ((uint32_t)0x00000004)
#define BLE_BLEM0_MASK   ((uint32_t)0x0000000F)
#define BLE_BLEM0_LSB    0
#define BLE_BLEM0_WIDTH  ((uint32_t)0x00000004)

#define BLE_BLEM7_RST    0x3
#define BLE_BLEM6_RST    0x4
#define BLE_BLEM5_RST    0x8
#define BLE_BLEM4_RST    0x9
#define BLE_BLEM3_RST    0xA
#define BLE_BLEM2_RST    0xD
#define BLE_BLEM1_RST    0xE
#define BLE_BLEM0_RST    0xF

__INLINE void ble_blemprio0_pack(uint8_t blem7, uint8_t blem6, uint8_t blem5, uint8_t blem4, uint8_t blem3, uint8_t blem2, uint8_t blem1, uint8_t blem0)
{
    ASSERT_ERR((((uint32_t)blem7 << 28) & ~((uint32_t)0xF0000000)) == 0);
    ASSERT_ERR((((uint32_t)blem6 << 24) & ~((uint32_t)0x0F000000)) == 0);
    ASSERT_ERR((((uint32_t)blem5 << 20) & ~((uint32_t)0x00F00000)) == 0);
    ASSERT_ERR((((uint32_t)blem4 << 16) & ~((uint32_t)0x000F0000)) == 0);
    ASSERT_ERR((((uint32_t)blem3 << 12) & ~((uint32_t)0x0000F000)) == 0);
    ASSERT_ERR((((uint32_t)blem2 << 8) & ~((uint32_t)0x00000F00)) == 0);
    ASSERT_ERR((((uint32_t)blem1 << 4) & ~((uint32_t)0x000000F0)) == 0);
    ASSERT_ERR((((uint32_t)blem0 << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_BLE_WR(BLE_BLEMPRIO0_ADDR,  ((uint32_t)blem7 << 28) | ((uint32_t)blem6 << 24) | ((uint32_t)blem5 << 20) | ((uint32_t)blem4 << 16) | ((uint32_t)blem3 << 12) | ((uint32_t)blem2 << 8) | ((uint32_t)blem1 << 4) | ((uint32_t)blem0 << 0));
}

__INLINE void ble_blemprio0_unpack(uint8_t* blem7, uint8_t* blem6, uint8_t* blem5, uint8_t* blem4, uint8_t* blem3, uint8_t* blem2, uint8_t* blem1, uint8_t* blem0)
{
    uint32_t localVal = REG_BLE_RD(BLE_BLEMPRIO0_ADDR);

    *blem7 = (localVal & ((uint32_t)0xF0000000)) >> 28;
    *blem6 = (localVal & ((uint32_t)0x0F000000)) >> 24;
    *blem5 = (localVal & ((uint32_t)0x00F00000)) >> 20;
    *blem4 = (localVal & ((uint32_t)0x000F0000)) >> 16;
    *blem3 = (localVal & ((uint32_t)0x0000F000)) >> 12;
    *blem2 = (localVal & ((uint32_t)0x00000F00)) >> 8;
    *blem1 = (localVal & ((uint32_t)0x000000F0)) >> 4;
    *blem0 = (localVal & ((uint32_t)0x0000000F)) >> 0;
}

__INLINE uint8_t ble_blem7_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_BLEMPRIO0_ADDR);
    return ((localVal & ((uint32_t)0xF0000000)) >> 28);
}

__INLINE void ble_blem7_setf(uint8_t blem7)
{
    ASSERT_ERR((((uint32_t)blem7 << 28) & ~((uint32_t)0xF0000000)) == 0);
    REG_BLE_WR(BLE_BLEMPRIO0_ADDR, (REG_BLE_RD(BLE_BLEMPRIO0_ADDR) & ~((uint32_t)0xF0000000)) | ((uint32_t)blem7 << 28));
}

__INLINE uint8_t ble_blem6_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_BLEMPRIO0_ADDR);
    return ((localVal & ((uint32_t)0x0F000000)) >> 24);
}

__INLINE void ble_blem6_setf(uint8_t blem6)
{
    ASSERT_ERR((((uint32_t)blem6 << 24) & ~((uint32_t)0x0F000000)) == 0);
    REG_BLE_WR(BLE_BLEMPRIO0_ADDR, (REG_BLE_RD(BLE_BLEMPRIO0_ADDR) & ~((uint32_t)0x0F000000)) | ((uint32_t)blem6 << 24));
}

__INLINE uint8_t ble_blem5_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_BLEMPRIO0_ADDR);
    return ((localVal & ((uint32_t)0x00F00000)) >> 20);
}

__INLINE void ble_blem5_setf(uint8_t blem5)
{
    ASSERT_ERR((((uint32_t)blem5 << 20) & ~((uint32_t)0x00F00000)) == 0);
    REG_BLE_WR(BLE_BLEMPRIO0_ADDR, (REG_BLE_RD(BLE_BLEMPRIO0_ADDR) & ~((uint32_t)0x00F00000)) | ((uint32_t)blem5 << 20));
}

__INLINE uint8_t ble_blem4_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_BLEMPRIO0_ADDR);
    return ((localVal & ((uint32_t)0x000F0000)) >> 16);
}

__INLINE void ble_blem4_setf(uint8_t blem4)
{
    ASSERT_ERR((((uint32_t)blem4 << 16) & ~((uint32_t)0x000F0000)) == 0);
    REG_BLE_WR(BLE_BLEMPRIO0_ADDR, (REG_BLE_RD(BLE_BLEMPRIO0_ADDR) & ~((uint32_t)0x000F0000)) | ((uint32_t)blem4 << 16));
}

__INLINE uint8_t ble_blem3_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_BLEMPRIO0_ADDR);
    return ((localVal & ((uint32_t)0x0000F000)) >> 12);
}

__INLINE void ble_blem3_setf(uint8_t blem3)
{
    ASSERT_ERR((((uint32_t)blem3 << 12) & ~((uint32_t)0x0000F000)) == 0);
    REG_BLE_WR(BLE_BLEMPRIO0_ADDR, (REG_BLE_RD(BLE_BLEMPRIO0_ADDR) & ~((uint32_t)0x0000F000)) | ((uint32_t)blem3 << 12));
}

__INLINE uint8_t ble_blem2_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_BLEMPRIO0_ADDR);
    return ((localVal & ((uint32_t)0x00000F00)) >> 8);
}

__INLINE void ble_blem2_setf(uint8_t blem2)
{
    ASSERT_ERR((((uint32_t)blem2 << 8) & ~((uint32_t)0x00000F00)) == 0);
    REG_BLE_WR(BLE_BLEMPRIO0_ADDR, (REG_BLE_RD(BLE_BLEMPRIO0_ADDR) & ~((uint32_t)0x00000F00)) | ((uint32_t)blem2 << 8));
}

__INLINE uint8_t ble_blem1_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_BLEMPRIO0_ADDR);
    return ((localVal & ((uint32_t)0x000000F0)) >> 4);
}

__INLINE void ble_blem1_setf(uint8_t blem1)
{
    ASSERT_ERR((((uint32_t)blem1 << 4) & ~((uint32_t)0x000000F0)) == 0);
    REG_BLE_WR(BLE_BLEMPRIO0_ADDR, (REG_BLE_RD(BLE_BLEMPRIO0_ADDR) & ~((uint32_t)0x000000F0)) | ((uint32_t)blem1 << 4));
}

__INLINE uint8_t ble_blem0_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_BLEMPRIO0_ADDR);
    return ((localVal & ((uint32_t)0x0000000F)) >> 0);
}

__INLINE void ble_blem0_setf(uint8_t blem0)
{
    ASSERT_ERR((((uint32_t)blem0 << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_BLE_WR(BLE_BLEMPRIO0_ADDR, (REG_BLE_RD(BLE_BLEMPRIO0_ADDR) & ~((uint32_t)0x0000000F)) | ((uint32_t)blem0 << 0));
}

#endif // RW_BLE_WLAN_COEX
#if RW_BLE_WLAN_COEX
/**
 * @brief BLEMPRIO1 register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:28          BLEMDEFAULT   0x3
 * </pre>
 */
#define BLE_BLEMPRIO1_ADDR   0x40000108
#define BLE_BLEMPRIO1_OFFSET 0x00000108
#define BLE_BLEMPRIO1_INDEX  0x00000042
#define BLE_BLEMPRIO1_RESET  0x30000000

__INLINE uint32_t ble_blemprio1_get(void)
{
    return REG_BLE_RD(BLE_BLEMPRIO1_ADDR);
}

__INLINE void ble_blemprio1_set(uint32_t value)
{
    REG_BLE_WR(BLE_BLEMPRIO1_ADDR, value);
}

// field definitions
#define BLE_BLEMDEFAULT_MASK   ((uint32_t)0xF0000000)
#define BLE_BLEMDEFAULT_LSB    28
#define BLE_BLEMDEFAULT_WIDTH  ((uint32_t)0x00000004)

#define BLE_BLEMDEFAULT_RST    0x3

__INLINE uint8_t ble_blemdefault_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_BLEMPRIO1_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0xF0000000)) == 0);
    return (localVal >> 28);
}

__INLINE void ble_blemdefault_setf(uint8_t blemdefault)
{
    ASSERT_ERR((((uint32_t)blemdefault << 28) & ~((uint32_t)0xF0000000)) == 0);
    REG_BLE_WR(BLE_BLEMPRIO1_ADDR, (uint32_t)blemdefault << 28);
}

#endif // RW_BLE_WLAN_COEX
#if RW_DM_SUPPORT
/**
 * @brief BLEPRIOSCHARB register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     15          BLEPRIOMODE   0
 *  07:00            BLEMARGIN   0x0
 * </pre>
 */
#define BLE_BLEPRIOSCHARB_ADDR   0x40000110
#define BLE_BLEPRIOSCHARB_OFFSET 0x00000110
#define BLE_BLEPRIOSCHARB_INDEX  0x00000044
#define BLE_BLEPRIOSCHARB_RESET  0x00000000

__INLINE uint32_t ble_bleprioscharb_get(void)
{
    return REG_BLE_RD(BLE_BLEPRIOSCHARB_ADDR);
}

__INLINE void ble_bleprioscharb_set(uint32_t value)
{
    REG_BLE_WR(BLE_BLEPRIOSCHARB_ADDR, value);
}

// field definitions
#define BLE_BLEPRIOMODE_BIT    ((uint32_t)0x00008000)
#define BLE_BLEPRIOMODE_POS    15
#define BLE_BLEMARGIN_MASK     ((uint32_t)0x000000FF)
#define BLE_BLEMARGIN_LSB      0
#define BLE_BLEMARGIN_WIDTH    ((uint32_t)0x00000008)

#define BLE_BLEPRIOMODE_RST    0x0
#define BLE_BLEMARGIN_RST      0x0

__INLINE void ble_bleprioscharb_pack(uint8_t blepriomode, uint8_t blemargin)
{
    ASSERT_ERR((((uint32_t)blepriomode << 15) & ~((uint32_t)0x00008000)) == 0);
    ASSERT_ERR((((uint32_t)blemargin << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_BLE_WR(BLE_BLEPRIOSCHARB_ADDR,  ((uint32_t)blepriomode << 15) | ((uint32_t)blemargin << 0));
}

__INLINE void ble_bleprioscharb_unpack(uint8_t* blepriomode, uint8_t* blemargin)
{
    uint32_t localVal = REG_BLE_RD(BLE_BLEPRIOSCHARB_ADDR);

    *blepriomode = (localVal & ((uint32_t)0x00008000)) >> 15;
    *blemargin = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

__INLINE uint8_t ble_blepriomode_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_BLEPRIOSCHARB_ADDR);
    return ((localVal & ((uint32_t)0x00008000)) >> 15);
}

__INLINE void ble_blepriomode_setf(uint8_t blepriomode)
{
    ASSERT_ERR((((uint32_t)blepriomode << 15) & ~((uint32_t)0x00008000)) == 0);
    REG_BLE_WR(BLE_BLEPRIOSCHARB_ADDR, (REG_BLE_RD(BLE_BLEPRIOSCHARB_ADDR) & ~((uint32_t)0x00008000)) | ((uint32_t)blepriomode << 15));
}

__INLINE uint8_t ble_blemargin_getf(void)
{
    uint32_t localVal = REG_BLE_RD(BLE_BLEPRIOSCHARB_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

__INLINE void ble_blemargin_setf(uint8_t blemargin)
{
    ASSERT_ERR((((uint32_t)blemargin << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_BLE_WR(BLE_BLEPRIOSCHARB_ADDR, (REG_BLE_RD(BLE_BLEPRIOSCHARB_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)blemargin << 0));
}

#endif // RW_DM_SUPPORT

#endif // _REG_BLECORE_H_

