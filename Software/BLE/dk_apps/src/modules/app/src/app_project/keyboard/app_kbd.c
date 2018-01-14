/**
 ****************************************************************************************
 *
 * @file app_kbd.c
 *
 * @brief Keyboard (HID) over GATT Application entry point.
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

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

//#define ASCII_DEBUG  

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <string.h>

#include "co_bt.h"
#include "arch.h"
#include "app.h"
#include "app_task.h"
#include "app_console.h"
#include "gpio.h"

#include "gap.h"
#include "gapc_task.h"
#include "llm_task.h"

#include "app_kbd.h"
#include "app_kbd_proj.h"
#include "app_kbd_key_matrix.h"
#include "app_kbd_matrix.h"
#include "app_kbd_leds.h"
#include "app_kbd_fsm.h"
#include "app_kbd_scan_fsm.h"
#include "app_kbd_debug.h"
#include "app_multi_bond.h"

#include "periph_setup.h"
#include "wkupct_quadec.h"

#if BLE_DIS_SERVER
#include "diss_task.h"
#endif // BLE_DIS_SERVER

#if BLE_BATT_SERVER
#include "bass_task.h"
#endif // BLE_BATT_SERVER

#include "hogpd_task.h"

#define KBRD_IRQ_IN_SEL2_REG            (0x50001416)                // GPIO interrupt selection for KBRD_IRQ for P3

#define __RETAINED __attribute__((section("retention_mem_area0"), zero_init))
#define __RETAINED_ALIGN_16 __RETAINED __attribute__((aligned (16)))

#if defined(REMOTE_WAKEUP_ON) && defined(INACTIVITY_TIMEOUT_ON)
#warning "Remote Host may not handle properly RemoteWakeup when the Inactivity Timeout is on (Some Hosts do not expect to receive LL_TERMINATE_IND from Wakeup capable devices while they are sleeping...)"
#endif

/*
 * RETAINED VARIABLE DECLARATIONS
 ****************************************************************************************
 */
extern volatile uint32_t pass_code;
 
enum REPORT_MODE kbd_reports_en __RETAINED;                         // flag to switch between key reporting and passcode entry modes
uint8_t kbd_proto_mode __RETAINED;                                  // Boot or Report protocol mode. Default is Report.
uint32_t passcode __RETAINED;                                       // used to store the passcode in passcode mode
struct keycode_buffer_tag kbd_keycode_buffer[KEYCODE_BUFFER_SIZE] __RETAINED;          // Buffer to hold the scan results for the key presses / releases
uint8_t kbd_keycode_buffer_head __RETAINED;                         // Read pointer for accessing the data of the keycode buffer
uint8_t kbd_keycode_buffer_tail __RETAINED;                         // Write pointer for writing data to the keycode buffer
bool keycode_buf_overflow __RETAINED;                               // Flag to indicate that the key buffer is full!
uint8_t kbd_key_report[MAX_REPORTS][8] __RETAINED_ALIGN_16;         // Key Report buffers
uint8_t normal_key_report_st[8] __RETAINED;                         // Holds the contents of the last Key Report for normal keys sent to the Host
uint8_t extended_key_report_st[3] __RETAINED;                       // Holds the contents of the last Key Report for special functions sent to the Host
kbd_rep_info report_list[MAX_REPORTS] __RETAINED;                   // The list of the reports instances (free or used)
kbd_rep_info *kbd_trm_list __RETAINED;                              // Linked list of the pending Key Reports
kbd_rep_info *kbd_free_list __RETAINED;                             // Linked list of the free Key Reports
//bool normal_key_report_ack_pending __RETAINED;                      // Keeps track of the acknowledgement of the last Key Report for normal keys sent to the Host
//bool extended_key_report_ack_pending __RETAINED;                    // Keeps track of the acknowledgement of the last Key Report for special functions sent to the Host
    
struct roll_over_tag roll_over_info  __RETAINED;                    // holds all the keys being pressed during a RollOver (Phantom state) state

bool user_disconnection_req __RETAINED;
bool user_extended_sleep __RETAINED;

// DELAYED_WAKEUP_ON
enum delay_monitor_status monitor_kbd_delayed_start_st __RETAINED;  // State of delayed start monitoring
enum delay_trigger_status trigger_kbd_delayed_start_st __RETAINED;  // Trigger event from the delayed start monitoring
bool ble_is_woken_up __RETAINED;                                    // Flag to indicate that the request to the BLE was triggered

/*
 * NON RETAINED VARIABLE DECLARATIONS (GLOBAL + STATIC)
 ****************************************************************************************
 */
 
bool systick_hit = false;
bool wkup_hit = false;      // does not have to be a RETAINED var since the KBD application wakes up from Deep Sleep only from a key press!

uint8_t kbd_membrane_status;                                        // 0: outputs are low => SW is prohibited, 1: outputs are high-Z => SW can do scanning
scan_t kbd_scandata[KBD_NR_OUTPUTS];                                // last reported status of the keyboard
bool kbd_active_row[KBD_NR_OUTPUTS];                                // last known status of the keyboard
scan_t kbd_new_scandata[KBD_NR_OUTPUTS];                            // current status of the keyboard
bool kbd_new_key_detected;                                          // flag to indicate that a new key was detected during the last scan
int kbd_fn_modifier;                                                // Fn key has been pressed
bool kbd_cntrl_active;                                              // flag to indicate the the Keyboard Controller is ON

int scan_cycle_time;                                                // time until the next wake up from SysTick
int scan_cycle_time_last;                                           // duration of the current scan cycle
bool full_scan;                                                     // when true a full keyboard scan is executed once. else only partial scan is done.
bool next_is_full_scan;                                             // Got an interrupt (key press) during partial scanning

uint8_t kbd_bounce_active;                                          // flag indicating we are still in debouncing mode
uint16_t kbd_bounce_intersections[DEBOUNCE_BUFFER_SIZE];            // holds output - input pair (key) for which debouncing is on
scan_t kbd_bounce_rows[KBD_NR_OUTPUTS];                             // holds the key mask ('1' is active) for each row that is being debounced
struct debounce_counter_t kbd_bounce_counters[DEBOUNCE_BUFFER_SIZE];// counter for each intersection (key)
uint8_t kbd_global_deb_cnt;                                         // counts down for press debouncing time when after a scan no new key has been detected
bool sync_key_press_evt;                                            // flag to indicate a Key press to the high-level FSM synchronously to the BLE
bool sync_passcode_entered_evt;                                     // flag to indicate to the high-level FSM that the Passcode has been entered by the user, synchronously to the BLE

/*
 * LOCAL FUNCTION FORWARD DECLARATIONS
 ****************************************************************************************
 */

static void kbd_enable_kbd_irq(void);
static inline void kbd_process_scandata(void);
static int prepare_kbd_keyreport(void);



/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

#ifdef ASCII_DEBUG
static inline uint8 hut_to_ascii(const uint8 hut)
{
	// roughly
	return (hut & 0x80) ? '?' : kbd_hut_to_ascii_table[hut];
}
#endif



/*
 * (Report) List management functions
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initializes the lists
 *
 * @param None
 *
 * @return None
 ****************************************************************************************
 */
 void kbd_init_lists(void)
{
	int i;
	kbd_rep_info *node;

	kbd_trm_list = NULL;
	
	for (i = 0; i < MAX_REPORTS; i++) 
    {
		node = &report_list[i];
		node->pBuf = kbd_key_report[i];
		node->type = FREE;
		node->modifier_report = false; // normal keys
		node->pNext = kbd_free_list;
		kbd_free_list = node;
	}
}


/**
 ****************************************************************************************
 * @brief Pulls one member from the beginning of the list
 *
 * @param[in] list The list to use
 *
 * @return the member at the beginning of the list, if found, else NULL
 ****************************************************************************************
 */
kbd_rep_info *kbd_pull_from_list(kbd_rep_info **list)
{
	kbd_rep_info *node;
	
	if (!(*list))
		return NULL;
	
	node = *list;
	*list = node->pNext;
	node->pNext = NULL;
	
	return node;
}


/**
 ****************************************************************************************
 * @brief Pushes one member at the end of the list
 *
 * @param[in]   list    The list to use
 * @param[in]   node    Member to add to the list
 *
 * @return   APP_KBD_ERR_OK if the member is successfully pushed or else APP_KBD_ERR_ERROR
 ****************************************************************************************
 */
int kbd_push_to_list(kbd_rep_info **list, kbd_rep_info *node)
{
	kbd_rep_info *p;
	
	if (!node)
		return APP_KBD_ERR_ERROR;
	
	if (!(*list))
		*list = node;
	else 
    {
		for (p = *list; p->pNext != NULL; p = p->pNext);
		
		p->pNext = node;
		node->pNext = NULL;
	}
	
	return APP_KBD_ERR_OK;
}


/**
 ****************************************************************************************
 * @brief Finds the last instance of a specific type in a list
 *
 * @param[in]   list    The list to use
 * @param[in]   type    The type of the instance to search for (FREE, PRESS, RELEASE, EXTENDED)
 *
 * @return  the last instance of a specific type in a list, if found, else NULL
 ****************************************************************************************
 */
kbd_rep_info *kbd_search_in_list(kbd_rep_info *list, enum KEY_BUFF_TYPE type)
{
	kbd_rep_info *node;
	kbd_rep_info *last_found = NULL;
	
	for (node = list; node != NULL; node = node->pNext)
		if (node->type == type)
			last_found = node;
	
	return last_found;
}



/*
 * SYSTICK functions
 ****************************************************************************************
 */
 

/**
 ****************************************************************************************
 * @brief starts the systick timer
 *
 * @param[in]   ticks
 * @param[in]   mode
 *
 * @return  void
 ****************************************************************************************
 */
static inline void systick_start(const int ticks, const int mode /* = 0*/)
{
    systick_hit = false;
	SetWord32(0xE000E010, 0x00000000);      // disable systick
	SetWord32(0xE000E014, ticks);           // set systick timeout based on 16MHz clock
	SetWord32(0xE000E018, ticks);           // set systick timeout based on 16MHz clock
	GetWord32(0xE000E010);                  // make sure COUNTFLAG is reset
	SetWord32(0xE000E010, 1|/*4|*/mode);    // enable systick on 1MHz clock (always ON) with interrupt (mode)
}

__attribute__((unused)) static inline void systick_wait(void) 
{
	while (0 == (GetWord32(0xE000E010) & 0x00010000)) {} // wait for COUNTFLAG to become 1
}


/**
 ****************************************************************************************
 * @brief stops the systick timer
 *
 * @param None
 *
 * @return  void
 ****************************************************************************************
 */
static inline void systick_stop(void)
{
	// leave systick in a known state
	SetWord32(0xE000E010, 0x00000000);      // disable systick
	GetWord32(0xE000E010);                  // make sure COUNTFLAG is reset
}



/*
 * GPIO handling 
 ****************************************************************************************
 */

__forceinline static void set_row_to_low(int idx)
{
    if (kbd_out_bitmasks[idx]) 
    {
        SetWord16(kbd_output_reset_data_regs[idx] + P0_DATA_REG, kbd_out_bitmasks[idx]);  // output low
        SetWord16(kbd_output_mode_regs[idx] + P0_DATA_REG, 0x300);                        // mode gpio output
    }
}

__forceinline static void set_column_to_input_pullup(int idx)
{
    if (kbd_input_mode_regs[idx])
        SetWord16(kbd_input_mode_regs[idx] + P0_DATA_REG, 0x100);                         // mode gpio input pullup
}

__forceinline static void set_row_to_input_highz(int idx)
{
    if (kbd_out_bitmasks[idx])
        SetWord16(kbd_output_mode_regs[idx] + P0_DATA_REG, 0x000);                        // mode gpio input highz
}



/*
 * Keyboard "membrane" handling
 ****************************************************************************************
 */

