#include <stdint.h>
#include "periph_setup.h"
#include "mc3410.h"
#include "global_io.h"
#include "gpio.h"
#include "app_console.h"

#define MC3410_SLAVE_ADDRESS 	0x4C
#define MC3410_ADDRESS_END   	0x40
#define MC3410_ADDRESS_PAGE   8
#define MC3410_SPEED_MODE    	2      	// Speed mode: 1 (100 kbits/s), 2 (400 kbits/s)
#define MC3410_ADDRESS_MODE  	0   		// Addressing mode: 0 (7 bit address), 1 (10 bit address)
#define MC3410_ADDRESS_SIZE  	0  			// Address width: 0 (1 byte), 1(2 bytes), 2(3 bytes)

#define MC3410_RES_2G10BITS 0x03
#define MC3410_RES_4G10BITS 0x07
#define MC3410_RES_8G10BITS 0x0B
#define MC3410_RES_8G14BITS	0x0F
#define MC3410_RES MC3410_RES_8G14BITS

#if (MC3410_RES == 0x03)
#define MC3410_LSB2G 256.0f
#elif (MC3410_RES == 0x07)
#define MC3410_LSB2G 128.0f
#elif (MC3410_RES == 0x0B)
#define MC3410_LSB2G 64.0f
#elif (MC3410_RES == 0x0F)
#define MC3410_LSB2G 1024.0f
#else
#error "MC3410_RES undefined" 
#endif

#define MC3410_LPF_512HZ	0x00
#define MC3410_LPF_256HZ	0x10
#define MC3410_LPF_128HZ	0x20
#define MC3410_LPF_64HZ		0x30
#define MC3410_LPF_32HZ		0x40
#define MC3410_LPF_16HZ		0x50
#define MC3410_LPF_8HZ		0x60

#define MC3410_GINT_LPF		0x80

#define MC3410_OUTPUT_START 0x0D
#define MC3410_OUTPUT_BYTES 6

// macros
#define SEND_I2C_COMMAND(X) SetWord16(I2C_DATA_CMD_REG, (X))
#define WAIT_WHILE_I2C_FIFO_IS_FULL() while( (GetWord16(I2C_STATUS_REG) & TFNF) == 0 )
#define WAIT_UNTIL_I2C_FIFO_IS_EMPTY() while( (GetWord16(I2C_STATUS_REG) & TFE) == 0 )
#define WAIT_UNTIL_NO_MASTER_ACTIVITY() while( (GetWord16(I2C_STATUS_REG) & MST_ACTIVITY) !=0 )
#define WAIT_FOR_RECEIVED_BYTE() while(GetWord16(I2C_RXFLR_REG) == 0)

static uint8_t mem_address_size;    // 2 byte address is used or not.
static uint8_t i2c_dev_address;     // Device addres

#define early_puts(s)       arch_puts(s)
#define early_putc(c)       arch_printf("%c", c);
#define early_puti(i)       arch_printf("%d", i);
#define early_puth(h, l)    arch_printf("0x%x", h);

static void mc3410_wait_until_ready(void)
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

static void mc3410_send_address(uint32_t address_to_send)
{
	switch(mem_address_size)
	{
		case 1:	// 2 bytes
			SetWord16(I2C_ENABLE_REG, 0x0);  
			SetWord16(I2C_TAR_REG, i2c_dev_address | ((address_to_send & 0x10000) >> 16));  // Set Slave device address
			SetWord16(I2C_ENABLE_REG, 0x1);
			while( (GetWord16(I2C_STATUS_REG) & 0x20) != 0 );   // Wait for I2C master FSM to be IDLE
			SEND_I2C_COMMAND( (address_to_send >> 8) & 0xFF );  // Set address MSB, write access        
		break;
		case 2:	// 3 bytes
			SEND_I2C_COMMAND( (address_to_send >> 16) & 0xFF);  // Set address MSB, write access
			SEND_I2C_COMMAND( (address_to_send >> 8) & 0xFF);   // Set address MSB, write access        
		break;
	}

	SEND_I2C_COMMAND(address_to_send & 0xFF);               // Set address LSB, write access    
}

static uint8_t mc3410_read_byte(uint32_t address)
{
	mc3410_wait_until_ready();
	mc3410_send_address(address);  
	
	WAIT_WHILE_I2C_FIFO_IS_FULL();                  // Wait if Tx FIFO is full
	SEND_I2C_COMMAND(0x0100);                       // Set R/W bit to 1 (read access)
	WAIT_UNTIL_I2C_FIFO_IS_EMPTY();                 // Wait until I2C Tx FIFO empty
	WAIT_FOR_RECEIVED_BYTE();                       // Wait for received data
	return (0xFF & GetWord16(I2C_DATA_CMD_REG));    // Get received byte
}

