/**
****************************************************************************************
*
* @file adc_driver.c
*
* @brief ADC module.
*
* Copyright (C) 2012. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
*
* <bluetooth.support@diasemi.com> and contributors.
*
****************************************************************************************
*/
#include "adc.h"

/**
 ****************************************************************************************
 * ADC module Functions
 ****************************************************************************************
*/
 

/**
 ****************************************************************************************
 * @brief Calculates battery level percentage for CR2032 batteries
 *
 * @param[in] mode  :0 = Differential mode, GP_ADC_SE(0x800) = Single ended mode
 * @param[in] sign  :0 = Default, GP_ADC_SIGN(0x400) = Conversion with opposite sign at input and output to cancel out the internal offset of the ADC and low-frequency
 *
 * @return void
 ****************************************************************************************
 */
void adc_init(uint16_t mode, uint16_t sign)
{ 
	SetWord16(GP_ADC_CTRL_REG,  GP_ADC_LDO_EN | mode | sign);
	SetWord16(GP_ADC_CTRL_REG,  GP_ADC_LDO_EN | mode | GP_ADC_EN | sign);
	SetWord16(GP_ADC_CTRL2_REG, 0x000B);                                    // Enable 3x attenuation
}

/**
 ****************************************************************************************
 * @brief Calculates battery level percentage for CR2032 batteries
 *
 * @param[in] input_selection Input channel. Defined in adc.h
 *
 * @return void
 ****************************************************************************************
 */
void adc_enable_channel(uint16_t input_selection)
{
	SetBits16(GP_ADC_CTRL_REG,GP_ADC_SEL,input_selection & 0xF);
}

/**
 ****************************************************************************************
 * @brief Disables ADC module.
 *
 * @return void
 ****************************************************************************************
 */ 

void adc_disable(void)
{
	SetWord16(GP_ADC_CTRL_REG,  0);
}

/**
 ****************************************************************************************
 * @brief Gets ADC sample.
 *
 * @return ADC sample
 ****************************************************************************************
 */
int adc_get_sample(void)
{
	int cnt = 100000;
    
	SetBits16(GP_ADC_CTRL_REG, GP_ADC_START, 1);
	while (cnt-- && (GetWord16(GP_ADC_CTRL_REG) & GP_ADC_START) != 0x0000); 
	SetWord16(GP_ADC_CLEAR_INT_REG, 0x0000); // Clear interrupt
	return GetWord16(GP_ADC_RESULT_REG); 
}


/**
 ****************************************************************************************
 * @brief Gets ADC sample from VBAT1V or VBAT3V power supplies.
 *
 * @param[in] sample_vbat1v :true = sample VBAT1V, false = sample VBAT3V
 *
 * @return ADC VBAT1V or VBAT3V sample
 ****************************************************************************************
 */
uint32_t adc_get_vbat_sample(bool sample_vbat1v)
{
    uint32_t adc_sample;
    
    adc_init(GP_ADC_SE, GP_ADC_SIGN);

    if (sample_vbat1v)
        adc_enable_channel(ADC_CHANNEL_VBAT1V);
    else
			adc_enable_channel(ADC_CHANNEL_P01);
  //      adc_enable_channel(ADC_CHANNEL_VBAT3V);
    adc_sample = adc_get_sample();

    adc_init(GP_ADC_SE, 0);

    if (sample_vbat1v)
        adc_enable_channel(ADC_CHANNEL_VBAT1V);
    else
			adc_enable_channel(ADC_CHANNEL_P01);

      //  adc_enable_channel(ADC_CHANNEL_VBAT3V);

    adc_sample += adc_get_sample();

    adc_disable();

    return adc_sample;
}