// Set all columns to "input pull-up" state
static void kbd_membrane_input_setup(void)
{	
	for (int i = 0; i < KBD_NR_INPUTS; ++i) 
        set_column_to_input_pullup(i);
}

// Set all rows to "high-Z" state to enable SW scanning
static void kbd_membrane_output_wakeup(void)
{
	for (int i = 0; i < KBD_NR_OUTPUTS; ++i)
        set_row_to_input_highz(i);
	
	kbd_membrane_status = 1;
}

// Set all rows to "low" state to enable HW scanning (WKUP-TIMER)
static void kbd_membrane_output_sleep(void)
{	
	kbd_membrane_status = 0;
	
	// Pull all outputs low to quickly discharge the membrane.
	for (int i = 0; i < KBD_NR_OUTPUTS; ++i) 
    {
        set_row_to_low(i);
	}
	
	// The inputs are pullup, so their transition to low will trigger the keyboard interrupt
}


/*
 * Keyboard initialization
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @brief Handles the initialization of all retained variables used for key scanning
 *
 * @param None
 *
 * @return  void
 ****************************************************************************************
 */
static void kbd_init_retained_scan_vars(void)
{
    int i;
    
	kbd_keycode_buffer_head = 0;
    kbd_keycode_buffer_tail = 0;
    keycode_buf_overflow = false;

    for (i = 0; i < ROLL_OVER_BUF_SZ; i++)
        roll_over_info.intersections[i] = RLOVR_INVALID_INTERSECTION;
    roll_over_info.cnt = 0;    
    kbd_proto_mode = HOGP_REPORT_PROTOCOL_MODE;
}


/**
 ****************************************************************************************
 * @brief Handles the initialization of all non-retained variables used for key scanning
 *
 * @param None
 *
 * @return  void
 ****************************************************************************************
 */
static void kbd_init_scan_vars(void)
{
    int i;
    
    full_scan = false;
    next_is_full_scan = false;
    
	kbd_membrane_status = 0;

	kbd_fn_modifier = 0;
    kbd_cntrl_active = false;
    
	for (i = 0; i < DEBOUNCE_BUFFER_SIZE ; ++i)
	{
		kbd_bounce_intersections[i] = 0xFFFF;
		kbd_bounce_counters[i].cnt = 0;
        kbd_bounce_counters[i].state = IDLE;
	}
    kbd_global_deb_cnt = 0;
	kbd_bounce_active = 0;
	
#ifdef ASCII_DEBUG
	kbd_keybuffer_idx = 0;
#endif
	
	for (i = 0; i < KBD_NR_OUTPUTS; ++i) 
    {
        const scan_t val = (1 << KBD_NR_INPUTS) - 1;
        
		kbd_scandata[i] = val;
        kbd_active_row[i] = false;
        kbd_bounce_rows[i] = 0;
    }
    
    kbd_new_key_detected = false;
    sync_key_press_evt = false;
    sync_passcode_entered_evt = false;
}


/**
 ****************************************************************************************
 * @brief Handles the initialization of the key reports and the report lists.
 *        Call this function once at the beginning since all data is retained.
 *
 * @param None
 *
 * @return  void
 ****************************************************************************************
 */
static void kbd_init_keyreport(void)
{
	int i;
	
	for (i = 0; i < MAX_REPORTS; i++)
		memset(kbd_key_report[i], 0, 8);
        
	kbd_init_lists();
    
	normal_key_report_st[0] = 0xFF;     // invalidate
    extended_key_report_st[0] = 0;
    extended_key_report_st[1] = 0;
    extended_key_report_st[2] = 0;
}


/*
 * Keyboard scanning
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Handles the scanning of the key matrix and the processing of the results
 *
 * @param[in] row - number of row to scan
 *
 * @return  false, when scanning of the rows in the current scan cycle is not completed
 *          true, when all rows have been scanned
 ****************************************************************************************
 */
bool app_kbd_scan_matrix(int *row)
{
    // 1. Find which row has the pressed key 
    // 2. Drive this row 'low' for 1 scan cycle
    // In the next scan cycle: 
    // 3. Read inputs
    // 4. Turn the row to 'high-Z'
    // 5. Check if there's another row with a pressed key and if so go to 2
    // 6. if no other rows remain to be scanned
    //    a. process results (debounce and deghost here)
    //    b. decrement debounce counters
    //    c. if there are keys in the buffer (presses and/or releases) prepare HID report(s)
    //    d. Program Keyboard Controller to scan the rest of the rows
    // 7. Exit
	
    uint8_t i = *row;
    int j;
    int8_t prev_line = -1;
    const scan_t scanmask = (1 << KBD_NR_INPUTS) - 1;

    ASSERT_ERROR(kbd_membrane_status != 0);
    
    // Update SysTick next tick time
    scan_cycle_time -= ( ROW_SCAN_TIME * SYSTICK_TICKS_PER_US );
	
    // Step 0. Check if we've already driven a row to low. In that case we should scan the inputs...
    if ( (i > 0) && (GetWord16(kbd_output_mode_regs[i-1] + P0_DATA_REG) == 0x300) ) 
        prev_line = i - 1;
	
    // Step 1. Find which row has the pressed key
    if (!full_scan)
    for (; i < KBD_NR_OUTPUTS; ++i) 
    {
        if(kbd_out_bitmasks[i])   // valid output
        {
            if(kbd_active_row[i]) //This line has a pressed key
                break;
        }
    }

    if ( prev_line != -1 )
    {
        scan_t scanword = 0;
        uint16 kbd_gpio_in[5];
		
        kbd_gpio_in[4] = 0xFFFF;
        
        // Step 3. Read the input registers.
#ifdef P0_HAS_INPUT					
        kbd_gpio_in[0] = GetWord16(P0_DATA_REG);
#endif
#ifdef P1_HAS_INPUT					
        kbd_gpio_in[1] = GetWord16(P1_DATA_REG);
#endif
#ifdef P2_HAS_INPUT					
        kbd_gpio_in[2] = GetWord16(P2_DATA_REG);
#endif
#ifdef P3_HAS_INPUT					
        kbd_gpio_in[3] = GetWord16(P3_DATA_REG);
#endif

        // Step 4. Turn previous output to "high-Z" (input)
        if (kbd_out_bitmasks[prev_line])
            SetWord16(kbd_output_mode_regs[prev_line] + P0_DATA_REG, 0x000);    // mode gpio input highz

        // Process input data
        // we're waiting for the signals to stabilize for the the next readout
        // so this is a great time to spend some cycles to process the previous readout
			
#if 1        
        // fill in the scanword bits (we have plenty of time to burn these cycles
        for (j = KBD_NR_INPUTS - 1; j >= 0; --j)
        {
            const uint8_t input_port = kbd_input_ports[j];
            scanword = (scanword << 1) | ((kbd_gpio_in[input_port >> 4] >> (input_port & 0x0F)) & 1);
        }
        
        // if key press and there's no debouncing counter set for the key => new key press is detected!
        if ( ((~scanword & scanmask) != kbd_bounce_rows[prev_line]) )   // if the button is not being debounced
            kbd_new_key_detected = true;
            
        // update scan status
        kbd_new_scandata[prev_line] = scanword;
#else
        kbd_new_scandata[prev_line] = scanmask; //test global debouncing counter...
#endif
    }

    // Step 2. Pull next "used" output low hard
    if (i < KBD_NR_OUTPUTS && kbd_out_bitmasks[i]) 
    {
        SetWord16(kbd_output_reset_data_regs[i] + P0_DATA_REG, kbd_out_bitmasks[i]);  // level 0
        SetWord16(kbd_output_mode_regs[i] + P0_DATA_REG, 0x300);                      // mode gpio output
    }

    *row = i + 1;
	
    // 6. processing of the results has to be done just after the last row has been scanned
    // no need to have a separate state for this! idle time follows the completion of the processing!
    if (i == KBD_NR_OUTPUTS) 
    {
        // a. process scan results
        kbd_process_scandata();

        
        // b. Update debouncing counters
        kbd_bounce_active = 0;
        for (j = DEBOUNCE_BUFFER_SIZE-1; j >= 0 ; j--) 
        {
            if (0xFFFF != kbd_bounce_intersections[j]) 
            {
                kbd_bounce_active = 1;
                
                if (kbd_bounce_counters[j].cnt) 
                {
                    --kbd_bounce_counters[j].cnt;
                }
            }
        }
        
        // update the global debouncing counter as well
        if (kbd_global_deb_cnt)         // FIXME: to check!
        {
            kbd_global_deb_cnt--;
            kbd_bounce_active++;        // 1 if no keys are being debounced/pressed, 2 if there are other keys being debounced/pressed
        }
        
        
        // c. check if there are key events in the buffer
        if (kbd_reports_en == REPORTS_DISABLED) // passcode mode
            app_kbd_prepare_keyreports();
        // The handling of 'Paused' and 'Normal' modes has been moved to a separate function that is called synchronously
        
        
        // d. Program Keyboard Controller to scan the inactive rows
        
        //All outputs are in high-Z now. Enable Keyboard Controller
        if (!kbd_cntrl_active)
            kbd_enable_kbd_irq();
        else
            ASSERT_ERROR(0);
        
        //Find which rows have not any key being pressed, drive them low and calculate for how long the Keyboard Controller will be active
        for (j = 0; j < KBD_NR_OUTPUTS; ++j) 
        {
            if ( (!kbd_active_row[j]) && (kbd_new_scandata[j] == scanmask) )   //This line is "used" and has no pressed keys
            {
                // Pull output low hard
                SetWord16(kbd_output_reset_data_regs[j] + P0_DATA_REG, kbd_out_bitmasks[j]);  // level 0
                SetWord16(kbd_output_mode_regs[j] + P0_DATA_REG, 0x300);                      // mode gpio output
            }
        }
            
        ASSERT_WARNING(scan_cycle_time > 0);
        
        if (scan_cycle_time <= 0)
            scan_cycle_time = 20;
        
        if (GetWord16(CLK_CTRL_REG) & RUNNING_AT_RC16M)                     // drift is around +10% of the elapsed time
            scan_cycle_time = scan_cycle_time - (scan_cycle_time_last >> 4) - (scan_cycle_time_last >> 5);
                   
        ASSERT_WARNING(scan_cycle_time > 0);
        
        if (scan_cycle_time <= 0)
            scan_cycle_time = 20;
        
        scan_cycle_time += GetWord32(0xE000E018);
        systick_stop();
        systick_start(scan_cycle_time, 2);
    }
    
    return (i == KBD_NR_OUTPUTS); 
}


/**
 ****************************************************************************************
 * @brief Does debouncing for a key (called even when the key is considered as pressed
 *        after press debouncing has finished).
 *
 * @param[in] output
 * @param[in] input
 * @param[in] pressed
 *
 * @return  0, key is ignored
 *          1, key is accepted and should be checked for ghosting
 ****************************************************************************************
 */
