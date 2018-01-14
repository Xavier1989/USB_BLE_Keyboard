#ifndef _REG_UART_H_
#define _REG_UART_H_

#include <stdint.h>
#include "_reg_uart.h"
#include "compiler.h"
#include "arch.h"
#include "reg_access.h"

#define REG_UART_COUNT 14

#define REG_UART_DECODING_MASK 0x0000003F

//REMARKS WIK, OPEN ITEMS
//CAN'T FIND REGISTER "UART_DLM_ADDR" IN 580 DS, SO NO REPLACEMENT YET?? SEE LINE 465
//action done DLM IS A divisor latch registers (RW) -> UART_DLH_REG(0X50001004) == UART_IER_DLH_REG

/**
 * @brief RHR register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00               RXDATA   0x0
 * </pre>
 */

//org
//#define UART_RHR_ADDR   0x10007000
//#define UART_RHR_OFFSET 0x00000000
//#define UART_RHR_INDEX  0x00000000
//#define UART_RHR_RESET  0x00000000

//wik->UART_RBR_THR_DLL_REG(0x50001000)
#define UART_RHR_ADDR   0x50001000
#define UART_RHR_OFFSET 0x00000000
#define UART_RHR_INDEX  0x00000000
#define UART_RHR_RESET  0x00000000


#define UART2_RHR_ADDR  0x50001100
#define UART2_RHR_OFFSET 0x00000000
#define UART2_RHR_INDEX  0x00000000
#define UART2_RHR_RESET  0x00000000

__INLINE uint32_t uart_rhr_get(void)
{
    return REG_PL_RD(UART_RHR_ADDR);
}
__INLINE uint32_t uart2_rhr_get(void)
{
    return REG_PL_RD(UART2_RHR_ADDR);
}


__INLINE void uart_rhr_set(uint32_t value)
{
    REG_PL_WR(UART_RHR_ADDR, value);
}
__INLINE void uart2_rhr_set(uint32_t value)
{
    REG_PL_WR(UART2_RHR_ADDR, value);
}


// field definitions
#define UART_RXDATA_MASK   ((uint32_t)0x000000FF)
#define UART_RXDATA_LSB    0
#define UART_RXDATA_WIDTH  ((uint32_t)0x00000008)

#define UART_RXDATA_RST    0x0

__INLINE uint8_t uart_rxdata_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_RHR_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}
__INLINE uint8_t uart2_rxdata_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_RHR_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}
/**
 * @brief THR register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00               TXDATA   0x0
 * </pre>
 */
//org
//#define UART_THR_ADDR   0x10007000
//#define UART_THR_OFFSET 0x00000000
//#define UART_THR_INDEX  0x00000000
//#define UART_THR_RESET  0x00000000

//wik->UART_RBR_THR_DLL_REG(0x50001000)
#define UART_THR_ADDR   0x50001000
#define UART_THR_OFFSET 0x00000000
#define UART_THR_INDEX  0x00000000
#define UART_THR_RESET  0x00000000

#define UART2_THR_ADDR   0x50001100
#define UART2_THR_OFFSET 0x00000000
#define UART2_THR_INDEX  0x00000000
#define UART2_THR_RESET  0x00000000

__INLINE void uart_thr_set(uint32_t value)
{
    REG_PL_WR(UART_THR_ADDR, value);
}
__INLINE void uart2_thr_set(uint32_t value)
{
    REG_PL_WR(UART2_THR_ADDR, value);
}

// field definitions
#define UART_TXDATA_MASK   ((uint32_t)0x000000FF)
#define UART_TXDATA_LSB    0
#define UART_TXDATA_WIDTH  ((uint32_t)0x00000008)

#define UART_TXDATA_RST    0x0

