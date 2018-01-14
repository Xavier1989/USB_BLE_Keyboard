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


#ifndef _APP_KBD_MATRIX_SETUP_2_H_
#define _APP_KBD_MATRIX_SETUP_2_H_

#include <stdint.h>
#include <stddef.h>

#include "app_kbd_config.h"


/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup HID
 *
 * @brief HID (Keyboard) Application matrix of setup #2.
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


#define KBD_NR_INPUTS		(18)
#define KBD_NR_OUTPUTS		(8)


#define COLUMN_0_PORT		(0)	//port: 0, pin: 5
#define COLUMN_0_PIN		(5)

#define COLUMN_1_PORT		(0)	//port: 0, pin: 4
#define COLUMN_1_PIN		(4)

#define COLUMN_2_PORT		(1)	//port: 1, pin: 3
#define COLUMN_2_PIN		(3)

#define COLUMN_3_PORT		(1)	//port: 1, pin: 2
#define COLUMN_3_PIN		(2)

#define COLUMN_4_PORT		(1)	//port: 1, pin: 1
#define COLUMN_4_PIN		(1)

#define COLUMN_5_PORT		(1)	//port: 1, pin: 0
#define COLUMN_5_PIN		(0)

#define COLUMN_6_PORT		(2)	//port: 2, pin: 7
#define COLUMN_6_PIN		(7)

#define COLUMN_7_PORT		(2)	//port: 2, pin: 6
#define COLUMN_7_PIN		(6)

#define COLUMN_8_PORT		(2)	//port: 2, pin: 5
#define COLUMN_8_PIN		(5)

#define COLUMN_9_PORT		(2)	//port: 2, pin: 4
#define COLUMN_9_PIN		(4)

#define COLUMN_10_PORT		(3)	//port: 3, pin: 7
#define COLUMN_10_PIN		(7)

#define COLUMN_11_PORT		(2)	//port: 2, pin: 3
#define COLUMN_11_PIN		(3)

#define COLUMN_12_PORT		(3)	//port: 3, pin: 6
#define COLUMN_12_PIN		(6)

#define COLUMN_13_PORT		(2)	//port: 2, pin: 2
#define COLUMN_13_PIN		(2)

#define COLUMN_14_PORT		(2)	//port: 2, pin: 1
#define COLUMN_14_PIN		(1)

#define COLUMN_15_PORT		(2)	//port: 2, pin: 0
#define COLUMN_15_PIN		(0)

#define COLUMN_16_PORT		(3)	//port: 3, pin: 1
#define COLUMN_16_PIN		(1)

#define COLUMN_17_PORT		(3)	//port: 3, pin: 2
#define COLUMN_17_PIN		(2)


// used for cycle optimization
#define P0_HAS_INPUT		(1)
#define P1_HAS_INPUT		(1)
#define P2_HAS_INPUT		(1)
#define P3_HAS_INPUT		(1)

#define ROW_0_PORT			(3)	//port: 3, pin: 4
#define ROW_0_PIN			(4)

#define ROW_1_PORT			(3)	//port: 3, pin: 3
#define ROW_1_PIN			(3)

#define ROW_2_PORT			(3)	//port: 3, pin: 5
#define ROW_2_PIN			(5)

#define ROW_3_PORT			(0)	//port: 0, pin: 6
#define ROW_3_PIN			(6)

#define ROW_4_PORT			(2)	//port: 2, pin: 9
#define ROW_4_PIN			(9)

#define ROW_5_PORT			(2)	//port: 2, pin: 8
#define ROW_5_PIN			(8)

#define ROW_6_PORT			(0)	//port: 0, pin: 7
#define ROW_6_PIN			(7)

#define ROW_7_PORT			(3)	//port: 3, pin: 0
#define ROW_7_PIN			(0)


// Masks for the initialization of the KBD controller
#define MASK_P0				(0x4000	| SET_MASK0_FROM_COLUMN(0)                | SET_MASK0_FROM_COLUMN(1)                | SET_MASK0_FROM_COLUMN(2)                | SET_MASK0_FROM_COLUMN(3)                \
									| SET_MASK0_FROM_COLUMN(4)                | SET_MASK0_FROM_COLUMN(5)                | SET_MASK0_FROM_COLUMN(6)                | SET_MASK0_FROM_COLUMN(7)                \
									| SET_MASK0_FROM_COLUMN(8)                | SET_MASK0_FROM_COLUMN(9)                | SET_MASK0_FROM_COLUMN(10)               | SET_MASK0_FROM_COLUMN(11)               \
									| SET_MASK0_FROM_COLUMN(12)               | SET_MASK0_FROM_COLUMN(13)               | SET_MASK0_FROM_COLUMN(14)               | SET_MASK0_FROM_COLUMN(15)               \
									| SET_MASK0_FROM_COLUMN(16)               | SET_MASK0_FROM_COLUMN(17)               )
