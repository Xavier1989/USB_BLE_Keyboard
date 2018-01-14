/**
 ****************************************************************************************
 *
 * @file arch_system.c
 *
 * @brief System setup.
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


/*
 * INCLUDES
 ****************************************************************************************
 */

#include "arch.h"
#include "arch_sleep.h"
#include <stdlib.h>
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include "rwip.h"     // BLE initialization
#include "llc.h"
#include "pll_vcocal_lut.h"
#include "gpio.h"
#include "rf_580.h"
#include "gtl_env.h"
#include "gtl_task.h"
#ifdef CFG_HCI_SPI
#include "spi_hci.h"
#ifdef CFG_HCI_UART
#include "periph_setup.h"
#endif
#endif

#include "hcic.h"

#if (USE_TRNG)
#include "trng.h"       // True random number generator API
#endif

/**
 * @addtogroup DRIVERS
 * @{
 */

// Delay sleep entrance at system startup 

uint32_t startup_sleep_delay = STARTUP_SLEEP_DELAY_DEFAULT;

#define LP_CLK_OTP_OFFSET 0x7f74     //OTP IQ_Trim offset

extern uint32_t last_temp_time;         // time of last temperature count measurement
extern uint16_t last_temp_count;        /// temperature counter

extern uint32_t lld_sleep_us_2_lpcycles_func(uint32_t us);
extern uint32_t lld_sleep_lpcycles_2_us_func(uint32_t lpcycles);

extern bool sys_startup_flag;

uint32_t lp_clk_sel __attribute__((section("retention_mem_area0"),zero_init));   //low power clock selection  
uint32_t rcx_freq __attribute__((section("retention_mem_area0"),zero_init));
uint8_t cal_enable  __attribute__((section("retention_mem_area0"),zero_init));
uint32_t rcx_period __attribute__((section("retention_mem_area0"),zero_init));
float rcx_slot_duration __attribute__((section("retention_mem_area0"),zero_init));;


//#define RCX_MEASURE
#ifdef RCX_MEASURE
uint32_t rcx_freq_min __attribute__((section("retention_mem_area0"),zero_init));
uint32_t rcx_freq_max __attribute__((section("retention_mem_area0"),zero_init));
uint32_t rcx_period_last __attribute__((section("retention_mem_area0"),zero_init));
uint32_t rcx_period_diff __attribute__((section("retention_mem_area0"),zero_init));
#endif

/*
 * EXPORTED FUNCTION DEFINITIONS
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @brief Read low power clock selection from 
 *
 * The Hclk and Pclk are set
 ****************************************************************************************
*/
void select_lp_clk()
{
#if (CFG_LP_CLK == LP_CLK_FROM_OTP)
    int cnt = 100000;

# ifndef APP_BOOT_FROM_OTP    
#define XPMC_MODE_MREAD   0x1
    uint16_t *lp_clk = (uint16_t *)(0x40000 + LP_CLK_OTP_OFFSET);     //where in OTP header is IQ_Trim
    
    SetBits16(CLK_AMBA_REG, OTP_ENABLE, 1);                           //enable OTP clock	
    while ((GetWord16(ANA_STATUS_REG) & LDO_OTP_OK) != LDO_OTP_OK && cnt--)
        /* Just wait */;
        
    // set OTP in read mode 
    SetWord32(OTPC_MODE_REG, XPMC_MODE_MREAD);
# else
    uint16_t *lp_clk = (uint16_t *)(0x20000000 + LP_CLK_OTP_OFFSET);  //where in OTP header is IQ_Trim
# endif //APP_BOOT_FROM_OTP   

    lp_clk_sel  = (*lp_clk);
    SetBits16(CLK_AMBA_REG, OTP_ENABLE, 0);                           //disable OTP clock     
    
# ifdef SUPPORT_1_8_V
    SetBits16(DCDC_CTRL2_REG, DCDC_VBAT3V_LEV, 0x0);                  ///--Support 1.8V boot
# endif

#else //CFG_LP_CLK 
    lp_clk_sel = CFG_LP_CLK; 
#endif    
}


/**
 ****************************************************************************************
 * @brief Initialisation of ble core, pwr and clk
 *
 * The Hclk and Pclk are set
 ****************************************************************************************
 */
