/************************************************************
  * @file:    XX.c
  * @brief:   XX
  * @author:  XIET
  * @version: V1.0
  * @data:    201x-xx-xx
  * @modify:  data        remarks
**************************************************************/
#include "bsp.h"
static void SystemPower_Config(void);
extern  void SystemClock_Config(void);
/*============================================================
  * @func:    XX
  * @brief:   XX
  * @param:   NA
  * @retval:  NA
  * @modify:  data      remarks
=============================================================*/
void System_Enter_StopMode(void)
{
    USB_Disable();
    SystemPower_Config();
    GPIO_Config_EXTI();
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
    SystemClock_Config();
    USB_Enable();
    bsp_Init();
    GPIO_BLE_Power(ENABLE);
    GPIO_LED_Power(ENABLE);
    Display_Init();
    Key_Scan_Init();
}
/*============================================================
  *  @func:    XX
  *  @brief:    XX
  *  @param:    NA
  *  @retval:  NA
  *  @modify:   data       remarks
=============================================================*/
static void SystemPower_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable Ultra low power mode */
    HAL_PWREx_EnableUltraLowPower();

    /* Enable the fast wake up from Ultra low power mode */
    HAL_PWREx_EnableFastWakeUp();

    /* Select HSI as system clock source after Wake Up from Stop mode */
    __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_HSI);

    /* Enable GPIOs clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
    GPIO_InitStructure.Pin = GPIO_PIN_All;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
    HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);

    /* Disable GPIOs clock */
    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOD_CLK_DISABLE();
    __HAL_RCC_GPIOH_CLK_DISABLE();
}
/*============================================================
  *  @func:    XX
  *  @brief:    XX
  *  @param:    NA
  *  @retval:  NA  
  *  @modify:   data       remarks 
=============================================================*/
void Pwr_Sleep_Check(void)
{
  if(bsp_CheckTimer(TMR_SLEEP))
  {
    g_Work_Mode = SLEEP_MODE;
  }
}
