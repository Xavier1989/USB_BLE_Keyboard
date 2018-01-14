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

#ifndef _CONFIG_H_
#define _CONFIG_H_

#define GPIO_COUNT      (32)

char disclaimer[] = "/**\n\
 ****************************************************************************************\n\
 *\n\
 * Copyright (C) 2014. Dialog Semiconductor Ltd, unpublished work. This computer\n\
 * program includes Confidential, Proprietary Information and is a Trade Secret of\n\
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited\n\
 * unless authorized in writing. All Rights Reserved.\n\
 *\n\
 * <bluetooth.support@diasemi.com> and contributors.\n\
 *\n\
 ****************************************************************************************\n\
 */\n";


char file_1_start_A[] = "#include <stdint.h>\n\
#include <stddef.h>\n\
\n\
#include \"app_kbd_config.h\"\n\
\n\
\n\
/**\n\
 ****************************************************************************************\n\
 * @addtogroup APP\n\
 * @ingroup HID\n\
 *\n\
 * @brief HID (Keyboard) Application matrix of setup #";

 char file_1_start_B[] = ".\n\
 *\n\
 * @{\n\
 ****************************************************************************************\n\
 */\n\
\n";

char file_1_eeprom_pair[] = "#if (HAS_EEPROM)\n\
# define PAIR               (0xF4F1)\n\
# define CLRP               (0xF4F2)\n\
#else\n\
# define PAIR               (0x0000)\n\
# define CLRP               (0x0000)\n\
#endif\n\
\n";

char file_1_ext_sleep[] = "#if (HAS_KEYBOARD_MEASURE_EXT_SLP)\n\
# define KB_EXTSLP          (0xF4F3)\n\
#else\n\
# define KB_EXTSLP          (0)\n\
#endif\n\
\n\
\n";

char file_1_input_array[] = "KBD_TYPE_QUALIFIER uint8_t kbd_input_ports[] KBD_ARRAY_ATTRIBUTE = \n{\n";

char file_1_close_array[] = "};\n\n";

char file_1_output_mode_array[] = "KBD_TYPE_QUALIFIER uint8_t kbd_output_mode_regs[] KBD_ARRAY_ATTRIBUTE =\n{\n";

char file_1_reset_data_array[] = "KBD_TYPE_QUALIFIER uint8_t kbd_output_reset_data_regs[] KBD_ARRAY_ATTRIBUTE =\n{\n";

char file_1_bitmask_array[] = "KBD_TYPE_QUALIFIER uint16_t kbd_out_bitmasks[] KBD_ARRAY_ATTRIBUTE =\n{\n";

char file_1_input_mode_array[] = "KBD_TYPE_QUALIFIER uint8_t kbd_input_mode_regs[] KBD_ARRAY_ATTRIBUTE =\n{\n";

char file_1_delayed_wakeup[] = \
"#ifdef DELAYED_WAKEUP_ON\n\
#define DELAYED_WAKEUP_GPIO_ROW\t\t\t(X)\t// UNDEFINED\n\
#define DELAYED_WAKEUP_GPIO_COLUMN\t\t(Y)\t// UNDEFINED\n\
#endif\n\n";


char file_1_keymap_start[] = \
"// unknown key code - nothing is sent to the other side but the key is examined for ghosting\n\
#define K_CODE\t\t\t(0xF4FF)\n\
\n\
\n\
// The key map.\n\
// 00xx means regular key\n\
// FCxx means modifier key.\n\
// F8xx means FN Modifier.\n\
// F4xy means special function (x = no of byte in the report, y no of bit set).\n\
KBD_TYPE_QUALIFIER uint16_t kbd_keymap[KBD_NR_SETS][KBD_NR_OUTPUTS][KBD_NR_INPUTS] KBD_ARRAY_ATTRIBUTE = \n\
{\n\
  {\n\
/*    No Fn key(s) pressed\n";


char file_1_keymap_end[] = \
"};\n\
\n\n\
#define KBD_NR_COMBINATIONS 0\n\
const struct key_combinations_t *const key_comb = NULL;\n\
\n\
/// @} APP\n";

char file_2_start_A[] = "/**\n\
 ****************************************************************************************\n\
 * @addtogroup APP\n\
 * @ingroup HID\n\
 *\n\
 * @brief HID (Keyboard) Application GPIO reservations for the matrix of setup #";

char file_2_start_B[] = ".\n\
 *\n\
 * @{\n\
 ****************************************************************************************\n\
 */\n\n\n";

char file_2_no_eeprom[] = "#define HAS_EEPROM\t\t\t(0)\n\
\n\
#if (HAS_MULTI_BOND)\n\
#error \"This keyboard setup does not support EEPROM!\"\n\
#endif\n\
\n\
// Switch required by the compiler\n\
#undef INIT_EEPROM_PINS\n\
#define INIT_EEPROM_PINS\t(0)\n\n";

char file_2_has_eeprom[] = "#ifdef EEPROM_ON\n\
\n\
#define HAS_EEPROM\t\t\t(1)\n\
\n\
#define EEPROM_BASE_ADDR\t(0)\t\t\t\t// Address offset of the bond info storage area in EEPROM\n\
#if (HAS_MULTI_BOND)\n\
#define MAX_BOND_PEER\t\t(0x04)\t\t\t// Maximum number of bonds\n\
#else\n\
#define MAX_BOND_PEER\t\t(0x01)\t\t\t// Maximum number of bonds\n\
#endif\n\
#define EEPROM_BOND_SIZE\t(256)\t\t\t// EEPROM_BOND_SIZE = (1 + no_of_bonds + no_of_bonds * 40) rounded up to the next multiple of 32\n\
\t\t\t\t\t\t\t\t\t\t\t// i.e. for no_of_bonds = 6, 1+6+6*40 = 247 ==> EEPROM_BOND_SIZE = 256\n\
\n\
#else\n\
#define HAS_EEPROM\t\t\t(0)\n\
#endif // EEPROM_ON\n\
\n\
#if (HAS_MULTI_BOND)\n\
 #if ( !(HAS_EEPROM) )\n\
  #error \"Multiple bonding support requires EEPROM!\"\n\
 #endif\n\
#endif // (HAS_MULTI_BOND)\n\
\n\
#undef INIT_EEPROM_PINS\n\
#define INIT_EEPROM_PINS\t(1)\n\n";

char file_2_no_leds[] = "\n\
#undef INIT_LED_PINS\n\
#define INIT_LED_PINS\t\t(0)\n\n";

#endif // _CONFIG_H_
