/**
 ****************************************************************************************
 *
 * @file app_kbd.h
 *
 * @brief Keyboard (HID) Application entry point header file.
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

#ifndef APP_KBD_H_
#define APP_KBD_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup HID
 *
 * @brief HID (Keyboard) Application entry point.
 *
 * @{
 ****************************************************************************************
 */

#define APP_KBD_ERR_ERROR (0)
#define APP_KBD_ERR_OK (1)

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"

#include <stdint.h>          // standard integer definition
#include <co_bt.h>

#include "app_kbd_config.h"

#if (FULL_SCAN_IN_MS < PARTIAL_SCAN_IN_MS)
#error "Wrong scanning parameters!"
#endif




/*
 * Code switches
 ****************************************************************************************
 */

#ifdef DEEPSLEEP_ON
#define HAS_DEEPSLEEP                           1
#else
#define HAS_DEEPSLEEP                           0
#endif

#ifdef PRIVACY_ON
#define HAS_PRIVACY                             1
#else
#define HAS_PRIVACY                             0
#endif

#ifdef WHITE_LIST_ON
#define HAS_WHITE_LIST                          1
#else
#define HAS_WHITE_LIST                          0
#endif

#ifdef VIRTUAL_WHITE_LIST_ON
#define HAS_VIRTUAL_WHITE_LIST                  1
#else
#define HAS_VIRTUAL_WHITE_LIST                  0
#endif

#ifdef SCAN_ALWAYS_ACTIVE_ON
#define HAS_SCAN_ALWAYS_ACTIVE                  1
#else
#define HAS_SCAN_ALWAYS_ACTIVE                  0
#endif

#ifdef DELAYED_WAKEUP_ON
#define HAS_DELAYED_WAKEUP                      1
#else
#define HAS_DELAYED_WAKEUP                      0
#endif

#ifdef HOGPD_BOOT_PROTO_ON
#define HAS_HOGPD_BOOT_PROTO                    1
#else
#define HAS_HOGPD_BOOT_PROTO                    0
#endif

#ifdef BATT_EXTERNAL_REPORT_ON
#define HAS_BATT_EXTERNAL_REPORT                1
#else
#define HAS_BATT_EXTERNAL_REPORT                0
#endif

#ifdef ALTERNATIVE_SCAN_TIMES_ON
#define HAS_ALTERNATIVE_SCAN_TIMES              1
#else
#define HAS_ALTERNATIVE_SCAN_TIMES              0
#endif

#ifdef USE_PREF_CONN_PARAMS_ON
#define HAS_KBD_SWITCH_TO_PREFERRED_CONN_PARAMS 1
#else
#define HAS_KBD_SWITCH_TO_PREFERRED_CONN_PARAMS 0
#endif

#ifdef SEND_LL_TERMINATE_IND_ON
#define HAS_SEND_LL_TERMINATE_IND               1
#else
#define HAS_SEND_LL_TERMINATE_IND               0
#endif

#ifdef MITM_ON
#define HAS_MITM                                1
#else
#define HAS_MITM                                0
#endif

#ifdef KEYBOARD_MEASURE_EXT_SLP_ON
#define HAS_KEYBOARD_MEASURE_EXT_SLP            1
#else
#define HAS_KEYBOARD_MEASURE_EXT_SLP            0
#endif

#ifdef KEYBOARD_LEDS_ON
#define HAS_KEYBOARD_LEDS                       1
#else
#define HAS_KEYBOARD_LEDS                       0
#endif

#if (MATRIX_SETUP != 3) && (MATRIX_SETUP != 8) && (MATRIX_SETUP != 10) && (HAS_KEYBOARD_LEDS)
#error "The chosen keyboard setup does not support LEDS!"
#endif



#ifdef NORMALLY_CONNECTABLE_ON
#define HAS_NORMALLY_CONNECTABLE                1
#else
#define HAS_NORMALLY_CONNECTABLE                0
#endif

#ifdef REMOTE_WAKEUP_ON
#define HAS_REMOTE_WAKEUP                       1
#else
#define HAS_REMOTE_WAKEUP                       0
#endif

#ifdef PASSCODE_TIMEOUT_ON
#define HAS_PASSCODE_TIMEOUT                    1
#else
#define HAS_PASSCODE_TIMEOUT                    0
#endif

#ifdef SECURITY_REQUEST_SEND_ON
#define HAS_SECURITY_REQUEST_SEND               1
#else
#define HAS_SECURITY_REQUEST_SEND               0
#endif

#ifdef INACTIVITY_TIMEOUT_ON
#define HAS_INACTIVITY_TIMEOUT                  1
#else
#define HAS_INACTIVITY_TIMEOUT                  0
#endif

#if ( HAS_INACTIVITY_TIMEOUT && HAS_NORMALLY_CONNECTABLE )
#warning "It does not make sense to use inactivity timeout when the NormallyConnectable flag is set..."
#endif

#ifdef REPORT_HISTORY_ON
#define HAS_REPORT_HISTORY                      1
#else
#define HAS_REPORT_HISTORY                      0
#endif

#ifdef EXTENDED_TIMERS_ON
#define HAS_EXTENDED_TIMERS                     1
#else
#define HAS_EXTENDED_TIMERS                     0
#endif


/*
 * Scanning 
 ****************************************************************************************
 */
 
#define KEY_STATUS_MASK (0x10)        // pressed or released
#define KEY_FN_SET_MASK (0x0F)        // mask for fn modifier

struct keycode_buffer_tag {
    uint8_t flags;
    uint8_t output;
    uint8_t input;
};

