/**
 ****************************************************************************************
 *
 * @file app_kbdtest_proj.c
 *
 * @brief Keyboard Tester - HID over GATT Application entry point.
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

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"             // SW configuration
#include "app.h" 
#include "app_task.h"
#include "app_console.h"
#include "gpio.h"

#include "app_kbdtest.h"


/*
 * GLOBAL VARs
 ****************************************************************************************
 */
enum KBD_TESTER_ST kbd_tester_st; 
uint32_t total_key_presses;
uint32_t total_key_presses_reported;
uint32_t total_key_releases_reported;
uint32_t kbd_test_press_time;
uint32_t kbd_test_release_time;
uint32_t kbd_test_release_time_initial;
uint32_t kbd_test_release_time_incr_step;
uint32_t kbd_test_press_time_initial;
uint32_t kbd_test_press_time_incr_step;
uint32_t kbd_test_recycle_limit;
bool key_press_report_pending;
bool key_release_report_pending;
struct kbd_time timestamp_press, timestamp_release;
struct kbd_time_statistics press_stat, release_stat;
bool stop_test_event;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
 static void start_single_test(void);
 void kbd_enable_kbd_irq(void);


/*
 * SYSTICK functions
 ****************************************************************************************
 */
static inline void systick_start(const int ticks, const int mode /* = 0*/)
{
	SetWord32(0xE000E010, 0x00000000);      // disable systick
	SetWord32(0xE000E014, ticks);           // set systick timeout based on 16MHz clock
	SetWord32(0xE000E018, ticks);           // set systick timeout based on 16MHz clock
	GetWord32(0xE000E010);                  // make sure COUNTFLAG is reset
	SetWord32(0xE000E010, 1|/*4|*/mode);    // enable systick on 1MHz clock (always ON) with interrupt (mode)
}


__attribute__((unused)) static inline void systick_wait() 
{
	while (0 == (GetWord32(0xE000E010) & 0x00010000)) {} // wait for COUNTFLAG to become 1
}


static inline void systick_stop(void)
{
	// leave systick in a known state
	SetWord32(0xE000E010, 0x00000000);      // disable systick
	GetWord32(0xE000E010);                  // make sure COUNTFLAG is reset
}


void SysTick_Handler(void) 
{ 
    switch(kbd_tester_st)
    {
        case IDLE:
        {
            start_single_test();
        }
        break;
        
        case RELEASE_CNT:
        {
            if (stop_test_event) {
                kbd_tester_st = STOPPING;
                systick_start(2000000, 2);      // 2 sec for any pending reports to arrive
            } else if (key_release_report_pending) {
                arch_puts(".");
                systick_start(10000, 2); // Wait for the Release (10ms)
            } else {
                kbd_tester_st = PRESS_CNT;
                GPIO_SetActive(Q24_PORT, Q24_PIN);
                timestamp_press.slots = GetWord32(BLE_BASETIMECNT_REG);
                timestamp_press.usec = GetWord32(BLE_FINETIMECNT_REG);
                key_press_report_pending = true;
                total_key_presses++;
                kbd_test_press_time += kbd_test_press_time_incr_step;
                if (kbd_test_press_time > kbd_test_recycle_limit) {
                    kbd_test_press_time = kbd_test_press_time_initial; //restart!
                    arch_puts("### Limit reached!\r\n");
                }
                systick_start(kbd_test_press_time, 2);
            }
        }
        break;
        
        case PRESS_CNT:
        {
            /* if (key_release_report_pending) {
                if (stop_test_event)
                    kbd_tester_st = STOPPING;
                else {
                    arch_puts(".");
                    systick_start(10000, 2); // Wait for the Release (10ms)
                }
            } else */{
                kbd_tester_st = RELEASE_CNT;
                GPIO_SetInactive(Q24_PORT, Q24_PIN);
                timestamp_release.slots = GetWord32(BLE_BASETIMECNT_REG);
                timestamp_release.usec = GetWord32(BLE_FINETIMECNT_REG);
                kbd_test_release_time += kbd_test_release_time_incr_step;
                if (kbd_test_release_time > kbd_test_recycle_limit) {
                    kbd_test_release_time = kbd_test_release_time_initial; //restart!
                    arch_puts("### Limit reached!\r\n");
                }
                systick_start(kbd_test_release_time, 2);
                key_release_report_pending = true;
            }
        }
        break;
        
        case STOPPING:
        {
            app_disconnect();
            systick_stop();
            stop_kbd_single_test();
            kbd_tester_st = IDLE;
        }
        break;
            
    }

    return;
}