void init_pwr_and_clk_ble(void) 
{
    SetBits16(CLK_RADIO_REG, BLE_DIV, 0);
    SetBits16(CLK_RADIO_REG, BLE_ENABLE, 1); 
    SetBits16(CLK_RADIO_REG, RFCU_DIV, 1);
    SetBits16(CLK_RADIO_REG, RFCU_ENABLE, 1);

    /* 
     * Power up BLE core & reset BLE Timers
    */
    SetBits16(CLK_32K_REG,  RC32K_ENABLE, 1);  
    SetBits16(SYS_CTRL_REG, CLK32_SOURCE, 0);  
    SetBits16(CLK_RADIO_REG, BLE_LP_RESET, 1);  
    SetBits16(PMU_CTRL_REG, RADIO_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & RAD_IS_UP)); // Just wait for radio to truely wake up
                                
    select_lp_clk();

    if ( ((lp_clk_sel == LP_CLK_XTAL32) && (CFG_LP_CLK == LP_CLK_FROM_OTP)) || (CFG_LP_CLK == LP_CLK_XTAL32) )
    {
        SetBits16(CLK_32K_REG, XTAL32K_ENABLE, 1);  // Enable XTAL32KHz 

        // Disable XTAL32 amplitude regulation in BOOST mode
        if (GetBits16(ANA_STATUS_REG, BOOST_SELECTED) == 0x1) 
            SetBits16(CLK_32K_REG,  XTAL32K_DISABLE_AMPREG, 1);  
        else
            SetBits16(CLK_32K_REG,  XTAL32K_DISABLE_AMPREG, 0);  
        SetBits16(CLK_32K_REG,  XTAL32K_CUR, 5);
        SetBits16(CLK_32K_REG,  XTAL32K_RBIAS, 3);
        SetBits16(SYS_CTRL_REG, CLK32_SOURCE, 1);  // Select XTAL32K as LP clock
        
     }
    else if ( ((lp_clk_sel == LP_CLK_RCX20) && (CFG_LP_CLK == LP_CLK_FROM_OTP)) || (CFG_LP_CLK == LP_CLK_RCX20) )
    {
        SetBits16(CLK_RCX20K_REG, RCX20K_NTC, 0xB);      
        SetBits16(CLK_RCX20K_REG, RCX20K_BIAS, 1);
        SetBits16(CLK_RCX20K_REG, RCX20K_TRIM, 0);
        SetBits16(CLK_RCX20K_REG, RCX20K_LOWF, 1);
                           
        SetBits16(CLK_RCX20K_REG, RCX20K_ENABLE, 1);
    
        SetBits16(CLK_RCX20K_REG, RCX20K_SELECT, 1);
    
        SetBits16(SYS_CTRL_REG, CLK32_SOURCE, 0);                                               

        SetBits16(CLK_32K_REG, XTAL32K_ENABLE, 0); // Disable Xtal32KHz
    }
    else
        ASSERT_WARNING(0);
    
    SetBits16(CLK_32K_REG,  RC32K_ENABLE, 0);   // Disable RC32KHz
                                
    SetBits16(CLK_RADIO_REG, BLE_LP_RESET, 0);
    
    if (GetBits16(ANA_STATUS_REG, BOOST_SELECTED) == 0x1) 
        SetWord16(DCDC_CTRL3_REG, 0x5);
    
    /* 
     * Just make sure that BLE core is stopped (if already running)
     */
    SetBits32(BLE_RWBTLECNTL_REG, RWBLE_EN, 0); 
 
    /* 
     * Since BLE is stopped (and powered), set CLK_SEL
     */    
    SetBits32(BLE_CNTL2_REG, BLE_CLK_SEL, 16);
    SetBits32(BLE_CNTL2_REG, BLE_RSSI_SEL, 1);    

    /* 
     * Set spi interface to software
     */   
#ifdef FPGA_USED     
    // the following 2 lines are for FPGA implementation
    SetBits32(BLE_CNTL2_REG, SW_RPL_SPI, 0);
    SetBits32(BLE_CNTL2_REG, BB_ONLY, 1);     
#endif    
}


/**
 ****************************************************************************************
 * @brief Sets AMBA clocks to minimum frequency
 *
 * @return void 
 ****************************************************************************************
 */
void use_lowest_amba_clocks(void)
{
    SetBits16(CLK_AMBA_REG, PCLK_DIV, 3);   // lowest is 250KHz (div 8, source is @2MHz)
    SetBits16(CLK_AMBA_REG, HCLK_DIV, 3);   // lowest is 2MHz (div 8, source is @16MHz)
}



