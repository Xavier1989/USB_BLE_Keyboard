#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H
#include "bsp.h"
/*
	�ڴ˶������ɸ������ʱ��ȫ�ֱ���
	ע���������__IO �� volatile����Ϊ����������жϺ���������ͬʱ�����ʣ��п�����ɱ����������Ż���
*/
#define TMR_COUNT					6		/* �����ʱ���ĸ��� ����ʱ��ID��Χ 0 - 3) */
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


/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef enum
{
	TMR_ONCE_MODE = 0,		/* һ�ι���ģʽ */
	TMR_AUTO_MODE = 1		/* �Զ���ʱ����ģʽ */
}TMR_MODE_E;

/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef struct
{
	volatile uint32_t Count;	/* ������ */
	volatile uint32_t PreLoad;	/* ������Ԥװֵ */
	volatile uint8_t Mode;		/* ������ģʽ��1���� */
	volatile uint8_t Flag;		/* ��ʱ�����־  */
}SOFT_TMR;

/* �ṩ������C�ļ����õĺ��� */
void bsp_InitTimer(void);
void bsp_DelayMS(uint32_t n);
void bsp_DelayUS(uint32_t n);
void bsp_StartTimer(uint8_t _id, uint32_t _period);
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period);
void bsp_StopTimer(uint8_t _id);
uint8_t bsp_CheckTimer(uint8_t _id);
int32_t bsp_GetRunTime(void);

#endif