uint16_t mask_p0 = MASK_P0;

#define MASK_P12			(0x0000	| SET_MASK12_FROM_COLUMN(0)               | SET_MASK12_FROM_COLUMN(1)               | SET_MASK12_FROM_COLUMN(2)               | SET_MASK12_FROM_COLUMN(3)               \
									| SET_MASK12_FROM_COLUMN(4)               | SET_MASK12_FROM_COLUMN(5)               | SET_MASK12_FROM_COLUMN(6)               | SET_MASK12_FROM_COLUMN(7)               \
									| SET_MASK12_FROM_COLUMN(8)               | SET_MASK12_FROM_COLUMN(9)               | SET_MASK12_FROM_COLUMN(10)              | SET_MASK12_FROM_COLUMN(11)              \
									| SET_MASK12_FROM_COLUMN(12)              | SET_MASK12_FROM_COLUMN(13)              | SET_MASK12_FROM_COLUMN(14)              | SET_MASK12_FROM_COLUMN(15)              \
									| SET_MASK12_FROM_COLUMN(16)              | SET_MASK12_FROM_COLUMN(17)              )
uint16_t mask_p12 = MASK_P12;

#define MASK_P3				(0x0000	| SET_MASK3_FROM_COLUMN(0)                | SET_MASK3_FROM_COLUMN(1)                | SET_MASK3_FROM_COLUMN(2)                | SET_MASK3_FROM_COLUMN(3)                \
									| SET_MASK3_FROM_COLUMN(4)                | SET_MASK3_FROM_COLUMN(5)                | SET_MASK3_FROM_COLUMN(6)                | SET_MASK3_FROM_COLUMN(7)                \
									| SET_MASK3_FROM_COLUMN(8)                | SET_MASK3_FROM_COLUMN(9)                | SET_MASK3_FROM_COLUMN(10)               | SET_MASK3_FROM_COLUMN(11)               \
									| SET_MASK3_FROM_COLUMN(12)               | SET_MASK3_FROM_COLUMN(13)               | SET_MASK3_FROM_COLUMN(14)               | SET_MASK3_FROM_COLUMN(15)               \
									| SET_MASK3_FROM_COLUMN(16)               | SET_MASK3_FROM_COLUMN(17)               )
uint16_t mask_p3 = MASK_P3;

// Masks for the initialization of the WKUP controller
#define WKUP_MASK_P0		(	SET_WKUP_MASK_FROM_COLUMN(0, 0)         | SET_WKUP_MASK_FROM_COLUMN(0, 1)         | SET_WKUP_MASK_FROM_COLUMN(0, 2)         | SET_WKUP_MASK_FROM_COLUMN(0, 3)         \
							  | SET_WKUP_MASK_FROM_COLUMN(0, 4)         | SET_WKUP_MASK_FROM_COLUMN(0, 5)         | SET_WKUP_MASK_FROM_COLUMN(0, 6)         | SET_WKUP_MASK_FROM_COLUMN(0, 7)         \
							  | SET_WKUP_MASK_FROM_COLUMN(0, 8)         | SET_WKUP_MASK_FROM_COLUMN(0, 9)         | SET_WKUP_MASK_FROM_COLUMN(0, 10)        | SET_WKUP_MASK_FROM_COLUMN(0, 11)        \
							  | SET_WKUP_MASK_FROM_COLUMN(0, 12)        | SET_WKUP_MASK_FROM_COLUMN(0, 13)        | SET_WKUP_MASK_FROM_COLUMN(0, 14)        | SET_WKUP_MASK_FROM_COLUMN(0, 15)        \
							  | SET_WKUP_MASK_FROM_COLUMN(0, 16)        | SET_WKUP_MASK_FROM_COLUMN(0, 17)        )
uint16_t wkup_mask_p0 = WKUP_MASK_P0;

