/**
****************************************************************************************
*
* @file battery.c
*
* @brief Battery driver. Provides Battery level. Uses ADC module to get current voltage.
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
#include "battery.h"

#define BATTERY_MEASUREMENT_BOOST_AT_1V5 (0x340)
#define BATTERY_MEASUREMENT_BOOST_AT_1V0 (0x230)
#define BATTERY_MEASUREMENT_BOOST_AT_0V9 (0x1F0)
#define BATTERY_MEASUREMENT_BOOST_AT_0V8 (0x1B0)

#define BATTERY_MEASUREMENT_BUCK_AT_3V0 (0x6B0)
#define BATTERY_MEASUREMENT_BUCK_AT_2V8 (0x640)
#define BATTERY_MEASUREMENT_BUCK_AT_2V6 (0x5D0)
#define BATTERY_MEASUREMENT_BUCK_AT_2V4 (0x550)


/**
 ****************************************************************************************
 * ADC module Functions
 ****************************************************************************************
*/


/**
 ****************************************************************************************
 * @brief Calculates battery level percentage for CR2032 batteries
 *
 * @param[in] adc_sample  adc sample 
 *
 * @return Battery level. 0 - 100
 ****************************************************************************************
 */
uint8_t batt_cal_cr2032(uint16_t adc_sample)
{
    uint8_t batt_lvl;

    if (adc_sample > 1705)
		batt_lvl = 100;
	else if (adc_sample <= 1705 && adc_sample > 1584) 
		batt_lvl = 28 + (uint8_t)(( ( ((adc_sample - 1584) << 16) / (1705 - 1584) ) * 72 ) >> 16) ;
	else if (adc_sample <= 1584 && adc_sample > 1360) 
		batt_lvl = 4 + (uint8_t)(( ( ((adc_sample - 1360) << 16) / (1584 - 1360) ) * 24 ) >> 16) ;
	else if (adc_sample <= 1360 && adc_sample > 1136) 
		batt_lvl = (uint8_t)(( ( ((adc_sample - 1136) << 16) / (1360 - 1136) ) * 4 ) >> 16) ;
	else 
		batt_lvl = 0;
		
	return batt_lvl;	
}


/**
 ****************************************************************************************
 * @brief Calculates battery level percentage for CR2032 batteries
 *
 * @param[in] adc_sample  adc sample 
 *
 * @return Battery level. 0 - 100
 ****************************************************************************************
 */
uint8_t batt_cal_cr1225(uint16_t adc_sample)
{
    uint8_t batt_lvl;
    
    //1705=3.0V, 1137=2V
    if(adc_sample >= 1137)
        batt_lvl = (adc_sample - 1137)*100/568;
    else
        batt_lvl = 0;
    
    return batt_lvl;
}


/**
 ****************************************************************************************
 * @brief Calculates battery level percentage for a single AAA battery.
 *  
 *
 * @param[in] adc_sample  adc sample 
 *
 * @return Battery level. 0 - 100
 ****************************************************************************************
 */
uint8_t batt_cal_aaa(uint16_t adc_sample)
{
    uint8_t batt_lvl;
      
    if (GetBits16(ANA_STATUS_REG, BOOST_SELECTED) == 0x1)
    {
        //Boost mode  
        if(adc_sample >= BATTERY_MEASUREMENT_BOOST_AT_0V8)
            batt_lvl = (adc_sample - BATTERY_MEASUREMENT_BOOST_AT_0V8)*100/
                        (BATTERY_MEASUREMENT_BOOST_AT_1V5-BATTERY_MEASUREMENT_BOOST_AT_0V8);
        else
            batt_lvl = 0;
    }
    else
    {    
        //Buck mode
        if(adc_sample >= BATTERY_MEASUREMENT_BUCK_AT_2V4)
            batt_lvl = (adc_sample - BATTERY_MEASUREMENT_BUCK_AT_2V4)*100/
                        (BATTERY_MEASUREMENT_BUCK_AT_3V0-BATTERY_MEASUREMENT_BUCK_AT_2V4);
        else
            batt_lvl = 0;      
    }
    return batt_lvl;
}


/**
 ****************************************************************************************
 * @brief Reads current voltage from adc module and returns battery level. 
 *
 * @param[in] batt_type     Battery type. Supported types defined in battery.h
 *
 * @return Battery level. 0 - 100%
 ****************************************************************************************
 */
uint8_t battery_get_lvl(uint8_t batt_type)
{
	uint8_t batt_lvl;
	uint16_t adc_sample;
	volatile int i;

	if (batt_type == BATT_AAA)
	{
        if (GetBits16(ANA_STATUS_REG, BOOST_SELECTED) == 0x1)
        {
            // BOOST mode (single AAA battery)
            adc_sample = adc_get_vbat_sample(true);
        }
        else
        {
            // BUCK mode (2 x AAA batteries in series)
            adc_sample = adc_get_vbat_sample(false);
        }
	}
	else
		adc_sample = adc_get_vbat_sample(false);
	
    adc_sample >>= 4;
    adc_sample <<= 4;
      
    switch (batt_type)
    {
#if defined(USED_BATTERY_TYPE)
# if (USED_BATTERY_TYPE == BATT_CR2032)
        case BATT_CR2032:
            batt_lvl = batt_cal_cr2032(adc_sample);
            break;
# elif (USED_BATTERY_TYPE == BATT_CR1225)
        case BATT_CR1225:
            batt_lvl = batt_cal_cr1225(adc_sample);
            break;
# elif (USED_BATTERY_TYPE == BATT_AAA)
        case BATT_AAA:
            batt_lvl = batt_cal_aaa(adc_sample);
            break;
#else
       #error "Monitoring is not supported for the defined battery type. Please, verify USED_BATTERY_TYPE constant"
# endif
#else        
        case BATT_CR2032:
            batt_lvl = batt_cal_cr2032(adc_sample);
            break;
        case BATT_CR1225:
            batt_lvl = batt_cal_cr1225(adc_sample);
            break;
        case BATT_AAA:
            batt_lvl = batt_cal_aaa(adc_sample);
            break;
#endif
        default:
            batt_lvl = 0;
    }   
    
	return batt_lvl;
}