static inline int debounce_key(const uint16 output, const uint16 input, const int pressed)
{
    const uint16 my_intersection = (output << 8) | input;
    const scan_t imask = 1 << input;
    int i;
    
    // to check whether a key is valid or not we always check the default keymap (#0).
    // the assumption is that a key will definitely appear in the default  
    // keymap and may appear in secondary keymaps as well.
    // if this is not sufficient for a specific application then additional 
    // logic needs to be implemented.
    if (kbd_keymap[0][output][input] == 0)
        return 0;                       // this key does not exist in the default 
                                        // keymap => it's a ghost!


    // *** DEBOUNCE ***
    
    // check if there's an entry for this intersection in the array. an entry is
    // reserved when a key is pressed. if an entry is reserved, 
    // then the entry remains reserved until the key is released!
    for (i = 0; i < DEBOUNCE_BUFFER_SIZE ; ++i)
        if (my_intersection == kbd_bounce_intersections[i])
            break;
           
    // the debouncing counter is 2-field structure. the field is the state. 
    // the second field is the debouncing counter that counts down when doing
    // press and release debouncing
    if (i != DEBOUNCE_BUFFER_SIZE)          // entry found!
    {
        struct debounce_counter_t *deb = &kbd_bounce_counters[i];
        
        switch(deb->state)
        {
            case IDLE:
                ASSERT_ERROR(0);            // cannot be in here!
                break;
            case PRESS_DEBOUNCING:
                if (deb->cnt == 0)          // debouncing done! check key status...
                {
                    if (pressed)            // go to WAIT_RELEASE and continue with deghosting
                        deb->state = WAIT_RELEASE;
                    else 
                    {
                        deb->state = IDLE;  // key is ignored since after debouncing is inactive
                        kbd_bounce_intersections[i] = 0xFFFF;
                        kbd_bounce_rows[output] &= ~imask;
                        return 0;
                    }
                } 
                else 
                {
                    kbd_new_scandata[output] |= imask;  // this bit is still toggling! reset it to the previous stable state so that
                                            // it doesn't affect deghosting of other valid keys below
                                            
                    return 0;               // still debouncing...
                }
                break;
            case WAIT_RELEASE:
                if (!pressed) 
                {
                    deb->state = RELEASE_DEBOUNCING;
                    deb->cnt = DEBOUNCE_COUNTER_RELEASE;
                    kbd_new_scandata[output] &= ~imask;  // this bit is still toggling! reset it to the previous stable state so that
                                            // is doesn't affect deghosting of other valid keys below
                                            
                    return 0;               // do debouncing...
                } 
                else 
                {                           // check if this key press has been reported (could be a ghost key)
                    if ( !(kbd_scandata[output] & imask) )
                        return 0;           // skip key press as it has already been reported!
                    // else, do deghosting so as to report the key press if need be
                }
                break;
            case RELEASE_DEBOUNCING:
                if (deb->cnt == 0)          // debouncing done! check key status...
                {
                    if (pressed)            // still pressed? fake release! return to WAIT_RELEASE
                    {
                        deb->state = WAIT_RELEASE;
                        return 0;
                    } 
                    else 
                    {
                        deb->state = IDLE;  // return to IDLE and continue to register the key release in the buffer if need be
                        kbd_bounce_intersections[i] = 0xFFFF;
                        kbd_bounce_rows[output] &= ~imask;
                        if (kbd_scandata[output] & imask)   // check if the key press had been reported
                            return 0;       // it hadn't so skip the release report!
                    }
                } 
                else 
                {
                    kbd_new_scandata[output] &= ~imask;  // this bit is still toggling! reset it to the previous stable state so that
                                            // is doesn't affect deghosting of other valid keys below
                                            
                    return 0;               // still debouncing...
                }
                break;
            default:
                break;
        }
    }
    else                                    // find an empty place
    {
        if (!pressed)
            return 0;                       // if it's a release then there should be a valid entry for this key in the array!
                                            // if there isn't then this happened because the array was full and the key is ignored

        for (i = 0; i < DEBOUNCE_BUFFER_SIZE ; ++i)
            if (kbd_bounce_intersections[i] == 0xFFFF) 
            {
                if (kbd_bounce_counters[i].state != IDLE)
                    ASSERT_ERROR(0);        // this should not happen!
                
                break;
            }

        if (i == DEBOUNCE_BUFFER_SIZE)      // no space available!
            return 0;

        kbd_bounce_intersections[i] = my_intersection;
        kbd_bounce_rows[output] |= imask;
        kbd_bounce_counters[i].cnt = DEBOUNCE_COUNTER_PRESS;
        kbd_bounce_counters[i].state = PRESS_DEBOUNCING;
        kbd_new_scandata[output] |= imask;  // this bit is still toggling! reset it to the previous stable state so that
                                            // is doesn't affect deghosting of other valid keys below
        
        return 0;                           // debouncing is started! the key will be put into the key buffer when it's finished!
    }

    // No bounce, continue to check for ghosting
    return 1;
}


/**
 ****************************************************************************************
 * @brief Does deghosting for the given key. If everything is in order, adds the
 *        corresponding keycode in the keycode_buffer.
 *
 * @param[in] output
 * @param[in] input
 * @param[in] pressed
 *
 * @return  0, if the key is ignored due to ghosting or keycode_buffer full
 *          1, if the keycode has been added to the keycode_buffer
 ****************************************************************************************
 */
static inline int record_key(const uint16 output, const uint16 input, const int pressed)
{
    const scan_t imask = 1 << input;
    int next_tail;

    if (pressed)
    {
        // Deghosting
        //
        // Three distinct cases:
        // A.     |  |                         |  |
        //        |  |                         |  |
        //     ---x--+---                   ---x--o---  
        //        |  |            ---->        |  |
        //     ---x--+---                   ---x--+---
        //        |  |                         |  |
        //        |  |                         |  |
        //
        // B.     |  |                         |  |
        //        |  |                         |  |
        //     ---x--x---                   ---x--x---
        //        |  |            ---->        |  |
        //     ---+--+---                   ---o--+---
        //        |  |                         |  |
        //        |  |                         |  |
        //
        // C.     |  |                         |  |
        //        |  |                         |  |
        //     ---x--+---                   ---x--+---
        //        |  |            ---->        |  |
        //     ---+--x---                   ---o--x---
        //        |  |                         |  |
        //        |  |                         |  |
        //
        // In all cases, the press of the 3rd key results in a "ghost" (the 4th key
        // of the square) because the three keys share both a row and a column.
        // Thus, the press of the key must be ignored until either of the other two 
        // keys is released. This is called "2-key roll-over (2KRO)". Obviously, 
        // the key press must be ignored ONLY if the 4th key is valid (there is a 
        // valid entry in the kbd_keymap[]; if there isn't then there is no key at
        // all at this position and only the specific third key could have caused 
        // this situation). Else, it should be reported normally.
        //

        int i, o;
        scan_t scandata;
        scan_t mask = 1;

        // also, skip this key if:
        // a. a "square of active keys" in formed in the newly scanned matrix that 
        //    includes this key
        // b. the corresponding "square" in the last reported status contains only 1 
        //    active key. this means that there's another new key in the row we're 
        //    examining that creates this "ghost" status (implicit 'B' and 'C' cases)
        scandata = kbd_new_scandata[output] & ~imask;
        if (scandata != ~imask)              // other columns are being driven in the row under examination
        {
            for (i = 0; i < KBD_NR_INPUTS; ++i, mask <<= 1)
            {
                if (input == i)
                    continue;               // skip this column (it's the input under investigation)

                if ( !(scandata & mask) )   // column (input) found
                {
                    // check if any other newly scanned row reports a key pressed in any of the two columns { input - i }
                    // if so, then a "square" is formed. 
                    for (o = 0; o < KBD_NR_OUTPUTS; ++o)
                    {
                        if (output == o)
                            continue;       // skip this row (it's the output under investigation)

                        scan_t combined_mask = (mask | imask);
                        scan_t new_scandata = ((~kbd_new_scandata[o]) & ((1 << KBD_NR_INPUTS) - 1));
                        if ( (new_scandata & combined_mask) != 0) { // if any is used => "square" of { (output, input) - (o, i) }
                            // the other "corner" is not detected because the scanning of the rows 
                            // is done in series (one after the other). thus, one row may be scanned 
                            // just before the key is released and the other just after. they are 
                            // scanned during the same scan cycle but the first row does not
                            // indicate the current status of the matrix but the previous one!
                            int cnt = 0;
                            
                            if (kbd_keymap[0][output][i]) cnt++;
                            if (kbd_keymap[0][o][input]) cnt++;
                            if (kbd_keymap[0][o][i]) cnt++;
                            
                            if (cnt == 3)
                                return 0;
                        }
                    }
                }
            }
        }

        // scan all rows once more just in case an input is "missed" in the row under examination
        for (o = 0; o < KBD_NR_OUTPUTS; ++o)
        {
            if (output == o)
                continue;                   // skip this row (it's the row under investigation)

            scandata = kbd_new_scandata[o];
            if ( !(scandata & imask) )      // this row has the same column (input) driven 
            {
                // check if any other column has a key pressed in any of the two rows { output - o } (or both)
                // if this is the case then a "square" is formed.
                mask = 1;
                for (i = 0; i < KBD_NR_INPUTS; ++i, mask <<= 1)
                {
                    if (input == i)
                        continue;           // skip this column (it's the input under investigation)

                    scan_t d1 = kbd_scandata[output] & mask;
                    scan_t d2 = scandata & mask;

                    if ( !d1 || !d2 )       // at least one row uses an extra column. a "square" is formed
                    {
                        int cnt = 0;
                        
                        if (kbd_keymap[0][output][i]) cnt++;
                        if (kbd_keymap[0][o][input]) cnt++;
                        if (kbd_keymap[0][o][i]) cnt++;
                        
                        if (cnt == 3)
                            return 0;
                    }
                }
            }
        }
    }

    // if no ghosting, then continue to buffer.
    bool block_key = false;
    
    if ((kbd_keymap[kbd_fn_modifier][output][input] >> 8) == 0xF8)  // 'Fn'
    {
        uint8_t keychar = kbd_keymap[kbd_fn_modifier][output][input] & 0xFF;
        kbd_fn_modifier = (kbd_fn_modifier & (~keychar)) | (pressed ? keychar : 0);
        block_key = true;   // do not send KEY_PRESS_EVT for 'Fn' press / release
                            // advertising does not start by pressing 'Fn' since 
                            // we are waiting to see if a "Clear EEPROM" command follows
    }
        
    // exception: 'Fn'+'c' is served asynchronously to make sure the EEPROM can be cleared even when we are disconnected
    if (HAS_EEPROM)
    {
        if (kbd_keymap[kbd_fn_modifier][output][input] == CLRP)
        {
            if (!pressed)
            {
                app_alt_pair_clear_all_bond_data();
                reset_bonding_request = true;
            }
            return 1;   // "Clear EEPROM" is not logged into the buffer any more and not reported as KEY_PRESS_EVT
        }
    }
                            
    if (!block_key)
    {
        if (current_fsm_state == IDLE_ST)
        {
            app_state_update(KEY_PRESS_EVT);
            if (reset_bonding_request) {
                reset_bonding_data();
                reset_bonding_request = false;
            }
        }
        else
            sync_key_press_evt = true;  // synchronize the call to app_state_update(KEY_PRESS_EVT);
    }
                            
    // 'Fn' key is written to the buffer. If we are connected an HID full release needs to be sent!
    next_tail = (kbd_keycode_buffer_tail + 1) % KEYCODE_BUFFER_SIZE;
    if (next_tail != kbd_keycode_buffer_head)
    {
        kbd_keycode_buffer[kbd_keycode_buffer_tail].flags = (pressed ? KEY_STATUS_MASK : 0) | (kbd_fn_modifier & KEY_FN_SET_MASK);
        kbd_keycode_buffer[kbd_keycode_buffer_tail].input = input;
        kbd_keycode_buffer[kbd_keycode_buffer_tail].output = output;
        kbd_keycode_buffer_tail = next_tail;
        
        return 1;
    }
    else
        keycode_buf_overflow = true;
    
    return 0;
}


/**
 ****************************************************************************************
 * @brief Processes key combinations (if set)
 *
 * @param None
 *
 * @return void
 ****************************************************************************************
 */
void process_key_combinations(void)
{
    int i, j;
    
    for (i = 0; i < KBD_NR_OUTPUTS; i++)
        for (j = 0; j < KBD_NR_COMBINATIONS; j++)
            if ( (kbd_scandata[i] | key_comb[j].scan_status) == key_comb[j].scan_status )
            {
                // execute action
                if (key_comb[j].action == 0xDEED)
                    if (HAS_KEYBOARD_MEASURE_EXT_SLP)
                    {
                        user_extended_sleep = true;
                    }
            }
}


/**
 ****************************************************************************************
 * @brief Processes scan results. If debouncing and deghosting allow it,
 *        the keycodes of the detected keys are placed in the keycode buffer. 
 *
 * @param None
 *
 * @return void
 ****************************************************************************************
 */