__INLINE void uart_txdata_setf(uint8_t txdata)
{
    ASSERT_ERR((((uint32_t)txdata << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(UART_THR_ADDR, (uint32_t)txdata << 0);
}
__INLINE void uart2_txdata_setf(uint8_t txdata)
{
    ASSERT_ERR((((uint32_t)txdata << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(UART2_THR_ADDR, (uint32_t)txdata << 0);
}
/**
 * @brief DLL register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00               BD_DIV   0x0
 * </pre>
 */
//org
//#define UART_DLL_ADDR   0x10007000
//#define UART_DLL_OFFSET 0x00000000
//#define UART_DLL_INDEX  0x00000000
//#define UART_DLL_RESET  0x00000000

//wik->UART_RBR_THR_DLL_REG(0x50001000)
#define UART_DLL_ADDR   0x50001000
#define UART_DLL_OFFSET 0x00000000
#define UART_DLL_INDEX  0x00000000
#define UART_DLL_RESET  0x00000000

#define UART2_DLL_ADDR  0x50001100
#define UART2_DLL_OFFSET 0x00000000
#define UART2_DLL_INDEX  0x00000000
#define UART2_DLL_RESET  0x00000000

__INLINE uint32_t uart_dll_get(void)
{
    return REG_PL_RD(UART_DLL_ADDR);
}
__INLINE uint32_t uart2_dll_get(void)
{
    return REG_PL_RD(UART2_DLL_ADDR);
}

__INLINE void uart_dll_set(uint32_t value)
{
    REG_PL_WR(UART_DLL_ADDR, value);
}
__INLINE void uart2_dll_set(uint32_t value)
{
    REG_PL_WR(UART2_DLL_ADDR, value);
}

// field definitions
#define UART_BD_DIV_MASK   ((uint32_t)0x000000FF)
#define UART_BD_DIV_LSB    0
#define UART_BD_DIV_WIDTH  ((uint32_t)0x00000008)

#define UART_BD_DIV_RST    0x0

__INLINE uint8_t uart_bd_div_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_DLL_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}
__INLINE uint8_t uart2_bd_div_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_DLL_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

__INLINE void uart_bd_div_setf(uint8_t bddiv)
{
    ASSERT_ERR((((uint32_t)bddiv << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(UART_DLL_ADDR, (uint32_t)bddiv << 0);
}
__INLINE void uart2_bd_div_setf(uint8_t bddiv)
{
    ASSERT_ERR((((uint32_t)bddiv << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(UART2_DLL_ADDR, (uint32_t)bddiv << 0);
}

/**
 * @brief IER register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     03           MODEM_STAT   0
 *     02            LINE_STAT   0
 *     01            THR_EMPTY   0
 *     00       REC_DATA_AVAIL   0
 * </pre>
 */
//REMARK WIK-> I MISS UART_IER_DLH_REG.PTIME_dlh7 (bit7) in above mentioned Bitfield description

//org
//#define UART_IER_ADDR   0x10007004
//#define UART_IER_OFFSET 0x00000004
//#define UART_IER_INDEX  0x00000001
//#define UART_IER_RESET  0x00000000

//wik->UART_IER_DLH_REG(0x50001004) 
#define UART_IER_ADDR   0x50001004 
#define UART_IER_OFFSET 0x00000004
#define UART_IER_INDEX  0x00000001
#define UART_IER_RESET  0x00000000

#define UART2_IER_ADDR   0x50001104 
#define UART2_IER_OFFSET 0x00000004
#define UART2_IER_INDEX  0x00000001
#define UART2_IER_RESET  0x00000000

__INLINE uint32_t uart_ier_get(void)
{
    return REG_PL_RD(UART_IER_ADDR);
}
__INLINE uint32_t uart2_ier_get(void)
{
    return REG_PL_RD(UART2_IER_ADDR);
}

__INLINE void uart_ier_set(uint32_t value)
{
    REG_PL_WR(UART_IER_ADDR, value);
}
__INLINE void uart2_ier_set(uint32_t value)
{
    REG_PL_WR(UART2_IER_ADDR, value);
}

// field definitions
#define UART_MODEM_STAT_BIT        ((uint32_t)0x00000008)
#define UART_MODEM_STAT_POS        3
#define UART_LINE_STAT_BIT         ((uint32_t)0x00000004)
#define UART_LINE_STAT_POS         2
#define UART_THR_EMPTY_BIT         ((uint32_t)0x00000002)
#define UART_THR_EMPTY_POS         1
#define UART_REC_DATA_AVAIL_BIT    ((uint32_t)0x00000001)
#define UART_REC_DATA_AVAIL_POS    0

#define UART_MODEM_STAT_RST        0x0
#define UART_LINE_STAT_RST         0x0
#define UART_THR_EMPTY_RST         0x0
#define UART_REC_DATA_AVAIL_RST    0x0

__INLINE void uart_ier_pack(uint8_t modemstat, uint8_t linestat, uint8_t thrempty, uint8_t recdataavail)
{
    ASSERT_ERR((((uint32_t)modemstat << 3) & ~((uint32_t)0x00000008)) == 0);
    ASSERT_ERR((((uint32_t)linestat << 2) & ~((uint32_t)0x00000004)) == 0);
    ASSERT_ERR((((uint32_t)thrempty << 1) & ~((uint32_t)0x00000002)) == 0);
    ASSERT_ERR((((uint32_t)recdataavail << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(UART_IER_ADDR,  ((uint32_t)modemstat << 3) | ((uint32_t)linestat << 2) | ((uint32_t)thrempty << 1) | ((uint32_t)recdataavail << 0));
}
__INLINE void uart2_ier_pack(uint8_t modemstat, uint8_t linestat, uint8_t thrempty, uint8_t recdataavail)
{
    ASSERT_ERR((((uint32_t)modemstat << 3) & ~((uint32_t)0x00000008)) == 0);
    ASSERT_ERR((((uint32_t)linestat << 2) & ~((uint32_t)0x00000004)) == 0);
    ASSERT_ERR((((uint32_t)thrempty << 1) & ~((uint32_t)0x00000002)) == 0);
    ASSERT_ERR((((uint32_t)recdataavail << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(UART2_IER_ADDR,  ((uint32_t)modemstat << 3) | ((uint32_t)linestat << 2) | ((uint32_t)thrempty << 1) | ((uint32_t)recdataavail << 0));
}


__INLINE void uart_ier_unpack(uint8_t* modemstat, uint8_t* linestat, uint8_t* thrempty, uint8_t* recdataavail)
{
    uint32_t localVal = REG_PL_RD(UART_IER_ADDR);

    *modemstat = (localVal & ((uint32_t)0x00000008)) >> 3;
    *linestat = (localVal & ((uint32_t)0x00000004)) >> 2;
    *thrempty = (localVal & ((uint32_t)0x00000002)) >> 1;
    *recdataavail = (localVal & ((uint32_t)0x00000001)) >> 0;
}
__INLINE void uart2_ier_unpack(uint8_t* modemstat, uint8_t* linestat, uint8_t* thrempty, uint8_t* recdataavail)
{
    uint32_t localVal = REG_PL_RD(UART2_IER_ADDR);

    *modemstat = (localVal & ((uint32_t)0x00000008)) >> 3;
    *linestat = (localVal & ((uint32_t)0x00000004)) >> 2;
    *thrempty = (localVal & ((uint32_t)0x00000002)) >> 1;
    *recdataavail = (localVal & ((uint32_t)0x00000001)) >> 0;
}

__INLINE uint8_t uart_modem_stat_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_IER_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}
__INLINE uint8_t uart2_modem_stat_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_IER_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}

__INLINE void uart_modem_stat_setf(uint8_t modemstat)
{
    ASSERT_ERR((((uint32_t)modemstat << 3) & ~((uint32_t)0x00000008)) == 0);
    REG_PL_WR(UART_IER_ADDR, (REG_PL_RD(UART_IER_ADDR) & ~((uint32_t)0x00000008)) | ((uint32_t)modemstat << 3));
}
__INLINE void uart2_modem_stat_setf(uint8_t modemstat)
{
    ASSERT_ERR((((uint32_t)modemstat << 3) & ~((uint32_t)0x00000008)) == 0);
    REG_PL_WR(UART2_IER_ADDR, (REG_PL_RD(UART2_IER_ADDR) & ~((uint32_t)0x00000008)) | ((uint32_t)modemstat << 3));
}

__INLINE uint8_t uart_line_stat_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_IER_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}
__INLINE uint8_t uart2_line_stat_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_IER_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}

__INLINE void uart_line_stat_setf(uint8_t linestat)
{
    ASSERT_ERR((((uint32_t)linestat << 2) & ~((uint32_t)0x00000004)) == 0);
    REG_PL_WR(UART_IER_ADDR, (REG_PL_RD(UART_IER_ADDR) & ~((uint32_t)0x00000004)) | ((uint32_t)linestat << 2));
}
__INLINE void uart2_line_stat_setf(uint8_t linestat)
{
    ASSERT_ERR((((uint32_t)linestat << 2) & ~((uint32_t)0x00000004)) == 0);
    REG_PL_WR(UART2_IER_ADDR, (REG_PL_RD(UART2_IER_ADDR) & ~((uint32_t)0x00000004)) | ((uint32_t)linestat << 2));
}

__INLINE uint8_t uart_thr_empty_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_IER_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}
__INLINE uint8_t uart2_thr_empty_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_IER_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

__INLINE void uart_thr_empty_setf(uint8_t thrempty) //enable transmit interrupt
{
    ASSERT_ERR((((uint32_t)thrempty << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_PL_WR(UART_IER_ADDR, (REG_PL_RD(UART_IER_ADDR) & ~((uint32_t)0x00000002)) | ((uint32_t)thrempty << 1));
}
__INLINE void uart2_thr_empty_setf(uint8_t thrempty) //enable transmit interrupt
{
    ASSERT_ERR((((uint32_t)thrempty << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_PL_WR(UART2_IER_ADDR, (REG_PL_RD(UART2_IER_ADDR) & ~((uint32_t)0x00000002)) | ((uint32_t)thrempty << 1));
}


__INLINE uint8_t uart_rec_data_avail_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_IER_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}
__INLINE uint8_t uart2_rec_data_avail_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_IER_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

__INLINE void uart_rec_data_avail_setf(uint8_t recdataavail)
{
    ASSERT_ERR((((uint32_t)recdataavail << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(UART_IER_ADDR, (REG_PL_RD(UART_IER_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)recdataavail << 0));
}
__INLINE void uart2_rec_data_avail_setf(uint8_t recdataavail)
{
    ASSERT_ERR((((uint32_t)recdataavail << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(UART2_IER_ADDR, (REG_PL_RD(UART2_IER_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)recdataavail << 0));
}

/**
 * @brief ISR register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     07            FIFO_EN_R   0
 *     06        FIFO_EN_CPY_R   0
 *  03:01             INT_PRIO   0x0
 *     00               NO_INT   0
 * </pre>
 */

//org
//#define UART_ISR_ADDR   0x10007008
//#define UART_ISR_OFFSET 0x00000008
//#define UART_ISR_INDEX  0x00000002
//#define UART_ISR_RESET  0x00000000

//wik->UART_IIR_FCR_REG(0x50001008) 
#define UART_ISR_ADDR   0x50001008  
#define UART_ISR_OFFSET 0x00000008
#define UART_ISR_INDEX  0x00000002
#define UART_ISR_RESET  0x00000000

#define UART2_ISR_ADDR   0x50001108  
#define UART2_ISR_OFFSET 0x00000008
#define UART2_ISR_INDEX  0x00000002
#define UART2_ISR_RESET  0x00000000


__INLINE uint32_t uart_isr_get(void)
{
    return REG_PL_RD(UART_ISR_ADDR);
}
__INLINE uint32_t uart2_isr_get(void)
{
    return REG_PL_RD(UART2_ISR_ADDR);
}


#if 0 //@wik, this register doesn't exist, only the FIFO_EN is relevant and bitfields in register IIR. Done with uart_init function
// field definitions
#define UART_FIFO_EN_R_BIT        ((uint32_t)0x00000080)
#define UART_FIFO_EN_R_POS        7
#define UART_FIFO_EN_CPY_R_BIT    ((uint32_t)0x00000040)
#define UART_FIFO_EN_CPY_R_POS    6
#define UART_INT_PRIO_MASK        ((uint32_t)0x0000000E)
#define UART_INT_PRIO_LSB         1
#define UART_INT_PRIO_WIDTH       ((uint32_t)0x00000003)
#define UART_NO_INT_BIT           ((uint32_t)0x00000001)
#define UART_NO_INT_POS           0

#define UART_FIFO_EN_R_RST        0x0
#define UART_FIFO_EN_CPY_R_RST    0x0
#define UART_INT_PRIO_RST         0x0
#define UART_NO_INT_RST           0x0

__INLINE void uart_isr_unpack(uint8_t* fifoenr, uint8_t* fifoencpyr, uint8_t* intprio, uint8_t* noint)
{
    uint32_t localVal = REG_PL_RD(UART_ISR_ADDR);

    *fifoenr = (localVal & ((uint32_t)0x00000080)) >> 7;
    *fifoencpyr = (localVal & ((uint32_t)0x00000040)) >> 6;
    *intprio = (localVal & ((uint32_t)0x0000000E)) >> 1;
    *noint = (localVal & ((uint32_t)0x00000001)) >> 0;
}
__INLINE void uart_isr_unpack(uint8_t* fifoenr, uint8_t* fifoencpyr, uint8_t* intprio, uint8_t* noint)
{
    uint32_t localVal = REG_PL_RD(UART2_ISR_ADDR);

    *fifoenr = (localVal & ((uint32_t)0x00000080)) >> 7;
    *fifoencpyr = (localVal & ((uint32_t)0x00000040)) >> 6;
    *intprio = (localVal & ((uint32_t)0x0000000E)) >> 1;
    *noint = (localVal & ((uint32_t)0x00000001)) >> 0;
}


__INLINE uint8_t uart_fifo_en_r_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_ISR_ADDR);
    return ((localVal & ((uint32_t)0x00000080)) >> 7);
}
__INLINE uint8_t uart2_fifo_en_r_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_ISR_ADDR);
    return ((localVal & ((uint32_t)0x00000080)) >> 7);
}

__INLINE uint8_t uart_fifo_en_cpy_r_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_ISR_ADDR);
    return ((localVal & ((uint32_t)0x00000040)) >> 6);
}
__INLINE uint8_t uart2_fifo_en_cpy_r_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_ISR_ADDR);
    return ((localVal & ((uint32_t)0x00000040)) >> 6);
}

__INLINE uint8_t uart_int_prio_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_ISR_ADDR);
    return ((localVal & ((uint32_t)0x0000000E)) >> 1);
}
__INLINE uint8_t uart2_int_prio_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_ISR_ADDR);
    return ((localVal & ((uint32_t)0x0000000E)) >> 1);
}


__INLINE uint8_t uart_no_int_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_ISR_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}
__INLINE uint8_t uart2_no_int_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_ISR_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}
#endif

#if 0 //@WIK, fifo is fixed and level are programmed in UART init
/**
 * @brief FCR register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:06             FIFO_LVL   0x0
 *     02           TXFIFO_RES   0
 *     01           RXFIFO_RES   0
 *     00            FIFO_EN_W   0
 * </pre>
 */

//org
//#define UART_FCR_ADDR   0x10007008
//#define UART_FCR_OFFSET 0x00000008
//#define UART_FCR_INDEX  0x00000002
//#define UART_FCR_RESET  0x00000000

//wik->UART_IIR_FCR_REG(0x50001008) 
#define UART_FCR_ADDR   0x50001008  
#define UART_FCR_OFFSET 0x00000008
#define UART_FCR_INDEX  0x00000002
#define UART_FCR_RESET  0x00000000

#define UART2_FCR_ADDR   0x50001108  
#define UART2_FCR_OFFSET 0x00000008
#define UART2_FCR_INDEX  0x00000002
#define UART2_FCR_RESET  0x00000000


__INLINE uint32_t uart_fcr_get(void)
{
    return REG_PL_RD(UART_FCR_ADDR);
}
__INLINE uint32_t uart2_fcr_get(void)
{
    return REG_PL_RD(UART2_FCR_ADDR);
}

__INLINE void uart_fcr_set(uint32_t value)
{
    REG_PL_WR(UART_FCR_ADDR, value);
}
__INLINE void uart2_fcr_set(uint32_t value)
{
    REG_PL_WR(UART2_FCR_ADDR, value);
}

// field definitions
#define UART_FIFO_LVL_MASK     ((uint32_t)0x000000C0)
#define UART_FIFO_LVL_LSB      6
#define UART_FIFO_LVL_WIDTH    ((uint32_t)0x00000002)
#define UART_TXFIFO_RES_BIT    ((uint32_t)0x00000004)
#define UART_TXFIFO_RES_POS    2
#define UART_RXFIFO_RES_BIT    ((uint32_t)0x00000002)
#define UART_RXFIFO_RES_POS    1
#define UART_FIFO_EN_W_BIT     ((uint32_t)0x00000001)
#define UART_FIFO_EN_W_POS     0

#define UART_FIFO_LVL_RST      0x0
#define UART_TXFIFO_RES_RST    0x0
#define UART_RXFIFO_RES_RST    0x0
#define UART_FIFO_EN_W_RST     0x0

__INLINE void uart_fcr_pack(uint8_t fifolvl, uint8_t txfifores, uint8_t rxfifores, uint8_t fifoenw)
{
    ASSERT_ERR((((uint32_t)fifolvl << 6) & ~((uint32_t)0x000000C0)) == 0);
    ASSERT_ERR((((uint32_t)txfifores << 2) & ~((uint32_t)0x00000004)) == 0);
    ASSERT_ERR((((uint32_t)rxfifores << 1) & ~((uint32_t)0x00000002)) == 0);
    ASSERT_ERR((((uint32_t)fifoenw << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(UART_FCR_ADDR,  ((uint32_t)fifolvl << 6) | ((uint32_t)txfifores << 2) | ((uint32_t)rxfifores << 1) | ((uint32_t)fifoenw << 0));
}
__INLINE void uart2_fcr_pack(uint8_t fifolvl, uint8_t txfifores, uint8_t rxfifores, uint8_t fifoenw)
{
    ASSERT_ERR((((uint32_t)fifolvl << 6) & ~((uint32_t)0x000000C0)) == 0);
    ASSERT_ERR((((uint32_t)txfifores << 2) & ~((uint32_t)0x00000004)) == 0);
    ASSERT_ERR((((uint32_t)rxfifores << 1) & ~((uint32_t)0x00000002)) == 0);
    ASSERT_ERR((((uint32_t)fifoenw << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(UART2_FCR_ADDR,  ((uint32_t)fifolvl << 6) | ((uint32_t)txfifores << 2) | ((uint32_t)rxfifores << 1) | ((uint32_t)fifoenw << 0));
}

__INLINE void uart_fcr_unpack(uint8_t* fifolvl, uint8_t* txfifores, uint8_t* rxfifores, uint8_t* fifoenw)
{
    uint32_t localVal = REG_PL_RD(UART_FCR_ADDR);

    *fifolvl = (localVal & ((uint32_t)0x000000C0)) >> 6;
    *txfifores = (localVal & ((uint32_t)0x00000004)) >> 2;
    *rxfifores = (localVal & ((uint32_t)0x00000002)) >> 1;
    *fifoenw = (localVal & ((uint32_t)0x00000001)) >> 0;
}
__INLINE void uart2_fcr_unpack(uint8_t* fifolvl, uint8_t* txfifores, uint8_t* rxfifores, uint8_t* fifoenw)
{
    uint32_t localVal = REG_PL_RD(UART2_FCR_ADDR);

    *fifolvl = (localVal & ((uint32_t)0x000000C0)) >> 6;
    *txfifores = (localVal & ((uint32_t)0x00000004)) >> 2;
    *rxfifores = (localVal & ((uint32_t)0x00000002)) >> 1;
    *fifoenw = (localVal & ((uint32_t)0x00000001)) >> 0;
}

__INLINE uint8_t uart_fifo_lvl_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_FCR_ADDR);
    return ((localVal & ((uint32_t)0x000000C0)) >> 6);
}
__INLINE uint8_t uart2_fifo_lvl_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_FCR_ADDR);
    return ((localVal & ((uint32_t)0x000000C0)) >> 6);
}

__INLINE void uart_fifo_lvl_setf(uint8_t fifolvl)
{
    ASSERT_ERR((((uint32_t)fifolvl << 6) & ~((uint32_t)0x000000C0)) == 0);
    REG_PL_WR(UART_FCR_ADDR, (REG_PL_RD(UART_FCR_ADDR) & ~((uint32_t)0x000000C0)) | ((uint32_t)fifolvl << 6));
}
__INLINE void uart2_fifo_lvl_setf(uint8_t fifolvl)
{
    ASSERT_ERR((((uint32_t)fifolvl << 6) & ~((uint32_t)0x000000C0)) == 0);
    REG_PL_WR(UART2_FCR_ADDR, (REG_PL_RD(UART2_FCR_ADDR) & ~((uint32_t)0x000000C0)) | ((uint32_t)fifolvl << 6));
}

__INLINE uint8_t uart_txfifo_res_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_FCR_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}
__INLINE uint8_t uart2_txfifo_res_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_FCR_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}

__INLINE void uart_txfifo_res_setf(uint8_t txfifores)
{
    ASSERT_ERR((((uint32_t)txfifores << 2) & ~((uint32_t)0x00000004)) == 0);
    REG_PL_WR(UART_FCR_ADDR, (REG_PL_RD(UART_FCR_ADDR) & ~((uint32_t)0x00000004)) | ((uint32_t)txfifores << 2));
}
__INLINE void uart2_txfifo_res_setf(uint8_t txfifores)
{
    ASSERT_ERR((((uint32_t)txfifores << 2) & ~((uint32_t)0x00000004)) == 0);
    REG_PL_WR(UART2_FCR_ADDR, (REG_PL_RD(UART2_FCR_ADDR) & ~((uint32_t)0x00000004)) | ((uint32_t)txfifores << 2));
}

__INLINE uint8_t uart_rxfifo_res_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_FCR_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}
__INLINE uint8_t uart2_rxfifo_res_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_FCR_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

__INLINE void uart_rxfifo_res_setf(uint8_t rxfifores)
{
    ASSERT_ERR((((uint32_t)rxfifores << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_PL_WR(UART_FCR_ADDR, (REG_PL_RD(UART_FCR_ADDR) & ~((uint32_t)0x00000002)) | ((uint32_t)rxfifores << 1));
}
__INLINE void uart2_rxfifo_res_setf(uint8_t rxfifores)
{
    ASSERT_ERR((((uint32_t)rxfifores << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_PL_WR(UART2_FCR_ADDR, (REG_PL_RD(UART2_FCR_ADDR) & ~((uint32_t)0x00000002)) | ((uint32_t)rxfifores << 1));
}

__INLINE uint8_t uart_fifo_en_w_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_FCR_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}
__INLINE uint8_t uart2_fifo_en_w_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_FCR_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

__INLINE void uart_fifo_en_w_setf(uint8_t fifoenw)
{
    ASSERT_ERR((((uint32_t)fifoenw << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(UART_FCR_ADDR, (REG_PL_RD(UART_FCR_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)fifoenw << 0));
}
__INLINE void uart2_fifo_en_w_setf(uint8_t fifoenw)
{
    ASSERT_ERR((((uint32_t)fifoenw << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(UART2_FCR_ADDR, (REG_PL_RD(UART2_FCR_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)fifoenw << 0));
}
#endif 

/**
 * @brief DLM register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  06:00               BD_SIV   0x0
 * </pre>
 */
//org
//#define UART_DLM_ADDR   0x10007008
//#define UART_DLM_OFFSET 0x00000008
//#define UART_DLM_INDEX  0x00000002
//#define UART_DLM_RESET  0x00000000

//wik->UART_IER_DLH_REG(0x50001004) 
#define UART_DLM_ADDR   0x50001004
#define UART_DLM_OFFSET 0x00000004
#define UART_DLM_INDEX  0x00000002
#define UART_DLM_RESET  0x00000000

#define UART2_DLM_ADDR   0x50001104
#define UART2_DLM_OFFSET 0x00000004
#define UART2_DLM_INDEX  0x00000002
#define UART2_DLM_RESET  0x00000000

__INLINE uint32_t uart_dlm_get(void)
{
    return REG_PL_RD(UART_DLM_ADDR);
}
__INLINE uint32_t uart2_dlm_get(void)
{
    return REG_PL_RD(UART2_DLM_ADDR);
}

__INLINE void uart_dlm_set(uint32_t value)
{
    REG_PL_WR(UART_DLM_ADDR, value);
}
__INLINE void uart2_dlm_set(uint32_t value)
{
    REG_PL_WR(UART2_DLM_ADDR, value);
}

// field definitions
#define UART_BD_SIV_MASK   ((uint32_t)0x0000007F)
#define UART_BD_SIV_LSB    0
#define UART_BD_SIV_WIDTH  ((uint32_t)0x00000007)

#define UART_BD_SIV_RST    0x0

__INLINE void uart_bd_siv_setf(uint8_t bdsiv)
{
    ASSERT_ERR((((uint32_t)bdsiv << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(UART_DLM_ADDR, (uint32_t)bdsiv << 0);
}

__INLINE void uart2_bd_siv_setf(uint8_t bdsiv)
{
    ASSERT_ERR((((uint32_t)bdsiv << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(UART2_DLM_ADDR, (uint32_t)bdsiv << 0);
}


/**
 * @brief LCR register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     07                 DLAB   0
 *     06             BREAK_TX   0
 *  05:04               PARITY   0x0
 *     03            PARITY_EN   0
 *     02                 STOP   0
 *  01:00             WORD_LEN   0x0
 * </pre>
 */

//org
//#define UART_LCR_ADDR   0x1000700C
//#define UART_LCR_OFFSET 0x0000000C
//#define UART_LCR_INDEX  0x00000003
//#define UART_LCR_RESET  0x00000000

//wik->UART_LCR_REG(0x5000100C)
#define UART_LCR_ADDR   0x5000100C
#define UART_LCR_OFFSET 0x0000000C
#define UART_LCR_INDEX  0x00000003
#define UART_LCR_RESET  0x00000000

#define UART2_LCR_ADDR   0x5000110C
#define UART2_LCR_OFFSET 0x0000000C
#define UART2_LCR_INDEX  0x00000003
#define UART2_LCR_RESET  0x00000000

__INLINE uint32_t uart_lcr_get(void)
{
    return REG_PL_RD(UART_LCR_ADDR);
}
__INLINE uint32_t uart2_lcr_get(void)
{
    return REG_PL_RD(UART2_LCR_ADDR);
}

__INLINE void uart_lcr_set(uint32_t value)
{
    REG_PL_WR(UART_LCR_ADDR, value);
}
__INLINE void uart2_lcr_set(uint32_t value)
{
    REG_PL_WR(UART2_LCR_ADDR, value);
}

// field definitions
#define UART_DLAB_BIT         ((uint32_t)0x00000080)
#define UART_DLAB_POS         7
#define UART_BREAK_TX_BIT     ((uint32_t)0x00000040)
#define UART_BREAK_TX_POS     6
#define UART_PARITY_MASK      ((uint32_t)0x00000030)
#define UART_PARITY_LSB       4
#define UART_PARITY_WIDTH     ((uint32_t)0x00000002)
#define UART_PARITY_EN_BIT    ((uint32_t)0x00000008)
#define UART_PARITY_EN_POS    3
#define UART_STOP_BIT         ((uint32_t)0x00000004)
#define UART_STOP_POS         2
#define UART_WORD_LEN_MASK    ((uint32_t)0x00000003)
#define UART_WORD_LEN_LSB     0
#define UART_WORD_LEN_WIDTH   ((uint32_t)0x00000002)

#define UART_DLAB_RST         0x0
#define UART_BREAK_TX_RST     0x0
#define UART_PARITY_RST       0x0
#define UART_PARITY_EN_RST    0x0
#define UART_STOP_RST         0x0
#define UART_WORD_LEN_RST     0x0

__INLINE void uart_lcr_pack(uint8_t dlab, uint8_t breaktx, uint8_t parity, uint8_t parityen, uint8_t stop, uint8_t wordlen)
{
    ASSERT_ERR((((uint32_t)dlab << 7) & ~((uint32_t)0x00000080)) == 0);
    ASSERT_ERR((((uint32_t)breaktx << 6) & ~((uint32_t)0x00000040)) == 0);
    ASSERT_ERR((((uint32_t)parity << 4) & ~((uint32_t)0x00000030)) == 0);
    ASSERT_ERR((((uint32_t)parityen << 3) & ~((uint32_t)0x00000008)) == 0);
    ASSERT_ERR((((uint32_t)stop << 2) & ~((uint32_t)0x00000004)) == 0);
    ASSERT_ERR((((uint32_t)wordlen << 0) & ~((uint32_t)0x00000003)) == 0);
    REG_PL_WR(UART_LCR_ADDR,  ((uint32_t)dlab << 7) | ((uint32_t)breaktx << 6) | ((uint32_t)parity << 4) | ((uint32_t)parityen << 3) | ((uint32_t)stop << 2) | ((uint32_t)wordlen << 0));
}
__INLINE void uart2_lcr_pack(uint8_t dlab, uint8_t breaktx, uint8_t parity, uint8_t parityen, uint8_t stop, uint8_t wordlen)
{
    ASSERT_ERR((((uint32_t)dlab << 7) & ~((uint32_t)0x00000080)) == 0);
    ASSERT_ERR((((uint32_t)breaktx << 6) & ~((uint32_t)0x00000040)) == 0);
    ASSERT_ERR((((uint32_t)parity << 4) & ~((uint32_t)0x00000030)) == 0);
    ASSERT_ERR((((uint32_t)parityen << 3) & ~((uint32_t)0x00000008)) == 0);
    ASSERT_ERR((((uint32_t)stop << 2) & ~((uint32_t)0x00000004)) == 0);
    ASSERT_ERR((((uint32_t)wordlen << 0) & ~((uint32_t)0x00000003)) == 0);
    REG_PL_WR(UART2_LCR_ADDR,  ((uint32_t)dlab << 7) | ((uint32_t)breaktx << 6) | ((uint32_t)parity << 4) | ((uint32_t)parityen << 3) | ((uint32_t)stop << 2) | ((uint32_t)wordlen << 0));
}


__INLINE void uart_lcr_unpack(uint8_t* dlab, uint8_t* breaktx, uint8_t* parity, uint8_t* parityen, uint8_t* stop, uint8_t* wordlen)
{
    uint32_t localVal = REG_PL_RD(UART_LCR_ADDR);

    *dlab = (localVal & ((uint32_t)0x00000080)) >> 7;
    *breaktx = (localVal & ((uint32_t)0x00000040)) >> 6;
    *parity = (localVal & ((uint32_t)0x00000030)) >> 4;
    *parityen = (localVal & ((uint32_t)0x00000008)) >> 3;
    *stop = (localVal & ((uint32_t)0x00000004)) >> 2;
    *wordlen = (localVal & ((uint32_t)0x00000003)) >> 0;
}
__INLINE void uart2_lcr_unpack(uint8_t* dlab, uint8_t* breaktx, uint8_t* parity, uint8_t* parityen, uint8_t* stop, uint8_t* wordlen)
{
    uint32_t localVal = REG_PL_RD(UART2_LCR_ADDR);

    *dlab = (localVal & ((uint32_t)0x00000080)) >> 7;
    *breaktx = (localVal & ((uint32_t)0x00000040)) >> 6;
    *parity = (localVal & ((uint32_t)0x00000030)) >> 4;
    *parityen = (localVal & ((uint32_t)0x00000008)) >> 3;
    *stop = (localVal & ((uint32_t)0x00000004)) >> 2;
    *wordlen = (localVal & ((uint32_t)0x00000003)) >> 0;
}


__INLINE uint8_t uart_dlab_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_LCR_ADDR);
    return ((localVal & ((uint32_t)0x00000080)) >> 7);
}
__INLINE uint8_t uart2_dlab_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_LCR_ADDR);
    return ((localVal & ((uint32_t)0x00000080)) >> 7);
}


__INLINE void uart_dlab_setf(uint8_t dlab)
{
    ASSERT_ERR((((uint32_t)dlab << 7) & ~((uint32_t)0x00000080)) == 0);
    REG_PL_WR(UART_LCR_ADDR, (REG_PL_RD(UART_LCR_ADDR) & ~((uint32_t)0x00000080)) | ((uint32_t)dlab << 7));
}
__INLINE void uart2_dlab_setf(uint8_t dlab)
{
    ASSERT_ERR((((uint32_t)dlab << 7) & ~((uint32_t)0x00000080)) == 0);
    REG_PL_WR(UART2_LCR_ADDR, (REG_PL_RD(UART2_LCR_ADDR) & ~((uint32_t)0x00000080)) | ((uint32_t)dlab << 7));
}


__INLINE uint8_t uart_break_tx_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_LCR_ADDR);
    return ((localVal & ((uint32_t)0x00000040)) >> 6);
}
__INLINE uint8_t uart2_break_tx_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_LCR_ADDR);
    return ((localVal & ((uint32_t)0x00000040)) >> 6);
}


__INLINE void uart_break_tx_setf(uint8_t breaktx)
{
    ASSERT_ERR((((uint32_t)breaktx << 6) & ~((uint32_t)0x00000040)) == 0);
    REG_PL_WR(UART_LCR_ADDR, (REG_PL_RD(UART_LCR_ADDR) & ~((uint32_t)0x00000040)) | ((uint32_t)breaktx << 6));
}
__INLINE void uart2_break_tx_setf(uint8_t breaktx)
{
    ASSERT_ERR((((uint32_t)breaktx << 6) & ~((uint32_t)0x00000040)) == 0);
    REG_PL_WR(UART2_LCR_ADDR, (REG_PL_RD(UART2_LCR_ADDR) & ~((uint32_t)0x00000040)) | ((uint32_t)breaktx << 6));
}


__INLINE uint8_t uart_parity_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_LCR_ADDR);
    return ((localVal & ((uint32_t)0x00000030)) >> 4);
}
__INLINE uint8_t uart2_parity_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_LCR_ADDR);
    return ((localVal & ((uint32_t)0x00000030)) >> 4);
}


__INLINE void uart_parity_setf(uint8_t parity)
{
    ASSERT_ERR((((uint32_t)parity << 4) & ~((uint32_t)0x00000030)) == 0);
    REG_PL_WR(UART_LCR_ADDR, (REG_PL_RD(UART_LCR_ADDR) & ~((uint32_t)0x00000030)) | ((uint32_t)parity << 4));
}
__INLINE void uart2_parity_setf(uint8_t parity)
{
    ASSERT_ERR((((uint32_t)parity << 4) & ~((uint32_t)0x00000030)) == 0);
    REG_PL_WR(UART2_LCR_ADDR, (REG_PL_RD(UART2_LCR_ADDR) & ~((uint32_t)0x00000030)) | ((uint32_t)parity << 4));
}


__INLINE uint8_t uart_parity_en_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_LCR_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}
__INLINE uint8_t uart2_parity_en_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_LCR_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}


__INLINE void uart_parity_en_setf(uint8_t parityen)
{
    ASSERT_ERR((((uint32_t)parityen << 3) & ~((uint32_t)0x00000008)) == 0);
    REG_PL_WR(UART_LCR_ADDR, (REG_PL_RD(UART_LCR_ADDR) & ~((uint32_t)0x00000008)) | ((uint32_t)parityen << 3));
}
__INLINE void uart2_parity_en_setf(uint8_t parityen)
{
    ASSERT_ERR((((uint32_t)parityen << 3) & ~((uint32_t)0x00000008)) == 0);
    REG_PL_WR(UART2_LCR_ADDR, (REG_PL_RD(UART2_LCR_ADDR) & ~((uint32_t)0x00000008)) | ((uint32_t)parityen << 3));
}


__INLINE uint8_t uart_stop_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_LCR_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}
__INLINE uint8_t uart2_stop_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_LCR_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}


__INLINE void uart_stop_setf(uint8_t stop)
{
    ASSERT_ERR((((uint32_t)stop << 2) & ~((uint32_t)0x00000004)) == 0);
    REG_PL_WR(UART_LCR_ADDR, (REG_PL_RD(UART_LCR_ADDR) & ~((uint32_t)0x00000004)) | ((uint32_t)stop << 2));
}
__INLINE void uart2_stop_setf(uint8_t stop)
{
    ASSERT_ERR((((uint32_t)stop << 2) & ~((uint32_t)0x00000004)) == 0);
    REG_PL_WR(UART2_LCR_ADDR, (REG_PL_RD(UART2_LCR_ADDR) & ~((uint32_t)0x00000004)) | ((uint32_t)stop << 2));
}


__INLINE uint8_t uart_word_len_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_LCR_ADDR);
    return ((localVal & ((uint32_t)0x00000003)) >> 0);
}
__INLINE uint8_t uart2_word_len_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_LCR_ADDR);
    return ((localVal & ((uint32_t)0x00000003)) >> 0);
}


__INLINE void uart_word_len_setf(uint8_t wordlen)
{
    ASSERT_ERR((((uint32_t)wordlen << 0) & ~((uint32_t)0x00000003)) == 0);
    REG_PL_WR(UART_LCR_ADDR, (REG_PL_RD(UART_LCR_ADDR) & ~((uint32_t)0x00000003)) | ((uint32_t)wordlen << 0));
}
__INLINE void uart2_word_len_setf(uint8_t wordlen)
{
    ASSERT_ERR((((uint32_t)wordlen << 0) & ~((uint32_t)0x00000003)) == 0);
    REG_PL_WR(UART2_LCR_ADDR, (REG_PL_RD(UART2_LCR_ADDR) & ~((uint32_t)0x00000003)) | ((uint32_t)wordlen << 0));
}

/**
 * @brief MCR register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     07             EXT_FUNC   0
 *     06             AUTO_RTS   0
 *     05             AUTO_CTS   0
 *     01                  RTS   0
 * </pre>
 */

//org
//#define UART_MCR_ADDR   0x10007010
//#define UART_MCR_OFFSET 0x00000010
//#define UART_MCR_INDEX  0x00000004
//#define UART_MCR_RESET  0x00000000

//wik->UART_MCR_REG(0x50001010)
#define UART_MCR_ADDR   0x50001010
#define UART_MCR_OFFSET 0x00000010
#define UART_MCR_INDEX  0x00000004
#define UART_MCR_RESET  0x00000000

#define UART2_MCR_ADDR   0x50001110
#define UART2_MCR_OFFSET 0x00000010
#define UART2_MCR_INDEX  0x00000004
#define UART2_MCR_RESET  0x00000000


__INLINE uint32_t uart_mcr_get(void)
{
    return REG_PL_RD(UART_MCR_ADDR);
}
__INLINE uint32_t uart2_mcr_get(void)
{
    return REG_PL_RD(UART2_MCR_ADDR);
}


__INLINE void uart_mcr_set(uint32_t value)
{
    REG_PL_WR(UART_MCR_ADDR, value);
}
__INLINE void uart2_mcr_set(uint32_t value)
{
    REG_PL_WR(UART2_MCR_ADDR, value);
}

// field definitions
#define UART_EXT_FUNC_BIT    ((uint32_t)0x00000080)
#define UART_EXT_FUNC_POS    7
#define UART_AUTO_RTS_BIT    ((uint32_t)0x00000040)
#define UART_AUTO_RTS_POS    6
#define UART_AUTO_CTS_BIT    ((uint32_t)0x00000020)
#define UART_AUTO_CTS_POS    5
#define UART_RTS_BIT         ((uint32_t)0x00000002)
#define UART_RTS_POS         1

#define UART_EXT_FUNC_RST    0x0
#define UART_AUTO_RTS_RST    0x0
#define UART_AUTO_CTS_RST    0x0
#define UART_RTS_RST         0x0

__INLINE void uart_mcr_pack(uint8_t extfunc, uint8_t autorts, uint8_t autocts, uint8_t rts)
{
    ASSERT_ERR((((uint32_t)extfunc << 7) & ~((uint32_t)0x00000080)) == 0);
    ASSERT_ERR((((uint32_t)autorts << 6) & ~((uint32_t)0x00000040)) == 0);
    ASSERT_ERR((((uint32_t)autocts << 5) & ~((uint32_t)0x00000020)) == 0);
    ASSERT_ERR((((uint32_t)rts << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_PL_WR(UART_MCR_ADDR,  ((uint32_t)extfunc << 7) | ((uint32_t)autorts << 6) | ((uint32_t)autocts << 5) | ((uint32_t)rts << 1));
}
__INLINE void uart2_mcr_pack(uint8_t extfunc, uint8_t autorts, uint8_t autocts, uint8_t rts)
{
    ASSERT_ERR((((uint32_t)extfunc << 7) & ~((uint32_t)0x00000080)) == 0);
    ASSERT_ERR((((uint32_t)autorts << 6) & ~((uint32_t)0x00000040)) == 0);
    ASSERT_ERR((((uint32_t)autocts << 5) & ~((uint32_t)0x00000020)) == 0);
    ASSERT_ERR((((uint32_t)rts << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_PL_WR(UART2_MCR_ADDR,  ((uint32_t)extfunc << 7) | ((uint32_t)autorts << 6) | ((uint32_t)autocts << 5) | ((uint32_t)rts << 1));
}


__INLINE void uart_mcr_unpack(uint8_t* extfunc, uint8_t* autorts, uint8_t* autocts, uint8_t* rts)
{
    uint32_t localVal = REG_PL_RD(UART_MCR_ADDR);

    *extfunc = (localVal & ((uint32_t)0x00000080)) >> 7;
    *autorts = (localVal & ((uint32_t)0x00000040)) >> 6;
    *autocts = (localVal & ((uint32_t)0x00000020)) >> 5;
    *rts = (localVal & ((uint32_t)0x00000002)) >> 1;
}
__INLINE void uart2_mcr_unpack(uint8_t* extfunc, uint8_t* autorts, uint8_t* autocts, uint8_t* rts)
{
    uint32_t localVal = REG_PL_RD(UART2_MCR_ADDR);

    *extfunc = (localVal & ((uint32_t)0x00000080)) >> 7;
    *autorts = (localVal & ((uint32_t)0x00000040)) >> 6;
    *autocts = (localVal & ((uint32_t)0x00000020)) >> 5;
    *rts = (localVal & ((uint32_t)0x00000002)) >> 1;
}


__INLINE uint8_t uart_ext_func_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_MCR_ADDR);
    return ((localVal & ((uint32_t)0x00000080)) >> 7);
}
__INLINE uint8_t uart2_ext_func_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_MCR_ADDR);
    return ((localVal & ((uint32_t)0x00000080)) >> 7);
}


__INLINE void uart_ext_func_setf(uint8_t extfunc)
{
    ASSERT_ERR((((uint32_t)extfunc << 7) & ~((uint32_t)0x00000080)) == 0);
    REG_PL_WR(UART_MCR_ADDR, (REG_PL_RD(UART_MCR_ADDR) & ~((uint32_t)0x00000080)) | ((uint32_t)extfunc << 7));
}
__INLINE void uart2_ext_func_setf(uint8_t extfunc)
{
    ASSERT_ERR((((uint32_t)extfunc << 7) & ~((uint32_t)0x00000080)) == 0);
    REG_PL_WR(UART2_MCR_ADDR, (REG_PL_RD(UART2_MCR_ADDR) & ~((uint32_t)0x00000080)) | ((uint32_t)extfunc << 7));
}


__INLINE uint8_t uart_auto_rts_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_MCR_ADDR);
    return ((localVal & ((uint32_t)0x00000040)) >> 6);
}
__INLINE uint8_t uart2_auto_rts_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_MCR_ADDR);
    return ((localVal & ((uint32_t)0x00000040)) >> 6);
}


__INLINE void uart_auto_rts_setf(uint8_t autorts)
{
    ASSERT_ERR((((uint32_t)autorts << 6) & ~((uint32_t)0x00000040)) == 0);
    REG_PL_WR(UART_MCR_ADDR, (REG_PL_RD(UART_MCR_ADDR) & ~((uint32_t)0x00000040)) | ((uint32_t)autorts << 6));
}
__INLINE void uart2_auto_rts_setf(uint8_t autorts)
{
    ASSERT_ERR((((uint32_t)autorts << 6) & ~((uint32_t)0x00000040)) == 0);
    REG_PL_WR(UART2_MCR_ADDR, (REG_PL_RD(UART2_MCR_ADDR) & ~((uint32_t)0x00000040)) | ((uint32_t)autorts << 6));
}


__INLINE uint8_t uart_auto_cts_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_MCR_ADDR);
    return ((localVal & ((uint32_t)0x00000020)) >> 5);
}
__INLINE uint8_t uart2_auto_cts_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_MCR_ADDR);
    return ((localVal & ((uint32_t)0x00000020)) >> 5);
}


