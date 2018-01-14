/**
 ****************************************************************************************
 *
 * @file gpio.c
 *
 * @brief Hardware GPIO abstruction layer.
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

#include "arch.h"
#include "gpio.h"
#include "periph_setup.h"

#if DEVELOPMENT_DEBUG
    #ifndef GPIO_DRV_PIN_ALLOC_MON_DISABLED

int GPIO[NO_OF_PORTS][NO_OF_MAX_PINS_PER_PORT];

static const uint16_t p_mask[4] = { 0xFF, 0x3F, 0x3FF, 0x1FF };

volatile uint64_t GPIO_status;


/*
 * Local Functions
 ****************************************************************************************
 */

    #endif //GPIO_DRV_PIN_ALLOC_MON_DISABLED
#endif //DEVELOPMENT_DEBUG

/*
 * Global Functions
 ****************************************************************************************
 */

#define NO_OF_IRQS (GPIO4_IRQn - GPIO0_IRQn + 1)
GPIO_handler_function_t GPIOHandlerFunction[NO_OF_IRQS];

/**
 ****************************************************************************************
 * @brief Initialize the GPIO assignemnt check variables
 *
 * @return void
 ****************************************************************************************
 */
void GPIO_init( void )
{
#if DEVELOPMENT_DEBUG
    #ifndef GPIO_DRV_PIN_ALLOC_MON_DISABLED
        #warning "GPIO assignment checking is active! Deactivate before burning OTP..."
    
    int i, j;

    for (i = 0; i < NO_OF_PORTS; i++)
        for (j = 0; j < NO_OF_MAX_PINS_PER_PORT; j++)
            GPIO[i][j] = 0;
    
    GPIO_reservations();
    
    GPIO_status = 0;
    
    for (i = 0; i < NO_OF_PORTS; i++)
        for (j = 0; j < NO_OF_MAX_PINS_PER_PORT; j++) {
            uint16_t bitmask = (1 << j);
            
            if ( !(p_mask[i] & bitmask) ) // port pin does not exist! continue to next port...
                break;
            
            if (GPIO[i][j] == -1) {
                volatile int port = i;
                volatile int col = j;
                
                __asm("BKPT #0\n"); // this pin has been previously reserved!
            }
            
            if (GPIO[i][j] == 0)
                continue;
            
            GPIO_status |= ((uint64_t)GPIO[i][j] << j) << (i * 16);
        }
    #endif //GPIO_DRV_PIN_ALLOC_MON_DISABLED
#endif //DEVELOPMENT_DEBUG    
}


/**
 ****************************************************************************************
 * @brief Set the pin type and mode
 *
 * @param[in] port     GPIO port
 * @param[in] pin      GPIO pin
 * @param[in] mode     GPIO pin mode.     INPUT = 0, INPUT_PULLUP = 0x100, INPUT_PULLDOWN = 0x200, OUTPUT = 0x300,
 * @param[in] function GPIO pin usage. GPIO_FUNCTION enumaration.
 *
 * @return void
 ****************************************************************************************
 */
void GPIO_SetPinFunction( GPIO_PORT port, GPIO_PIN pin, GPIO_PUPD mode, GPIO_FUNCTION function )
{
#if DEVELOPMENT_DEBUG
    #ifndef GPIO_DRV_PIN_ALLOC_MON_DISABLED
    if ( !(GPIO_status & ( ((uint64_t)1 << pin) << (port * 16) )) )
        __asm("BKPT #0\n"); // this pin has not been previously reserved!        
    #endif //GPIO_DRV_PIN_ALLOC_MON_DISABLED
#endif //DEVELOPMENT_DEBUG
    if (port == GPIO_PORT_3)
        port = GPIO_PORT_3_REMAP; // Set to 4 due to P30_MODE_REG address (0x50003086 instead of 0x50003066)
    
    const int data_reg = GPIO_BASE + (port << 5);
    const int mode_reg = data_reg + 0x6 + (pin << 1);
    
    SetWord16(mode_reg, mode | function);
}


/**
 ****************************************************************************************
 * @brief Combined function to set the state and the type and mode of the GPIO pin 
 *
 * @param[in] port     GPIO port
 * @param[in] pin      GPIO pin
 * @param[in] mode     GPIO pin mode.     INPUT = 0, INPUT_PULLUP = 0x100, INPUT_PULLDOWN = 0x200, OUTPUT = 0x300,
 * @param[in] function GPIO pin usage. GPIO_FUNCTION enumaration.
 * @param[in] high     set to TRUE to set the pin into high else low
 *
 * @return void
 ****************************************************************************************
 */
