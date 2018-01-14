#include "bsp.h"
const  uint8_t keymaps[][MATRIX_ROWS][MATRIX_COLS]={
	    /* 0: qwerty */
KEYMAP_ANSI(
		ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10,  F11,  F12,  PSCR,  PAUS,  DEL, \
        GRAVE, 1,   2,   3,   4,   5,   6,   7,   8,   9,    0,  MINS, EQL,  BSPC,  		 HOME, \
        TAB,  Q,   W,   E,   R,   T,   Y,   U,   I,   O,    P,  LBRC,RBRC,  BSLS,   	 END, \
        CAPS, A,   S,   D,   F,   G,   H,   J,   K,   L, SCLN,  QUOT, 		ENT,  		 PGUP, \
        LSFT, Z,   X,   C,   V,   B,   N,   M,COMM,	DOT, SLSH,  RSFT, 				UP,	 PGDN, \
        LCTL,LGUI,LALT,          	SPC,                 RALT,	 FN0, RCTL,	LEFT,  DOWN,RGHT),
};