/**
 ****************************************************************************************
 * @brief Sets AMBA clocks to maximum frequency
 *
 * @return void 
 ****************************************************************************************
 */
void use_highest_amba_clocks(void)
{
    SetBits16(CLK_AMBA_REG, PCLK_DIV, 0);
    SetBits16(CLK_AMBA_REG, HCLK_DIV, 0);
}



/**
 ****************************************************************************************
 * @brief Creates sw cursor in power profiler tool. Used for Development/ Debugging 
 *
 * @return void 
 ****************************************************************************************
 */
void set_pxact_gpio(void)
{
#if DEVELOPMENT_DEBUG    
    uint32_t i;

    SetWord16(P13_MODE_REG, PID_GPIO|OUTPUT);
    SetWord16(P1_SET_DATA_REG, 0x8);
    for (i = 0; i < 150; i++); //20 is almost 7.6usec of time.
    SetWord16(P1_RESET_DATA_REG, 0x8);
#endif
    
    return;
}


/**
 ****************************************************************************************
 * @brief Starts RCX20 calibration. 
 *
 * @param[in]   cal_time. Calibration time in RCX20 cycles. 
 *
 * @return void 
 ****************************************************************************************
 */
void calibrate_rcx20(uint16_t cal_time)
{
    if ((CFG_LP_CLK == LP_CLK_FROM_OTP) || (CFG_LP_CLK == LP_CLK_RCX20))
    {
        SetWord16(CLK_REF_CNT_REG, cal_time);
        SetBits16(CLK_REF_SEL_REG, REF_CLK_SEL, 0x3); //RCX select 
        SetBits16(CLK_REF_SEL_REG, REF_CAL_START, 0x1); //Start Calibration
        cal_enable = 1;
    }
}


/**
 ****************************************************************************************
 * @brief Calculates RCX20 frequency. 
 *
 * @param[in]   cal_time. Calibration time in RCX20 cycles. 
 *
 * @return void 
 ****************************************************************************************
 */
void read_rcx_freq(uint16_t cal_time)
{
    if ( (cal_enable) && ((CFG_LP_CLK == LP_CLK_FROM_OTP) || (CFG_LP_CLK == LP_CLK_RCX20)) )
    {
        while(GetBits16(CLK_REF_SEL_REG, REF_CAL_START) == 1);
        volatile uint32_t high = GetWord16(CLK_REF_VAL_H_REG);
        volatile uint32_t low = GetWord16(CLK_REF_VAL_L_REG);
        volatile uint32_t value = ( high << 16 ) + low;
        volatile uint32_t f = (16000000 * cal_time) / value;

        cal_enable = 0;

        rcx_freq = f;
        rcx_period = ((float) 1000000/f) * 1024;
        rcx_slot_duration = 0.000625 * (float)rcx_freq;
        
#ifdef RCX_MEASURE
        if (rcx_period_last)
        {
            volatile int diff = rcx_period_last - rcx_period;
            if (abs(diff) > rcx_period_diff)
                rcx_period_diff = abs(diff);
        }
        rcx_period_last = rcx_period;
        
        if (rcx_freq_min == 0)
        {
            rcx_freq_min = rcx_freq;
            rcx_freq_max = rcx_freq;
        }
        if (rcx_freq < rcx_freq_min)
            rcx_freq_min = rcx_freq;
        else if (rcx_freq > rcx_freq_max)
            rcx_freq_max = rcx_freq;
#endif    
    }
}


/**
 ****************************************************************************************
 * @brief Sets the BLE wake-up delay.  
 *
 * @return void 
 ****************************************************************************************
 */
