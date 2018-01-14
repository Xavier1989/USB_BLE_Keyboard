/**
 ****************************************************************************************
 *
 * @file wucpt_quadec.c
 *
 * @brief Wakeup IRQ & Quadrature Decoder driver.
 *
 * Copyright (C) 2013. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */
 
 /* Important note: WKUP_ENABLED and QUADEC_ENABLED control the inclusion of the parts of the
 *                  code that add support for to wakeup timer and quadrature decoder respectively.
 */ 
 
 /* Important note: If, upon reception of interrupt from the wakeup timer or the quadrature
 *                  decoder, the system resumes from sleep mode and you wish to resume peripherals
 *                  functionality , it is necessary to include in your interrupt handler function(s)
 *                  - the ones you register using wkupct_register_callback() and/or 
 *                  quad_decoder_register_callback() - the following lines:
 *
 *                  // Init System Power Domain blocks: GPIO, WD Timer, Sys Timer, etc.
 *                  // Power up and init Peripheral Power Domain blocks,
 *                  // and finally release the pad latches.
 *                  if(GetBits16(SYS_STAT_REG, PER_IS_DOWN))
 *                      periph_init();
 *                        
 */         
                

#include "global_io.h"
#include "ARMCM0.h"

#include "wkupct_quadec.h"

wkupct_quad_IRQ_status_t wkupct_quad_IRQ_status __attribute__((section("retention_mem_area0"), zero_init));

/**
 ****************************************************************************************
 * @brief internal function to disable WKUP_QUADEC_IRQn only when both Quadrature
 * @brief Decoder and Wakeup Timer have unregistered from using it
 *
 * @return void WKUPCT_QUADEC_ERR_RESERVED, WKUPCT_QUADEC_ERR_OK
 ****************************************************************************************
 */ 
wkupct_quadec_error_t wkupct_quad_disable_IRQ(void)
{
    if (wkupct_quad_IRQ_status.reservation_status == RESERVATION_STATUS_FREE)
    {
        NVIC_DisableIRQ(WKUP_QUADEC_IRQn);     
        return WKUPCT_QUADEC_ERR_OK;
    }
    return WKUPCT_QUADEC_ERR_RESERVED;
}


#ifdef WKUP_ENABLED    
void* WKUPCT_callback       __attribute__((section("retention_mem_area0"), zero_init)); // Wakeup handler callback
#endif //WKUP_ENABLED 

#ifdef QUADEC_ENABLED    
uint32_t* QUADDEC_callback  __attribute__((section("retention_mem_area0"), zero_init)); // Quadrature decoder handler callback
#endif //QUADEC_ENABLED


/**
 * WUPCT_Handler:
 */
 
/**
 ****************************************************************************************
 * @brief WKUPCT IRQ Handler
 *
 * @return void
 ****************************************************************************************
 */ 