#define WKUP_MASK_P1		(	SET_WKUP_MASK_FROM_COLUMN(1, 0)         | SET_WKUP_MASK_FROM_COLUMN(1, 1)         | SET_WKUP_MASK_FROM_COLUMN(1, 2)         | SET_WKUP_MASK_FROM_COLUMN(1, 3)         \
							  | SET_WKUP_MASK_FROM_COLUMN(1, 4)         | SET_WKUP_MASK_FROM_COLUMN(1, 5)         | SET_WKUP_MASK_FROM_COLUMN(1, 6)         | SET_WKUP_MASK_FROM_COLUMN(1, 7)         \
							  | SET_WKUP_MASK_FROM_COLUMN(1, 8)         | SET_WKUP_MASK_FROM_COLUMN(1, 9)         | SET_WKUP_MASK_FROM_COLUMN(1, 10)        | SET_WKUP_MASK_FROM_COLUMN(1, 11)        \
							  | SET_WKUP_MASK_FROM_COLUMN(1, 12)        | SET_WKUP_MASK_FROM_COLUMN(1, 13)        | SET_WKUP_MASK_FROM_COLUMN(1, 14)        | SET_WKUP_MASK_FROM_COLUMN(1, 15)        \
							  | SET_WKUP_MASK_FROM_COLUMN(1, 16)        | SET_WKUP_MASK_FROM_COLUMN(1, 17)        )
uint16_t wkup_mask_p1 = WKUP_MASK_P1;

#define WKUP_MASK_P2		(	SET_WKUP_MASK_FROM_COLUMN(2, 0)         | SET_WKUP_MASK_FROM_COLUMN(2, 1)         | SET_WKUP_MASK_FROM_COLUMN(2, 2)         | SET_WKUP_MASK_FROM_COLUMN(2, 3)         \
							  | SET_WKUP_MASK_FROM_COLUMN(2, 4)         | SET_WKUP_MASK_FROM_COLUMN(2, 5)         | SET_WKUP_MASK_FROM_COLUMN(2, 6)         | SET_WKUP_MASK_FROM_COLUMN(2, 7)         \
							  | SET_WKUP_MASK_FROM_COLUMN(2, 8)         | SET_WKUP_MASK_FROM_COLUMN(2, 9)         | SET_WKUP_MASK_FROM_COLUMN(2, 10)        | SET_WKUP_MASK_FROM_COLUMN(2, 11)        \
							  | SET_WKUP_MASK_FROM_COLUMN(2, 12)        | SET_WKUP_MASK_FROM_COLUMN(2, 13)        | SET_WKUP_MASK_FROM_COLUMN(2, 14)        | SET_WKUP_MASK_FROM_COLUMN(2, 15)        \
							  | SET_WKUP_MASK_FROM_COLUMN(2, 16)        | SET_WKUP_MASK_FROM_COLUMN(2, 17)        )
uint16_t wkup_mask_p2 = WKUP_MASK_P2;

#define WKUP_MASK_P3		(	SET_WKUP_MASK_FROM_COLUMN(3, 0)         | SET_WKUP_MASK_FROM_COLUMN(3, 1)         | SET_WKUP_MASK_FROM_COLUMN(3, 2)         | SET_WKUP_MASK_FROM_COLUMN(3, 3)         \
							  | SET_WKUP_MASK_FROM_COLUMN(3, 4)         | SET_WKUP_MASK_FROM_COLUMN(3, 5)         | SET_WKUP_MASK_FROM_COLUMN(3, 6)         | SET_WKUP_MASK_FROM_COLUMN(3, 7)         \
							  | SET_WKUP_MASK_FROM_COLUMN(3, 8)         | SET_WKUP_MASK_FROM_COLUMN(3, 9)         | SET_WKUP_MASK_FROM_COLUMN(3, 10)        | SET_WKUP_MASK_FROM_COLUMN(3, 11)        \
							  | SET_WKUP_MASK_FROM_COLUMN(3, 12)        | SET_WKUP_MASK_FROM_COLUMN(3, 13)        | SET_WKUP_MASK_FROM_COLUMN(3, 14)        | SET_WKUP_MASK_FROM_COLUMN(3, 15)        \
							  | SET_WKUP_MASK_FROM_COLUMN(3, 16)        | SET_WKUP_MASK_FROM_COLUMN(3, 17)        )
uint16_t wkup_mask_p3 = WKUP_MASK_P3;

KBD_TYPE_QUALIFIER uint8_t kbd_input_ports[] KBD_ARRAY_ATTRIBUTE =
{
	COL(0),                     // column 0 (P0[5])
	COL(1),                     // column 1
	COL(2),                     // column 2
	COL(3),                     // column 3
	COL(4),                     // column 4
	COL(5),                     // column 5
	COL(6),                     // column 6
	COL(7),                     // column 7
	COL(8),                     // column 8
	COL(9),                     // column 9
	COL(10),                    // column 10
	COL(11),                    // column 11
	COL(12),                    // column 12
	COL(13),                    // column 13
	COL(14),                    // column 14
	COL(15),                    // column 15
	COL(16),                    // column 16
	COL(17)                     // column 17
};

