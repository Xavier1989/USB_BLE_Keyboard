/**
 ****************************************************************************************
 *
 * @file i2c_eeprom.c
 *
 * @brief eeprom driver over i2c interface.
 *
 * Copyright (C) 2012. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

#include <stdint.h>
#include "global_io.h"
#include "gpio.h"
#include "periph_setup.h"
#include "i2c_eeprom.h"

// macros
#define SEND_I2C_COMMAND(X) SetWord16(I2C_DATA_CMD_REG, (X))
#define WAIT_WHILE_I2C_FIFO_IS_FULL() while( (GetWord16(I2C_STATUS_REG) & TFNF) == 0 )
#define WAIT_UNTIL_I2C_FIFO_IS_EMPTY() while( (GetWord16(I2C_STATUS_REG) & TFE) == 0 )
#define WAIT_UNTIL_NO_MASTER_ACTIVITY() while( (GetWord16(I2C_STATUS_REG) & MST_ACTIVITY) !=0 )
#define WAIT_FOR_RECEIVED_BYTE() while(GetWord16(I2C_RXFLR_REG) == 0)

static uint8_t mem_address_size;    // 2 byte address is used or not.
static uint8_t i2c_dev_address;     // Device addres

/**
 ****************************************************************************************
 * @brief Initialize I2C controller as a master for EEPROM handling.
 ****************************************************************************************
 */
void i2c_eeprom_init(uint16_t dev_address, uint8_t speed, uint8_t address_mode, uint8_t address_size)
{
    mem_address_size = address_size;
    SetBits16(CLK_PER_REG, I2C_ENABLE, 1);                                        // enable  clock for I2C 
    SetWord16(I2C_ENABLE_REG, 0x0);                                               // Disable the I2C controller	
    SetWord16(I2C_CON_REG, I2C_MASTER_MODE | I2C_SLAVE_DISABLE | I2C_RESTART_EN); // Slave is disabled
    SetBits16(I2C_CON_REG, I2C_SPEED, speed);                                     // Set speed
    SetBits16(I2C_CON_REG, I2C_10BITADDR_MASTER, address_mode);                   // Set addressing mode
    SetWord16(I2C_TAR_REG, dev_address & 0x3FF);                                  // Set Slave device address
    SetWord16(I2C_ENABLE_REG, 0x1);                                               // Enable the I2C controller
    while( (GetWord16(I2C_STATUS_REG) & 0x20) != 0 );                             // Wait for I2C master FSM to be IDLE
    i2c_dev_address = dev_address;
}


/**
 ****************************************************************************************
 * @brief Disable I2C controller and clock
 ****************************************************************************************
 */
void i2c_eeprom_release(void)
{	
    SetWord16(I2C_ENABLE_REG, 0x0);                             // Disable the I2C controller	
    SetBits16(CLK_PER_REG, I2C_ENABLE, 0);                      // Disable clock for I2C
}


/**
 ****************************************************************************************
 * @brief Polls until I2C eeprom is ready
 ****************************************************************************************
 */
void i2c_wait_until_eeprom_ready(void)
{
    uint16_t abort_SR_Status; // TX Abort Source Register
    // Polling until EEPROM ACK to detect busy period
    do {
        SEND_I2C_COMMAND(0x08);                                 // Make a dummy access
        WAIT_UNTIL_I2C_FIFO_IS_EMPTY();                         // Wait until Tx FIFO is empty
        WAIT_UNTIL_NO_MASTER_ACTIVITY();                        // Wait until no master activity   
        abort_SR_Status = GetWord16(I2C_TX_ABRT_SOURCE_REG);    // Read the Tx abort source register
        GetWord16(I2C_CLR_TX_ABRT_REG);                         // Clear the Tx abort flag
    } while( (abort_SR_Status & ABRT_7B_ADDR_NOACK) != 0 );     // Repeat if not ACK    
}


/**
 ********************************************************************m********************
 * @brief Send I2C EEPROM's memory address
 *
 * @param[in] address of the I2C EEPROM memory
 ****************************************************************************************
 */