void set_sleep_delay(void)
{
    int16_t delay;
    
#ifndef USE_POWER_OPTIMIZATIONS
    uint32_t sleep_delay;
    
    delay = 0;
    
    if ( ((lp_clk_sel == LP_CLK_RCX20) && (CFG_LP_CLK == LP_CLK_FROM_OTP)) || (CFG_LP_CLK == LP_CLK_RCX20) )
    {
        if (rcx_period > (RCX_PERIOD_MAX << 10) )
            ASSERT_ERROR(0);
        
        sleep_delay = SLP_PROC_TIME + SLEEP_PROC_TIME + (4 * RCX_PERIOD_MAX); // 400
//#ifdef USE_POWER_OPTIMIZATIONS
//        delay += RCX_CORRECTION_TIME * RCX_PERIOD_MAX;
//#endif        
    }
    else // if (lp_clk_sel == LP_CLK_XTAL32)
    {
        sleep_delay = /*SLP_PROC_TIME + SLEEP_PROC_TIME + */(4 * XTAL32_PERIOD_MAX); // ~200
    }
    
    // Actual "delay" is application specific and is the execution time of the BLE_WAKEUP_LP_Handler(), which depends on XTAL trimming delay.
    // In case of OTP copy, this is done while the XTAL is settling. Time unit of delay is usec.
    delay += XTAL_TRIMMING_TIME_USEC;
    // Icrease time taking into account the time from the setting of DEEP_SLEEP_ON until the assertion of DEEP_SLEEP_STAT.   
    delay += sleep_delay;
    // Add any application specific delay
    delay += APP_SLEEP_DELAY_OFFSET;
    
#else // USE_POWER_OPTIMIZATIONS
    delay = MINIMUM_SLEEP_DURATION;

    // if XTAL_TRIMMING_TIME_USEC changes (i.e. gets larger) then this
    // will make sure that the user gets notified to increase "delay" by 1 or more
    // slots so there's enough time for XTAL to settle
# if ( (3125 + (MINIMUM_SLEEP_DURATION + 625)) < (LP_ISR_TIME_USEC + 1100)) //1.1ms max power-up time
# error "Minimum sleep duration is too small for the 16MHz crystal used..."
# endif
#endif // USE_POWER_OPTIMIZATIONS
    
    rwip_wakeup_delay_set(delay);
}


/**
 ****************************************************************************************
 * @brief get_rc16m_count_func_patched(). (Patch.
 *
 * @return void 
 ****************************************************************************************
 */
#ifdef __GNUC__
extern uint16_t __real_get_rc16m_count_func(void);

uint16_t __wrap_get_rc16m_count_func(void)
{
    uint16_t count;
    uint16_t trim = GetBits16(CLK_16M_REG, RC16M_TRIM);
    
    count = __real_get_rc16m_count_func();

    SetBits16(CLK_16M_REG, RC16M_TRIM, trim);
    
    return count;
}

#else

extern uint16_t $Super$$get_rc16m_count_func(void);

uint16_t $Sub$$get_rc16m_count_func(void)
{
    uint16_t count;
    uint16_t trim = GetBits16(CLK_16M_REG, RC16M_TRIM);
    
    count = $Super$$get_rc16m_count_func();

    SetBits16(CLK_16M_REG, RC16M_TRIM, trim);
    
    return count;
}
#endif //__GNUC__


/**
 ****************************************************************************************
 * @brief conditionally_run_radio_cals(). Runs conditionally (time + temperature) RF and coarse calibration.                                                                                                                                                                                                                                                                                 ltime and temp changes) RF and coarse calibration  
 *
 * @return void 
 ****************************************************************************************
 */
void conditionally_run_radio_cals(void) 
{
    uint16_t count, count_diff;
    uint8_t force_rf_cal = 0;
    bool rf_cal_stat;
    
    uint32_t current_time = lld_evt_time_get();    
    
    if (current_time < last_temp_time)
    { 
        last_temp_time = 0;
    }

    if (force_rf_cal)
    {
        force_rf_cal = 0;
        
        last_temp_time = current_time;
        last_temp_count = get_rc16m_count();
#if LUT_PATCH_ENABLED
        pll_vcocal_LUT_InitUpdate(LUT_UPDATE);    //Update pll look up table
#endif          
        rf_cal_stat = rf_calibration();
        if ( rf_cal_stat == false)
            force_rf_cal = 1;        

        return;
    }
    
    if ( (current_time - last_temp_time) >= 3200) //2 sec
    {    
        last_temp_time = current_time;
        count = get_rc16m_count();                  // Estimate the RC16M frequency
        
        if (count > last_temp_count)
            count_diff = count - last_temp_count;
        else
            count_diff = last_temp_count - count ;
        
        if (count_diff >= 24)// If corresponds to 5 C degrees difference
        { 

            // Update the value of last_count
            last_temp_count = count;
#if LUT_PATCH_ENABLED
             pll_vcocal_LUT_InitUpdate(LUT_UPDATE);    //Update pll look up table
#endif            
            rf_cal_stat = rf_calibration();
            
            if ( rf_cal_stat == false)
                force_rf_cal = 1;         // Perform the readio calibrations

        }
    }
}