/*
 * Tester functions
 ****************************************************************************************
 */

static void start_single_test(void)
{
    systick_stop();
    
    // configure gpio (high is press, low is release)
    GPIO_ConfigurePin(Q24_PORT, Q24_PIN, OUTPUT, PID_GPIO, false);
    // configure button gpio (active low)
    GPIO_SetPinFunction(GPIO_PORT_0, GPIO_PIN_6, INPUT_PULLUP, PID_GPIO);
    
    kbd_enable_kbd_irq();
    
    kbd_tester_st = RELEASE_CNT;
    // start SysTick (to count the release time)
    systick_start(kbd_test_release_time, 2);
}


/* 
 * time base is usec
 */
bool start_kbd_single_test(uint32_t press_time, uint32_t press_time_incr_step, uint32_t release_time, uint32_t release_time_incr_step, uint32_t recycle_limit)
{
    bool status = true;
    
    // press time must not be too large (i.e. 10sec)
    if (press_time >= 10000000)
        status = false;
        
    // release time must be larger than release debouncing time (i.e. 8msec)
    if (release_time < 8000)
        status = false;
        
    // step value for incrementing release_time after each key press should be 0 < value < 10ms
    if (release_time_incr_step > 10000)
        status = false;
        
    if (kbd_tester_st != IDLE)
        status = false;
        
    if (!status)
        return status;
        
    stop_test_event = false;
    
    // Implement a delay here to give time to the DUT to prepare (i.e. 2sec)
    NVIC_SetPriority(SysTick_IRQn, 3);         
    NVIC_EnableIRQ(SysTick_IRQn);
    systick_start(2000000, 2);

    total_key_presses = 0;
    total_key_presses_reported = 0;
    total_key_releases_reported = 0;
    kbd_test_press_time = press_time;
    kbd_test_press_time_incr_step = press_time_incr_step;
    kbd_test_press_time_initial = press_time;
    kbd_test_release_time = release_time;
    kbd_test_release_time_incr_step = release_time_incr_step;
    kbd_test_recycle_limit = recycle_limit;
    kbd_test_release_time_initial = release_time;
    key_press_report_pending = false;
    key_release_report_pending = false;
    timestamp_press.slots = 0;
    timestamp_press.usec = 0;
    timestamp_release.slots = 0;
    timestamp_release.usec = 0;
    press_stat.total = 0;
    press_stat.max = 0;
    press_stat.min = 0;
    release_stat.total = 0;
    release_stat.max = 0;
    release_stat.min = 0;

    return status;
}


int report_press(void)
{
    struct kbd_time curr_time; 
    uint32_t time;
    int ret = 0;
    
    if (key_release_report_pending) {
        arch_puts("$$$ Missed a RELEASE report!\r\n");
        key_release_report_pending = false;
        ret = total_key_releases_reported;
    }
        
    total_key_presses_reported++;
    
    if (!key_press_report_pending) {
        arch_puts("$$$ Got an extra PRESS report!\r\n");
        ret = total_key_presses_reported;
    }
        
    key_press_report_pending = false;
    
    curr_time.slots = GetWord32(BLE_BASETIMECNT_REG);
    curr_time.usec = GetWord32(BLE_FINETIMECNT_REG);
    
    time = (curr_time.slots - timestamp_press.slots) * 625 + (curr_time.usec - timestamp_press.usec);
    
    if (press_stat.total == 0) {
        press_stat.total = time;
        press_stat.max = time;
        press_stat.min = time;
    } else {
        press_stat.total += time;
        if (press_stat.max < time)
            press_stat.max = time;
        if (press_stat.min > time)
            press_stat.min = time;
    }
    
    return ret;
}