void WKUP_QUADEC_Handler(void)
{
#ifdef WKUP_ENABLED    
	wakeup_handler_function_t wakeupHandlerFunction;   
#endif //WKUP_ENABLED 
    
#ifdef QUADEC_ENABLED    
    quad_encoder_handler_function_t quadEncoderHandlerFunction;   
#endif //QUADEC_ENABLED
    
#if defined(WKUP_ENABLED) || defined(QUADEC_ENABLED)
    uint8_t source = 0;
#endif //defined(WKUP_ENABLED) || defined(QUADEC_ENABLED)
    
	/*
	 * The system operates with RC16 clk
	 */
    
	/*
	 * Restore clock 
	 */
	use_highest_amba_clocks();
    
#ifdef QUADEC_ENABLED
    if ((GetBits16(CLK_PER_REG, QUAD_ENABLE) != 0) && (GetBits16(QDEC_CTRL_REG, QD_IRQ_STATUS) !=0 ))
    { // Quadrature Decoder clock is enabled & Quadrature Decoder interrupt has triggered
        source = SRC_QUAD_IRQ;
        SetBits16(QDEC_CTRL_REG, QD_IRQ_CLR, 1);  // write 1 to clear Quadrature Decoder interrupt
        SetBits16(QDEC_CTRL_REG, QD_IRQ_MASK, 0); // write 0 to mask the Quadrature Decoder interrupt
    }
    else 
#endif // QUADEC_ENABLED
    {
#ifdef WKUP_ENABLED
        if  ((GetBits16(CLK_PER_REG, WAKEUPCT_ENABLE) != 0))
        { // since the interrupt does not come from the Quadrature controller, it is from the wakeup timer
            source = SRC_WKUP_IRQ;
            SetWord16(WKUP_RESET_IRQ_REG, 1); // Acknowledge it
            SetBits16(WKUP_CTRL_REG, WKUP_ENABLE_IRQ, 0); // No more interrupts of this kind
        }
#endif // WKUP_ENABLED
    }
    /* Note: in case of simultaneous triggering of quadrature decoder and wakeup timer, quadrature decoder
             interrupt will be handled. */ 

	
	///To check SetBits16(CLK_PER_REG, WAKEUPCT_ENABLE, 1);  // enable clock of Wakeup Controller

	//wkupct_quad_disable_IRQ(); // Disable WKUP_QUADEC_IRQn irq (only if both Quadrature Decoder and Wakeup Timer have unregistered)
	
	//NVIC_DisableIRQ(WKUP_QUADEC_IRQn);

	/*
	* run callback functions
	*/
#ifdef QUADEC_ENABLED    
    if (source == SRC_QUAD_IRQ)
    {
        if (QUADDEC_callback != NULL) // Quadrature Decoder callback has been set-up by the application clock is enabled
        {
            int16_t x,y,z;
            
            x = quad_decoder_get_x_counter();
            y = quad_decoder_get_y_counter();
            z = quad_decoder_get_z_counter();
            quadEncoderHandlerFunction = (quad_encoder_handler_function_t)(QUADDEC_callback);    
            quadEncoderHandlerFunction(x,y,z);
        }
    }        
    else 
#endif //QUADEC_ENABLED
    {
#ifdef WKUP_ENABLED        
        if (source == SRC_WKUP_IRQ)
        {
            if (WKUPCT_callback != NULL)
            {
                wakeupHandlerFunction = (wakeup_handler_function_t)(WKUPCT_callback);    
                wakeupHandlerFunction();
            }
        }
#endif //WKUP_ENABLED
    }
	return;
}


#ifdef WKUP_ENABLED
/**
 ****************************************************************************************
 * @brief Enable Wakeup IRQ.
 *
 * @param[in] sel_pins      Select enabled inputs. Bits 0-7 -> port 0(P00..P07), Bits 8-13 -> port 1(P10..P15), Bits 14-15 -> port 2(P28,P29), Bits -> 16-23 port 2(P00..P07), Bits 24-31 -> port 3(P30..P37). 0-disabled, 1-enabled.
 * @param[in] pol_pins      Inputs' polarity. Bits 0-7 -> port 0(P00..P07), Bits 8-13 -> port 1(P10..P15), Bits 14-15 -> port 2(P28,P29), Bits -> 16-23 port 2(P00..P07), Bits 24-31 -> port 3(P30..P37). 0-high, 1-low.
 * @param[in] events_num    Number of events before wakeup interrupt. Max 255.
 * @param[in] deb_time      Debouncing time. Max 0x3F.
 *
 * @return void
 ****************************************************************************************
 */