KBD_TYPE_QUALIFIER uint8_t kbd_output_mode_regs[] KBD_ARRAY_ATTRIBUTE =
{
	SET_OUTPUT_MODE_REG(0),
	SET_OUTPUT_MODE_REG(1),
	SET_OUTPUT_MODE_REG(2),
	SET_OUTPUT_MODE_REG(3),
	SET_OUTPUT_MODE_REG(4),
	SET_OUTPUT_MODE_REG(5),
	SET_OUTPUT_MODE_REG(6),
	SET_OUTPUT_MODE_REG(7)
};

KBD_TYPE_QUALIFIER uint8_t kbd_output_reset_data_regs[] KBD_ARRAY_ATTRIBUTE =
{
	SET_RESET_REG(0),
	SET_RESET_REG(1),
	SET_RESET_REG(2),
	SET_RESET_REG(3),
	SET_RESET_REG(4),
	SET_RESET_REG(5),
	SET_RESET_REG(6),
	SET_RESET_REG(7)
};

KBD_TYPE_QUALIFIER uint16_t kbd_out_bitmasks[] KBD_ARRAY_ATTRIBUTE =
{
	SET_BITMAP(0),
	SET_BITMAP(1),
	SET_BITMAP(2),
	SET_BITMAP(3),
	SET_BITMAP(4),
	SET_BITMAP(5),
	SET_BITMAP(6),
	SET_BITMAP(7)
};

KBD_TYPE_QUALIFIER uint8_t kbd_input_mode_regs[] KBD_ARRAY_ATTRIBUTE =
{
	SET_INPUT_MODE_REG(0),
	SET_INPUT_MODE_REG(1),
	SET_INPUT_MODE_REG(2),
	SET_INPUT_MODE_REG(3),
	SET_INPUT_MODE_REG(4),
	SET_INPUT_MODE_REG(5),
	SET_INPUT_MODE_REG(6),
	SET_INPUT_MODE_REG(7),
	SET_INPUT_MODE_REG(8),
	SET_INPUT_MODE_REG(9),
	SET_INPUT_MODE_REG(10),
	SET_INPUT_MODE_REG(11),
	SET_INPUT_MODE_REG(12),
	SET_INPUT_MODE_REG(13),
	SET_INPUT_MODE_REG(14),
	SET_INPUT_MODE_REG(15),
	SET_INPUT_MODE_REG(16),
	SET_INPUT_MODE_REG(17)
};

#ifdef DELAYED_WAKEUP_ON
#define DELAYED_WAKEUP_GPIO_ROW			(X)	// UNDEFINED
#define DELAYED_WAKEUP_GPIO_COLUMN		(Y)	// UNDEFINED
#endif