/**
 ****************************************************************************************
 * @brief       Converts us to low power cycles for RCX20 clock.  
 *
 * @param[in]   us. microseconds
 *
 * @return      uint32. Low power cycles       
 ****************************************************************************************
 */
uint32_t lld_sleep_us_2_lpcycles_rcx_func(uint32_t us)
{
    volatile uint32_t lpcycles;

    //RCX
    //Compute the low power clock cycles - case of a 16.342kHz clock
    if ((CFG_LP_CLK == LP_CLK_FROM_OTP) || (CFG_LP_CLK == LP_CLK_RCX20))
        lpcycles = (us * rcx_freq) / 1000000;
    
    return(lpcycles);
}


/**
 ****************************************************************************************
 * @brief       Converts low power cycles to us for RCX20 clock.  
 *
 * @param[in]   lpcycles. Low power cycles 
 *
 * @return      uint32. microseconds
 ****************************************************************************************
 */
uint32_t lld_sleep_lpcycles_2_us_rcx_func(uint32_t lpcycles)
{
    volatile uint32_t us;

    // Sanity check: The number of lp cycles should not be too high to avoid overflow
    ASSERT_ERR(lpcycles < 1000000);

    // Compute the sleep duration in us - case of a 16.342kHz clock (61.19202)
    if ((CFG_LP_CLK == LP_CLK_FROM_OTP) || (CFG_LP_CLK == LP_CLK_RCX20))
        us = (lpcycles * rcx_period) >> 10;
    
    return(us);
}    


/**
 ****************************************************************************************
 * @brief       Selects convertion function (XTAL32 or RCX20) for us to low power cycles.  
 *
 * @param[in]   us. microseconds
 *
 * @return      uint32. Low power cycles       
 ****************************************************************************************
 */
uint32_t lld_sleep_us_2_lpcycles_sel_func(uint32_t us)
{
    volatile uint32_t lpcycles;

    if ( ((lp_clk_sel == LP_CLK_XTAL32) && (CFG_LP_CLK == LP_CLK_FROM_OTP)) || (CFG_LP_CLK == LP_CLK_XTAL32) )
        lpcycles = lld_sleep_us_2_lpcycles_func(us);
    else if ( ((lp_clk_sel == LP_CLK_RCX20) && (CFG_LP_CLK == LP_CLK_FROM_OTP)) || (CFG_LP_CLK == LP_CLK_RCX20) )
        lpcycles = lld_sleep_us_2_lpcycles_rcx_func(us);

    return(lpcycles);        
}


/**
 ****************************************************************************************
 * @brief       Selects convertion function (XTAL32 or RCX20) for low power cycles to us.  
 *
 * @param[in]   lpcycles. Low power cycles 
 *
 * @return      uint32. microseconds
 ****************************************************************************************
 */
uint32_t lld_sleep_lpcycles_2_us_sel_func(uint32_t lpcycles)
{
    volatile uint32_t us;
    if ( ((lp_clk_sel == LP_CLK_XTAL32) && (CFG_LP_CLK == LP_CLK_FROM_OTP)) || (CFG_LP_CLK == LP_CLK_XTAL32) )
        us = lld_sleep_lpcycles_2_us_func(lpcycles);
    else  if ( ((lp_clk_sel == LP_CLK_RCX20) && (CFG_LP_CLK == LP_CLK_FROM_OTP)) || (CFG_LP_CLK == LP_CLK_RCX20) )
        us = lld_sleep_lpcycles_2_us_rcx_func(lpcycles);

    return(us);        
}


#if (!BLE_APP_PRESENT)
/**
 ****************************************************************************************
 * @brief Check gtl state.
 *
 * @return bool  If gtl is idle returns true. Otherwise return false.
 ****************************************************************************************
 */