void i2c_send_address(uint32_t address_to_send)
{
    switch(mem_address_size)
    {
        case I2C_2BYTES_ADDR:
            SetWord16(I2C_ENABLE_REG, 0x0);  
            SetWord16(I2C_TAR_REG, i2c_dev_address | ((address_to_send & 0x10000) >> 16));  // Set Slave device address
            SetWord16(I2C_ENABLE_REG, 0x1);
            while( (GetWord16(I2C_STATUS_REG) & 0x20) != 0 );   // Wait for I2C master FSM to be IDLE
            SEND_I2C_COMMAND( (address_to_send >> 8) & 0xFF );  // Set address MSB, write access        
        break;
        
        case I2C_3BYTES_ADDR:
            SEND_I2C_COMMAND( (address_to_send >> 16) & 0xFF);  // Set address MSB, write access
            SEND_I2C_COMMAND( (address_to_send >> 8) & 0xFF);   // Set address MSB, write access        
        break;
    }

    SEND_I2C_COMMAND(address_to_send & 0xFF);                   // Set address LSB, write access    
}



/**
 ****************************************************************************************
 * @brief Read single byte from I2C EEPROM.
 *
 * @param[in] Memory address to read the byte from.
 *
 * @return Read byte.
 ****************************************************************************************
 */
uint8_t i2c_eeprom_read_byte(uint32_t address)
{
    i2c_wait_until_eeprom_ready();
    i2c_send_address(address);  
    
    WAIT_WHILE_I2C_FIFO_IS_FULL();                  // Wait if Tx FIFO is full
    SEND_I2C_COMMAND(0x0100);                       // Set R/W bit to 1 (read access)
    WAIT_UNTIL_I2C_FIFO_IS_EMPTY();                 // Wait until I2C Tx FIFO empty
    WAIT_FOR_RECEIVED_BYTE();                       // Wait for received data
    return (0xFF & GetWord16(I2C_DATA_CMD_REG));    // Get received byte
}



/**
 ****************************************************************************************
 * @brief Read single series of bytes from I2C EEPROM (for driver's internal use)
 *
 * @param[in] p    Memory address to read the series of bytes from (all in the same page)
 * @param[in] size count of bytes to read (must not cross page)
 ****************************************************************************************
 */
static void read_data_single(uint8_t **p, uint32_t address, uint32_t size)
{
    int j;
    
    i2c_send_address(address);
                
    for (j = 0; j < size; j++)
    {    
        WAIT_WHILE_I2C_FIFO_IS_FULL();              // Wait if Tx FIFO is full
        SEND_I2C_COMMAND(0x0100);                   // Set read access for <size> times
    }
    
    // Critical section
    GLOBAL_INT_DISABLE();
    
    // Get the received data
    for (j = 0; j < size; j++)                                         
    {
        WAIT_FOR_RECEIVED_BYTE();                   // Wait for received data
        **p =(0xFF & GetWord16(I2C_DATA_CMD_REG));  // Get the received byte
        (*p)++;
    }
    // End of critical section
    GLOBAL_INT_RESTORE();
}



/**
 ****************************************************************************************
 * @brief Reads data from I2C EEPROM to memory position of given pointer.
 *
 * @param[in] rd_data_ptr     Read data pointer.
 * @param[in] address         Starting memory address.
 * @param[in] size            Size of the data to be read.
 *
 * @return Bytes that were actually read (due to memory size limitation).
 ****************************************************************************************
 */
uint32_t i2c_eeprom_read_data(uint8_t *rd_data_ptr, uint32_t address, uint32_t size)
{
    uint32_t tmp_size;
    uint32_t bytes_read; 

    if (size == 0)
        return 0;
    
    if (size > I2C_EEPROM_SIZE - address)           // Check for max bytes to be read from a (MAX_SIZE x 8) EEPROM
    {
        tmp_size = I2C_EEPROM_SIZE - address;
        bytes_read = tmp_size;
    }
    else
    {
        tmp_size = size;
        bytes_read = size;
    }

    i2c_wait_until_eeprom_ready();

    // Read 32 bytes at a time
    while (tmp_size >= 32)
    {
        read_data_single(&rd_data_ptr, address, 32);
            
        address += 32;                              // Update base address for read
        tmp_size -= 32;                             // Update tmp_size for bytes remaining to be read
    }

    if (tmp_size)
        read_data_single(&rd_data_ptr, address, tmp_size);

    return bytes_read;
}

