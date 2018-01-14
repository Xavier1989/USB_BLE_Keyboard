/************************************************************
	*	@file:		XX.c
	*	@brief:		XX
	*	@author:	XIET
	*	@version:	V1.0
	*	@data:		201x-xx-xx
	*	@modify:	data  			remarks
**************************************************************/
#include "bsp.h"
/*============================================================
	*	@func:		BSP_Init
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
void bsp_Init(void)
{	
	bsp_Init_GPIO();
	bsp_InitUart();
	bsp_Init_PWM();
	bsp_74hc595_Init();
	bsp_InitTimer();
}
/*============================================================
	*	@func:		bsp_RunPer10ms
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
void bsp_RunPer10ms(void)
{
	
}
/*============================================================
	*	@func:		bsp_RunPer1ms
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
void bsp_RunPer1ms(void)
{

}
/*============================================================
	*	@func:		bsp_Idle
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
void bsp_Idle(void)
{

}
