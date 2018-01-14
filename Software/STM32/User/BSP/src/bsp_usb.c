/************************************************************
	*	@file:		XX.c
	*	@brief:		XX
	*	@author:	XIET
	*	@version:	V1.0
	*	@data:		201x-xx-xx
	*	@modify:	data  			remarks
**************************************************************/
#include "bsp.h"
USBD_HandleTypeDef USBD_Device;
/*============================================================
	*	@func:		XX
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
static void bsp_USB_Init(void)
{
	/* Init Device Library */
  USBD_Init(&USBD_Device, &HID_Desc, 0);
  
  /* Register the HID class */
  USBD_RegisterClass(&USBD_Device, &USBD_HID);
  
  /* Start Device Process */
  USBD_Start(&USBD_Device);
}
/*============================================================
  * @func:    USBClock_Enable
  * @brief:   ??USB??
  * @param:   NA
  * @retval:  NA  
  * @modify:  data      remarks 
=============================================================*/
static void USBClock_Enable(void)
{
	  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  static RCC_CRSInitTypeDef RCC_CRSInitStruct;

  /* Enable HSI48 Oscillator to be used as USB clock source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  /* Select HSI48 as USB clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  
  /*Configure the clock recovery system (CRS)**********************************/
  
  /*Enable CRS Clock*/
  __HAL_RCC_CRS_CLK_ENABLE(); 
  
  /* Default Synchro Signal division factor (not divided) */
  RCC_CRSInitStruct.Prescaler = RCC_CRS_SYNC_DIV1;  
  /* Set the SYNCSRC[1:0] bits according to CRS_Source value */
  RCC_CRSInitStruct.Source = RCC_CRS_SYNC_SOURCE_USB;  
  /* HSI48 is synchronized with USB SOF at 1KHz rate */
  RCC_CRSInitStruct.ReloadValue =  __HAL_RCC_CRS_RELOADVALUE_CALCULATE(48000000, 1000);
  RCC_CRSInitStruct.ErrorLimitValue = RCC_CRS_ERRORLIMIT_DEFAULT;  
  /* Set the TRIM[5:0] to the default value*/
  RCC_CRSInitStruct.HSI48CalibrationValue = 0x20;   
  /* Start automatic synchronization */ 
  HAL_RCCEx_CRSConfig (&RCC_CRSInitStruct);
}
/*============================================================
	*	@func:		XX
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
static void USBClock_Disable(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable HSI48 Oscillator to be used as USB clock source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_OFF;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /*Disable CRS Clock*/
  __HAL_RCC_CRS_CLK_DISABLE();
}
/*============================================================
	*	@func:		XX
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
void USB_Enable(void)
{
	USBClock_Enable();
	bsp_USB_Init();
}
/*============================================================
	*	@func:		XX
	*	@brief:		XX
	*	@param:		NA
	*	@retval:	NA	
	*	@modify: 	data 			remarks 
=============================================================*/
void USB_Disable(void)
{
    USBClock_Disable();
    USBD_DeInit(&USBD_Device);
}


