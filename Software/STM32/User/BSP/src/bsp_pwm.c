/************************************************************
    *   @file:      bsp_pwm.c
    *   @brief:     XX
    *   @author:    XIET
    *   @version:   V1.0
    *   @data:      201x-xx-xx
    *   @modify:    data            remarks
**************************************************************/
#include "bsp.h"

#define LUMINANCE_LEVEL             (8)     //范围1---10

#define PERIOD_VALUE							(1000-1)

static void PWM_Config_74HC595_OE(void);
/*============================================================
    *   @func:      XX
    *   @brief:     XX
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void bsp_Init_PWM(void)
{
    PWM_Config_74HC595_OE();
}
/*============================================================
    *   @func:      PWM_Config_74HC595_OE
    *   @brief:     74HC595 OE 脚PWM配置,使用TIM15
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
static void PWM_Config_74HC595_OE(void)
{
	uint16_t prescalerValue;
	 uint16_t Channel1Pulse ;

    LL_GPIO_InitTypeDef GPIO_InitStruct;
    LL_TIM_InitTypeDef TIM_InitStruct;
    LL_TIM_OC_InitTypeDef TIM_OC_InitStruct;

    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	
	  prescalerValue = (SystemCoreClock / 1000000 ) - 1;          //1MHZ
		Channel1Pulse = LUMINANCE_LEVEL*PERIOD_VALUE / 10;
	
    TIM_InitStruct.Prescaler = prescalerValue;   //1K 
    TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
    TIM_InitStruct.Autoreload = PERIOD_VALUE;
    TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
    LL_TIM_Init(TIM2, &TIM_InitStruct);

    LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH1);

    TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
    TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_ENABLE;
    TIM_OC_InitStruct.CompareValue = Channel1Pulse;
    TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_LOW;
    LL_TIM_OC_Init(TIM2, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
    LL_TIM_OC_DisableFast(TIM2, LL_TIM_CHANNEL_CH1);
    LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
    LL_TIM_DisableMasterSlaveMode(TIM2);

    /**TIM2 GPIO Configuration
    PA0     ------> TIM2_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_HC595_OE;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
    LL_GPIO_Init(GPIO_PORT_HC595_OE, &GPIO_InitStruct);
		
		LL_TIM_EnableCounter(TIM2);
}