/**
 ****************************************************************************************
 * @brief Write single byte to I2C EEPROM.
 *
 * @param[in] address     Memory position to write the byte to.
 * @param[in] wr_data     Byte to be written.
 ****************************************************************************************
 */
void i2c_eeprom_write_byte(uint32_t address, uint8_t wr_data)
{
    i2c_wait_until_eeprom_ready();
    i2c_send_address(address);
        
    WAIT_WHILE_I2C_FIFO_IS_FULL();                  // Wait if I2C Tx FIFO is full
    SEND_I2C_COMMAND(wr_data & 0xFF);               // Send write data
    WAIT_UNTIL_I2C_FIFO_IS_EMPTY();                 // Wait until Tx FIFO is empty
    
    WAIT_UNTIL_NO_MASTER_ACTIVITY();                // wait until no master activity 
}


/**
 ****************************************************************************************
 * @brief Writes page to I2C EEPROM.
 *
 * @param[in] address         Starting address of memory page.
 * @param[in] wr_data_ptr     Pointer to the first of bytes to be written.
 * @param[in] size            Size of the data to be written (MUST BE LESS OR EQUAL TO I2C_EEPROM_PAGE).
 *
 * @return                    Count of bytes that were actually written
 ****************************************************************************************
 */
uint16_t i2c_eeprom_write_page(uint8_t *wr_data_ptr, uint32_t address, uint16_t size)
{
	uint16_t feasible_size;
    uint16_t bytes_written = 0;

    if (address < I2C_EEPROM_SIZE)
    {
        // max possible write size without crossing page boundary
        feasible_size = I2C_EEPROM_PAGE - (address % I2C_EEPROM_PAGE);
        
        if (size < feasible_size)                                                                    
            feasible_size = size;                   // adjust limit accordingly
        
        i2c_wait_until_eeprom_ready();
        
        // Critical section
        GLOBAL_INT_DISABLE();
        
        i2c_send_address(address);
        
        do
        {
            WAIT_WHILE_I2C_FIFO_IS_FULL();          // Wait if I2c Tx FIFO is full
            SEND_I2C_COMMAND(*wr_data_ptr & 0xFF);  // Send write data
            wr_data_ptr++;
            feasible_size--;
            bytes_written++;
        } 
        while (feasible_size != 0);

        // End of critical section
        GLOBAL_INT_RESTORE();
        
        WAIT_UNTIL_I2C_FIFO_IS_EMPTY();             // Wait until Tx FIFO is empty
        WAIT_UNTIL_NO_MASTER_ACTIVITY();            // Wait until no master activity
    }
    
    return bytes_written;
}

/**
 ****************************************************************************************
 * @brief Writes data to I2C EEPROM.
 *
 * @param[in] address         Starting address of the write process.
 * @param[in] wr_data_ptr     Pointer to the first of bytes to be written.
 * @param[in] size            Size of the data to be written.
 *
 * @return Bytes that were actually written (due to memory size limitation).
 ****************************************************************************************
 */
uint32_t i2c_eeprom_write_data(uint8_t *wr_data_ptr, uint32_t address, uint32_t size)
{
	uint32_t bytes_written; 
	uint32_t feasible_size = size;
    uint32_t bytes_left_to_send;  

    if (address >= I2C_EEPROM_SIZE)                 // address is not it the EEPROM address space    
        return 0;
   
	// limit to the maximum count of bytes that can be written to an (I2C_EEPROM_SIZE x 8) EEPROM
	if (size > I2C_EEPROM_SIZE - address)
        feasible_size = I2C_EEPROM_SIZE - address;
 
    bytes_left_to_send = feasible_size;
    bytes_written = 0;
    
    while (bytes_left_to_send)
    {
		uint16_t cnt = i2c_eeprom_write_page(wr_data_ptr + bytes_written, address + bytes_written, bytes_left_to_send);
        bytes_written += cnt;  
        bytes_left_to_send -= cnt;
    }
    
    return bytes_written;
}
