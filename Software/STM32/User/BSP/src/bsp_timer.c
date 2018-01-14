/************************************************************
	*	@file:		XX.c
	*	@brief:		XX
	*	@author:	XIET
	*	@version:	V1.0
	*	@data:		201x-xx-xx
	*	@modify:	data  			remarks
**************************************************************/
#include "bsp.h"

static volatile uint32_t s_uiDelayCount = 0;
static volatile uint8_t s_ucTimeOutFlag = 0;

static SOFT_TMR s_tTmr[TMR_COUNT];

__IO int32_t g_iRunTime = 0;

static void bsp_SoftTimerDec(SOFT_TMR *_tmr);

/*============================================================
	*	@func:		bsp_InitTimer
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
void bsp_InitTimer(void)
{
	uint8_t i;

	/* 清零所有的软件定时器 */
	for (i = 0; i < TMR_COUNT; i++)
	{
		s_tTmr[i].Count = 0;
		s_tTmr[i].PreLoad = 0;
		s_tTmr[i].Flag = 0;
		s_tTmr[i].Mode = TMR_ONCE_MODE;	/* 缺省是一次性工作模式 */
	}
	SysTick_Config(SystemCoreClock/1000U);
}
/*============================================================
	*	@func:		bsp_InitTimer
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
extern void bsp_RunPer1ms(void);
extern void bsp_RunPer10ms(void);
void SysTick_ISR(void)
{
	static uint8_t s_count = 0;
	uint8_t i;

	if (s_uiDelayCount > 0)
	{
		if (--s_uiDelayCount == 0)
		{
			s_ucTimeOutFlag = 1;
		}
	}

	for (i = 0; i < TMR_COUNT; i++)
	{
		bsp_SoftTimerDec(&s_tTmr[i]);
	}

	g_iRunTime++;
	if (g_iRunTime == 0x7FFFFFFF)	
	{
		g_iRunTime = 0;
	}

	bsp_RunPer1ms();		

	if (++s_count >= 10)
	{
		s_count = 0;

		bsp_RunPer10ms();	
	}
}

/*============================================================
	*	@func:		bsp_SoftTimerDec
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
static void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
	if (_tmr->Count > 0)
	{
		
		if (--_tmr->Count == 0)
		{
			_tmr->Flag = 1;

			if(_tmr->Mode == TMR_AUTO_MODE)
			{
				_tmr->Count = _tmr->PreLoad;
			}
		}
	}
}
/*============================================================
	*	@func:		bsp_DelayMS
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
void bsp_DelayMS(uint32_t n)
{
	if (n == 0)
	{
		return;
	}
	else if (n == 1)
	{
		n = 2;
	}

	DISABLE_INT();  			

	s_uiDelayCount = n;
	s_ucTimeOutFlag = 0;

	ENABLE_INT();  				

	while (1)
	{
		bsp_Idle();				
		if (s_ucTimeOutFlag == 1)
		{
			break;
		}
	}
}

/*============================================================
	*	@func:		bsp_DelayUS
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
void bsp_DelayUS(uint32_t n)
{
    uint32_t ticks;
    uint32_t told;
    uint32_t tnow;
    uint32_t tcnt = 0;
    uint32_t reload;
       
		reload = SysTick->LOAD;                
    ticks = n * (SystemCoreClock / 1000000);	 
    
    tcnt = 0;
    told = SysTick->VAL;            

    while (1)
    {
        tnow = SysTick->VAL;    
        if (tnow != told)
        {    
 
            if (tnow < told)
            {
                tcnt += told - tnow;    
            }
          
            else
            {
                tcnt += reload - tnow + told;    
            }        
            told = tnow;

    
            if (tcnt >= ticks)
            {
            	break;
            }
        }  
    }
} 


/*============================================================
	*	@func:		bsp_StartTimer
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
void bsp_StartTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		while(1); 
	}

	DISABLE_INT();  			

	s_tTmr[_id].Count = _period;		
	s_tTmr[_id].PreLoad = _period;		
	s_tTmr[_id].Flag = 0;				
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	

	ENABLE_INT();  				
}

/*============================================================
	*	@func:		bsp_StartAutoTimer
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		while(1); 
	}

	DISABLE_INT();  		

	s_tTmr[_id].Count = _period;			
	s_tTmr[_id].PreLoad = _period;		/*  */
	s_tTmr[_id].Flag = 0;				/*  */
	s_tTmr[_id].Mode = TMR_AUTO_MODE;	/*  */

	ENABLE_INT();  			/*  */
}

/*============================================================
	*	@func:		bsp_StopTimer
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
void bsp_StopTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		/*  */
		while(1); /*  */
	}

	DISABLE_INT();  	/*  */

	s_tTmr[_id].Count = 0;				/*  */
	s_tTmr[_id].Flag = 0;				/*  */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/*  */

	ENABLE_INT();  		/*  */
}

/*============================================================
	*	@func:		bsp_CheckTimer
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
uint8_t bsp_CheckTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		return 0;
	}

	if (s_tTmr[_id].Flag == 1)
	{
		s_tTmr[_id].Flag = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

/*============================================================
	*	@func:		bsp_GetRunTime
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
int32_t bsp_GetRunTime(void)
{
	int32_t runtime;

	DISABLE_INT();  	/*  */

	runtime = g_iRunTime;	/*  */

	ENABLE_INT();  		/*  */

	return runtime;
}

/*============================================================
	*	@func:		SysTick_Handler
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
void SysTick_Handler(void)
{
	SysTick_ISR();
}