void GPIO_ConfigurePin( GPIO_PORT port, GPIO_PIN pin, GPIO_PUPD mode, GPIO_FUNCTION function,
						 const bool high )
{
#if DEVELOPMENT_DEBUG
    #ifndef GPIO_DRV_PIN_ALLOC_MON_DISABLED
    if ( !(GPIO_status & ( ((uint64_t)1 << pin) << (port * 16) )) )
        __asm("BKPT #0\n"); // this pin has not been previously reserved!        
    #endif //GPIO_DRV_PIN_ALLOC_MON_DISABLED
#endif //DEVELOPMENT_DEBUG   
    
    if (high)
        GPIO_SetActive( port, pin );
    else
        GPIO_SetInactive( port, pin );
    
	GPIO_SetPinFunction( port, pin, mode, function );
}


/**
 ****************************************************************************************
 * @brief Sets a pin high. The GPIO should have been previously configured as output!
 *
 * @param[in] port     GPIO port
 * @param[in] pin      GPIO pin
 *
 * @return void
 ****************************************************************************************
*/
void GPIO_SetActive( GPIO_PORT port, GPIO_PIN pin )
{
#if DEVELOPMENT_DEBUG
    #ifndef GPIO_DRV_PIN_ALLOC_MON_DISABLED
    if ( !(GPIO_status & ( ((uint64_t)1 << pin) << (port * 16) )) )
        __asm("BKPT #0\n"); // this pin has not been previously reserved!        
    #endif //GPIO_DRV_PIN_ALLOC_MON_DISABLED
#endif //DEVELOPMENT_DEBUG    
    if (port == GPIO_PORT_3)
        port = GPIO_PORT_3_REMAP; // Set to 4 due to P30_MODE_REG address (0x50003086 instead of 0x50003066)
    
    const int data_reg = GPIO_BASE + (port << 5);
    const int set_data_reg = data_reg + 2;
    
	SetWord16(set_data_reg, 1 << pin);
}


/**
 ****************************************************************************************
 * @brief Sets the GPIO low. The GPIO should have been previously configured as output!
 *
 * @param[in] port     GPIO port
 * @param[in] pin      GPIO pin
 *
 * @return void
 ****************************************************************************************
*/
void GPIO_SetInactive( GPIO_PORT port, GPIO_PIN pin )
{
#if DEVELOPMENT_DEBUG
    #ifndef GPIO_DRV_PIN_ALLOC_MON_DISABLED
    if ( !(GPIO_status & ( ((uint64_t)1 << pin) << (port * 16) )) )
        __asm("BKPT #0\n"); // this pin has not been previously reserved!        
    #endif //GPIO_DRV_PIN_ALLOC_MON_DISABLED
#endif //DEVELOPMENT_DEBUG       
    if (port == GPIO_PORT_3)
        port = GPIO_PORT_3_REMAP; // Set to 4 due to P30_MODE_REG address (0x50003086 instead of 0x50003066)
    
    const int data_reg = GPIO_BASE + (port << 5);
    const int reset_data_reg = data_reg + 4;
    
	SetWord16(reset_data_reg, 1 << pin);
}


/**
 ****************************************************************************************
 * @brief Gets the GPIO status. The GPIO should have been previously configured as input!
 *
 * @param[in] port     GPIO port
 * @param[in] pin      GPIO pin
 *
 * @return bool        TRUE if the pin is high, FALSE if low.
 ****************************************************************************************
*/
bool GPIO_GetPinStatus( GPIO_PORT port, GPIO_PIN pin )
{
#if DEVELOPMENT_DEBUG
    #ifndef GPIO_DRV_PIN_ALLOC_MON_DISABLED
    if ( !(GPIO_status & ( ((uint64_t)1 << pin) << (port * 16) )) )
        __asm("BKPT #0\n"); // this pin has not been previously reserved!        
    #endif //GPIO_DRV_PIN_ALLOC_MON_DISABLED
#endif //DEVELOPMENT_DEBUG       
    if (port == GPIO_PORT_3)
        port = GPIO_PORT_3_REMAP; // Set to 4 due to P30_MODE_REG address (0x50003086 instead of 0x50003066)
    
    const int data_reg = GPIO_BASE + (port << 5);
    
    return ( (GetWord16(data_reg) & (1 << pin)) != 0 );
}



#ifndef GPIO_DRV_IRQ_HANDLING_DISABLED

/**
 ****************************************************************************************
 * @brief Function to set the interrupt generated by the GPIO pin 
 *
 * @param[in] port  	    GPIO port
 * @param[in] pin   	    GPIO pin
 * @param[in] irq           GPIO IRQ
 * @param[in] low_input     TRUE generates an IRQ if the input is low
 * @param[in] release_wait  TRUE waits for key release after interrupt was reset
 * @param[in] debounce_ms	duration of a debounce sequence before an IRQ is generated
 *
 * @return void
 ****************************************************************************************
 */
