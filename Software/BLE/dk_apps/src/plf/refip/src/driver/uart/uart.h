/**
 ****************************************************************************************
 *
 * @file uart.h
 *
 * @brief UART Driver for HCI over UART operation.
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * $Rev: 5769 $
 *
 ****************************************************************************************
 */

#ifndef _UART_H_
#define _UART_H_

/**
 ****************************************************************************************
 * @defgroup UART UART
 * @ingroup DRIVERS
 *
 * @brief UART driver
 *
 * @{
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>       // integer definition
#include <stdbool.h>      // boolean definition

/*
 * DEFINES
 *****************************************************************************************
 */

/// Divider for 921600 bits/s
//#define UART_BAUDRATE_921K6         tbd
/// Divider for 460800 bits/s
//#define UART_BAUDRATE_460K8         tbd
/// Divider for 230400 bits/s
//#define UART_BAUDRATE_230K4         tbd2
/// Divider for 115200 bits/s
#define UART_BAUDRATE_115K2         9 // 9 is according 115200 in stead of 4
/// Divider for 57600 bits/s
//#define UART_BAUDRATE_57K6          tbd
/// Divider for 38400 bits/s
//#define UART_BAUDRATE_38K4          tbd
/// Divider for 28800 bits/s
//#define UART_BAUDRATE_28K8          tbd
/// Divider for 19200 bits/s
//#define UART_BAUDRATE_19K2          tbd
/// Divider for 9600 bits/s
//#define UART_BAUDRATE_9K6           tbd

/// Baudrate used on the UART
#ifndef CFG_ROM
#define UART_BAUDRATE  UART_BAUDRATE_115K2
#else //CFG_ROM
#define UART_BAUDRATE  UART_BAUDRATE_460K8
#endif //CFG_ROM

#if (UART_BAUDRATE == UART_BAUDRATE_921K6)
#define UART_CHAR_DURATION        11
#else
#define UART_CHAR_DURATION       (UART_BAUDRATE * 22)
#endif // (UART_BAUDRATE == UART_BAUDRATE_921K6)


/*
 * ENUMERATION DEFINITIONS
 *****************************************************************************************
 */

/// Generic enable/disable enum for UART driver
enum
{
    /// uart disable
    UART_DISABLE = 0,
    /// uart enable
    UART_ENABLE  = 1
};

/// Character format
enum
{
    /// char format 5
    UART_CHARFORMAT_5 = 0,
    /// char format 6
    UART_CHARFORMAT_6 = 1,
    /// char format 7
    UART_CHARFORMAT_7 = 2,
    /// char format 8
    UART_CHARFORMAT_8 = 3
};

/// Stop bit
enum
{
    /// stop bit 1
    UART_STOPBITS_1 = 0,
    /* Note: The number of stop bits is 1.5 if a
     * character format with 5 bit is chosen */
    /// stop bit 2
    UART_STOPBITS_2 = 1
};

/// Parity bit
enum
{
    /// even parity
    UART_PARITYBIT_EVEN  = 0,
    /// odd parity
    UART_PARITYBIT_ODD   = 1,
    /// space parity
    UART_PARITYBIT_SPACE = 2, // The parity bit is always 0.
    /// mark parity
    UART_PARITYBIT_MARK  = 3  // The parity bit is always 1.
};

/* Error detection */
enum
{
    /// error detection disabled
    UART_ERROR_DETECT_DISABLED = 0,
    /// error detection enabled
    UART_ERROR_DETECT_ENABLED  = 1
};

/// status values
enum
{
    /// status ok
    UART_STATUS_OK,
    /// status not ok
    UART_STATUS_ERROR
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

void UART2_Handler_func(void);

/**
 ****************************************************************************************
 * @brief Initializes the UART to default values.
 *****************************************************************************************
 */
void uart_init( uint8_t, uint8_t);
void uart2_init( uint8_t, uint8_t);
void uart2_init_func(uint8_t baudr, uint8_t mode);

#ifndef CFG_ROM
/**
 ****************************************************************************************
 * @brief Enable UART flow.
 *****************************************************************************************
 */
void uart_flow_on(void);
void uart2_flow_on(void);

/**
 ****************************************************************************************
 * @brief Disable UART flow.
 *****************************************************************************************
 */
bool uart_flow_off(void);
void uart2_flow_off(void);

/**
 ****************************************************************************************
 * @brief Finish current UART transfers
 *****************************************************************************************
 */
void uart_finish_transfers(void);
void uart2_finish_transfers(void);
#endif //CFG_ROM

/**
 ****************************************************************************************
 * @brief Starts a data reception.
 *
 * As soon as the end of the data transfer or a buffer overflow is detected,
 * the hci_uart_rx_done function is executed.
 *
 * @param[in,out]  bufptr Pointer to the RX buffer
 * @param[in]      size   Size of the expected reception
 *****************************************************************************************
 */
//void uart_read(uint8_t *bufptr, uint32_t size);
void uart_read(uint8_t *bufptr, uint32_t size, void (*callback) (uint8_t));
void uart2_read(uint8_t *bufptr, uint32_t size, void (*callback) (uint8_t));

/**
 ****************************************************************************************
 * @brief Starts a data transmission.
 *
 * As soon as the end of the data transfer is detected, the hci_uart_tx_done function is
 * executed.
 *
 * @param[in]  bufptr Pointer to the TX buffer
 * @param[in]  size   Size of the transmission
 *****************************************************************************************
 */
//void uart_write(uint8_t *bufptr, uint32_t size);
void uart_write(uint8_t *bufptr, uint32_t size, void (*callback) (uint8_t));
void uart2_write(uint8_t *bufptr, uint32_t size, void (*callback) (uint8_t));
void uart2_write_func(uint8_t *bufptr, uint32_t size, void (*callback) (uint8_t));
void uart2_flow_on_func(void);
bool uart2_flow_off_func(void);
void uart2_finish_transfers_func(void);
void uart2_read_func(uint8_t *bufptr, uint32_t size,void (*callback) (uint8_t));
void uart2_write_func(uint8_t *bufptr, uint32_t size,void (*callback) (uint8_t));\
void UART2_Handler_func(void);

/**
 ****************************************************************************************
 * @brief Serves the data transfer interrupt requests.
 *
 * It clears the requests and executes the appropriate callback function.
 *****************************************************************************************
 */
void uart_isr(void);

/// @} UART
#endif /* _UART_H_ */