__INLINE void uart_auto_cts_setf(uint8_t autocts)
{
    ASSERT_ERR((((uint32_t)autocts << 5) & ~((uint32_t)0x00000020)) == 0);
    REG_PL_WR(UART_MCR_ADDR, (REG_PL_RD(UART_MCR_ADDR) & ~((uint32_t)0x00000020)) | ((uint32_t)autocts << 5));
}
__INLINE void uart2_auto_cts_setf(uint8_t autocts)
{
    ASSERT_ERR((((uint32_t)autocts << 5) & ~((uint32_t)0x00000020)) == 0);
    REG_PL_WR(UART2_MCR_ADDR, (REG_PL_RD(UART2_MCR_ADDR) & ~((uint32_t)0x00000020)) | ((uint32_t)autocts << 5));
}


__INLINE uint8_t uart_rts_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_MCR_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}
__INLINE uint8_t uart2_rts_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_MCR_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}


__INLINE void uart_rts_setf(uint8_t rts)
{
    ASSERT_ERR((((uint32_t)rts << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_PL_WR(UART_MCR_ADDR, (REG_PL_RD(UART_MCR_ADDR) & ~((uint32_t)0x00000002)) | ((uint32_t)rts << 1));
}
__INLINE void uart2_rts_setf(uint8_t rts)
{
    ASSERT_ERR((((uint32_t)rts << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_PL_WR(UART2_MCR_ADDR, (REG_PL_RD(UART2_MCR_ADDR) & ~((uint32_t)0x00000002)) | ((uint32_t)rts << 1));
}

/**
 * @brief LSR register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     07             FIFO_ERR   0
 *     06                 TEMT   0
 *     05                 THRE   0
 *     04             BREAK_RX   0
 *     03            FRAME_ERR   0
 *     02           PARITY_ERR   0
 *     01              OVERRUN   0
 *     00             DATA_RDY   0
 * </pre>
 */

//org
//#define UART_LSR_ADDR   0x10007014
//#define UART_LSR_OFFSET 0x00000014
//#define UART_LSR_INDEX  0x00000005
//#define UART_LSR_RESET  0x00000000

//wik->UART_LSR_REG(0x50001014)
#define UART_LSR_ADDR   0x50001014
#define UART_LSR_OFFSET 0x00000014
#define UART_LSR_INDEX  0x00000005
#define UART_LSR_RESET  0x00000000

#define UART2_LSR_ADDR   0x50001114
#define UART2_LSR_OFFSET 0x00000014
#define UART2_LSR_INDEX  0x00000005
#define UART2_LSR_RESET  0x00000000


__INLINE uint32_t uart_lsr_get(void)
{
    return REG_PL_RD(UART_LSR_ADDR);
}
__INLINE uint32_t uart2_lsr_get(void)
{
    return REG_PL_RD(UART2_LSR_ADDR);
}

// field definitions
#define UART_FIFO_ERR_BIT      ((uint32_t)0x00000080)
#define UART_FIFO_ERR_POS      7
#define UART_TEMT_BIT          ((uint32_t)0x00000040)
#define UART_TEMT_POS          6
#define UART_THRE_BIT          ((uint32_t)0x00000020)
#define UART_THRE_POS          5
#define UART_BREAK_RX_BIT      ((uint32_t)0x00000010)
#define UART_BREAK_RX_POS      4
#define UART_FRAME_ERR_BIT     ((uint32_t)0x00000008)
#define UART_FRAME_ERR_POS     3
#define UART_PARITY_ERR_BIT    ((uint32_t)0x00000004)
#define UART_PARITY_ERR_POS    2
#define UART_OVERRUN_BIT       ((uint32_t)0x00000002)
#define UART_OVERRUN_POS       1
#define UART_DATA_RDY_BIT      ((uint32_t)0x00000001)
#define UART_DATA_RDY_POS      0

#define UART_FIFO_ERR_RST      0x0
#define UART_TEMT_RST          0x0
#define UART_THRE_RST          0x0
#define UART_BREAK_RX_RST      0x0
#define UART_FRAME_ERR_RST     0x0
#define UART_PARITY_ERR_RST    0x0
#define UART_OVERRUN_RST       0x0
#define UART_DATA_RDY_RST      0x0

__INLINE void uart_lsr_unpack(uint8_t* fifoerr, uint8_t* temt, uint8_t* thre, uint8_t* breakrx, uint8_t* frameerr, uint8_t* parityerr, uint8_t* overrun, uint8_t* datardy)
{
    uint32_t localVal = REG_PL_RD(UART_LSR_ADDR);

    *fifoerr = (localVal & ((uint32_t)0x00000080)) >> 7;
    *temt = (localVal & ((uint32_t)0x00000040)) >> 6;
    *thre = (localVal & ((uint32_t)0x00000020)) >> 5;
    *breakrx = (localVal & ((uint32_t)0x00000010)) >> 4;
    *frameerr = (localVal & ((uint32_t)0x00000008)) >> 3;
    *parityerr = (localVal & ((uint32_t)0x00000004)) >> 2;
    *overrun = (localVal & ((uint32_t)0x00000002)) >> 1;
    *datardy = (localVal & ((uint32_t)0x00000001)) >> 0;
}
__INLINE void uart2_lsr_unpack(uint8_t* fifoerr, uint8_t* temt, uint8_t* thre, uint8_t* breakrx, uint8_t* frameerr, uint8_t* parityerr, uint8_t* overrun, uint8_t* datardy)
{
    uint32_t localVal = REG_PL_RD(UART2_LSR_ADDR);

    *fifoerr = (localVal & ((uint32_t)0x00000080)) >> 7;
    *temt = (localVal & ((uint32_t)0x00000040)) >> 6;
    *thre = (localVal & ((uint32_t)0x00000020)) >> 5;
    *breakrx = (localVal & ((uint32_t)0x00000010)) >> 4;
    *frameerr = (localVal & ((uint32_t)0x00000008)) >> 3;
    *parityerr = (localVal & ((uint32_t)0x00000004)) >> 2;
    *overrun = (localVal & ((uint32_t)0x00000002)) >> 1;
    *datardy = (localVal & ((uint32_t)0x00000001)) >> 0;
}


__INLINE uint8_t uart_fifo_err_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_LSR_ADDR);
    return ((localVal & ((uint32_t)0x00000080)) >> 7);
}
__INLINE uint8_t uart2_fifo_err_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_LSR_ADDR);
    return ((localVal & ((uint32_t)0x00000080)) >> 7);
}


__INLINE uint8_t uart_temt_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_LSR_ADDR);
    return ((localVal & ((uint32_t)0x00000040)) >> 6);
}
__INLINE uint8_t uart2_temt_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_LSR_ADDR);
    return ((localVal & ((uint32_t)0x00000040)) >> 6);
}


