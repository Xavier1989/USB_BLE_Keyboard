/**
 ****************************************************************************************
 *
 * @file app_kbd_matrix.h
 *
 * @brief HID Keyboard matrix.
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

#ifndef APP_KBD_MATRIX_H_
#define APP_KBD_MATRIX_H_

#include <stdint.h>
#include <stddef.h>

#include "app_kbd_config.h"

#include "app_kbd_macros.h"
#include "app_kbd_fsm.h"

struct key_combinations_t {
    uint32_t scan_status;
    uint32_t action;
};


#ifdef ASCII_DEBUG

// Quick and dirty conversion table to allow debugging
const uint8_t kbd_hut_to_ascii_table[128] = 
{
	'?', '?', '?', '?', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',  // 0x00
	'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2',  // 0x10
	'3', '4', '5', '6', '7', '8', '9', '0', '\n', 27,  8, '\t', ' ', '-', '=', '[',  // 0x20
	']', '\\','#', ';', '\'','`', ',', '.', '/', '^', '1', '2', '3', '4', '5', '6',  // 0x30
	'7', '8', '9', 'A', 'B', 'C', 'P', 'S', 'P', 'I', 'H', 'U', 127, 'E', 'D', 'R',  // 0x40
	'L', 'D', 'U', 'N', '/', '*', '-', '+', '\n','1', '2', '3', '4', '5', '6', '7',  // 0x50
	'8', '9', '0', '.', '\\','A', 'P', '=', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',  // 0x60
	'L', 'M', 'N', 'O', 'E', 'H', 'M', 'S', 'S', 'A', 'U', 'C', 'C', 'P', 'F', 'M'   // 0x70
};

// note: must be power of 2
#define MAX_KEYBUFFER (16)
uint8_t kbd_buffer_ascii[MAX_KEYBUFFER];
uint16_t kbd_keybuffer_idx;

#endif // ASCII_DEBUG







#if MATRIX_SETUP == 0
/*****************************************************************************************************************************************
 *
 * FREE - Left for customer applications
 *
 *****************************************************************************************************************************************/
 
#include "app_kbd_matrix_setup_0.h"





#elif MATRIX_SETUP == 1
/*****************************************************************************************************************************************
 *
 * Rev.3 DK with QFN48 (keyboard #1)
 *
 *****************************************************************************************************************************************/
 
#include "app_kbd_matrix_setup_1.h"





#elif MATRIX_SETUP == 2
/*****************************************************************************************************************************************
 *
 * Rev.3 DK with QFN48 (keyboard #2)
 *
 *****************************************************************************************************************************************/

#include "app_kbd_matrix_setup_2.h"





#elif MATRIX_SETUP == 3
/*****************************************************************************************************************************************
 *
 * Rev.3 DK with QFN48 (keyboard #3)
 *
 *****************************************************************************************************************************************/

#include "app_kbd_matrix_setup_3.h"




#elif MATRIX_SETUP == 4
/*****************************************************************************************************************************************
 *
 * Rev.3 DK with QFN48 (keyboard #1 with EEPROM)
 *
 *****************************************************************************************************************************************/
 
#include "app_kbd_matrix_setup_4.h"





#elif MATRIX_SETUP == 5
/*****************************************************************************************************************************************
 *
 * Rev.3 DK with QFN48 (keyboard #1 for Apple products)
 *
 *****************************************************************************************************************************************/

#include "app_kbd_matrix_setup_5.h"





#elif MATRIX_SETUP == 6
/*****************************************************************************************************************************************
 *
 * Rev.3 DK with QFN48 (keyboard #1 for Apple products)
 *
 *****************************************************************************************************************************************/
 
#include "app_kbd_matrix_setup_6.h"





#elif MATRIX_SETUP == 7
/*****************************************************************************************************************************************
 *
 * Rev.3 DK with QFN48 (keyboard #2 with EEPROM)
 *
 *****************************************************************************************************************************************/

#include "app_kbd_matrix_setup_7.h"





#elif MATRIX_SETUP == 8
/*****************************************************************************************************************************************
 *
 * Reference Design (#1)
 *
 *****************************************************************************************************************************************/

