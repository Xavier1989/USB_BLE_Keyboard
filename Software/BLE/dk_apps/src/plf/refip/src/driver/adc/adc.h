/**
****************************************************************************************
*
* @file adc_driver.h
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
 #ifndef _ADC_
#define _ADC_

#include "global_io.h"
#include <stdint.h>
#include <stdbool.h>


/* ADC channels */
#define ADC_CHANNEL_P00         0
#define ADC_CHANNEL_P01         1
#define ADC_CHANNEL_P02         2
#define ADC_CHANNEL_P03         3
#define ADC_CHANNEL_AVS         4
#define ADC_CHANNEL_VDD_REF     5
#define ADC_CHANNEL_VDD_RTT     6
#define ADC_CHANNEL_VBAT3V      7
#define ADC_CHANNEL_VDCDC       8
#define ADC_CHANNEL_VBAT1V      9

#define ADC_POLARITY_SIGNED   0x400
#define ADC_POLARITY_UNSIGNED   0x0

/**
 ****************************************************************************************
 * @brief ADC module initialization
 *
 ****************************************************************************************
 */

void adc_init(uint16_t mode, uint16_t sign);

/**
 ****************************************************************************************
 * @brief Enable selected channel 
 *
 ****************************************************************************************
 */

void adc_enable_channel(uint16_t input_selection);

/**
 ****************************************************************************************
 * @brief Disable ADC Module
 *
 ****************************************************************************************
 */
 
void adc_disable(void);

/**
 ****************************************************************************************
 * @brief Get ADC sample
 *
 ****************************************************************************************
 */
 
int adc_get_sample(void);

/**
 ****************************************************************************************
 * @brief Gets ADC sample from VBAT1V or VBAT3V power supplies.
 *
 ****************************************************************************************
 */
uint32_t adc_get_vbat_sample(bool sample_vbat1v);

#endif

