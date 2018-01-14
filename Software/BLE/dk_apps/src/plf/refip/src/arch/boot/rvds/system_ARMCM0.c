/**************************************************************************//**
 * @file     system_ARMCM0.c
 * @brief    CMSIS Cortex-M0 Device System Source File
 *           for CM0 Device Series
 * @version  V1.05
 * @date     26. July 2011
 *
 * @note
 * Copyright (C) 2010-2011 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M 
 * processor based microcontrollers.  This file can be freely distributed 
 * within development tools that are supporting such ARM based processors. 
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/

#include "ARMCM0.h"
#include <string.h>

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define __HSI             ( 8000000UL)
#define __XTAL            (12000000UL)    /* Oscillator frequency             */

#define __SYSTEM_CLOCK    (4*__XTAL)




/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void)
{
    int i;

    SetWord16(TIMER0_CTRL_REG, 0x6);                                // stop timer
    NVIC_DisableIRQ(SWTIM_IRQn);                                    // disable software timer interrupt

#ifdef __DA14581__    
    while ( (GetWord16(SYS_STAT_REG) & XTAL16_SETTLED) == 0 );      // wait for XTAL16 settle
    SetBits16(CLK_CTRL_REG, SYS_CLK_SEL, 0);                        // switch to  XTAL16
    while ( (GetWord16(CLK_CTRL_REG) & RUNNING_AT_XTAL16M) == 0 );  // wait for actual switch
#endif    
    
    SetBits32(GP_CONTROL_REG, EM_MAP, 23);
    
    // Set clocks to 16 MHz to speed up memeset operation
    SetWord16(CLK_AMBA_REG, 0x00);

    SetBits16(PMU_CTRL_REG, RETENTION_MODE, 0xF);
    
    // Fill 0x80000 - 0x83000 with zeros
    unsigned int *p_retmem = (unsigned int *)0x80000;
    for (i = 0xBFF; i >= 0; i--)
        *(volatile unsigned *)p_retmem++ = 0;

    __enable_irq();

#ifdef __USE_GPIO
    ARM_GPIO0->DATA[0].WORD = 0;
    ARM_GPIO0->IE = 0;
    ARM_GPIO0->DIR = 0xff83;

    ARM_GPIO1->DATA[0].WORD = 0;
    ARM_GPIO1->IE = 0;
    ARM_GPIO1->DIR = 0;

    ARM_GPIO2->DATA[0].WORD = 0;
    ARM_GPIO2->IE = 0;
    ARM_GPIO2->DIR = 0;
#endif
}