__INLINE uint8_t uart_thre_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_LSR_ADDR);
    return ((localVal & ((uint32_t)0x00000020)) >> 5);
}
__INLINE uint8_t uart2_thre_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_LSR_ADDR);
    return ((localVal & ((uint32_t)0x00000020)) >> 5);
}


__INLINE uint8_t uart_break_rx_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_LSR_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}
__INLINE uint8_t uart2_break_rx_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_LSR_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}


__INLINE uint8_t uart_frame_err_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_LSR_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}
__INLINE uint8_t uart2_frame_err_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_LSR_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}


__INLINE uint8_t uart_parity_err_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_LSR_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}
__INLINE uint8_t uart2_parity_err_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_LSR_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}


__INLINE uint8_t uart_overrun_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_LSR_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}
__INLINE uint8_t uart2_overrun_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_LSR_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}


__INLINE uint8_t uart_data_rdy_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_LSR_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}
__INLINE uint8_t uart2_data_rdy_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_LSR_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/**
 * @brief MSR register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     04                  CTS   0
 *     00            DELTA_CTS   0
 * </pre>
 */

//org
//#define UART_MSR_ADDR   0x10007018
//#define UART_MSR_OFFSET 0x00000018
//#define UART_MSR_INDEX  0x00000006
//#define UART_MSR_RESET  0x00000000

