#ifndef __BSP_PWM_H
#define __BSP_PWM_H 
#include "bsp.h"


#define GPIO_PORT_HC595_OE		GPIOA
#define GPIO_PIN_HC595_OE		LL_GPIO_PIN_0


void bsp_Init_PWM(void);

#endif