static inline void kbd_process_scandata(void)
{	
    scan_t new_scan_status[KBD_NR_OUTPUTS];
    int i;
    uint8_t leading_zeros;
    
    // do debouncing for all keys
    for (i = 0; i < KBD_NR_OUTPUTS; ++i) 
    {
        if (kbd_out_bitmasks[i])
        {
            scan_t xorword = (kbd_scandata[i] ^ kbd_new_scandata[i]);
            
            new_scan_status[i] = kbd_new_scandata[i];
            
            // look into kbd_bounce_intersections[] for any keys that are being debounced 
            // that might not be reported in the xorword
            xorword |= kbd_bounce_rows[i];
            
            if (xorword) // if any key state changed
            {
                scan_t bit;
                scan_t mask;
                int press;
                
                kbd_active_row[i] = true;
                
                do
                {
                    leading_zeros = __clz((uint32_t)xorword);
                    if (leading_zeros == 32)
                        break;
                    else {
                        bit = 31 - leading_zeros;
                        mask = 1 << bit;
                    }
                        
                    press = !(new_scan_status[i] & mask);
                    if (!debounce_key(i, bit, press))
                    {
                        // This key is ignored, copy the bit from kbd_scandata[i] to newscanword => always keep last key status!
                        new_scan_status[i] = (new_scan_status[i] & (~mask)) | (kbd_scandata[i] & mask);
                    }
                    xorword &= ~mask;
                    
                } while (xorword);
            }
            else
                kbd_active_row[i] = false;
        }
    }
        
    // do deghosting for valid debounced keys and record those that are valid
    for(i = 0; i < KBD_NR_OUTPUTS; ++i) 
    {
        if (kbd_out_bitmasks[i])
        {
            scan_t xorword = (kbd_scandata[i] ^ new_scan_status[i]);
            
            if (xorword) // if any key state changed
            {
                scan_t bit;
                scan_t mask;
                int press;
                
                do
                {
                    leading_zeros = __clz((uint32_t)xorword);
                    if (leading_zeros == 32)
                        break;
                    else {
                        bit = 31 - leading_zeros;
                        mask = 1 << bit;
                    }
                        
                    press = !(new_scan_status[i] & mask);
                    if (!record_key(i, bit, press))
                    {
                        // This keypress is ignored, copy the bit from kbd_scandata[i] to newscanword => always keep last key status!
                        new_scan_status[i] = (new_scan_status[i] & (~mask)) | (kbd_scandata[i] & mask);
                    }
                    xorword &= ~mask;
                    
                } while (xorword);
            }
        }
    }

    for (i = 0; i < KBD_NR_OUTPUTS; i++) 
        kbd_scandata[i] = new_scan_status[i]; 
    
    if (key_comb != NULL)
        process_key_combinations();
}


/**
 ****************************************************************************************
 * @brief Updates the value of the scan cycle time and starts SysTick.
 *
 * @param None
 *
 * @return void
 ****************************************************************************************
 */
static void update_scan_times(void)
{
    // Re-init scan period time
    if (HAS_ALTERNATIVE_SCAN_TIMES)
    {
        if (full_scan)
            scan_cycle_time = (FULL_SCAN_TIME * SYSTICK_TICKS_PER_US) - (ROW_SCAN_TIME * SYSTICK_TICKS_PER_US);
        else
            scan_cycle_time = (PARTIAL_SCAN_TIME * SYSTICK_TICKS_PER_US) - (ROW_SCAN_TIME * SYSTICK_TICKS_PER_US);                
    }
    else
    {
        scan_cycle_time = (FULL_SCAN_TIME * SYSTICK_TICKS_PER_US) - (ROW_SCAN_TIME * SYSTICK_TICKS_PER_US);
    }

    scan_cycle_time_last = scan_cycle_time;
    
    systick_start( (ROW_SCAN_TIME * SYSTICK_TICKS_PER_US), 2);
}


/**
 ****************************************************************************************
 * @brief Update full_scan status according to whether an interrupt was
 *        reported from the keyboard controller or not. Decide whether the next state
 *        will be KEY_SCANNING (because there's keyboard activity) or INACTIVE.
 *
 * @param None
 *
 * @return true, if scanning will continue
 *         false, if scanning stops and IDLE is the next state
 ****************************************************************************************
 */
bool app_kbd_update_status(void)
{
    bool ret;
    
    // Stop SysTick
    systick_stop();

    GLOBAL_INT_DISABLE();
    if (kbd_cntrl_active) 
    {
        // Stop KBD Controller
        kbd_cntrl_active = false;
        SetWord16(KBRD_IRQ_IN_SEL2_REG, 0);
        SetWord16(KBRD_IRQ_IN_SEL1_REG, 0);
        SetWord16(KBRD_IRQ_IN_SEL0_REG, 0x4000);
        NVIC_DisableIRQ(KEYBRD_IRQn);
    } 
    GLOBAL_INT_RESTORE();
    
    if (full_scan) 
    {
        if (!kbd_new_key_detected) 
        {
            // set a global debounce counter. until it reaches zero or a new key is detected
            // (whatever happens first) we remain in full scan mode and keep on scanning!
            full_scan = true;
            
            if (kbd_global_deb_cnt == 0)
            {
                kbd_bounce_active = 1;
                kbd_global_deb_cnt = DEBOUNCE_COUNTER_PRESS + 1;    // FIXME: to test!
            }
            else if (kbd_global_deb_cnt == 1)
            {
                kbd_bounce_active--;
                kbd_global_deb_cnt = 0;
                full_scan = false;
            }
        } 
        else 
        {
            // reset global debounce counter
            kbd_global_deb_cnt = 0;
            
            // disable full scan unlees a keyboard interrupt occurred during the last partial 
            // scan. in this case do a full scan next time!
            full_scan = next_is_full_scan;
        }
    } 
    else
        full_scan = next_is_full_scan;

    kbd_new_key_detected = false;
    
    // Start SysTick
    update_scan_times();
    
    // Turn all outputs to 'high-Z' so as to be ready for the next scan and to preserve some power in the meantime
    for (int i = 0; i < KBD_NR_OUTPUTS; ++i)
        set_row_to_input_highz(i);
            
    // if a keyboard interrupt occurred during the last partial scan do a full scan next time!
    if (next_is_full_scan) 
    {
        ret = true;
        next_is_full_scan = false;
    }
    else if (kbd_bounce_active) // debouncing (press or release) or a key is being pressed
        ret = true; // scan again.
    else 
    {
        if (HAS_SCAN_ALWAYS_ACTIVE)
        {
            ret = true;
        }
        else
        {
            systick_stop();
            systick_hit = false;
            ret = false;
        }
    }
    
    return ret;
}


/**
 ****************************************************************************************
 * @brief Activates key scanning hardware and state machine (by enabling the SysTick IRQ).
 *
 * @param None
 *
 * @return void
 ****************************************************************************************
 */
static void kbd_start_sw_scanning(void)
{
//    dbg_puts(DBG_SCAN_LVL, "KBD SW scan active\r\n");

    full_scan = true;                   // Force a full key scan
    
    update_scan_times();                // Start SysTick
    GLOBAL_INT_DISABLE();
    kbd_membrane_output_wakeup();       // Set outputs to 'high-Z' to enable SW scanning

    NVIC_SetPriority(SysTick_IRQn, 3);         
    NVIC_EnableIRQ(SysTick_IRQn);
    GLOBAL_INT_RESTORE();
}



/*
 * Reporting
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Flushes all the entries in the keycode_buffer
 *        
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_kbd_flush_buffer(void)
{
    kbd_keycode_buffer_tail = kbd_keycode_buffer_head;
}


/**
 ****************************************************************************************
 * @brief Flushes all the pending HID reports
 *        
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_kbd_flush_reports(void)
{
    kbd_rep_info *pReportInfo;
      
    // Clear trm list
    while (kbd_trm_list)
    {
        pReportInfo = kbd_pull_from_list(&kbd_trm_list);
        pReportInfo->type = FREE;
        kbd_push_to_list(&kbd_free_list, pReportInfo);
    }
    
    // Clear (or invalidate) the content of the last reports sent to the old host
    normal_key_report_st[0] = 0xFF;     // invalidate  
    extended_key_report_st[0] = 0;
    extended_key_report_st[1] = 0;
    extended_key_report_st[2] = 0;
    
    // Clear Roll-Over status
    for (int i = 0; i < ROLL_OVER_BUF_SZ; i++)
        roll_over_info.intersections[i] = RLOVR_INVALID_INTERSECTION;
    
    roll_over_info.cnt = 0;    
}


/**
 ****************************************************************************************
 * @brief Calculates the number of unread entries in the keycode_buffer.
 *        
 * @param   None
 *
 * @return  The number of unread entries in the keycode_buffer
 ****************************************************************************************
 */
__forceinline static int keycode_buffer_written_sz(void)
{
	int ret_val;
	
	if (kbd_keycode_buffer_tail >= kbd_keycode_buffer_head)
		ret_val = kbd_keycode_buffer_tail - kbd_keycode_buffer_head;
	else
		ret_val = KEYCODE_BUFFER_SIZE + kbd_keycode_buffer_tail - kbd_keycode_buffer_head;
	
	return ret_val;
}


/**
 ****************************************************************************************
 * @brief Gets the last report pending of the specified type.
 *        
 * @param   char_id The report type to search for (NORMAL_REPORT, EXTENDED_REPORT)
 *
 * @return  A pointer to the report if found, else NULL.
 ****************************************************************************************
 */
kbd_rep_info* get_last_report(enum REPORT_TYPE char_id)
{
    kbd_rep_info *p_report, *p_tmp;
    
    p_report = NULL;
    p_tmp = kbd_trm_list;
    
    // Get the last pending key report of this type, if any
    while (p_tmp)
    {
        if (p_tmp->char_id == char_id) 
            p_report = p_tmp;
            
        p_tmp = p_tmp->pNext;
    }
    
    return p_report;
}


/**
 ****************************************************************************************
 * @brief Allocates and initializes a normal report
 *        
 * @param[in]   last        The last pending report of the desired type (see type param) in the reports list
 * @param[in]   type        The type of the report to add (PRESS, RELEASE)
 * @param[in]   modifier    false: normal keys, true: modifier keys 
 *
 * @return  A pointer to the report if one was available, else NULL.
 ****************************************************************************************
 */
kbd_rep_info* prepare_normal_report(kbd_rep_info *last, enum KEY_BUFF_TYPE type, bool modifier)
{
    kbd_rep_info *p_report;

    // add one <type> report
    p_report = kbd_pull_from_list(&kbd_free_list);
    ASSERT_WARNING(p_report);
    if (p_report)
    {
        p_report->type = type;
        p_report->modifier_report = modifier;
        p_report->char_id = NORMAL_REPORT;
        p_report->len = 8;
        
        if (last == NULL)   // first entry - copy last one sent
        {
            if (normal_key_report_st[0] != 0xFF) 
            {
                memcpy(p_report->pBuf, normal_key_report_st, 8);
            } 
            else 
            {
                memset(p_report->pBuf, 0, 8);
            }
        } 
        else /*if (_pReportInfo)*/ // last report pending 
            memcpy(p_report->pBuf, last->pBuf, 8);

        kbd_push_to_list(&kbd_trm_list, p_report);
    }
    
    return p_report;
}


/**
 ****************************************************************************************
 * @brief Allocates and initializes an extended report
 *        
 * @param[in]   last   The last pending report of the EXTENDED report type
 *
 * @return  A pointer to the report if one was available, else NULL
 ****************************************************************************************
 */