//wik->UART_MSR_REG(0x50001018)
#define UART_MSR_ADDR   0x50001018
#define UART_MSR_OFFSET 0x00000018
#define UART_MSR_INDEX  0x00000006
#define UART_MSR_RESET  0x00000000

#define UART2_MSR_ADDR   0x50001118
#define UART2_MSR_OFFSET 0x00000018
#define UART2_MSR_INDEX  0x00000006
#define UART2_MSR_RESET  0x00000000

__INLINE uint32_t uart_msr_get(void)
{
    return REG_PL_RD(UART_MSR_ADDR);
}
__INLINE uint32_t uart2_msr_get(void)
{
    return REG_PL_RD(UART2_MSR_ADDR);
}


// field definitions
#define UART_CTS_BIT          ((uint32_t)0x00000010)
#define UART_CTS_POS          4
#define UART_DELTA_CTS_BIT    ((uint32_t)0x00000001)
#define UART_DELTA_CTS_POS    0

#define UART_CTS_RST          0x0
#define UART_DELTA_CTS_RST    0x0

__INLINE void uart_msr_unpack(uint8_t* cts, uint8_t* deltacts)
{
    uint32_t localVal = REG_PL_RD(UART_MSR_ADDR);

    *cts = (localVal & ((uint32_t)0x00000010)) >> 4;
    *deltacts = (localVal & ((uint32_t)0x00000001)) >> 0;
}
__INLINE void uart2_msr_unpack(uint8_t* cts, uint8_t* deltacts)
{
    uint32_t localVal = REG_PL_RD(UART2_MSR_ADDR);

    *cts = (localVal & ((uint32_t)0x00000010)) >> 4;
    *deltacts = (localVal & ((uint32_t)0x00000001)) >> 0;
}