void wkupct_enable_irq(uint32_t sel_pins, uint32_t pol_pins, uint16_t events_num, uint16_t deb_time)
{
    uint8_t temp;
    
    SetBits16(CLK_PER_REG, WAKEUPCT_ENABLE, 1);                     // enable clock of Wakeup Controller
    
    SetWord16(WKUP_RESET_CNTR_REG, 0);                              // Clear event counter (for safety...)
    SetWord16(WKUP_COMPARE_REG, (events_num & 0xFF));               // Wait for 1 event and wakeup

    for(int i = 0; i < 4; ++i)
    {   
        temp = (uint8_t)((sel_pins >> (8 * i)) & 0xFF);
        SetWord16(WKUP_SELECT_P0_REG + (2 * i), temp);
        temp = (uint8_t)((pol_pins >> (8 * i)) & 0xFF);
        SetWord16(WKUP_POL_P0_REG + (2 * i), temp);
    }
    
    // Set P28, P29 configuration
    SetBits16(WKUP_SELECT_P2_REG, (3<<8), (sel_pins>>14)&3);
    SetBits16(WKUP_POL_P2_REG, (3<<8), (pol_pins>>14)&3);
        
    SetWord16(WKUP_RESET_IRQ_REG, 1);                               // clear any garbagge
    NVIC_ClearPendingIRQ(WKUP_QUADEC_IRQn);                         // clear it to be on the safe side...

    SetWord16(WKUP_CTRL_REG, 0x80 | (deb_time & 0x3F));             // Setup IRQ: Enable IRQ, T ms debounce
    NVIC_SetPriority(WKUP_QUADEC_IRQn, 1);                          // Set irq Priority to 1
    wkupct_quad_IRQ_status.reservations.reserved_by_wkupct = true;  // Wakeup Timer registers IRQ use
    NVIC_EnableIRQ(WKUP_QUADEC_IRQn);    
}

/**
 ****************************************************************************************
 * @brief Unregisters WKUP_QUADEC_IRQn use by Wakeup Timer and calls 
 * @brief wkupct_quad_disable_IRQ. Important Note: WKUP_QUADEC_IRQn interrupt will be
 * @brief disabled, only if also the Quadrature Decoder has unregistered from usin the IRQ.
 *
 * @return WKUPCT_QUADEC_ERR_OK, WKUPCT_QUADEC_ERR_RESERVED
 ****************************************************************************************
 */
wkupct_quadec_error_t wkupct_disable_irq()
{
    wkupct_quad_IRQ_status.reservations.reserved_by_wkupct = false;         // Wakeup Timer unregisters IRQ use
    
    SetWord16(WKUP_RESET_IRQ_REG, 1); //Acknowledge it
    SetBits16(WKUP_CTRL_REG, WKUP_ENABLE_IRQ, 0);                           //No more interrupts of this kind
    
    return wkupct_quad_disable_IRQ();
}    


/**
 ****************************************************************************************
 * @brief Register Callback function for Wakeup IRQ.
 *
 * @param[in] callback      Callback function's reference.
 *
 * @return void
 ****************************************************************************************
 */
void wkupct_register_callback(wakeup_handler_function_t callback)
{
    WKUPCT_callback = callback;
}

#endif //WKUP_ENABLED

#ifdef QUADEC_ENABLED
/**
 ****************************************************************************************
 * @brief Register Callback function for Quadrature Decoder IRQ.
 *
 * @param[in] callback      Callback function's reference.
 *
 * @return void
 ****************************************************************************************
 */
void quad_decoder_register_callback(uint32_t* callback)
{
    QUADDEC_callback = callback;
}


/**
 ****************************************************************************************
 * @brief 
 *
 * @param[in]
 *
 * @return 
 ****************************************************************************************
 */
 
void quad_decoder_init(QUAD_DEC_INIT_PARAMS_t *quad_dec_init_params)
{
   // Enable the Quadrature clock    
    SetBits16(CLK_PER_REG, QUAD_ENABLE , true);
   // Setup Quadrature Decoder pin assignment    
    SetWord16(QDEC_CTRL2_REG, quad_dec_init_params->chx_port_sel | quad_dec_init_params->chy_port_sel | quad_dec_init_params->chz_port_sel);
    SetWord16(QDEC_CLOCKDIV_REG, quad_dec_init_params->qdec_clockdiv);
           
}

