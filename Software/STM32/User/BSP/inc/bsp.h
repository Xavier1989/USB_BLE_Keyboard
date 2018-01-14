#ifndef __BSP_H
#define __BSP_H
#include "stm32L0xx_hal.h"
#include "stm32l053xx_ll_conf.h"
#include "stdbool.h"

#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_hid.h"



#include "bsp_gpio.h"
#include "bsp_timer.h"
#include "bsp_usb.h"
#include "bsp_uart_fifo.h"
#include "bsp_pwm.h"
#include "bsp_74hc595.h"
#include "bsp_lowpwr.h"
#include "user_display.h"

#include "keycode.h"
#include "keymap.h"
#include "user_key.h"
#include "keyboard.h"




/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */


#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif
/****BSP***/

void bsp_Init(void);
void bsp_RunPer10ms(void);
void bsp_RunPer1ms(void);
void bsp_Idle(void);

#endif