kbd_rep_info* prepare_extended_report(kbd_rep_info *last)
{
    kbd_rep_info *p_report;

    // add one <type> report
    p_report = kbd_pull_from_list(&kbd_free_list);
    ASSERT_WARNING(p_report);
    if (p_report)
    {
        p_report->type = EXTENDED;
        p_report->modifier_report = false;
        p_report->char_id = EXTENDED_REPORT;
        p_report->len = 3;
        
        if (last == NULL)   // first entry - copy last one sent
            memcpy(p_report->pBuf, extended_key_report_st, 3);
        else /*if (_pReportInfo)*/ // last report pending 
            memcpy(p_report->pBuf, last->pBuf, 3);

        kbd_push_to_list(&kbd_trm_list, p_report);
    }
    
    return p_report;
}


/**
 ****************************************************************************************
 * @brief Adds a NORMAL key report of the in the trm list. 
 *
 * @param[in]   type        The type of the report to add (PRESS, RELEASE)       
 * @param[in]   modifier    false: normal keys, true: modifier keys 
 *
 * @return  A pointer to the newly allocated report
 ****************************************************************************************
 */
static kbd_rep_info* add_normal_report(enum KEY_BUFF_TYPE type, bool modifier)
{
    kbd_rep_info *pReportInfo, *_pReportInfo;
    
    _pReportInfo = get_last_report(NORMAL_REPORT);

    // add one <type> report
    pReportInfo = prepare_normal_report(_pReportInfo, type, modifier);

    return pReportInfo;
}


/**
 ****************************************************************************************
 * @brief Brings all used entries at the beginning of the report
 *
 * @param[in]   buf     The buffer       
 * @param[in]   start   the entry index at which to start sorting       
 * @param[in]   len     the count of the entries to sort     
 *
 * @return  void
 ****************************************************************************************
 */
static void sort_report_data(uint8_t *buf, int start, int len)
{
    // bring all used entries at the beginning of the report
    for (int i = start; i < start + len; i++) 
    {
        if (buf[i])
            continue;   // not zero
            
        for (int j = i + 1; j < start + len; j++)
        {
            if (buf[j] != 0) 
            {
                buf[i] = buf[j];
                buf[j] = 0;
                break;
            }
        }
    }
}


/**
 ****************************************************************************************
 * @brief Sorts the rollover data
 *
 * @param[in]   buf     The buffer       
 * @param[in]   len     the count of the entries to sort     
 *
 * @return  void
 ****************************************************************************************
 */
static void sort_rollover_data(uint16_t *buf, int start, int len)
{
    // bring all used entries at the beginning of the report
    for (int i = start; i < start + len; i++) 
    {
        if (buf[i] != RLOVR_INVALID_INTERSECTION)
            continue;   // valid
            
        for (int j = i + 1; j < start + len; j++)
        {
            if (buf[j] != RLOVR_INVALID_INTERSECTION) 
            {
                buf[i] = buf[j];
                buf[j] = RLOVR_INVALID_INTERSECTION;
                break;
            }
        }
    }
}


/**
 ****************************************************************************************
 * @brief Based on the type and the status od the key determine whether 
 *        1. a new press report needs to be allocated to report a key press
 *        2. a new release report needs to be allocated to report a key release or 
 *        3. a pending release report can be used for this purpose.
 *        Prepare the report and add it to the corresponding trm list. 
 *
 * @param[in]	keymode
 * @param[in]	keychar
 * @param[in] 	pressed
 *
 * @return  0, when full
 *          1, when done
 ****************************************************************************************
 */
static int modify_kbd_keyreport(const char keymode, const char keychar, uint8_t pressed)
{
    int i;
    kbd_rep_info *pReportInfo, *_pReportInfo;

    // 1. The Key Report is filled from pos 2 to pos 7. It monitors the state of up to 6 keys. If more are pressed then
    //    RollOver functionality (Phantom state) should be applied.
    // 2. When something is written at a pos the host translates it as a key press.
    // 3. When something is written at (pos+1) the host translates it as a key press for key@(pos+1)
    // 4. If the report has '0' in all other positions then the host we'll be informed for only these two key presses.  
    //    The host will think that the last entry written (i.e. key@(pos+1)) is still being pressed and update it 
    //    (i.e. in a Word Processing application) with the "key repetition rate" as set in its settings.
    // 5. For the host to be informed for a key release of key@(pos+1) there are two cases:
    //    a. A Key Report is sent with all '0's which clears all key presses.
    //    b. The last Key Report is sent with 0x00@(pos+1) which means that the rest of the keys are still being pressed.
    // 6. Allowed trm sequence for all keys: PRESS (x N) -> RELEASE -> PRESS (x N) -> RELEASE..., 
    //                                       RELEASE -> PRESS (x N) -> RELEASE -> PRESS (x N) -> RELEASE...

    switch(keymode & 0xFC)
    {
        case 0x00: // normal key
        {
            if (pressed) 
            {
                if (roll_over_info.cnt > 7)
                    return 1; // consumed
                
                // add one press report
                pReportInfo = add_normal_report(PRESS, false);
                if (!pReportInfo)
                    return 0;

                if (roll_over_info.cnt == 7)
                {
                    // format report as a Roll-Over Report. Modifiers are still reported.
                    for (i = 2; i < 8; i++)
                        pReportInfo->pBuf[i] = 0x01;
                    
                    return 1;
                }

                for (i = 2; i < 8; i++)
                    if (!pReportInfo->pBuf[i])
                        break;

                if (i == 8) // Full! Should never happen!
                    ASSERT_ERROR(0);
                
                pReportInfo->pBuf[i] = keychar; 				
			} 
            else   // released
            {
                // check if in Phantom state (Roll-Over)
                if (roll_over_info.cnt > 6)
                    ASSERT_ERROR(0);
                
                // add one RELEASE report
                pReportInfo = add_normal_report(RELEASE, false);
                if (!pReportInfo)
                    return 0;
                
                if ( (roll_over_info.cnt <= 6) && (roll_over_info.intersections[6] == RLOVR_INDICATION_CODE) )
                {
                    // Phantom state is over. Send a RELEASE report with the current status.
                    
                    for (i = 2; i < 8; i++)
                        pReportInfo->pBuf[i] = 0;

                    roll_over_info.intersections[6] = RLOVR_INVALID_INTERSECTION;
                    sort_rollover_data(roll_over_info.intersections, 0, 6);
                    
                    for (i = 0; i < roll_over_info.cnt; i++)
                    {
                        int fn_mod = roll_over_info.intersections[i] >> 14;
                        uint8_t input = roll_over_info.intersections[i];
                        uint8_t output = (roll_over_info.intersections[i] >> 8) & 0x3F;
                        
                        pReportInfo->pBuf[i+2] = kbd_keymap[fn_mod][output][input];
                    }
                    
                    return 1;
                }
                
                for (i = 2; i < 8; i++)
                {
                    if (pReportInfo->pBuf[i] == keychar) 
                    {
                        pReportInfo->pBuf[i] = 0x00;
                        break;
                    }
                }
                
                if (i == 8) // consume release (key not found)
                    return 1;
                
                // bring all used entries at the beginning of the report
                sort_report_data(pReportInfo->pBuf, 2, 6);
			}
            
            break;
        }
			
        case 0xFC: // modifier key
        {
            // if pressed, add a PRESS report after copying the data of the last report pending
            // if released, add a RELEASE report after copying the data of the last report pending
            char modifier, new_modifier;

            modifier = 0;
            
            // get last report to find the modifiers' status
            _pReportInfo = get_last_report(NORMAL_REPORT);
            if (_pReportInfo)
                modifier = _pReportInfo->pBuf[0];
            else if (normal_key_report_st[0] != 0xFF)
                modifier = normal_key_report_st[0];
            // else pLastKeyStatus = NULL
            
            new_modifier = (modifier & (~keychar)) | (pressed ? keychar : 0);
            
            if (new_modifier != modifier)   // normally this will always be true
            {
                enum KEY_BUFF_TYPE type;
                
                // add "modifier" report in the trm list
                if (pressed) 
                    type = PRESS;
                else
                    type = RELEASE;
                
                pReportInfo = prepare_normal_report(_pReportInfo, type, modifier);
                if (!pReportInfo)
                    return 0;
                
                pReportInfo->pBuf[0] = new_modifier;
            }
            break;
        }

        default: // Other key that is not directly reportable in the kbd_key_report
            break;
    }
	
    return 1;
}

/**
 ****************************************************************************************
 * @brief Check if there is data available in the keycode_buffer
 *
 * @param   None       
 *
 * @return  false, if buffer is empty
 *          true, if data exists in the buffer
 ****************************************************************************************
 */
bool app_kbd_buffer_has_data(void) 
{ 
    return (kbd_keycode_buffer_head != kbd_keycode_buffer_tail); 
}

/*
 * Description  : Insert key data in the keycode_buffer at the current or next read position
 *
 * Returns      : false, if buffer is full
 *              : true, if data was written in the buffer
 *
 */
//static bool kbd_buffer_insert_data(struct key_code_info *info, bool pressed, bool overwrite)
//{
//    uint8_t wr_pos = kbd_keycode_buffer_head;
//    uint8_t next_tail = (kbd_keycode_buffer_tail + 1) % KEYCODE_BUFFER_SIZE;
//    bool ret = false;
//    
//    if ( (next_tail == kbd_keycode_buffer_head) && !overwrite)
//        return ret;   // no space available
//    
//    if (overwrite)
//    {
//        kbd_keycode_buffer[wr_pos].flags = (pressed ? 0 : KEY_STATUS_MASK) | (kbd_fn_modifier & KEY_FN_SET_MASK);
//        kbd_keycode_buffer[wr_pos].input = info->input;     // Special keys must be mapped to unused inputs and outputs!!!
//        kbd_keycode_buffer[wr_pos].output = info->output;   // This is needed for Roll-Over check and to retrieve the keycode later
//        ret = true;
//    }
//    else
//    {
//        uint8_t rd_pos = wr_pos;
//        wr_pos++;
//        
//        while (kbd_keycode_buffer_head != kbd_keycode_buffer_tail)
//    }
//    
//    return ret;
//}

/**
 ****************************************************************************************
 * @brief Process a keycode that has been placed into the keycode_buffer 
 *
 * @param[in]   keycode_idx The keycode index
 *
 * @return  0, if a report had to be added but the reports list is full
 *          1, if the keycode has been processed
 ****************************************************************************************
 */
