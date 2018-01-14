/**
 ****************************************************************************************
 *
 * @file uart_ftdi.c
 *
 * @brief UART driver for FTDI device
 *
 * Copyright (C) RivieraWaves 2009-2013
 *
 * $Rev: 5768 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @defgroup UART_FTDI
 * @ingroup UART
 *
 * @{
 ****************************************************************************************
 */
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stddef.h>                 // standard definition
#include "arch.h"                   // arch definition
#include "compiler.h"               // for inline functions
#include "uart.h"                   // uart definitions
#include "hci_uart.h"               // hci uart definitions
#include "reg_dmac_common.h"        // dma register
#include "reg_dmac_channel.h"       // dma channel register
#include "reg_ftdi.h"               // ftdi register

/*
 * DEFINES
 ****************************************************************************************
 */
#define RX_CHANNEL        1    // RX is using DMA channel 1
#define TX_CHANNEL        0    // TX is using DMA channel 0

#define DMAIT_RX_CHANNEL (1<<RX_CHANNEL)
#define DMAIT_TX_CHANNEL (1<<TX_CHANNEL)

#define FTDI_MEM_ADD      0x30000000

/// Mask to check if address is on master0 or master1 bus
#define DMA_MASTER1_BUS_MASK              (0x40000000)

/// Macro to select master0 or master1 bus for DMA source/destination buffer addresses
#define DMA_IS_MASTER1(address)           ((address & DMA_MASTER1_BUS_MASK) != 0)

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Starts a DMA transfer for transmission.
 *
 * @param[in] src_addr  RAM address, start of the space where data is transfered from.
 * @param[in] size      Transfer size, to set to DMA Byte Count register which counts down.
 *****************************************************************************************
 */
static void dma_tx(uint32_t src_addr, uint16_t size)
{
    // Set the source address
    dmac_sar_set(TX_CHANNEL, src_addr);

    // Check on which bus is source buffer
    if(DMA_IS_MASTER1(src_addr))
    {
        // Set the transfer size
        dmac_ccfgr1_set(TX_CHANNEL, (size << DMAC_BLKXFERSIZE_LSB) | DMAC_MSTRSELDST_BIT |
                DMAC_MSTRSELSRC_BIT | DMAC_ADDRINCSRC_BIT);
    }
    else
    {
        // Set the transfer size
        dmac_ccfgr1_set(TX_CHANNEL, (size << DMAC_BLKXFERSIZE_LSB) | DMAC_MSTRSELDST_BIT |
                DMAC_ADDRINCSRC_BIT);
    }

    // Enable the transfer
    dmac_ccfgr2_set(TX_CHANNEL, DMAC_TCMASK_BIT | DMAC_DEMASK_BIT | DMAC_SWSGLREQSRC_BIT |
                    DMAC_HWREQDSTEN_BIT);
}


/**
 ****************************************************************************************
 * @brief Program a DMA transfer for reception.
 *
 * @param[in] dest_addr RAM address, start of the space where data is transfered from.
 * @param[in] size      Transfer size, to set to DMA Byte Count register which counts down.
 *****************************************************************************************
 */
static void dma_rx(uint32_t dest_addr, uint16_t size)
{
    // Set the destination address
    dmac_dar_set(RX_CHANNEL, dest_addr);

    // Check on which bus is destination buffer
    if(DMA_IS_MASTER1(dest_addr))
    {
        // Set the transfer size
        dmac_ccfgr1_set(RX_CHANNEL, (size << DMAC_BLKXFERSIZE_LSB) | DMAC_MSTRSELSRC_BIT |
                DMAC_MSTRSELDST_BIT | DMAC_ADDRINCDST_BIT);
    }
    else
    {
        // Set the transfer size
        dmac_ccfgr1_set(RX_CHANNEL, (size << DMAC_BLKXFERSIZE_LSB) | DMAC_MSTRSELSRC_BIT |
                DMAC_ADDRINCDST_BIT);
    }

    // Enable the transfer
    dmac_ccfgr2_set(RX_CHANNEL, DMAC_TCMASK_BIT | DMAC_DEMASK_BIT | DMAC_SWSGLREQDST_BIT |
                    DMAC_HWREQSRCEN_BIT);
}

void uart_init(void)
{
    // Enable FTDI reception
    ftdi_rx_en_setf(1);

    // Put default configuration of RX channel
    dmac_sar_set(RX_CHANNEL, FTDI_MEM_ADD);
    dmac_llptr_set(RX_CHANNEL, 0);

    // Put default configuration of TX channel
    dmac_dar_set(TX_CHANNEL, FTDI_MEM_ADD);
    dmac_llptr_set(TX_CHANNEL, 0);
}


void uart_read(uint8_t *bufptr, uint32_t size)
{
    // Sanity check
    ASSERT_ERR(bufptr != NULL);
    ASSERT_ERR(size != 0);

    // Program RX DMA
    dma_rx((uint32_t)bufptr, size);
}


void uart_write(uint8_t *bufptr, uint32_t size)
{
    // Sanity check
    ASSERT_ERR(bufptr != NULL);
    ASSERT_ERR((0 < size) && (size < 1025));

    // Program TX DMA
    dma_tx((uint32_t)bufptr, size);
}

void uart_flow_on(void)
{
    // On FTDI, no possibility to stop the flow
}

bool uart_flow_off(void)
{
    // On FTDI, no possibility to stop the flow
    return (true);
}

void uart_finish_transfers(void)
{
    // Wait TX FIFO empty
    while(dmac_swsglreqsrc_getf(TX_CHANNEL));
}


void uart_isr(void)
{
    uint32_t irq_status;

    // First read the status register
    irq_status = dmac_isr_get();

    // Clear the interrupt
    dmac_iclr_set(irq_status);

    // Check if some error occurred
    ASSERT_ERR((irq_status & (DMAC_DEINTDST_MASK | DMAC_DEINTSRC_MASK | DMAC_DEINTLLI_MASK)) == 0);

    // Handle RX interrupt
    if (irq_status & DMAIT_RX_CHANNEL)
    {
        // Inform HCI about the end of transmission
        hci_uart_rx_done(UART_STATUS_OK);
    }

    // Handle TX interrupt
    if (irq_status & DMAIT_TX_CHANNEL)
    {
        // Inform HCI about the end of transmission
        hci_uart_tx_done(UART_STATUS_OK);
    }
}

/// @} UART_FTDI