__INLINE uint8_t uart_cts_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_MSR_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}
__INLINE uint8_t uart2_cts_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_MSR_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}


__INLINE uint8_t uart_delta_cts_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_MSR_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}
__INLINE uint8_t uart2_delta_cts_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_MSR_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

#if 0 //@wik, I don't think next functions not are available in the 580 hardware, so I comment them
//wik, I think the UART_RXLVL_ADDR AND UART_TXLVL_ADDR are fixed for 580, and Read Only

/**
 * @brief RXLVL register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  06:00             RX_LEVEL   0x0
 * </pre>
 */
//org 
//#define UART_RXLVL_ADDR   0x1000701C
//#define UART_RXLVL_OFFSET 0x0000001C
//#define UART_RXLVL_INDEX  0x00000007
//#define UART_RXLVL_RESET  0x00000000

//wik->UART_RFL_REG(0x5000107C) 
#define UART_RXLVL_ADDR   0x5000107C
#define UART_RXLVL_OFFSET 0x0000007C
#define UART_RXLVL_INDEX  0x00000007
#define UART_RXLVL_RESET  0x00000000

#define UART2_RXLVL_ADDR   0x5000117C
#define UART2_RXLVL_OFFSET 0x0000007C
#define UART2_RXLVL_INDEX  0x00000007
#define UART2_RXLVL_RESET  0x00000000