static int kbd_process_keycode(struct keycode_buffer_tag *keycode_idx)
{
    int ret = 1;
    const int fn_mod = keycode_idx->flags & KEY_FN_SET_MASK;
    const int pressed = keycode_idx->flags & KEY_STATUS_MASK;
    const uint8_t output = keycode_idx->output;
    const uint8_t input = keycode_idx->input;
    const uint16_t keycode = kbd_keymap[fn_mod][output][input];
    const int intersection = (output << 8) | (input);

    // Roll-Over processing (all key releases are cleared from the Roll-Over buffer)
    if (!pressed)
    {
        int i;
        
        // check if in Phantom state (Roll-Over)
        if (roll_over_info.cnt > 0)
        {
            int keymode;
            
            // find the key and delete it
            for (i = 0; i < roll_over_info.cnt; i++)
                if (intersection == (roll_over_info.intersections[i] & 0x3FFF))
                    break;
                
            keymode = (keycode >> 8) & 0xFC;
            
            if (i == roll_over_info.cnt) // not found
            {
                if (keymode == 0x00)
                {
                    return 1;   // consume all not found normal releases
                }
            }
            else // found
            {
                roll_over_info.intersections[i] = RLOVR_INVALID_INTERSECTION;
                sort_rollover_data(roll_over_info.intersections, 0, roll_over_info.cnt);
                roll_over_info.cnt--;
                
                if (roll_over_info.cnt > 6)
                    return 1;   // consume the release. Still in Phantom state.
                
                if (roll_over_info.cnt == 6)
                {
                    roll_over_info.intersections[6] = RLOVR_INDICATION_CODE;
                    
                    if (keycode)
                    {
                        if (keymode != 0x00)    // not a normal key ==> force an HID report update! (FIXME: extend to modifier key handling)
                            ret = modify_kbd_keyreport(0, 0, 0);
                    }
                }
            }
        }
    }
    
    
    if (keycode)
    {
        const char keymode = keycode >> 8;
        const char keychar = keycode & 0xFF;

#ifdef ASCII_DEBUG
        if (pressed)
        {
            switch(keymode & 0xFE)
            {
                case 0x00: // normal key
                    kbd_buffer_ascii[kbd_keybuffer_idx] = kbd_hut_to_ascii(keychar);
                    break;
                default: // Other key
                    kbd_buffer_ascii[kbd_keybuffer_idx] = '~';
                    break;
            }
            arch_printf("[%c]\r\n", kbd_buffer_ascii[kbd_keybuffer_idx]);
            kbd_keybuffer_idx = (kbd_keybuffer_idx + 1) & (MAX_KEYBUFFER-1);
        }
#endif

        switch(keymode & 0xFC)
        {
            case 0x00: // normal key
                // Roll-Over processing (only normal key presses are logged)
                if (pressed)
                {
                    int i;
                    
                    for (i = 0; i < roll_over_info.cnt; i++)
                        if (intersection == (roll_over_info.intersections[i] & 0x3FFF))
                            break;
                        
                    if (i < roll_over_info.cnt)
                        ASSERT_ERROR(0);
                        
                    if (roll_over_info.cnt < ROLL_OVER_BUF_SZ)
                    {
                        roll_over_info.intersections[roll_over_info.cnt] = (kbd_fn_modifier << 14) | (output << 8) | (input);
                        roll_over_info.cnt++;
                    }
                }
                // fall through
            case 0xFC: // modifier key
                ret = modify_kbd_keyreport(keymode, keychar, pressed);
				break;
			case 0xF8: // Fn Modifier
            {
                // When Fn key is pressed or released all key state must be reset
                // On release, optionally, a reset of kbd_scandata and a full key scan can be done to
                // re-detect any other keys that were pressed with the Fn key!
                kbd_rep_info *pReportInfo;
                
                // add a "full release" report in the trm list.
                pReportInfo = add_normal_report(RELEASE, false);
                if (!pReportInfo)
                    return 0;
                memset(pReportInfo->pBuf, 0, 8); 
                
                //clear Roll-Over info
                for (int i = 0; i < ROLL_OVER_BUF_SZ; i++)
                    roll_over_info.intersections[i] = RLOVR_INVALID_INTERSECTION;
                
                roll_over_info.cnt = 0;    
                break;
            }
            case 0xF4: // Special function
            {
                int byte = keychar >> 4;
                uint8_t code = keychar & 0x0F;
                uint8_t mask = 1 << (code);                     // Low order nibble gets values up to 7!
                kbd_rep_info *pReportInfo;
                
                if (byte == 0xF)                                // Exception: do something proprietary!
                {
                    switch (code) 
                    {
                    case 0:     // ~
//                        if (pressed) 
//                        {
//                            kbd_process_keycode(0xFC02);        // Left Shift
//                            kbd_process_keycode(0x0035);        // ~
//                        } 
//                        else 
//                        {
//                            kbd_process_keycode(0xFD02);        // Clear Left Shift
//                            kbd_process_keycode(0x0135);        // Clear ~
//                        }
                        break;
                    case 1:     // pair to another
                        if (HAS_MULTI_BOND)
                        {
                            if (!pressed)
                                user_disconnection_req = true;
                        }
                        break;
                    case 2:     // clear pairing data (EEPROM) - handled elsewhere now!
                        ASSERT_WARNING(0);
                        break;
                    case 3:     // put the chip in extended sleep constantly (for measurements)
                        if (HAS_KEYBOARD_MEASURE_EXT_SLP)
                        {
                            if (pressed)
                                user_extended_sleep = true;
                        }
                        break;
                    case 0xF:   // ignore 
                        break;
                    default:
                        ASSERT_WARNING(0);
                        break;
                    }
                } 
                else   // Normal case
                {
                    kbd_rep_info *_pReportInfo;
                    
                    // Add an extended key report for each press / release
                    _pReportInfo = get_last_report(EXTENDED_REPORT);
                    
                    pReportInfo = prepare_extended_report(_pReportInfo);
                    if (!pReportInfo)
                        return 0;
    
                    if (pressed)
                        pReportInfo->pBuf[byte] |= mask;
                    else
                        pReportInfo->pBuf[byte] &= ~mask;
                }
                break;
            }
            default: // Other key that is not directly reportable in the kbd_key_report
                ASSERT_WARNING(0); // what if we sent a full release HID report instead?
                break;
        }
    }
    return ret;
}


/**
 ****************************************************************************************
 * @brief Processes the keycodes that have been written in the keycode_buffer
 *        and prepares the necessary HID reports. 
 *
 * @param   None
 *
 * @return  0, if the processing of the keycodes in the keycode_buffer failed
 *          1, if the processes of the keycodes in the keycode_buffer succeeded
 ****************************************************************************************
 */
static int prepare_kbd_keyreport(void)
{
    int ret;
    
    if (kbd_free_list == NULL)
        return 0;

    if (kbd_keycode_buffer_head == kbd_keycode_buffer_tail) 
        return 0;

    // The report will be placed in the trm list. It's the FSM's task to clear the
    // list in case of disconnection with this host that is followed by a connection 
    // establishment to a new host.
    
    do 
    {
        ret = kbd_process_keycode(&kbd_keycode_buffer[kbd_keycode_buffer_head]);
        if (ret)
            kbd_keycode_buffer_head = (kbd_keycode_buffer_head + 1) % KEYCODE_BUFFER_SIZE;
    } 
    while ( ret && kbd_free_list && (keycode_buffer_written_sz() > 0) );

    return 1;
}


/**
 ****************************************************************************************
 * @brief Sends a boot report
 *
 * @param   None
 *
 * @return  0, if sending the HID boot report failed
 *          1, if the HID boot report was successfully sent
 ****************************************************************************************
 */
static int send_boot_report(void)
{
    kbd_rep_info *p;
    int ret = 0;
    
    if (HAS_HOGPD_BOOT_PROTO)
    {
        struct hogpd_boot_report_info *req;

        do 
        {
            // Allocate the message
            req = KE_MSG_ALLOC_DYN(HOGPD_BOOT_REPORT_UPD_REQ, TASK_HOGPD, TASK_APP, hogpd_boot_report_info, 8);
            
            if (!req)
                break;

            p = kbd_pull_from_list(&kbd_trm_list);
            
            ASSERT_WARNING(p);
            
            if (p->char_id == NORMAL_REPORT)
            {
                // Fill in the parameter structure
                req->conhdl = app_env.conhdl;
                req->hids_nb = 0;
                req->char_code = HOGPD_BOOT_KB_IN_REPORT_CHAR;
                req->report_length = p->len;
                memcpy(req->boot_report, p->pBuf, 8);

                dbg_printf(DBG_SCAN_LVL, "Sending HOGPD_REPORT_UPD_REQ %02x:[%02x:%02x:%02x:%02x:%02x:%02x]\r\n", 
                            (int)p->pBuf[0], (int)p->pBuf[2], (int)p->pBuf[3], (int)p->pBuf[4], (int)p->pBuf[5], (int)p->pBuf[6], (int)p->pBuf[7]);
                            
                ke_msg_send(req);

                memcpy(normal_key_report_st, p->pBuf, 8);
            }
                
            p->type = FREE;
            kbd_push_to_list(&kbd_free_list, p);
            
            ret = 1;
        } while (0);
    }
    
    return ret;
}


/**
 ****************************************************************************************
 * @brief Sends an hid report
 *
 * @param   None
 *
 * @return  0, if sending the HID report failed
 *          1, if the HID report was successfully sent
 ****************************************************************************************
 */
static int send_hid_report(void)
{
    kbd_rep_info *p;
    int ret = 0;
    
    struct hogpd_report_info *req;
    
    do 
    {
        // Allocate the message
        req = KE_MSG_ALLOC_DYN(HOGPD_REPORT_UPD_REQ, TASK_HOGPD, TASK_APP, hogpd_report_info, 8);
        
        if (!req)
            break;

        p = kbd_pull_from_list(&kbd_trm_list);
        
        ASSERT_WARNING(p);
        
        // Fill in the parameter structure
        req->conhdl = app_env.conhdl;
        req->hids_nb = 0;
        req->report_nb = p->char_id;
        req->report_length = p->len;
        memcpy(req->report, p->pBuf, 8);

        dbg_printf(DBG_SCAN_LVL, "Sending HOGPD_REPORT_UPD_REQ %02x:[%02x:%02x:%02x:%02x:%02x:%02x]\r\n", 
                    (int)p->pBuf[0], (int)p->pBuf[2], (int)p->pBuf[3], (int)p->pBuf[4], (int)p->pBuf[5], (int)p->pBuf[6], (int)p->pBuf[7]);
                    
        ke_msg_send(req);

        switch (p->char_id) 
        {
        case NORMAL_REPORT:
            memcpy(normal_key_report_st, p->pBuf, 8);
            break;
        case EXTENDED_REPORT:
            memcpy(extended_key_report_st, p->pBuf, 3);
            break;
        default:
            break;
        }
        
        p->type = FREE;
        kbd_push_to_list(&kbd_free_list, p);
        
        ret = 1;
    } while (0);

    return ret;
}




/**
 ****************************************************************************************
 * @brief Sends the first key report in the trm list, if any, to HOGPD.
 *        A notification will be sent to the Host, if notifications are enabled. 
 *
 * @param   None
 *
 * @return  0, if sending the key report failed
 *          1, if the key report was successfully sent
 ****************************************************************************************
 */
__forceinline static int send_kbd_keyreport(void)
{
    int ret = 0;
    
    if (HAS_HOGPD_BOOT_PROTO)
    {
        if (kbd_proto_mode == HOGP_BOOT_PROTOCOL_MODE)
        {
            ret = send_boot_report();
        }
        else
        {
            ret = send_hid_report();
        }
    }
    else
    {
        ret = send_hid_report();
    }
    
    return ret;
}


/*
 * Keyboard Controller
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief It programs the Keyboard Controller to scan any key press 
 *        coming from the "inactive rows" and inform the system after 
 *        DEBOUNCE_TIME_PRESS msec (optionally) to do a full key scan.
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
static void kbd_enable_kbd_irq(void)
{
    SetWord16(GPIO_DEBOUNCE_REG, 0x2000 | DEBOUNCE_TIME_PRESS); // T ms debouncing

    SetWord16(KBRD_IRQ_IN_SEL2_REG, mask_p3);
    SetWord16(KBRD_IRQ_IN_SEL1_REG, mask_p12);
    SetWord16(KBRD_IRQ_IN_SEL0_REG, mask_p0);

    SetWord16(GPIO_RESET_IRQ_REG, 0x20);    // clear any garbage
    NVIC_ClearPendingIRQ(KEYBRD_IRQn);      // clear it to be on the safe side...
    NVIC_SetPriority(KEYBRD_IRQn, 2);         
    NVIC_EnableIRQ(KEYBRD_IRQn);
    
    kbd_cntrl_active = true;
}


/**
 ****************************************************************************************
 * @brief ISR of the Keyboard Controller IRQ. It clears the interrupt and triggers
 *        a full key scan.
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void KEYBRD_Handler(void)
{
    // Clear it first
    SetWord16(KBRD_IRQ_IN_SEL0_REG, 0); 
    SetWord16(KBRD_IRQ_IN_SEL1_REG, 0);
    SetWord16(GPIO_RESET_IRQ_REG, 0x20); 

    // No more KEYBRD interrupts from now on
    NVIC_DisableIRQ(KEYBRD_IRQn); 

    // We do not know which row has the key. Rescan all rows!
    next_is_full_scan = true;
        
    kbd_cntrl_active = false;
}



/*
 * Wakeup timer
 ****************************************************************************************
 */

#ifdef DELAYED_WAKEUP_ON