int report_release(void)
{
    struct kbd_time curr_time; 
    uint32_t time;
    int ret = 0;
    
    if (key_press_report_pending) {
        arch_puts("$$$ Missed a PRESS report!\r\n");
        key_press_report_pending = false;
        ret = total_key_presses_reported;
    }
    
    total_key_releases_reported++;
    
    if (!key_release_report_pending) {
        arch_puts("$$$ Got an extra RELEASE report!\r\n");
        ret = total_key_releases_reported;
    }
        
    key_release_report_pending = false;

    curr_time.slots = GetWord32(BLE_BASETIMECNT_REG);
    curr_time.usec = GetWord32(BLE_FINETIMECNT_REG);
    
    time = (curr_time.slots - timestamp_release.slots) * 625 + (curr_time.usec - timestamp_release.usec);
    
    if (release_stat.total == 0) {
        release_stat.total = time;
        release_stat.max = time;
        release_stat.min = time;
    } else {
        release_stat.total += time;
        if (release_stat.max < time)
            release_stat.max = time;
        if (release_stat.min > time)
            release_stat.min = time;
    }
    
    return ret;
}


#define REP_DELAY 0x8000
static inline void _delay(void)
{
    volatile int i;
    
    for (i = 0; i < REP_DELAY; i++) {}; // add delay so as not to overflow UART
}

bool stop_kbd_single_test(void)
{
    volatile int i;
    bool status = true;
    
    if (kbd_tester_st == IDLE)
        status = false;
        
    if (!status)
        return status;
        
    systick_stop();
    kbd_tester_st = IDLE;
    GPIO_SetInactive(Q24_PORT, Q24_PIN);
    
    arch_puts("### Test is stopped!\r\n");
    _delay();
    arch_puts("    Statistics\r\n");
    _delay();
    arch_puts("---------------------\r\n");
    _delay();
    arch_printf("Key presses sent:      %d\r\n", total_key_presses);
    _delay();
    arch_printf("Key presses reported:  %d\r\n", total_key_presses_reported);
    _delay();
    arch_printf("Key releases reported: %d\r\n", total_key_releases_reported);
    _delay();
    arch_puts("---------------------\r\n");
    _delay();
    arch_puts(  "Press times (usec): \r\n");
    _delay();
    arch_printf("    - max: %d\r\n", press_stat.max);
    _delay();
    arch_printf("    - min: %d\r\n", press_stat.min);
    _delay();
    if (total_key_presses_reported == 0) total_key_presses_reported = 1;
    arch_printf("    - avg: %u\r\n", (unsigned int)(press_stat.total / total_key_presses_reported));
    _delay();
    arch_puts("---------------------\r\n");
    _delay();
    arch_puts(  "Release times (usec): \r\n");
    _delay();
    arch_printf("    - max: %d\r\n", release_stat.max);
    _delay();
    arch_printf("    - min: %d\r\n", release_stat.min);
    _delay();
    if (total_key_releases_reported == 0) total_key_releases_reported = 1;
    arch_printf("    - avg: %u\r\n", (unsigned int)(release_stat.total / total_key_releases_reported));
    _delay();
    arch_puts("------ TEST END ------\r\n");
    _delay();

    return status;
}


void kbd_enable_kbd_irq(void)
{
#define KBRD_IRQ_IN_SEL2_REG            (0x50001416) /* GPIO interrupt selection for KBRD_IRQ for P3 */    
    SetWord16(GPIO_DEBOUNCE_REG, 0x2000 | 0);   // 0 ms debouncing

    SetWord16(KBRD_IRQ_IN_SEL2_REG, 0);
    SetWord16(KBRD_IRQ_IN_SEL1_REG, 0);
    SetWord16(KBRD_IRQ_IN_SEL0_REG, 0x4040);    // P0_6, active low

    SetWord16(GPIO_RESET_IRQ_REG, 0x20);        // clear any garbage
    NVIC_ClearPendingIRQ(KEYBRD_IRQn);          // clear it to be on the safe side...

    NVIC_SetPriority(KEYBRD_IRQn, 2);         
    NVIC_EnableIRQ(KEYBRD_IRQn);
}


void KEYBRD_Handler(void)
{
    stop_test_event = true;

    // Clear it first
    SetWord16(KBRD_IRQ_IN_SEL0_REG, 0); 
    SetWord16(KBRD_IRQ_IN_SEL1_REG, 0);
    SetWord16(GPIO_RESET_IRQ_REG, 0x20); 

    // No more KEYBRD interrupts from now on
    NVIC_DisableIRQ(KEYBRD_IRQn); 
}