/**
 ****************************************************************************************
 * @brief 
 *
 * @param[in]
 *
 * @return 
 ****************************************************************************************
 */
void quad_decoder_release(void)
{
   // Reset Quadrature Decoder pin assignment (PLEASE REVIEW)
    SetWord16(QDEC_CTRL2_REG, QUAD_DEC_CHXA_NONE_AND_CHXB_NONE | QUAD_DEC_CHYA_NONE_AND_CHYB_NONE | QUAD_DEC_CHZA_NONE_AND_CHZB_NONE);
   // Disable the Quadrature clock    
    SetBits16(CLK_PER_REG, QUAD_ENABLE , false);
    quad_decoder_disable_irq(); //Note: Will disable the IRQ only if Wakeup Timer has also unregistered from using it
}


/**
 ****************************************************************************************
 * @brief 
 *
 * @param[in]
 *
 * @return 
 ****************************************************************************************
 */
inline int16_t quad_decoder_get_x_counter(void)
{
    return GetWord16(QDEC_XCNT_REG);
}   


/**
 ****************************************************************************************
 * @brief 
 *
 * @param[in]
 *
 * @return 
 ****************************************************************************************
 */
inline int16_t quad_decoder_get_y_counter(void)
{
    return GetWord16(QDEC_YCNT_REG);
}   


/**
 ****************************************************************************************
 * @brief 
 *
 * @param[in]
 *
 * @return 
 ****************************************************************************************
 */
inline int16_t quad_decoder_get_z_counter(void)
{
    return GetWord16(QDEC_ZCNT_REG);
}   


/**
 ****************************************************************************************
 * @brief 
 *
 * @param[in]
 *
 * @return 
 ****************************************************************************************
 */
void quad_decoder_enable_irq(uint8_t event_count)
{
    SetBits16(QDEC_CTRL_REG, QD_IRQ_CLR, 1);                               // clear any garbagge
    NVIC_ClearPendingIRQ(WKUP_QUADEC_IRQn);                                // clear it to be on the safe side...
    
    SetBits16(QDEC_CTRL_REG, QD_IRQ_THRES, event_count);                   // Set event counter
    SetBits16(QDEC_CTRL_REG, QD_IRQ_MASK, 1);                              // interrupt not masked                               
    NVIC_SetPriority(WKUP_QUADEC_IRQn, 1);                                 // Set irq Priority to 1
    wkupct_quad_IRQ_status.reservations.reserved_by_quad = true;           // IRQ freeing reserved by Quadrature Decoder
    NVIC_EnableIRQ(WKUP_QUADEC_IRQn);                                      // enable the WKUP_QUADEC_IRQn
}    

/**
 ****************************************************************************************
 * @brief Unregisters WKUP_QUADEC_IRQn use by Quadrature Decoder and calls 
 * @brief wkupct_quad_disable_IRQ. Important Note: WKUP_QUADEC_IRQn interrupt will be
 * @brief disabled, only if also the Wakeup Timer has unregisterd from usin the IRQ.
 *
 * @return WKUPCT_QUADEC_ERR_OK, WKUPCT_QUADEC_ERR_RESERVED
 ****************************************************************************************
 */
wkupct_quadec_error_t quad_decoder_disable_irq(void)
{
    wkupct_quad_IRQ_status.reservations.reserved_by_quad = false;          // Quadrature Decoder unregisters IRQ use
    
    SetBits16(QDEC_CTRL_REG, QD_IRQ_CLR, 1);  // write 1 to clear Quadrature Decoder interrupt
    SetBits16(QDEC_CTRL_REG, QD_IRQ_MASK, 0); // write 0 to mask the Quadrature Decoder interrupt
    
    return wkupct_quad_disable_IRQ();
}    
#endif //QUADEC_ENABLED
