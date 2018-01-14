/**
 ****************************************************************************************
 *
 * @file periph_setup.c
 *
 * @brief Peripherals setup and initialization. 
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
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"             // SW configuration
#include "periph_setup.h"             // periphera configuration
#include "global_io.h"
#include "gpio.h"
#include "app_spotar.h"
/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/*
 * Name         : GPIO_reservations - Globally reserved GPIOs 
 *
 * Scope        : LOCAL
 *
 * Arguments    : none
 *
 * Description  : Each application reserves its own GPIOs here.
 *                If there are GPIOs that have to be globally reserved (i.e. UART)
 *                then their reservation MUST be done BEFORE any application reservations.
 *
 * Returns      : void
 *
 */
//

#if DEVELOPMENT_DEBUG
void GPIO_reservations(void)
{
    /*
     * Globally reserved GPIOs reservation
     */

    // UART GPIOs
#ifdef PROGRAM_ENABLE_UART
    RESERVE_GPIO( UART1_TX, GPIO_PORT_0,  GPIO_PIN_4, PID_UART1_TX);
    RESERVE_GPIO( UART1_RX, GPIO_PORT_0,  GPIO_PIN_5, PID_UART1_RX);    
# ifdef PROGRAM_ALTERNATE_UART_PINS
    #if !(BLE_APP_PRESENT)
    RESERVE_GPIO( UART1_TX, GPIO_PORT_0, GPIO_PIN_7, PID_UART1_RTSN);
    RESERVE_GPIO( UART1_RX, GPIO_PORT_0, GPIO_PIN_6, PID_UART1_CTSN);  
    # endif // !BLE_APP_PRESENT
# else
    #if !(BLE_APP_PRESENT)
    RESERVE_GPIO( UART1_TX, GPIO_PORT_0, GPIO_PIN_3, PID_UART1_RTSN);
    RESERVE_GPIO( UART1_RX, GPIO_PORT_0, GPIO_PIN_2, PID_UART1_CTSN);  
    # endif // !BLE_APP_PRESENT    
# endif // PROGRAM_ALTERNATE_UART_PINS
#endif // PROGRAM_ENABLE_UART

    /*
     * Application specific GPIOs reservation
     */    
#if (BLE_APP_PRESENT)
    RESERVE_GPIO( SPI_CLK, GPIO_PORT_0, GPIO_PIN_0, PID_SPI_CLK);
    RESERVE_GPIO( SPI_DO, GPIO_PORT_0, GPIO_PIN_6, PID_SPI_DO);
    RESERVE_GPIO( SPI_DI, GPIO_PORT_0, GPIO_PIN_5, PID_SPI_DI);
    RESERVE_GPIO( SPI_EN, GPIO_PORT_0, GPIO_PIN_3, PID_SPI_EN);
    
    //RESERVE_GPIO( I2C_SCL, GPIO_PORT_0, GPIO_PIN_2, PID_I2C_SCL);
    //RESERVE_GPIO( I2C_SDA, GPIO_PORT_0, GPIO_PIN_3, PID_I2C_SDA);
#endif

#if BLE_BATT_SERVER
#if !BLE_HID_DEVICE   
	//Setup LED GPIO for battery alert
    RESERVE_GPIO( RED_LED, GPIO_PORT_1, GPIO_PIN_0, PID_GPIO);
#endif	
#endif

}
#endif

/**
 ****************************************************************************************
 * @brief Map port pins
 *
 * The Uart and SPI port pins and GPIO ports(for debugging) are mapped
 ****************************************************************************************
 */
void set_pad_functions(void)        // set gpio port function mode
{

#ifdef PROGRAM_ENABLE_UART
    GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_4, OUTPUT, PID_UART1_TX, false );
    GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_5, INPUT, PID_UART1_RX, false );    
#endif // PROGRAM_ENABLE_UART

#if (BLE_APP_PRESENT)
    //GPIO_ConfigurePin(GPIO_PORT_0, GPIO_PIN_2, INPUT, PID_I2C_SCL, false);
    //GPIO_ConfigurePin(GPIO_PORT_0, GPIO_PIN_3, INPUT, PID_I2C_SDA, false);
    
    GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_3, OUTPUT, PID_SPI_EN, true );
    GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_0, OUTPUT, PID_SPI_CLK, false );
    GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_6, OUTPUT, PID_SPI_DO, false );	
    GPIO_ConfigurePin( GPIO_PORT_0, GPIO_PIN_5, INPUT, PID_SPI_DI, false );
#endif
}


/**
 ****************************************************************************************
 * @brief Enable pad's and peripheral clocks assuming that peripherals' power domain is down.
 *
 * The Uart and SPi clocks are set. 
 ****************************************************************************************
 */
void periph_init(void)  // set i2c, spi, uart, uart2 serial clks
{
	// Power up peripherals' power domain
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & PER_IS_UP)) ; 
    
	
	// TODO: Application specific - Modify accordingly!
	// Example: Activate UART and SPI.
	
    // Initialize UART component
#ifdef PROGRAM_ENABLE_UART
    SetBits16(CLK_PER_REG, UART1_ENABLE, 1);    // enable clock - always @16MHz
	
    // baudr=9-> 115k2
    // mode=3-> no parity, 1 stop bit 8 data length
#ifdef UART_MEGABIT
    uart_init(UART_BAUDRATE_1M, 3);
#else
    uart_init(UART_BAUDRATE_115K2, 3);
#endif // UART_MEGABIT
    //NVIC_SetPriority(UART_IRQn, 1);
#endif // PROGRAM_ENABLE_UART

#if 0
	//Example: Do something with the timer if need be...
    SetWord16(TIMER0_CTRL_REG, 0); 
    SetWord16(TIMER0_RELOAD_M_REG, 0);
    SetWord16(TIMER0_RELOAD_N_REG, 0);
    SetWord16(TIMER0_ON_REG, 0);
#endif
	
	//rom patch
	patch_func();
	
	//Init pads
	set_pad_functions();

#if (BLE_APP_PRESENT)
#if BLE_BATTERY_SERVER
    app_batt_port_reinit();
#endif //BLE_BATTERY_SERVER
#endif //BLE_APP_PRESENT

    // Enable the pads
	SetBits16(SYS_CTRL_REG, PAD_LATCH_EN, 1);
}