static void mc3410_write_byte(uint32_t address, uint8_t wr_data)
{
	mc3410_wait_until_ready();
	mc3410_send_address(address);
			
	WAIT_WHILE_I2C_FIFO_IS_FULL();                  // Wait if I2C Tx FIFO is full
	SEND_I2C_COMMAND(wr_data & 0xFF);               // Send write data
	WAIT_UNTIL_I2C_FIFO_IS_EMPTY();                 // Wait until Tx FIFO is empty
	
	WAIT_UNTIL_NO_MASTER_ACTIVITY();                // wait until no master activity 
}

static void mc3410_set_range(uint8_t range)
{
	mc3410_write_byte(0x0020, range);
}

static void mc3410_enable(int enable)
{
//#define _CHECK_ENABLE_STATUS
#ifdef _CHECK_ENABLE_STATUS
	uint8_t data;
#endif
	
	// enable device	
	if (enable) {
		mc3410_write_byte(0x0007, 0x01);	// wake
	} else {
		mc3410_write_byte(0x0007, 0x03);	// stand by
	}
	
#ifdef _CHECK_ENABLE_STATUS
	data = mc3410_read_byte(0x0004);
	if ((data & 0x03) == 0x01)
		early_puts("*** mc3410_init() wake\r\n");
	else if (((data & 0x03) == 0x03) || ((data & 0x03) == 0x00)) {
		early_puts("*** mc3410_init() stand by\r\n");
	} else {
		early_puts("*** mc3410_init() fails\r\n");
		early_puth(data, 1);
	}
#endif
}

static int mc3410_validate(void)
{
	uint8_t data = mc3410_read_byte(0x003B);	// read product code
	if ((data & 0xFF) == 0xB8) {
		early_puts("*** mc3410_init() done\r\n");
		return 0;
	}
	
	return -1;
}

static void mc3410_brust_read_data(uint8_t **p, uint32_t address, uint32_t size)
{
	int j;
	
	mc3410_send_address(address);
							
	for (j = 0; j < size; j++) {
			WAIT_WHILE_I2C_FIFO_IS_FULL();              // Wait if Tx FIFO is full
			SEND_I2C_COMMAND(0x0100);                   // Set read access for <size> times
	}
	
	// Critical section
	GLOBAL_INT_DISABLE();
	
	// Get the received data
	for (j = 0; j < size; j++) {                                        
			WAIT_FOR_RECEIVED_BYTE();                   // Wait for received data
			**p =(0xFF & GetWord16(I2C_DATA_CMD_REG));  // Get the received byte
			(*p)++;
	}
	// End of critical section
	GLOBAL_INT_RESTORE();
}

static uint32_t mc3410_read_data(uint8_t *rd_data_ptr, uint32_t address, uint32_t size)
{
	uint32_t tmp_size;
	uint32_t bytes_read; 

	if (size == 0)
		return 0;
	
	if (size > MC3410_ADDRESS_END - address) {          // Check for max bytes to be read from
		tmp_size = MC3410_ADDRESS_END - address;
		bytes_read = tmp_size;
	} else {
		tmp_size = size;
		bytes_read = size;
	}

	mc3410_wait_until_ready();

	// Read 32 bytes at a time
	while (tmp_size >= 32) {
		mc3410_brust_read_data(&rd_data_ptr, address, 32);
		address += 32;                              // Update base address for read
		tmp_size -= 32;                             // Update tmp_size for bytes remaining to be read
	}

	if (tmp_size)
		mc3410_brust_read_data(&rd_data_ptr, address, tmp_size);

	return bytes_read;
}

void mc3410_reserve(void)
{
#ifdef MC3410_ENABLE
	RESERVE_GPIO(MC3410_SCL, GPIO_PORT_1, GPIO_PIN_0, PID_I2C_SCL);
	RESERVE_GPIO(MC3410_SDA, GPIO_PORT_1, GPIO_PIN_1, PID_I2C_SDA);
#endif
}

void mc3410_config(void)
{
#ifdef MC3410_ENABLE
	GPIO_ConfigurePin(GPIO_PORT_1, GPIO_PIN_0, INPUT, PID_I2C_SCL, false);
	GPIO_ConfigurePin(GPIO_PORT_1, GPIO_PIN_1, INPUT, PID_I2C_SDA, false);
#endif
}