void GPIO_EnableIRQ( GPIO_PORT port, GPIO_PIN pin, IRQn_Type irq, bool low_input,
					 bool release_wait, uint8_t debounce_ms )
{
    #if DEVELOPMENT_DEBUG
        #ifndef GPIO_DRV_PIN_ALLOC_MON_DISABLED
    if ( !(GPIO_status & ( ((uint64_t)1 << pin) << (port * 16) )) )
        __asm("BKPT #0\n"); // this pin has not been previously reserved!        
        #endif //GPIO_DRV_PIN_ALLOC_MON_DISABLED   
    #endif //DEVELOPMENT_DEBUG       
    
    const uint8_t   KBRD_IRQn_SEL_BASE[] = {
        1,
        9,
        15,
        25
    };
    
	SetBits16(GPIO_DEBOUNCE_REG, (DEB_ENABLE0 << (irq-GPIO0_IRQn)), (debounce_ms > 0) );
	SetBits16(GPIO_DEBOUNCE_REG, DEB_VALUE, debounce_ms);
	SetBits16(GPIO_INT_LEVEL_CTRL_REG, EDGE_LEVELn0 << (irq-GPIO0_IRQn), release_wait);
	SetBits16(GPIO_INT_LEVEL_CTRL_REG, INPUT_LEVEL0 << (irq-GPIO0_IRQn), low_input);
	SetBits16(GPIO_IRQ0_IN_SEL_REG + 2*(irq-GPIO0_IRQn), KBRD_IRQn_SEL, KBRD_IRQn_SEL_BASE[port] + pin);
    
	NVIC_SetPriority(irq, 2);
	NVIC_EnableIRQ(irq);
}


/**
 ****************************************************************************************
 * @brief Function to reset the interrupt generated by the GPIO pin 
 *
 * @param[in] irq	GPIO IRQ
 *
 * @return void
 ****************************************************************************************
 */
void GPIO_ResetIRQ( IRQn_Type irq )
{ 
	SetBits16(GPIO_RESET_IRQ_REG, RESET_GPIO0_IRQ << (irq-GPIO0_IRQn), 1);
}


/**
 ****************************************************************************************
 * @brief Register Callback function for GPIO IRQ.
 *
 * @param[in] irq	    The handler of this IRQ will call the function
 *
 * @param[in] callback  Callback function's reference.
 *
 * @return void
 ****************************************************************************************
 */
void GPIO_RegisterCallback(IRQn_Type irq, GPIO_handler_function_t callback)
{
	GPIOHandlerFunction[irq - GPIO0_IRQn] = callback;
}


/**
 ****************************************************************************************
 * @brief GPIO IRQn Handler
 *
 * @param[in] irq	The IRQ that this handler services
 *
 * @return void
 ****************************************************************************************
 */ 
void GPIOn_Handler(IRQn_Type irq)
{
	GPIO_ResetIRQ(irq);		

	GPIO_handler_function_t callback = GPIOHandlerFunction[irq-GPIO0_IRQn];
	if (callback != NULL)
        callback();
    else
        NVIC_ClearPendingIRQ(irq);     
}


/**
 ****************************************************************************************
 * @brief GPIO IRQ0 Handler
 *
 * @return void
 ****************************************************************************************
 */ 
void GPIO0_Handler(void)
{
	GPIOn_Handler(GPIO0_IRQn);
} 


/**
 ****************************************************************************************
 * @brief GPIO IRQ1 Handler
 *
 * @return void
 ****************************************************************************************
 */ 
void GPIO1_Handler(void)
{
	GPIOn_Handler(GPIO1_IRQn);
} 


/**
 ****************************************************************************************
 * @brief GPIO IRQ2 Handler
 *
 * @return void
 ****************************************************************************************
 */ 
void GPIO2_Handler(void)
{
	GPIOn_Handler(GPIO2_IRQn);
} 


/**
 ****************************************************************************************
 * @brief GPIO IRQ3 Handler
 *
 * @return void
 ****************************************************************************************
 */
void GPIO3_Handler(void)
{
	GPIOn_Handler(GPIO3_IRQn);
} 


/**
 ****************************************************************************************
 * @brief GPIO IRQ4 Handler
 *
 * @return void
 ****************************************************************************************
 */ 
void GPIO4_Handler(void)
{
	GPIOn_Handler(GPIO4_IRQn);
} 

#endif //GPIO_DRV_IRQ_HANDLING_DISABLED
