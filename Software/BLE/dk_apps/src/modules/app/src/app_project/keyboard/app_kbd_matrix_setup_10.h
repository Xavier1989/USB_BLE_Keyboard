/**
 ****************************************************************************************
 *
 * Copyright (C) 2014. Dialog Semiconductor Ltd, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */


#ifndef _APP_KBD_MATRIX_SETUP_10_H_
#define _APP_KBD_MATRIX_SETUP_10_H_

#include <stdint.h>
#include <stddef.h>

#include "app_kbd_config.h"


/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup HID
 *
 * @brief HID (Keyboard) Application matrix of setup #10.
 *
 * @{
 ****************************************************************************************
 */

#if (HAS_EEPROM)
# define PAIR               (0xF4F1)
# define CLRP               (0xF4F2)
#else
# define PAIR               (0x0000)
# define CLRP               (0x0000)
#endif

#if (HAS_KEYBOARD_MEASURE_EXT_SLP)
# define KB_EXTSLP          (0xF4F3)
#else
# define KB_EXTSLP          (0)
#endif


#define KBD_NR_INPUTS		(2)
#define KBD_NR_OUTPUTS		(1)


#define COLUMN_0_PORT		(1)	//port: 1, pin: 1
#define COLUMN_0_PIN		(1)

#define COLUMN_1_PORT		(0)	//port: 0, pin: 6
#define COLUMN_1_PIN		(6)


// used for cycle optimization
#define P0_HAS_INPUT		(1)
#define P1_HAS_INPUT		(1)
#define P2_HAS_INPUT		(0)
#define P3_HAS_INPUT		(0)

#define ROW_0_PORT			(1)	//port: 1, pin: 3
#define ROW_0_PIN			(3)


// Masks for the initialization of the KBD controller
#define MASK_P0				(0x4000	| SET_MASK0_FROM_COLUMN(0)                | SET_MASK0_FROM_COLUMN(1)                )
uint16_t mask_p0 = MASK_P0;

#define MASK_P12			(0x0000	| SET_MASK12_FROM_COLUMN(0)               | SET_MASK12_FROM_COLUMN(1)               )
uint16_t mask_p12 = MASK_P12;

#define MASK_P3				(0x0000	| SET_MASK3_FROM_COLUMN(0)                | SET_MASK3_FROM_COLUMN(1)                )
uint16_t mask_p3 = MASK_P3;

// Masks for the initialization of the WKUP controller
#define WKUP_MASK_P0		(	SET_WKUP_MASK_FROM_COLUMN(0, 0)         | SET_WKUP_MASK_FROM_COLUMN(0, 1)         )
uint16_t wkup_mask_p0 = WKUP_MASK_P0;

#define WKUP_MASK_P1		(	SET_WKUP_MASK_FROM_COLUMN(1, 0)         | SET_WKUP_MASK_FROM_COLUMN(1, 1)         )
uint16_t wkup_mask_p1 = WKUP_MASK_P1;

#define WKUP_MASK_P2		(	SET_WKUP_MASK_FROM_COLUMN(2, 0)         | SET_WKUP_MASK_FROM_COLUMN(2, 1)         )
uint16_t wkup_mask_p2 = WKUP_MASK_P2;

#define WKUP_MASK_P3		(	SET_WKUP_MASK_FROM_COLUMN(3, 0)         | SET_WKUP_MASK_FROM_COLUMN(3, 1)         )
uint16_t wkup_mask_p3 = WKUP_MASK_P3;

KBD_TYPE_QUALIFIER uint8_t kbd_input_ports[] KBD_ARRAY_ATTRIBUTE =
{
	COL(0),                     // column 0 (P1[1])
	COL(1)                      // column 1
};

KBD_TYPE_QUALIFIER uint8_t kbd_output_mode_regs[] KBD_ARRAY_ATTRIBUTE =
{
	SET_OUTPUT_MODE_REG(0)
};

KBD_TYPE_QUALIFIER uint8_t kbd_output_reset_data_regs[] KBD_ARRAY_ATTRIBUTE =
{
	SET_RESET_REG(0)
};

KBD_TYPE_QUALIFIER uint16_t kbd_out_bitmasks[] KBD_ARRAY_ATTRIBUTE =
{
	SET_BITMAP(0)
};

KBD_TYPE_QUALIFIER uint8_t kbd_input_mode_regs[] KBD_ARRAY_ATTRIBUTE =
{
	SET_INPUT_MODE_REG(0),
	SET_INPUT_MODE_REG(1)
};

#ifdef DELAYED_WAKEUP_ON
#define DELAYED_WAKEUP_GPIO_ROW			(X)	// UNDEFINED
#define DELAYED_WAKEUP_GPIO_COLUMN		(Y)	// UNDEFINED
#endif


// extra sets for 'hidden modifiers', e.g. the 'Fn' key
#define KBD_NR_SETS		(1)

// unknown key code - nothing is sent to the other side but the key is examined for ghosting
#define K_CODE			(0xF4FF)


// The key map.
// 00xx means regular key
// FCxx means modifier key.
// F8xx means FN Modifier.
// F4xy means special function (x = no of byte in the report, y no of bit set).
KBD_TYPE_QUALIFIER uint16_t kbd_keymap[KBD_NR_SETS][KBD_NR_OUTPUTS][KBD_NR_INPUTS] KBD_ARRAY_ATTRIBUTE =
{
  {
/*    No Fn key(s) pressed
      0         1
      --------------------
      p         o
      --------------------*/
	{ 0x0013,   0x0012      }  // 0
  }
};


#ifdef KEYBOARD_MEASURE_EXT_SLP_ON
#define KBD_NR_COMBINATIONS 1
// Inputs are active low. Thus, the scan result when both keys are pressed will be 0.
// In general, for an 1xN matrix, to scan a 2-key combination the following scan status would be expected:
//   0b000...0111...101...101...1
//            ^      ^     ^
//            |      |     |
//            |      |     ---- 2nd key is pressed
//            |      ---- 1st key is pressed
//            ---- highest input (column) of the key matrix (the key is not pressed in this example)
//
// 'action' could be something pre-defined or an EVENT sent to the main FSM to trigger an action
// The priority of key_comb members is decreasing. The first member has the highest priority and it will be checked first!
const struct key_combinations_t key_comb[KBD_NR_COMBINATIONS] =
    {
        {0x0, 0xDEED},
    };
#else
#define KBD_NR_COMBINATIONS 0
const struct key_combinations_t *const key_comb = NULL;
#endif //KEYBOARD_MEASURE_EXT_SLP_ON


/// @} APP

#endif //_APP_KBD_MATRIX_SETUP_10_H_