__INLINE uint32_t uart_rxlvl_get(void)
{
    return REG_PL_RD(UART_RXLVL_ADDR);
}
__INLINE uint32_t uart2_rxlvl_get(void)
{
    return REG_PL_RD(UART2_RXLVL_ADDR);
}


__INLINE void uart_rxlvl_set(uint32_t value)
{
    REG_PL_WR(UART_RXLVL_ADDR, value);
}
__INLINE void uart2_rxlvl_set(uint32_t value)
{
    REG_PL_WR(UART2_RXLVL_ADDR, value);
}

// field definitions
#define UART_RX_LEVEL_MASK   ((uint32_t)0x0000007F)
#define UART_RX_LEVEL_LSB    0
#define UART_RX_LEVEL_WIDTH  ((uint32_t)0x00000007)

#define UART_RX_LEVEL_RST    0x0

__INLINE uint8_t uart_rx_level_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_RXLVL_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0000007F)) == 0);
    return (localVal >> 0);
}
__INLINE uint8_t uart2_rx_level_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_RXLVL_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0000007F)) == 0);
    return (localVal >> 0);
}


__INLINE void uart_rx_level_setf(uint8_t rxlevel)
{
    ASSERT_ERR((((uint32_t)rxlevel << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(UART_RXLVL_ADDR, (uint32_t)rxlevel << 0);
}
__INLINE void uart2_rx_level_setf(uint8_t rxlevel)
{
    ASSERT_ERR((((uint32_t)rxlevel << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(UART2_RXLVL_ADDR, (uint32_t)rxlevel << 0);
}

/**
 * @brief TXLVL register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  06:00             TX_LEVEL   0x0
 * </pre>
 */
//org
//#define UART_TXLVL_ADDR   0x10007020
//#define UART_TXLVL_OFFSET 0x00000020
//#define UART_TXLVL_INDEX  0x00000008
//#define UART_TXLVL_RESET  0x00000000

//wik->UART_TFL_REG(0x50001080) 
#define UART_TXLVL_ADDR   0x50001080
#define UART_TXLVL_OFFSET 0x00001080
#define UART_TXLVL_INDEX  0x00000008
#define UART_TXLVL_RESET  0x00000000

#define UART2_TXLVL_ADDR   0x50001180
#define UART2_TXLVL_OFFSET 0x00001080
#define UART2_TXLVL_INDEX  0x00000008
#define UART2_TXLVL_RESET  0x00000000


__INLINE uint32_t uart_txlvl_get(void)
{
    return REG_PL_RD(UART_TXLVL_ADDR);
}
__INLINE uint32_t uart2_txlvl_get(void)
{
    return REG_PL_RD(UART2_TXLVL_ADDR);
}


__INLINE void uart_txlvl_set(uint32_t value)
{
    REG_PL_WR(UART_TXLVL_ADDR, value);
}
__INLINE void uart2_txlvl_set(uint32_t value)
{
    REG_PL_WR(UART2_TXLVL_ADDR, value);
}


// field definitions
#define UART_TX_LEVEL_MASK   ((uint32_t)0x0000007F)
#define UART_TX_LEVEL_LSB    0
#define UART_TX_LEVEL_WIDTH  ((uint32_t)0x00000007)

#define UART_TX_LEVEL_RST    0x0

__INLINE uint8_t uart_tx_level_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_TXLVL_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0000007F)) == 0);
    return (localVal >> 0);
}
__INLINE uint8_t uart2_tx_level_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART2_TXLVL_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0000007F)) == 0);
    return (localVal >> 0);
}


