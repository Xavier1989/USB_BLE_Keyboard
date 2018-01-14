/**
 ****************************************************************************************
 *
 * @file app_kbdtest.h
 *
 * @brief Keyboard Tester - core functionality header file.
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


enum KBD_TESTER_ST {
    IDLE,
    RELEASE_CNT,
    PRESS_CNT,
    STOPPING,
};

struct kbd_time {
    uint32_t slots;
    uint32_t usec;
};

struct kbd_time_statistics {
    uint32_t min;
    uint32_t max;
    uint32_t total;
};


#define Q1_PORT     GPIO_PORT_2
#define Q1_PIN      GPIO_PIN_9
#define Q2_PORT     GPIO_PORT_0
#define Q2_PIN      GPIO_PIN_2
#define Q3_PORT     GPIO_PORT_2
#define Q3_PIN      GPIO_PIN_8
#define Q4_PORT     GPIO_PORT_0
#define Q4_PIN      GPIO_PIN_3
#define Q5_PORT     GPIO_PORT_2
#define Q5_PIN      GPIO_PIN_7
#define Q6_PORT     GPIO_PORT_0
#define Q6_PIN      GPIO_PIN_4
#define Q7_PORT     GPIO_PORT_2
#define Q7_PIN      GPIO_PIN_0
#define Q8_PORT     GPIO_PORT_0
#define Q8_PIN      GPIO_PIN_5
#define Q9_PORT     GPIO_PORT_2
#define Q9_PIN      GPIO_PIN_1
#define Q10_PORT    GPIO_PORT_0
#define Q10_PIN     GPIO_PIN_6
#define Q11_PORT    GPIO_PORT_0
#define Q11_PIN     GPIO_PIN_7
#define Q12_PORT    GPIO_PORT_1
#define Q12_PIN     GPIO_PIN_0
#define Q13_PORT    GPIO_PORT_1
#define Q13_PIN     GPIO_PIN_1
#define Q14_PORT    GPIO_PORT_2
#define Q14_PIN     GPIO_PIN_2
#define Q15_PORT    GPIO_PORT_2
#define Q15_PIN     GPIO_PIN_3
#define Q16_PORT    GPIO_PORT_2
#define Q16_PIN     GPIO_PIN_4
#define Q17_PORT    GPIO_PORT_2
#define Q17_PIN     GPIO_PIN_5
#define Q18_PORT    GPIO_PORT_2
#define Q18_PIN     GPIO_PIN_6
#define Q19_PORT    GPIO_PORT_3
#define Q19_PIN     GPIO_PIN_0
#define Q20_PORT    GPIO_PORT_3
#define Q20_PIN     GPIO_PIN_1
#define Q21_PORT    GPIO_PORT_3
#define Q21_PIN     GPIO_PIN_2
#define Q22_PORT    GPIO_PORT_3
#define Q22_PIN     GPIO_PIN_3
#define Q23_PORT    GPIO_PORT_3
#define Q23_PIN     GPIO_PIN_4
#define Q24_PORT    GPIO_PORT_3
#define Q24_PIN     GPIO_PIN_5
#define Q25_PORT    GPIO_PORT_3
#define Q25_PIN     GPIO_PIN_6
#define Q26_PORT    GPIO_PORT_3
#define Q26_PIN     GPIO_PIN_7

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
bool start_kbd_single_test(uint32_t press_time, uint32_t press_time_incr_step, uint32_t release_time, uint32_t release_time_incr_step, uint32_t recycle_limit);
int report_press(void);
int report_release(void);
bool stop_kbd_single_test(void);