// extra sets for 'hidden modifiers', e.g. the 'Fn' key
#define KBD_NR_SETS		(2)

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
      0         1         2         3         4         5           6         7         8         9         10        11        12        13        14        15        16        17
      ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
      #WWW Fav  #Med Sel  L-Ctrl    #WWW Ref  #Wake up  Home        Pg Up     Del       Insert    F9        -         F8        =         6         5         F2        F1        `
      #N Trk    Calc                #WWW Sea  Print     End         Pg Down   F11       F12       F10       0         9         8         7         4         3         2         1
      App       Vol+      Vol-      Mute      Scroll    + (num)     9 (num)   7 (num)   8 (num)   #         p         o         i         u         r         e         Caps      q
      #Pre-Trk  #Sleep    #LVT      L-Shift   #My Comp  #           6 (num)   4 (num)   5 (num)   Backspc   [         F7        ]         y         t         F3        w         Tab
      L-Win     Fn        #WWW Fwd  #WWW Back #         Enter (num) 3 (num)   1 (num)   2 (num)   \         ;         L         k         j         f         d         #         a
      Play      #WWW Home                     L-Alt     Up arrow    , (num)   #         0 (num)   F5        '         #         F6        h         g         F4        s         Esc
      #Stop     R-Win     R-Ctrl    R-Shift   #Power    Pause       * (num)   Num Lk    / (num)   Enter     \         .         ,         m         v         c         x         z
      #e-mail   #Fn-wrong                     R-Alt     Left arrow  - (num)   Dn arrow  Rgt arrow Space     /         #         #         n         b         #R-K      #L-K      #
      ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
	{ 0x0000,   0x0000,   0xFC01,   0x0000,   0x0000,   0x004A,     0x004B,   0x004C,   0x0049,   0x0042,   0x002D,   0x0041,   0x002E,   0x0023,   0x0022,   0x003B,   0x003A,   0x0035 }, // 0
	{ 0x0000,   0xF412,   0x0000,   0x0000,   0x0046,   0x004D,     0x004E,   0x0044,   0x0045,   0x0043,   0x0027,   0x0026,   0x0025,   0x0024,   0x0021,   0x0020,   0x001F,   0x001E }, // 1
	{ 0x0065,   0xF406,   0xF407,   0xF405,   0x0047,   0x0057,     0x0061,   0x005F,   0x0060,   0x0000,   0x0013,   0x0012,   0x000C,   0x0018,   0x0015,   0x0008,   0x0039,   0x0014 }, // 2
	{ 0x0000,   0x0000,   0x0000,   0xFC02,   0x0000,   0x0000,     0x005E,   0x005C,   0x005D,   0x002A,   0x002F,   0x0040,   0x0030,   0x001C,   0x0017,   0x003C,   0x001A,   0x002B }, // 3
	{ 0xFC08,   0xF801,   0x0000,   0x0000,   0x0000,   0x0058,     0x005B,   0x0059,   0x005A,   0x0064,   0x0033,   0x000F,   0x000E,   0x000D,   0x0009,   0x0007,   0x0000,   0x0004 }, // 4
	{ 0xF404,   0x0000,   0x0000,   0x0000,   0xFC04,   0x0052,     0x0063,   0x0000,   0x0062,   0x003E,   0x0034,   0x0000,   0x003F,   0x000B,   0x000A,   0x003D,   0x0016,   0x0029 }, // 5
	{ 0x0000,   0xFC80,   0xFC10,   0xFC20,   0x0000,   0x0048,     0x0055,   0x0053,   0x0054,   0x0028,   0x0064,   0x0037,   0x0036,   0x0010,   0x0019,   0x0006,   0x001B,   0x001D }, // 6
	{ 0x0000,   0x0000,   0x0000,   0x0000,   0xFC40,   0x0050,     0x0056,   0x0051,   0x004F,   0x002C,   0x0038,   0x0000,   0x0000,   0x0011,   0x0005,   0x0000,   0x0000,   0x0000 }  // 7
  },
  {
/*    Fn key 0x01 pressed
      0         1         2         3         4         5           6         7         8         9         10        11        12        13        14        15        16        17
      ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
      #         #         #         #         #         #           #         #         #         #         #         Nxt Trk   #         #         #         #         WWW Home  #
      #         #         #         #         Insert    #           #         Break     Scroll Lk #         #         #         #         #         #         #         #         #
      #         #         #         #         #         #           #         #         #         #         #         #         #         #         #         #         #         #
      #         #         #         #         #         #           #         #         #         #         #         Prv Trk   #         #         #         #         #         #
      #         Fn        #         #         #         #           #         #         #         #         #         #         #         #         #         #         #         #
      #         #         #         #         #         #           #         #         #         #         #         #         Stop      #         #         Eject     #         #
      #         #         #         #         #         #           #         #         #         #         #         #         #         #         #         #         #         #
      #         #         #         #         #         #           #         #         #         EXT_SLEEP #         #         #         #         #         #         #         #
      ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
	{ 0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,     0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0xF400,   0x0000,   0x0000,   0x0000,   0x0000,   0xF415,   0x0000,  }, // 0
	{ 0x0000,   0x0000,   0x0000,   0x0000,   0x0049,   0x0000,     0x0000,   0x0048,   0x0047,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,  }, // 1
	{ 0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,     0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,  }, // 2
	{ 0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,     0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0xF401,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,  }, // 3
	{ 0x0000,   0xF801,   0x0000,   0x0000,   0x0000,   0x0000,     0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,  }, // 4
	{ 0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,     0x0000,   0x0000,   0x0000,   0xF404,   0x0000,   0x0000,   0xF402,   0x0000,   0x0000,   0xF403,   0x0000,   0x0000,  }, // 5
	{ 0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,     0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,  }, // 6
	{ 0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,     0x0000,   0x0000,   0x0000,   KB_EXTSLP,0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,  }  // 7
  }
};


#define KBD_NR_COMBINATIONS 0
const struct key_combinations_t *const key_comb = NULL;

/// @} APP

#endif //_APP_KBD_MATRIX_SETUP_2_H_