__INLINE void uart_tx_level_setf(uint8_t txlevel)
{
    ASSERT_ERR((((uint32_t)txlevel << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(UART_TXLVL_ADDR, (uint32_t)txlevel << 0);
}
__INLINE void uart2_tx_level_setf(uint8_t txlevel)
{
    ASSERT_ERR((((uint32_t)txlevel << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(UART2_TXLVL_ADDR, (uint32_t)txlevel << 0);
}





/**
 * @brief RTSRUN register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  06:00              RTS_RUN   0x0
 * </pre>
 */
#define UART_RTSRUN_ADDR   0x10007024
#define UART_RTSRUN_OFFSET 0x00000024
#define UART_RTSRUN_INDEX  0x00000009
#define UART_RTSRUN_RESET  0x00000000

__INLINE uint32_t uart_rtsrun_get(void)
{
    return REG_PL_RD(UART_RTSRUN_ADDR);
}

__INLINE void uart_rtsrun_set(uint32_t value)
{
    REG_PL_WR(UART_RTSRUN_ADDR, value);
}

// field definitions
#define UART_RTS_RUN_MASK   ((uint32_t)0x0000007F)
#define UART_RTS_RUN_LSB    0
#define UART_RTS_RUN_WIDTH  ((uint32_t)0x00000007)

#define UART_RTS_RUN_RST    0x0

__INLINE uint8_t uart_rts_run_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_RTSRUN_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0000007F)) == 0);
    return (localVal >> 0);
}

__INLINE void uart_rts_run_setf(uint8_t rtsrun)
{
    ASSERT_ERR((((uint32_t)rtsrun << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(UART_RTSRUN_ADDR, (uint32_t)rtsrun << 0);
}

/**
 * @brief RTSHLT register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  06:00             RTS_HALT   0x0
 * </pre>
 */
#define UART_RTSHLT_ADDR   0x10007028
#define UART_RTSHLT_OFFSET 0x00000028
#define UART_RTSHLT_INDEX  0x0000000A
#define UART_RTSHLT_RESET  0x00000000

__INLINE uint32_t uart_rtshlt_get(void)
{
    return REG_PL_RD(UART_RTSHLT_ADDR);
}

__INLINE void uart_rtshlt_set(uint32_t value)
{
    REG_PL_WR(UART_RTSHLT_ADDR, value);
}

// field definitions
#define UART_RTS_HALT_MASK   ((uint32_t)0x0000007F)
#define UART_RTS_HALT_LSB    0
#define UART_RTS_HALT_WIDTH  ((uint32_t)0x00000007)

#define UART_RTS_HALT_RST    0x0

__INLINE uint8_t uart_rts_halt_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_RTSHLT_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0000007F)) == 0);
    return (localVal >> 0);
}

__INLINE void uart_rts_halt_setf(uint8_t rtshalt)
{
    ASSERT_ERR((((uint32_t)rtshalt << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(UART_RTSHLT_ADDR, (uint32_t)rtshalt << 0);
}

#endif //0 WIK
/**
 * @brief ESTAT register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     00          TXFIFO_FULL   0
 * </pre>
 */
//org
//#define UART_ESTAT_ADDR   0x1000702C
//#define UART_ESTAT_OFFSET 0x0000002C
//#define UART_ESTAT_INDEX  0x0000000B
//#define UART_ESTAT_RESET  0x00000000

////wik->UART_USR_REG(0x5000107C) 
#define UART_USR_REG    (0x5000107C)
#define UART_USR_OFFSET 0x0000007C
#define UART_USR_INDEX  0x00000005
#define UART_USR_RESET  0x00000000

#define UART2_USR_REG    (0x5000117C)
#define UART2_USR_OFFSET 0x0000007C
#define UART2_USR_INDEX  0x00000005
#define UART2_USR_RESET  0x00000000

__INLINE uint32_t uart_estat_get(void)
{
    //return REG_PL_RD(UART_ESTAT_ADDR);
	  return REG_PL_RD(UART_USR_REG);
}
__INLINE uint32_t uart2_estat_get(void)
{
    //return REG_PL_RD(UART_ESTAT_ADDR);
	  return REG_PL_RD(UART2_USR_REG);
}

// field definitions
#define UART_TXFIFO_FULL_BIT    ((uint32_t)0x00000002)
#define UART_TXFIFO_FULL_POS    1

#define UART_TXFIFO_FULL_RST    0x1

__INLINE uint8_t uart_txfifo_full_getf(void)
{
    //uint32_t localVal = REG_PL_RD(UART_USR_REG);
    uint8_t localVal = (uint8_t)GetBits16(UART_USR_REG,UART_TFNF);
	  return localVal; 
}
__INLINE uint8_t uart2_txfifo_full_getf(void)
{
    //uint32_t localVal = REG_PL_RD(UART_USR_REG);
    uint8_t localVal = (uint8_t)GetBits16(UART2_USR_REG,UART_TFNF);
	  return localVal; 
}


#if 0//WIK, not applicable for our 580 so comment

/**
 * @brief VERSIONREL register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:06              FIFO_SZ   0x0
 *  05:00                 UREL   0x0
 * </pre>
 */
#define UART_VERSIONREL_ADDR   0x10007030
#define UART_VERSIONREL_OFFSET 0x00000030
#define UART_VERSIONREL_INDEX  0x0000000C
#define UART_VERSIONREL_RESET  0x00000000

__INLINE uint32_t uart_versionrel_get(void)
{
    return REG_PL_RD(UART_VERSIONREL_ADDR);
}

// field definitions
#define UART_FIFO_SZ_MASK   ((uint32_t)0x000000C0)
#define UART_FIFO_SZ_LSB    6
#define UART_FIFO_SZ_WIDTH  ((uint32_t)0x00000002)
#define UART_UREL_MASK      ((uint32_t)0x0000003F)
#define UART_UREL_LSB       0
#define UART_UREL_WIDTH     ((uint32_t)0x00000006)

#define UART_FIFO_SZ_RST    0x0
#define UART_UREL_RST       0x0

__INLINE void uart_versionrel_unpack(uint8_t* fifosz, uint8_t* urel)
{
    uint32_t localVal = REG_PL_RD(UART_VERSIONREL_ADDR);

    *fifosz = (localVal & ((uint32_t)0x000000C0)) >> 6;
    *urel = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

__INLINE uint8_t uart_fifo_sz_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_VERSIONREL_ADDR);
    return ((localVal & ((uint32_t)0x000000C0)) >> 6);
}

__INLINE uint8_t uart_urel_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_VERSIONREL_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief VERSIONUPG register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00                 UUPG   0x0
 * </pre>
 */
#define UART_VERSIONUPG_ADDR   0x10007034
#define UART_VERSIONUPG_OFFSET 0x00000034
#define UART_VERSIONUPG_INDEX  0x0000000D
#define UART_VERSIONUPG_RESET  0x00000000

__INLINE uint32_t uart_versionupg_get(void)
{
    return REG_PL_RD(UART_VERSIONUPG_ADDR);
}

// field definitions
#define UART_UUPG_MASK   ((uint32_t)0x000000FF)
#define UART_UUPG_LSB    0
#define UART_UUPG_WIDTH  ((uint32_t)0x00000008)

#define UART_UUPG_RST    0x0

__INLINE uint8_t uart_uupg_getf(void)
{
    uint32_t localVal = REG_PL_RD(UART_VERSIONUPG_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}
#endif //WIK, 0

#endif // _REG_UART_H_