bool check_gtl_state(void)
{
    uint8_t ret = true;

#if BLE_HOST_PRESENT	
    if ((ke_state_get(TASK_GTL) != GTL_TX_IDLE) ||
            ((gtl_env.rx_state != GTL_STATE_RX_START) &&
            (gtl_env.rx_state != GTL_STATE_RX_OUT_OF_SYNC)) )
            ret = false;
#else
    if ((ke_state_get(TASK_HCI) != HCI_STATE_TX_IDLE) ||
            ((hci_env.rx_state != HCI_STATE_RX_START) &&
            (hci_env.rx_state != HCI_STATE_RX_OUT_OF_SYNC)) )
            ret = false;

#endif
    return ret;
}
#endif


#ifndef __DA14581__
/*
 ****************************************************************************************
 * Replacement of ROM function uart_flow_off_func
 ****************************************************************************************
 */

#include "uart.h"
#include "reg_uart.h"   // uart register

static bool uart_is_rx_fifo_empty(void)
{
    return (uart_data_rdy_getf() == 0);
}

bool uart_flow_off_func(void)
{
    bool flow_off = true;
    volatile unsigned int i;

    do
    {
        // First check if no transmission is ongoing
        if ((uart_temt_getf() == 0) || (uart_thre_getf() == 0))
        {
            flow_off = false;
            break;
        }

        // Configure modem (HW flow control disable, 'RTS flow off')
#if 0
        uart_mcr_pack(UART_ENABLE,     // extfunc
                      UART_DISABLE,    // autorts
                      UART_ENABLE,     // autocts
                      UART_DISABLE);   // rts
#endif
        SetWord32(UART_MCR_REG, 0);

        // Wait for 1 character duration to ensure host has not started a transmission at the
        // same time
        for (i = 0; i < 350; i++);

        #ifndef __GNUC__
//        timer_wait(UART_CHAR_DURATION);
        #endif //__GNUC__
        // Check if data has been received during wait time
        if(!uart_is_rx_fifo_empty())
        {
            // Re-enable UART flow
            uart_flow_on();

            // We failed stopping the flow
            flow_off = false;
        }
    } while(false);

    return (flow_off);
}
#endif


#if ((EXTERNAL_WAKEUP) && (!BLE_APP_PRESENT)) // only in full embedded designs
/**
 ****************************************************************************************
 * @brief Enable external wake up GPIO Interrupt. 
 * @param[in] GPIO_PORT. The GPIO port of the external wake up signal
 * @param[in] GPIO_PIN. The GPIO pin of the external wake up signal
 * @param[in] polarity. The polarity of the external wake up interrupt. 0=active low. 1=active high
 * @return void.
 ****************************************************************************************
 */
void ext_wakeup_enable(uint32_t port, uint32_t pin, uint8_t polarity)
{
	rwip_env.ext_wakeup_enable = 2;
#if DEVELOPMENT_DEBUG
	RESERVE_GPIO( EXT_WAKEUP_GPIO, (GPIO_PORT) port, (GPIO_PIN) pin, PID_GPIO );
#endif
	if ( polarity == 0 ) // active low
		GPIO_ConfigurePin((GPIO_PORT) port, (GPIO_PIN) pin, INPUT_PULLUP, PID_GPIO, false ); // active low. Set default to input high.
	else // active high
		GPIO_ConfigurePin( (GPIO_PORT) port, (GPIO_PIN) pin, INPUT_PULLDOWN, PID_GPIO, false ); // active high. Set default to input low.
	
	wkupct_register_callback(ext_wakeup_cb);

	wkupct_enable_irq(1<<pin, (uint32_t)(polarity == 0)?(1<<pin):(0), 1, 0); // pin, active low/high, 1 event, debouncing time = 0ms
}


/**
 ****************************************************************************************
 * @brief Disable external wake up GPIO Interrupt. Used on self wakup.
 *
 * @return void.
 ****************************************************************************************
 */
void ext_wakeup_disable(void)
{
	SetWord16(WKUP_RESET_IRQ_REG, 1); // Acknowledge it
	SetBits16(WKUP_CTRL_REG, WKUP_ENABLE_IRQ, 0); // No more interrupts of this kind
}


/**
 ****************************************************************************************
 * @brief Callback function, called when external wakeup function is triggered.
 *
 * @return void.
 ****************************************************************************************
 */
void ext_wakeup_cb(void)
{ 	
	if (GetBits16(SYS_STAT_REG, PER_IS_DOWN)) {
		// Return GPIO functionality from external wakeup GPIO
#if DEVELOPMENT_DEBUG
		GPIO_reservations();
#endif		
		set_pad_functions();
  } 
	
	SetBits32(GP_CONTROL_REG, BLE_WAKEUP_REQ, 1);
}
#endif // ((EXTERNAL_WAKEUP) && (!BLE_APP_PRESENT))