/**
 ****************************************************************************************
 * @brief Handler for the WKUP_QUADEC_IRQn interrupt (when DELAYED WAKEUP is enabled)
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void wakeup_handler(void)
{
    NVIC_DisableIRQ(WKUP_QUADEC_IRQn);
	
    /*
     * Init System Power Domain blocks: GPIO, WD Timer, Sys Timer, etc.
     * Power up and init Peripheral Power Domain blocks,
     * and finally release the pad latches.
     */
    if(GetBits16(SYS_STAT_REG, PER_IS_DOWN))
        periph_init();

    /*
     * Notify HID Application to start delay monitoring
     */
    if (current_fsm_state == IDLE_ST)
    {
        ASSERT_WARNING(monitor_kbd_delayed_start_st != MONITOR_IDLE);
        
        if (monitor_kbd_delayed_start_st == MONITOR_PRESS)  // press
        {
            trigger_kbd_delayed_start_st = PRESSED_TRIGGER;
        }
        else                                                // release
        {
            trigger_kbd_delayed_start_st = RELEASED_TRIGGER;
        }
        
        monitor_kbd_delayed_start_st = MONITOR_IDLE;
    }
    else
    {
        trigger_kbd_delayed_start_st = NO_TRIGGER;
        
        wkup_hit = true;
    }
}

#else

/**
 ****************************************************************************************
 * @brief Handler for the WKUP_QUADEC_IRQn interrupt (when DELAYED WAKEUP is enabled)
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void wakeup_handler(void)
{
    NVIC_DisableIRQ(WKUP_QUADEC_IRQn);

    /*
     * Init System Power Domain blocks: GPIO, WD Timer, Sys Timer, etc.
     * Power up and init Peripheral Power Domain blocks,
     * and finally release the pad latches.
     */
    if(GetBits16(SYS_STAT_REG, PER_IS_DOWN))
        periph_init();

    /*
     * Notify HID Application to start scanning
     */
    wkup_hit = true;
}

#endif


/**
 ****************************************************************************************
 * @brief Programs the Wakeup Timer to scan a specific key press for a predefined
 *        duration and wakes-up the system.
 *
 * @para[in]   press
 *
 * @return  void
 ****************************************************************************************
 */
static void app_kbd_enable_delayed_wakeup_irq(bool press)
{
    if (HAS_DELAYED_WAKEUP)
    {
        uint16_t port_pins[4] = {0, 0, 0, 0};
        
        monitor_kbd_delayed_start_st = press ? MONITOR_PRESS : MONITOR_RELEASE;
        
        SetBits16(CLK_PER_REG, WAKEUPCT_ENABLE, 1);                         // enable clock of Wakeup Controller
        
        wkupct_register_callback(wakeup_handler);

        SetWord16(WKUP_RESET_CNTR_REG, 0);                                  // Clear event counter (for safety...)
        SetWord16(WKUP_COMPARE_REG, 1);                                     // Wait for 1 event and wakeup

        const int gpio = kbd_input_ports[DELAYED_WAKEUP_GPIO_COLUMN];
        const int bit = gpio & 0x0F;
        const int port = (gpio & 0xF0) >> 4;

        ASSERT_WARNING(port < 4);
        
        if (port < 4)
            port_pins[port] |= (1 << bit);
        
        SetWord16(WKUP_SELECT_P0_REG, port_pins[0]);
        SetWord16(WKUP_SELECT_P1_REG, port_pins[1]);
        SetWord16(WKUP_SELECT_P2_REG, port_pins[2]);
        SetWord16(WKUP_SELECT_P3_REG, port_pins[3]);
        
        if (!press)
            port_pins[port] = 0;

        SetWord16(WKUP_POL_P0_REG, port_pins[0]);                           // generate INT when input goes either low (press) or high (release)
        SetWord16(WKUP_POL_P1_REG, port_pins[1]);    
        SetWord16(WKUP_POL_P2_REG, port_pins[2]);    
        SetWord16(WKUP_POL_P3_REG, port_pins[3]);    

        SetWord16(WKUP_RESET_IRQ_REG, 1);                                   // clear any garbagge
        NVIC_ClearPendingIRQ(WKUP_QUADEC_IRQn);                             // clear it to be on the safe side...

        if (press)
            SetWord16(WKUP_CTRL_REG, 0x80 | (DEBOUNCE_TIME_DELAYED_PRESS & 0x3F));  // Setup IRQ: Enable IRQ, T ms debounce
        else
            SetWord16(WKUP_CTRL_REG, 0x80 | (DEBOUNCE_TIME_DELAYED_RELEASE & 0x3F));// Setup IRQ: Enable IRQ, T ms debounce
            
        NVIC_SetPriority(WKUP_QUADEC_IRQn, 1);
        NVIC_EnableIRQ(WKUP_QUADEC_IRQn);    
    }
}


/**
 ****************************************************************************************
 * @brief Enables the delayed scanning
 *
 * @para[in]   press
 *
 * @return  void
 ****************************************************************************************
 */
void app_kbd_enable_delayed_scanning(bool press)
{
    if (HAS_DELAYED_WAKEUP)
    {
        int i;
        
//        dbg_puts(DBG_SCAN_LVL, "KBD active (HW scan)\r\n");
        
        // Read current status. If we are looking for release and column is high then report it immediately!
        ASSERT_WARNING(kbd_out_bitmasks[DELAYED_WAKEUP_GPIO_ROW]);
        
        kbd_membrane_input_setup();                                     // Setup the "inputs" of the key-matrix
        
        for (i = 0; i < KBD_NR_OUTPUTS; ++i)
            set_row_to_input_highz(i);                                  // Set all rows to 'high-Z'
        
        app_kbd_enable_delayed_wakeup_irq(press);                       // Setup the wakeup irq
        
        set_row_to_low(DELAYED_WAKEUP_GPIO_ROW);                        // Setup the "output" of the key-matrix

        GLOBAL_INT_DISABLE();
        if (!press)
        {
            uint16 const *base = (uint16 *)P0_DATA_REG;
        
            int port = kbd_input_ports[DELAYED_WAKEUP_GPIO_COLUMN];
            const int databit = (port & 0xF);

            ASSERT_WARNING(port < 0x40);
                
            if (port >= 0x30) port += 0x10; // fix for the mapping of port 3 regs!
                
            uint16 const *data_reg = &base[port & 0xF0];

            uint16_t status = *data_reg;
            
            if ( ( (status & (1 << databit)) != 0) && (GetWord16(WKUP_COUNTER_REG) == 0) ) {
                // Emulate interrupt hit
                monitor_kbd_delayed_start_st = MONITOR_IDLE;
                trigger_kbd_delayed_start_st = RELEASED_TRIGGER;
                
                SetBits16(WKUP_CTRL_REG, WKUP_ENABLE_IRQ, 0); //No more interrupts of this kind
                // close WKUP
                SetWord16(WKUP_RESET_IRQ_REG, 1); //Acknowledge it
                NVIC_DisableIRQ(WKUP_QUADEC_IRQn);
           }
        }
        GLOBAL_INT_RESTORE();
    }
}


/**
 ****************************************************************************************
 * @brief Programs the Wakeup Timer to scan any key press and wake-up the system
 *        (optionally after DEBOUNCE_TIME_PRESS msec)
 *
 * @param  None
 *
 * @return  void
 ****************************************************************************************
 */
static void app_kbd_enable_wakeup_irq(void)
{
	SetBits16(CLK_PER_REG, WAKEUPCT_ENABLE, 1); // enable clock of Wakeup Controller
    
    wkupct_register_callback(wakeup_handler);

    SetWord16(WKUP_RESET_CNTR_REG, 0);                              // Clear event counter (for safety...)
    SetWord16(WKUP_COMPARE_REG, 1);                                 // Wait for 1 event and wakeup

    SetWord16(WKUP_SELECT_P0_REG, wkup_mask_p0);
    SetWord16(WKUP_SELECT_P1_REG, wkup_mask_p1);
    SetWord16(WKUP_SELECT_P2_REG, wkup_mask_p2);
    SetWord16(WKUP_SELECT_P3_REG, wkup_mask_p3);

    SetWord16(WKUP_POL_P0_REG, wkup_mask_p0);                       // generate INT when input goes low
    SetWord16(WKUP_POL_P1_REG, wkup_mask_p1);    
    SetWord16(WKUP_POL_P2_REG, wkup_mask_p2);    
    SetWord16(WKUP_POL_P3_REG, wkup_mask_p3);    

    SetWord16(WKUP_RESET_IRQ_REG, 1);                               // clear any garbagge
    NVIC_ClearPendingIRQ(WKUP_QUADEC_IRQn);                         // clear it to be on the safe side...

    SetWord16(WKUP_CTRL_REG, 0x80 | (DEBOUNCE_TIME_PRESS & 0x3F));  // Setup IRQ: Enable IRQ, T ms debounce
    NVIC_SetPriority(WKUP_QUADEC_IRQn, 1);
    NVIC_EnableIRQ(WKUP_QUADEC_IRQn);    
}


/*
 * SysTick handler
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @brief ISR of the SysTick IRQ - Forces execution of the state machine once every
 *        SysTick interval
 *
 * @param  None
 *
 * @return  void
 ****************************************************************************************
 */
void SysTick_Handler(void) 
{ 
	ASSERT_ERROR(kbd_membrane_status != 0);
    
    systick_hit = true;
}



/**
 * Keyboard (HID) Application Functions
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initializes keycode buffer and keyreport variables
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_keyboard_init(void)
{
    systick_stop();                 // Make sure SysTick is stopped

    kbd_init_keyreport();           // Initialize key report buffers and vars and the fn modifier var

    kbd_init_retained_scan_vars();  // Initialize retained variables
    kbd_init_scan_vars();           // Initialize non-retained variables

    kbd_reports_en = REPORTS_PAUSED;// reporting mode is 'Disconnected' => Keys are buffered but no HID reports are generated
    if (HAS_KEYBOARD_MEASURE_EXT_SLP)
    {
        user_extended_sleep = false;    // default mode is ON
    }
    // Uncomment if scanning needs to be active from startup
// 	kbd_membrane_input_setup();     // Setup the "inputs" of the key-matrix
// 	app_kbd_enable_wakeup_irq();    // Setup the wakeup irq
// 	kbd_membrane_output_sleep();    // Setup the "outputs" of the key-matrix
}

extern uint8_t atts_find_uuid(uint16_t *start_hdl, uint16_t end_hdl, uint8_t uuid_len, uint8_t *uuid);
extern uint16_t atts_find_end(uint16_t start_hdl);
extern uint8_t atts_get_att_chk_perm(uint8_t conidx, uint8_t access, uint16_t handle, struct attm_elmt** attm_elmt);

/**
 ****************************************************************************************
 * @brief Creates the HID over GATT database
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_hid_create_db(void)
{
    // Add HOGPD in the database
    // see rw-ble-prf-hogpd-is section 2.2.1.1
    struct hogpd_create_db_req * req = KE_MSG_ALLOC(HOGPD_CREATE_DB_REQ, TASK_HOGPD,
                                                    TASK_APP, hogpd_create_db_req);
    struct hogpd_hids_cfg *cfg = &req->cfg[0];              // table 5.1
    struct hogpd_features *features = &cfg->features;       // table 5.2 features supported in the hid service
    struct hids_hid_info *hid_info = &cfg->hid_info;        // table 5.5 value of the hid information characteristic

    req->hids_nb = 1;

    features->svc_features = HOGPD_CFG_KEYBOARD | HOGPD_CFG_PROTO_MODE;
    if (HAS_BATT_EXTERNAL_REPORT)
    {
        features->svc_features |= HOGPD_CFG_MAP_EXT_REF;
    }
    if (HAS_HOGPD_BOOT_PROTO)
    {
        features->svc_features |= HOGPD_CFG_BOOT_KB_WR;
    } 

    features->report_nb          = 3; 
    features->report_char_cfg[0] = HOGPD_CFG_REPORT_IN | HOGPD_REPORT_NTF_CFG_MASK | HOGPD_CFG_REPORT_WR;
    features->report_char_cfg[1] = HOGPD_CFG_REPORT_OUT;
    features->report_char_cfg[2] = HOGPD_CFG_REPORT_IN | HOGPD_REPORT_NTF_CFG_MASK | HOGPD_CFG_REPORT_WR;
    features->report_char_cfg[3] = 0;
    features->report_char_cfg[4] = 0;

    hid_info->bcdHID = 0x100;
    hid_info->bCountryCode = 0;
    if (HAS_REMOTE_WAKEUP)
    {
        hid_info->flags = HIDS_REMOTE_WAKE_CAPABLE;
    }
    if (HAS_NORMALLY_CONNECTABLE)
    {
        hid_info->flags |= HIDS_NORM_CONNECTABLE;
    }

    if (HAS_BATT_EXTERNAL_REPORT)
    {
        struct att_incl_desc *ext_rep_ref = &cfg->ext_rep_ref;  // table 5.7 external report reference = included service value
        uint16_t start_hdl = 0;
        uint16_t end_hdl = 0xffff;
        uint16_t uuid = ATT_DECL_PRIMARY_SERVICE;
        uint16_t val = 0x180F;
        uint8_t ret;
        struct attm_elmt * attm_elmt = NULL;
        
        do 
        {
            ret = atts_find_uuid(&start_hdl, end_hdl, 2, (uint8_t *)&uuid);
        
            if (ret == ATT_ERR_NO_ERROR) 
            {
                /* retrieve attribute +  check permission */
                ret = atts_get_att_chk_perm(app_env.conidx, 0x00, start_hdl, &attm_elmt);
                
                /* look for the Battery Service (0x180F) */
                if (ret == ATT_ERR_NO_ERROR && ((uint16_t *)attm_elmt->value)[0] == val) 
                {
                    ext_rep_ref->start_hdl = start_hdl;
                    ext_rep_ref->end_hdl = atts_find_end(start_hdl);
                    ext_rep_ref->uuid = 0x180F;
                    break;
                } else
                    ret = ATT_ERR_ATTRIBUTE_NOT_FOUND;
            }
            start_hdl++;
        } while (start_hdl != 0 && ret != ATT_ERR_NO_ERROR);
        
        cfg->ext_rep_ref_uuid = ATT_CHAR_BATTERY_LEVEL; // external report reference value
    }    
    
    ke_msg_send(req);
}


