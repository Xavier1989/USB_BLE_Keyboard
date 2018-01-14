/************************************************************
	*	@file:		XX.c
	*	@brief:		XX
	*	@author:	XIET
	*	@version:	V1.0
	*	@data:		201x-xx-xx
	*	@modify:	data  			remarks
**************************************************************/
#include "bsp.h"
static void GPIO_Config(void);

/*============================================================
	*	@func:		XX
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA
	*	@modify: 	data 			remarks
=============================================================*/
void bsp_Init_GPIO(void)
{
    GPIO_Config();
}
/*============================================================
	*	@func:		XX
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA
	*	@modify: 	data 			remarks
=============================================================*/
static void GPIO_Config(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStructure;
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);

    LL_GPIO_ResetOutputPin(KEY_ROW_PORT, KEY_ROW_ALL_PIN);
    LL_GPIO_SetOutputPin(LED_ROW_PORT, LED_ROW_ALL_PIN);
    LL_GPIO_ResetOutputPin(BLE_PWR_PORT,BLE_PWR_PIN);
    LL_GPIO_ResetOutputPin(LED_PWR_PORT,LED_PWR_PIN);

    /***行 按键扫描输出配置***/
    GPIO_InitStructure.Pin = KEY_ROW_ALL_PIN;
    GPIO_InitStructure.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStructure.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(KEY_ROW_PORT, &GPIO_InitStructure);

    /********列按键扫描输入配置*************/
    GPIO_InitStructure.Pin = KEY_COL_ALL_PIN;
    GPIO_InitStructure.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_DOWN;
    LL_GPIO_Init(KEY_COL_PORT, &GPIO_InitStructure);

    /***行 LED输出配置***/
    GPIO_InitStructure.Pin = LED_ROW_ALL_PIN;
    GPIO_InitStructure.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStructure.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(LED_ROW_PORT, &GPIO_InitStructure);

    /***********LED、74HC595电源引脚配置*********/
    GPIO_InitStructure.Pin = LED_PWR_PIN;
    GPIO_InitStructure.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStructure.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(LED_PWR_PORT, &GPIO_InitStructure);

    /*********BEL电源控制引脚配置******/
    GPIO_InitStructure.Pin = BLE_PWR_PIN;
    GPIO_InitStructure.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStructure.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(BLE_PWR_PORT, &GPIO_InitStructure);
}
/*============================================================
	*	@func:		XX
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA
	*	@modify: 	data 			remarks
=============================================================*/
void GPIO_BLE_Power(uint8_t state)
{
    if(state == ENABLE)
    {
        LL_GPIO_SetOutputPin(BLE_PWR_PORT, BLE_PWR_PIN);
    }
    else
    {
        LL_GPIO_ResetOutputPin(BLE_PWR_PORT, BLE_PWR_PIN);
    }
}

/*============================================================
	*	@func:		XX
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA
	*	@modify: 	data 			remarks
=============================================================*/
void GPIO_LED_Power(uint8_t state)
{
    if(state == ENABLE)
    {
        LL_GPIO_SetOutputPin(LED_PWR_PORT, LED_PWR_PIN);
    }
    else
    {
        LL_GPIO_ResetOutputPin(LED_PWR_PORT, LED_PWR_PIN);
    }
}
/*============================================================
    *    @func:        XX
    *    @brief:        XX
    *    @param:        NA
    *    @retval:    NA    
    *    @modify:     data             remarks 
=============================================================*/
void GPIO_Config_EXTI(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);

	//行按键扫描输出，配置为高电平输出
	
	GPIO_InitStructure.Pin = KEY_ROW_ALL_PIN;			
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY_ROW_PORT, &GPIO_InitStructure);
	    
	LL_GPIO_SetOutputPin(KEY_ROW_PORT, KEY_ROW_ALL_PIN);

//  列 按键扫描输入，配置为外部中断
    GPIO_InitStructure.Pin = KEY_COL_ALL_PIN;
    GPIO_InitStructure.Mode =GPIO_MODE_IT_RISING;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStructure.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(KEY_COL_PORT, &GPIO_InitStructure);
	
	NVIC_SetPriority(EXTI0_1_IRQn,3);
	NVIC_SetPriority(EXTI2_3_IRQn,3);
	NVIC_SetPriority(EXTI4_15_IRQn,3);
	NVIC_EnableIRQ(EXTI0_1_IRQn);
	NVIC_EnableIRQ(EXTI2_3_IRQn);
	NVIC_EnableIRQ(EXTI4_15_IRQn);

}
/*============================================================
    *    @func:        XX
    *    @brief:        XX
    *    @param:        NA
    *    @retval:    NA    
    *    @modify:     data             remarks 
=============================================================*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Clear Wake Up Flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
}
