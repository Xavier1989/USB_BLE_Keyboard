#ifndef __BSP_74HC595_H
#define __BSP_74HC595_H 
#include "bsp.h"
#define SPI_74HC595			SPI1

#define GPIO_PORT_HC595			GPIOA
#define GPIO_PIN_HC595_CLK		LL_GPIO_PIN_5
#define GPIO_PIN_HC595_DATA		LL_GPIO_PIN_7

#define GPIO_PORT_HC595_LAT		GPIOC 
#define GPIO_PIN_HC595_LAT		LL_GPIO_PIN_7

#define HC595_LAT_HIGH()		LL_GPIO_SetOutputPin(GPIO_PORT_HC595_LAT,GPIO_PIN_HC595_LAT)
#define HC595_LAT_LOW()			LL_GPIO_ResetOutputPin(GPIO_PORT_HC595_LAT,GPIO_PIN_HC595_LAT)

void bsp_74hc595_Init(void);			//74hc595初始化
void HC595_SendByte(uint8_t byte);		//发送一字节数据
void HC595_Send2Byte(uint16_t byte);	//发送两个字节
void HC595_SendData(uint16_t *pbuf, uint8_t len);		//发送指定长度数据

#endif