static void mc3410_i2c_master_init(void)
{
	mem_address_size = MC3410_ADDRESS_SIZE;
	SetBits16(CLK_PER_REG, I2C_ENABLE, 1);                                        // enable clock for I2C 
	SetWord16(I2C_ENABLE_REG, 0x0);                                               // Disable the I2C controller	
	SetWord16(I2C_CON_REG, I2C_MASTER_MODE | I2C_SLAVE_DISABLE | I2C_RESTART_EN); // Slave is disabled
	SetBits16(I2C_CON_REG, I2C_SPEED, MC3410_SPEED_MODE);                         // Set speed
	SetBits16(I2C_CON_REG, I2C_10BITADDR_MASTER, MC3410_ADDRESS_MODE);            // Set addressing mode
	SetWord16(I2C_TAR_REG, MC3410_SLAVE_ADDRESS & 0x3FF);                         // Set Slave device address
	SetWord16(I2C_ENABLE_REG, 0x1);                                               // Enable the I2C controller
	while( (GetWord16(I2C_STATUS_REG) & 0x20) != 0 );                             // Wait for I2C master FSM to be IDLE
	i2c_dev_address = MC3410_SLAVE_ADDRESS;
}

void mc3410_init(void)
{
#ifdef MC3410_ENABLE
	
	mc3410_i2c_master_init();
	
	mc3410_enable(0);	
	//mc3410_write_byte(0x05, 0x00);	// SPCNT
	mc3410_write_byte(0x06, 0x00);	// INTEN
	mc3410_write_byte(0x08, 0x00);	// SR
	mc3410_write_byte(0x09, 0x00);	// PSET
	mc3410_write_byte(0x0a, 0x00);	// PD
	mc3410_write_byte(0x0b, 0x00);	// DROP
	mc3410_write_byte(0x0c, 0x00);	// SHAKE_DB
	mc3410_write_byte(0x20, MC3410_RES);	// RANGE
	mc3410_write_byte(0x2b, 0x00);	// SHAKE_TH
	mc3410_write_byte(0x2c, 0x00);	// UD_Z_TH
	mc3410_write_byte(0x2d, 0x00);	// UD_X_TH
	mc3410_write_byte(0x2e, 0x00);	// RL_Z_TH
	mc3410_write_byte(0x2f, 0x00);	// RL_Y_TH
	mc3410_write_byte(0x30, 0x00);	// FB_Z_TH
	mc3410_write_byte(0x31, 0x00);	// DROP_TH
	mc3410_write_byte(0x32, 0x00);	// TAP_TH
	mc3410_enable(1);	// enable device
	
	// mc3410_dump();

	if (0 <= mc3410_validate()) {
		early_puts("*** mc3410_init() done\r\n");
	} else {
		early_puts("*** mc3410_init() fails\r\n");
	}
#endif
}

void mc3410_release(void)
{
#ifdef MC3410_ENABLE
	mc3410_enable(0);
	SetWord16(I2C_ENABLE_REG, 0x0);          	// Disable the I2C controller	
  SetBits16(CLK_PER_REG, I2C_ENABLE, 0);    // Disable clock for I2C
#endif
}

void mc3410_dump(void)
{
#ifdef MC3410_ENABLE
	uint8_t regmap[0x3F] = {0};
	uint32_t r;
	
	mc3410_enable(0);
	
	r = mc3410_read_data(&regmap[0], 0x00, 0x3F);
	if (r == 0x3F) {
		for (r = 0; r < 0x40; ++r) {
			early_puth(regmap[r], 1);
			if ((r % 8) == 7) {
				early_putc('\r');
				early_putc('\n');
			} else {
				early_putc(' ');
			}
		}
	}
	
	mc3410_enable(1);
#endif
}

void mc3410_output_raw(int16_t *xyz)
{
	uint8_t data[MC3410_OUTPUT_BYTES] = {0};
	uint32_t r;
	
	if (! xyz)
		return;
	
	//mc3410_enable(0);
	
	r = mc3410_read_data(&data[0], MC3410_OUTPUT_START, MC3410_OUTPUT_BYTES);
	if (r != MC3410_OUTPUT_BYTES) {
		early_puts("Failed to read output data\r\n");
		return;
	}
	
	//mc3410_enable(1);
	for (r = 0; r < 3; ++r) {
		*xyz = (int16_t)(((uint16_t)data[r*2]) | (((uint16_t)data[r*2+1]) << 8));
		
		early_puth(*xyz, 2);
		early_putc('(');
		early_puti(*xyz);
		early_putc(')');
		early_putc(' ');
		
		++xyz;
	}

	early_putc('\r');
	early_putc('\n');

}

void mc3410_output(float *xyz)
{
	int16_t data[3] = {0};
	int8_t i;
	
	if (! xyz)
		return;
	
	mc3410_output_raw(&data[0]);
	
	for (i = 0; i < 3; ++i) {
		*xyz++ = ((float)data[i])/MC3410_LSB2G;
	}
}

void mc3410_wake_init(void)
{
    mc3410_i2c_master_init();
}

void mc3410_wake_release(void)
{
    SetWord16(I2C_ENABLE_REG, 0x0);             // Disable the I2C controller   
    SetBits16(CLK_PER_REG, I2C_ENABLE, 0);    // Disable clock for I2C
}