#if (!BLE_HOST_PRESENT)
 
#ifdef CFG_HCI_UART
// Creation of uart external interface api
extern const struct rwip_eif_api uart_api;
#endif

#ifdef CFG_HCI_SPI
// Creation of SPI external interface api
const struct rwip_eif_api spi_api =
{
    spi_hci_read_func,
    spi_hci_write_func,
    spi_hci_flow_on_func,
    spi_hci_flow_off_func,
};
#endif

const struct rwip_eif_api* rwip_eif_get_func(uint8_t type)
{
    const struct rwip_eif_api* ret = NULL;
    switch(type)
    {
        case RWIP_EIF_AHI:
        case RWIP_EIF_HCIC:
        {
#ifdef CFG_HCI_BOTH_EIF
            if (GPIO_GetPinStatus(HCI_EIF_SELECT_PORT,HCI_EIF_SELECT_PIN))
                ret = &uart_api;
            else
                ret = &spi_api;
#else
  #ifdef CFG_HCI_SPI
            ret = &spi_api;     // select spi api
  #else
            ret = &uart_api;    // select uart api
  #endif
#endif
        }
        break;
        default:
        {
            ASSERT_ERR(0);
        }
        break;
    }
    return ret;
}
#endif // (!BLE_HOST_PRESENT)


/**
 ****************************************************************************************
 * @brief Calculates optimal ADV_IND interval in uSec
 * @param[in] Advertise data length. Max value = 28
 * @param[in] Scan Response. Max value = 31
 * @return uint32   optimal ADV_IND interval in uSec
 ****************************************************************************************
 */
uint32_t calculate_arch_adv_time (uint8_t adv_len, uint8_t scan_rsp_len)
{
    uint32_t arch_adv_int;
    
    arch_adv_int = (ARCH_ADV_MIN_DUR + adv_len*8) +  (ARCH_SCAN_RSP_MIN_DUR + scan_rsp_len*8) + ARCH_SCAN_REQ_DUR + (3 * ARCH_INT_FRAME_TIME);
    
    return arch_adv_int;
}


#if (USE_TRNG)
/**
 ****************************************************************************************
 * @brief Initialise random number generator seed using random bits acquired from TRNG
 ****************************************************************************************
 */
void init_rand_seed_from_trng(void)
{
    uint8_t trng_bits[16];
    uint32_t seed = 0;

    trng_acquire(trng_bits);
    seed = *( (uint32_t *) trng_bits );
    srand(seed);
}
#endif // (USE_TRNG)


/**
 ****************************************************************************************
 * @brief Checks if system is in startup phase.

 * @return uint8   1 if system is in startup phase, otherwise 0.
 ****************************************************************************************
 */
uint8_t check_sys_startup_period(void)
{
   uint8_t ret_value = 0;  
    
    if (sys_startup_flag)
    {
        uint32_t current_time;
        
        current_time = lld_evt_time_get();
                
        if (current_time < startup_sleep_delay) // startup_sleep_delay after system startup to allow system to sleep
            ret_value = 1;
        else // After 2 seconds system can sleep
        {
            sys_startup_flag = false;
            if ( (app_get_sleep_mode() == 2) || (app_get_sleep_mode() == 1) )
            {               
                 SetWord16(SET_FREEZE_REG, FRZ_WDOG);            // Stop WDOG until debugger is removed
                 while ((GetWord16(SYS_STAT_REG) & DBG_IS_UP) == DBG_IS_UP) {}; 
                 SetBits16(SYS_CTRL_REG, DEBUGGER_ENABLE, 0);    // close debugger
            }

#if (USE_WDOG)
            SetWord16(RESET_FREEZE_REG, FRZ_WDOG);  // Start WDOG
#endif
         
            ret_value = 0;
        }
    }

    return ret_value;
}


/**
 ****************************************************************************************
 * @brief Checks if system clocks (AHB & APB) can be lowered (weak definition).

 * @return bool   true if system clocks can be lowered, otherwise false.
 ****************************************************************************************
 */
__weak bool app_use_lower_clocks_check(void)
{
    return false;
}


/// @} 
