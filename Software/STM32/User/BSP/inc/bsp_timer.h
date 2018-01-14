#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H
#include "bsp.h"
/*
	在此定义若干个软件定时器全局变量
	注意必须增加__IO 即 volatile，因为这个变量在中断和主程序中同时被访问，有可能造成编译器错误优化。
*/
#define TMR_COUNT					6		/* 软件定时器的个数 （定时器ID范围 0 - 3) */
#define TMR_PERIOD_15MS		15
#define TMR_PERIOD_20MS		20
#define TMR_PERIOD_30MS		30
#define TMR_PERIOD_40MS		40
#define TMR_PERIOD_50MS		50
#define TMR_PERIOD_80MS		80
#define TMR_PERIOD_3MS      3
#define TMR_PERIOD_2MS      2
#define TMR_PERIOD_200MS    200
#define TMR_PERIOD_500MS    500
#define TMR_PERIOD_10MIN 	(1000*60*10)


enum
{
	TMR_FLOW=0,
	TMR_LED_CTRL=1,
	TMR_LED_INIT=2,
	TMR_KEY_SCAN=3,
	TMR_SLEEP
};


/* 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题 */
typedef enum
{
	TMR_ONCE_MODE = 0,		/* 一次工作模式 */
	TMR_AUTO_MODE = 1		/* 自动定时工作模式 */
}TMR_MODE_E;

/* 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题 */
typedef struct
{
	volatile uint32_t Count;	/* 计数器 */
	volatile uint32_t PreLoad;	/* 计数器预装值 */
	volatile uint8_t Mode;		/* 计数器模式，1次性 */
	volatile uint8_t Flag;		/* 定时到达标志  */
}SOFT_TMR;

/* 提供给其他C文件调用的函数 */
void bsp_InitTimer(void);
void bsp_DelayMS(uint32_t n);
void bsp_DelayUS(uint32_t n);
void bsp_StartTimer(uint8_t _id, uint32_t _period);
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period);
void bsp_StopTimer(uint8_t _id);
uint8_t bsp_CheckTimer(uint8_t _id);
int32_t bsp_GetRunTime(void);

#endif