/**
 ****************************************************************************************
 * @brief Enables the HOGPD profile 
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_keyboard_enable(void)
{
    // Allocate the message
    struct hogpd_enable_req * req = KE_MSG_ALLOC(HOGPD_ENABLE_REQ, TASK_HOGPD, TASK_APP,
                                                 hogpd_enable_req);

    // Fill in the parameter structure
    req->conhdl = app_env.conhdl;
#ifdef MITM_ON
    req->sec_lvl = PERM(SVC, AUTH); 
#else
# ifdef EEPROM_ON        
    req->sec_lvl = PERM(SVC, UNAUTH); 
# else        
    req->sec_lvl = PERM(SVC, ENABLE); 
# endif        
#endif                                                   
    req->con_type = PRF_CON_NORMAL; // The Server always does PRF_CON_NORMAL
    req->ntf_cfg[0].boot_kb_in_report_ntf_en = 0;
    req->ntf_cfg[0].boot_mouse_in_report_ntf_en = 0;
#ifdef DEBUG_WITH_TESTER    
    req->ntf_cfg[0].report_ntf_en[0] = 1;   // used with tester only!
    req->ntf_cfg[0].report_ntf_en[2] = 1;   // used with tester only!
#else
    req->ntf_cfg[0].report_ntf_en[0] = 0;
    req->ntf_cfg[0].report_ntf_en[2] = 0;
#endif    

    // Send the message
    ke_msg_send(req);
}


/**
 ****************************************************************************************
 * @brief Handles the HID Notification confirmation indication 
 *
 * @param   None
 *
 * @return  void
 *
 * @remarks The stack sends NTF to inform the app about the result status of the requested procedure
 * (i.e. send Report Notification). The function below does not check this status at all. 
 * It is not simple to implement this checking when sending multiple requests to the stack
 * (i.e. Report NTF for key press + Report Release NTF) since at this point you would like to  
 * be able to check for which of these requests you got the reported result (i.e. error) and 
 * in this case, probably try to retransmit it (considering that it has been buffered 
 * somewhere...).
 * In any case, the assumption that this NTF can be used as a trigger to send the next
 * pending Report Notification is completely wrong. This functionality has now been commented
 * out from the code.
 ****************************************************************************************
 */
void app_hid_ntf_cfm(uint8_t status)
{
    //arch_puts("app_hid_ntf_cfm()\r\n");
    ASSERT_ERROR(status == PRF_ERR_OK);
}


/**
 ****************************************************************************************
 * @brief Activates key scanning hardware and state machine. 
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_kbd_start_scanning(void)
{
    kbd_init_scan_vars();       // initalize non-retained scan variables
	kbd_start_sw_scanning();    // start SW scanning
}


/**
 ****************************************************************************************
 * @brief Sets up the hardware for key scanning. Normally, the keyboard scanning is
 *        active only when connected or disconnected but bonded
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_kbd_enable_scanning(void)
{
//    dbg_puts(DBG_SCAN_LVL, "KBD active (HW scan)\r\n");

    kbd_membrane_input_setup();     // Setup the "inputs" of the key-matrix
    app_kbd_enable_wakeup_irq();    // Setup the wakeup irq
    kbd_membrane_output_sleep();    // Setup the "outputs" of the key-matrix
}


/**
 ****************************************************************************************
 * @brief Disables the hardware for key scanning
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_kbd_disable_scanning(void)
{
    //Deactivate Wakeup Timer interrupt
    SetBits16(WKUP_CTRL_REG, WKUP_ENABLE_IRQ, 0);
    NVIC_DisableIRQ(WKUP_QUADEC_IRQn);

    // Clear status
    wkup_hit = false;

    // Set outputs to 'high-Z' to disable current flow
    kbd_membrane_output_wakeup();
}


/**
 ****************************************************************************************
 * @brief Activates reporting mode
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_kbd_start_reporting(void)
{   
    // may be called multiple times
	dbg_puts(DBG_SCAN_LVL, "KBD HID rep ON\r\n");
    
    kbd_reports_en = REPORTS_ENABLED;
}


/**
 ****************************************************************************************
 * @brief Activates passcode mode (default)
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_kbd_start_passcode(void)
{
    dbg_puts(DBG_SCAN_LVL, "KBD HID rep OFF (Passcode)\r\n");

    passcode = 0;
		pass_code = 0;
    kbd_reports_en = REPORTS_DISABLED;
}


/**
 ****************************************************************************************
 * @brief Stops HID reporting
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_kbd_stop_reporting(void)
{
    dbg_puts(DBG_SCAN_LVL, "KBD HID rep OFF\r\n");

    passcode = 0;
		pass_code = 0;
    kbd_reports_en = REPORTS_PAUSED;
}


/**
 ****************************************************************************************
 * @brief Prepares HID reports based on keycode buffer data
 *        Called in case of free list depletion (in this case calls to prepare_kbd_keyreport()
 *        fail). This way, captured key events that have been stored in the keycode_buffer
 *        will not be missed.
 *
 * @param   None
 *
 * @return  0, if preparing the HID report failed
 *          1, if HID report has been successfully prepared
 ****************************************************************************************
 */
int app_kbd_prepare_keyreports(void)
{
    int ret = 0;
    
    do
    {
    if (kbd_reports_en == REPORTS_PAUSED)
        break;
    
    if (keycode_buf_overflow)
    {
        kbd_rep_info *pReportInfo;
        
        app_kbd_flush_buffer();     // reset buffer
        app_kbd_flush_reports();    // reset report status
        
        // add a "full release" report in the trm list.
        pReportInfo = add_normal_report(RELEASE, false);
        if (!pReportInfo)
            break;
        memset(pReportInfo->pBuf, 0, 8); 
                
        // Add an "full release" extended key report
        pReportInfo = prepare_extended_report(NULL);
        if (!pReportInfo)
            break;
        memset(pReportInfo->pBuf, 0, 3); 
        
        // clear flag
        keycode_buf_overflow = false;
        
        ret = 1;
    }
    else if (app_kbd_buffer_has_data()) 
    {
        ret = 1;
        
        if (kbd_reports_en == REPORTS_ENABLED)
        {
            // normal mode
            ret = prepare_kbd_keyreport();
        } 
        else 
        {
            // passcode mode
            while (kbd_keycode_buffer_head != kbd_keycode_buffer_tail) 
            {
                int fn_mod = kbd_keycode_buffer[kbd_keycode_buffer_head].flags & KEY_FN_SET_MASK;
                int pressed = kbd_keycode_buffer[kbd_keycode_buffer_head].flags & KEY_STATUS_MASK;
                uint8_t output = kbd_keycode_buffer[kbd_keycode_buffer_head].output;
                uint8_t input = kbd_keycode_buffer[kbd_keycode_buffer_head].input;
                uint16_t keycode = kbd_keymap[fn_mod][output][input];
                const char keymode = keycode >> 8;
                const char keychar = keycode & 0xFF;
                
                if (!pressed) 
                {
                    if ( (keymode & 0xFC) == 0x00 )   // normal key
                    {
                        if ((keychar == 0x28) || (keychar == 0x58))   // Enter or Keypad Enter
                        {
                            app_mitm_passcode_report(passcode);
                        } 
                        else 
                        {
                            int num = -1;
                            
                            if (keychar >= 0x1E && keychar <= 0x27)
                                num = keychar - 0x1D;
                            else if (keychar >= 0x59 && keychar <= 0x62)
                                num = keychar - 0x58;
                            if (num == 10)
                                num = 0;
                            
                            if (num != -1) 
                            {
                                passcode *= 10;
                                passcode += num;
                            }
                        }
                    }
                }
                kbd_keycode_buffer_head = (kbd_keycode_buffer_head + 1) % KEYCODE_BUFFER_SIZE;
            }
        }
    }
    } while(0);
    
    return ret;
}


/**
 ****************************************************************************************
 * @brief Sends a Key Report Notification to the Host
 *
 * @param   None
 *
 * @return  0, if sending the HID report failed
 *          1, if the HID report has been sent
 ****************************************************************************************
 */
int app_kbd_send_key_report(void)
{
    return send_kbd_keyreport();
}


/**
 ****************************************************************************************
 * @brief Checks if the device is connected or not
 *
 * @param   None
 *
 * @return  false, if the device is disconnected
 *          true, if the device is connected
 ****************************************************************************************
 */
int app_kbd_check_conn_status(void)
{
    bool connected = true;
    ke_state_t app_state = ke_state_get(TASK_APP);
    
	if (app_state != APP_SECURITY && app_state != APP_PARAM_UPD && app_state != APP_CONNECTED) 
        connected = false;
    
	return (connected); 
}


/**
 ****************************************************************************************
 * @brief Re-initializes the scan matrix after wakeup
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_kbd_reinit_matrix(void)
{
    if (!HAS_DELAYED_WAKEUP)
    {
        kbd_membrane_input_setup();             // setup inputs (pull-up)
        kbd_membrane_output_sleep();            // setup outputs 'low'. they are latched but have to be reinitialized
    }
    else
    {
        kbd_membrane_input_setup();             // setup inputs (pull-up)
        if (current_fsm_state != IDLE_ST)
            kbd_membrane_output_sleep();        // setup outputs 'low'. they are latched but have to be reinitialized
        else
        {
            // Not active
            for (int i = 0; i < KBD_NR_OUTPUTS; ++i)
                set_row_to_input_highz(i);          // Set all rows to 'high-Z'
            
            set_row_to_low(DELAYED_WAKEUP_GPIO_ROW);// Setup the "output" of the key-matrix
        }
    }
}

/// @} APP