#include "app_kbd_matrix_setup_8.h"





#elif MATRIX_SETUP == 9
/*****************************************************************************************************************************************
 *
 * RC 12-keys with QFN48 (based on keyboard #3)
 *
 *****************************************************************************************************************************************/

#include "app_kbd_matrix_setup_9.h"





#elif MATRIX_SETUP == 10
/*****************************************************************************************************************************************
 *
 * DK 2-keys with QFN48
 *
 *****************************************************************************************************************************************/

#include "app_kbd_matrix_setup_10.h"





#elif MATRIX_SETUP == 11
/*****************************************************************************************************************************************
 *
 * 5-keys with QFN48
 *
 *****************************************************************************************************************************************/

#include "app_kbd_matrix_setup_11.h"

#endif // MATRIX_SETUP





#if KBD_NR_INPUTS < 17
typedef uint16_t scan_t;
#else
typedef uint32_t scan_t;
#endif

#ifndef DELAYED_WAKEUP_GPIO_ROW
#ifdef DELAYED_WAKEUP_ON
#error "Delayed wakeup selected but wakeup pin is undefined!"
#endif
#define DELAYED_WAKEUP_GPIO_ROW     (0)
#define DELAYED_WAKEUP_GPIO_COLUMN  (0)
#endif
    
#undef COL
#undef ROW
#undef SET_OUTPUT_MODE_REG
#undef SET_RESET_REG
#undef SET_BITMAP
#undef SET_INPUT_MODE_REG

typedef int kbd_input_ports_check[ (sizeof(kbd_input_ports) / sizeof(uint8_t)) == KBD_NR_INPUTS];                   // on error: the kbd_input_ports[] is not defined properly!
typedef int kbd_output_mode_regs_check[ (sizeof(kbd_output_mode_regs) / sizeof(uint8_t)) == KBD_NR_OUTPUTS];        // on error: the kbd_output_mode_regs[] is not defined properly!
typedef int kbd_output_reset_regs_check[ (sizeof(kbd_output_reset_data_regs) / sizeof(uint8_t)) == KBD_NR_OUTPUTS]; // on error: the kbd_output_reset_data_regs[] is not defined properly!
typedef int kbd_output_bitmasks_check[ (sizeof(kbd_out_bitmasks) / sizeof(uint16_t)) == KBD_NR_OUTPUTS];            // on error: the kbd_out_bitmasks[] is not defined properly!
typedef int kbd_output_input_mode_regs_check[ (sizeof(kbd_input_mode_regs) / sizeof(uint8_t)) == KBD_NR_INPUTS];    // on error: the kbd_input_mode_regs[] is not defined properly!

#define CHECK_KBD_CTRL_MASKS        (  (MASK_P0 & CHECK_MASK0_FROM_COLUMN(KBD_NR_INPUTS-1))     \
                                     | (MASK_P12 & CHECK_MASK12_FROM_COLUMN(KBD_NR_INPUTS-1))   \
                                     | (MASK_P3 & CHECK_MASK3_FROM_COLUMN(KBD_NR_INPUTS-1)) )

#if (!CHECK_KBD_CTRL_MASKS)
#error "KBD controller masks not defined properly!"
#endif

#define CHECK_WKUP_CTRL_MASKS       (  (WKUP_MASK_P0 & CHECK_WKUP_MASK0_FROM_COLUMN(KBD_NR_INPUTS-1))   \
                                     | (WKUP_MASK_P1 & CHECK_WKUP_MASK1_FROM_COLUMN(KBD_NR_INPUTS-1))   \
                                     | (WKUP_MASK_P2 & CHECK_WKUP_MASK2_FROM_COLUMN(KBD_NR_INPUTS-1))   \
                                     | (WKUP_MASK_P3 & CHECK_WKUP_MASK3_FROM_COLUMN(KBD_NR_INPUTS-1)) )

#if (!CHECK_WKUP_CTRL_MASKS)
#error "WKUP controller masks not defined properly!"
#endif

#endif // APP_KBD_MATRIX_H_