enum delay_monitor_status {
    MONITOR_IDLE,
    MONITOR_PRESS,
    MONITOR_RELEASE,
};

enum delay_trigger_status {
    NO_TRIGGER,
    PRESSED_TRIGGER,
    RELEASED_TRIGGER,
    PRESSED_TRIGGER_SYNCED,
    RELEASED_TRIGGER_SYNCED,
};

struct hid_msg {
    enum delay_trigger_status trigger;
};


/*
 * Debouncing internals
 ****************************************************************************************
 */
 
#define DEBOUNCE_COUNTER_PRESS      ((int)(1 + ( ((DEBOUNCE_COUNTER_P_IN_MS - FULL_SCAN_IN_MS) / PARTIAL_SCAN_IN_MS) + 0.999 ) ) - 1)

#define DEBOUNCE_COUNTER_RELEASE    ((int)( (DEBOUNCE_COUNTER_R_IN_MS / PARTIAL_SCAN_IN_MS) + 0.999 ) - 1)

#define SYSTICK_CLOCK_RATE          (1000000)
#define SYSTICK_TICKS_PER_US        (SYSTICK_CLOCK_RATE / 1000000)


enum DEBOUNCE_STATE {
    IDLE = 0,
    PRESS_DEBOUNCING,
    WAIT_RELEASE,
    RELEASE_DEBOUNCING,
};

struct debounce_counter_t {
    enum DEBOUNCE_STATE state;
    uint8_t cnt;
};



/*
 * Reporting
 ****************************************************************************************
 */

#define MAX_REPORTS 5 

enum KEY_BUFF_TYPE {
	FREE,
	PRESS,
	RELEASE,
    EXTENDED
};

enum REPORT_TYPE {
    NORMAL_REPORT = 0,
    EXTENDED_REPORT = 2
};

typedef struct __kbd_rep_info {
	enum KEY_BUFF_TYPE type;
	bool modifier_report;
    enum REPORT_TYPE char_id;
    uint8_t len;
	uint8_t *pBuf;
	struct __kbd_rep_info *pNext;
} kbd_rep_info;

enum REPORT_MODE {
    REPORTS_DISABLED,   // PassCode mode
    REPORTS_ENABLED,    // Normal mode
    REPORTS_PAUSED,     // Disconnected mode
};


// The roll-over buffer must be bigger than the debounce buffer.
#define ROLL_OVER_BUF_SZ    (DEBOUNCE_BUFFER_SIZE * 2)
#if (ROLL_OVER_BUF_SZ < 7)
#error "Too small Roll-Over buffer!"
#endif
struct roll_over_tag {
    // bits [15:14] : kbd_fn_modifier
    // bits  [13:8] : output
    // bits   [7:0] : input
    uint16_t intersections[ROLL_OVER_BUF_SZ];
    int cnt;
};
#define RLOVR_INVALID_INTERSECTION  (0xFFFF)
#define RLOVR_INDICATION_CODE       (0xFEFE)

/*
 * Public variables
 ****************************************************************************************
 */

extern kbd_rep_info *kbd_trm_list;
extern kbd_rep_info *kbd_free_list;
//extern bool normal_key_report_ack_pending;
//extern bool extended_key_report_ack_pending;
extern bool user_disconnection_req;
extern bool user_extended_sleep;
extern bool systick_hit;
extern bool wkup_hit;
extern bool sync_key_press_evt;
extern bool sync_passcode_entered_evt;
extern uint8_t kbd_proto_mode;

// DELAYED_WAKEUP_ON
extern enum delay_monitor_status monitor_kbd_delayed_start_st;
extern enum delay_trigger_status trigger_kbd_delayed_start_st;
extern bool ble_is_woken_up;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
 
/**
 ****************************************************************************************
 * @brief Creates the HID over GATT database
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_hid_create_db(void);

/**
 ****************************************************************************************
 * @brief Initializes keycode buffer and keyreport variables
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */        
void app_keyboard_init(void);

/**
 ****************************************************************************************
 * @brief Enables the HOGPD profile 
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_keyboard_enable(void);

void app_kbd_enable_scanning(void);

void app_kbd_enable_delayed_scanning(bool press);

void app_kbd_start_scanning(void);

bool app_kbd_scan_matrix(int *row);

bool app_kbd_update_status(void);

void app_kbd_disable_scanning(void);

/**
 ****************************************************************************************
 * @brief Handles the Notification confirmation 
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
void app_hid_ntf_cfm(uint8_t status);

void app_kbd_reinit_matrix(void);

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
bool app_kbd_buffer_has_data(void);

/**
 ****************************************************************************************
 * @brief Activates reporting mode 
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_kbd_start_reporting(void);

/**
 ****************************************************************************************
 * @brief Stops HID reporting
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_kbd_stop_reporting(void);

/**
 ****************************************************************************************
 * @brief Activates passcode mode (default)
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_kbd_start_passcode(void);

/**
 ****************************************************************************************
 * @brief Flushes all the entries in the keycode_buffer
 *        
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_kbd_flush_buffer(void);

/**
 ****************************************************************************************
 * @brief Flushes all the pending HID reports
 *        
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void app_kbd_flush_reports(void);

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
int app_kbd_prepare_keyreports(void);

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
int app_kbd_send_key_report(void);

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
int app_kbd_check_conn_status(void);

/**
 ****************************************************************************************
 * @brief Initializes the quadrature decoder (mouse wheel interface)
 *        (enables also the external pullups)
 *
 * @param   None
 *
 * @return  void
 ****************************************************************************************
 */
void mouse_quad_decoder_init(void);

/// @} APP

#endif // APP_KBD_H_
