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

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../include/stdbool.h"

#include "../include/config.h"

#ifdef WIN32
#define snprintf _snprintf
#endif

char buf[256];


bool get_user_feedback(char *msg)
{
    bool ret = false;
    char ch;

    do {
        do {
            printf("%s", msg);
            fgets(buf, sizeof(buf), stdin);
        } while( (buf[0] != '\n') && !isalpha(buf[0]) );
        sscanf(buf, "%c", &ch);

        if ((ch == '\n') || (ch == 'Y') || (ch == 'y')) {
            ret = true;
            break;
        }
        else if ((ch == 'N') || (ch == 'n')) {
            ret = false;
            break;
        }
    } while(1);

    return ret;
}


void get_numeric_input(char *msg, int *value)
{
    do {
        printf("%s", msg);
        fgets(buf, sizeof(buf), stdin);
    } while(!isdigit(buf[0]));
    sscanf(buf, "%d", value);
}


void pins_input(int *pin_array, int num, char *name)
{
    int i;
    char sbuf[32];

    for (i = 0; i < num; i++) {
        snprintf(sbuf, sizeof(sbuf), "\t%s[%d]: ", name, i);
        get_numeric_input(sbuf, &pin_array[i]);
    }
}

void prepare_file1(int setup, int fn_no, int row_no, int *row, int col_no, int *col, int led_no, int *led, int eeprom_no, int *eeprom);
void prepare_file2(int setup, int fn_no, int row_no, int *row, int col_no, int *col, int led_no, int *led, bool led_level, int eeprom_no, int *eeprom);

int uart_no;
int uart_pins[2];

int app_main(void)
{
    bool answer;
    int column_no, row_no, setup;
    int led_no = 0;
    bool led_active;
    int eeprom_no = 0;
    int fn_no = 0;
    int column_pins[GPIO_COUNT], row_pins[GPIO_COUNT], led_pins[GPIO_COUNT], eeprom_pins[GPIO_COUNT], all_pins[4 * GPIO_COUNT];
    int i, j, total_pin_count;
    bool reused = false;

    printf("*\n* Keyboard application matrix and GPIOs configuration tool\n**********************************************************\n\n");

    // Get the dimensions of the Key Matrix
    do {
        answer = true;

        get_numeric_input("\nGive the number of inputs (columns) of the Key Matrix: ", &column_no);

        get_numeric_input("\nGive the number of outputs (rows) of the Key Matrix: ", &row_no);

        if ( (row_no > GPIO_COUNT) || (column_no > GPIO_COUNT) || (row_no < 0) || (column_no < 0) || ((column_no + row_no) > GPIO_COUNT) ) {
            printf(">>>> (Error) Invalid number of rows and/or columns. Must be [0..%d] and the total be less than %d...\n", GPIO_COUNT, GPIO_COUNT);
            answer = false;
        }
    } while (!answer);

    if (row_no > column_no) {
        printf("\n>>>> (Warning) The number of rows should be smaller than the number of columns to consume less power!\n\n");
        answer = get_user_feedback("Continue (Y/n)?");
        if (!answer)
            exit(1);
    }

    // Get the column pins
    printf("\nGive the column pins as XY, where X is the port and Y is the pin of that port.\n");
    pins_input(column_pins, column_no, "Column");

    // Get the row pins
    printf("\nGive the row pins as XY, where X is the port and Y is the pin of that port.\n");
    pins_input(row_pins, row_no, "Row");

    // Get the LED pins
    answer = get_user_feedback("\nWill you define any LED pins (Y/n)?");
    if (answer) {
#if 0
        // Get the number of the LED pins
        get_numeric_input("\nGive the number of the pins assigned to LEDs: ", &led_no);
#else
        printf(">>>> (Warning) The number of LED pins is preset to 2. The first pin controls the "\
               "Green LED and the second the Red LED.\n");
        led_no = 2;
#endif

        led_active = get_user_feedback("\nAre the LEDs active high (Y/n)?");

        // Get the LED pins
        if ( (led_no > 0) && (led_no < GPIO_COUNT) ) {
            printf("\nGive the LED pins as XY, where X is the port and Y is the pin of that port.\n");
            pins_input(led_pins, led_no, "LED pin");
        }
        else {
            printf(">>>> (Error) Invalid number of LED pins.\n");
            exit(1);
        }
    }

    // Get the EEPROM pins
    answer = get_user_feedback("\nDoes this setup include an EEPROM (Y/n)?");
    if (answer) {
#if 0
        // Get the number of the LED pins
        get_numeric_input("\nGive the number of the pins assigned to the EEPROM: ", &eeprom_no);
#else
        printf(">>>> (Warning) Only I2C EEPROM is supported by this application. The first pin is "\
               "the SCL and the second the SDA.\n");
        eeprom_no = 2;
#endif

        // Get the LED pins
        if ( (eeprom_no > 0) && (eeprom_no < GPIO_COUNT) ) {
            printf("\nGive the EEPROM pins as XY, where X is the port and Y is the pin of that port.\n");
            pins_input(eeprom_pins, eeprom_no, "EEPROM pin");
        }
        else {
            printf(">>>> (Error) Invalid number of EEPROM pins.\n");
            exit(1);
        }
    }

    // Store the UART pins
    answer = get_user_feedback("\nDoes this setup include a UART (Y/n)?");
    if (answer) {
        // Get the number of the LED pins
        do {
            get_numeric_input("\nGive the number of the pins assigned to the UART (1 or 2): ", &uart_no);
        } while ((uart_no != 1) && (uart_no != 2));

        // Get the UART pins
        printf("\nGive the UART pins as XY, where X is the port and Y is the pin of that port.\n");
        pins_input(uart_pins, uart_no, "UART pin");
    }

    // Check pin re-use
    memset(all_pins, 0, sizeof(all_pins));
    j = 0;
    for (i = 0; i < column_no; i++) {
        all_pins[j] = column_pins[i];
        j++;
    }
    for (i = 0; i < row_no; i++) {
        all_pins[j] = row_pins[i];
        j++;
    }
    for (i = 0; i < led_no; i++) {
        all_pins[j] = led_pins[i];
        j++;
    }
    for (i = 0; i < eeprom_no; i++) {
        all_pins[j] = eeprom_pins[i];
        j++;
    }
    total_pin_count = j;
    for (i = 0; (i < total_pin_count) && !reused; i++) {
        for (j = i + 1; j < total_pin_count; j++) {
            if (all_pins[i] == all_pins[j]) {
                reused = true;
                break;
            }
        }
    }
    if (reused) {
        printf(">>>> (Error) One or more pins are used more than 1 time.\n");
        answer = get_user_feedback("\nContinue (Y/n)?");
        if(!answer)
            exit(1);
    }
    for (i = 0; (i < led_no) && !reused; i++) {
        for (j = 0; j < uart_no; j++) {
            if (led_pins[i] == uart_pins[j]) {
                reused = true;
                break;
            }
        }
    }
    if (reused) {
        printf(">>>> (Error) A UART pin is shared with a LED pin.\n");
        answer = get_user_feedback("\nContinue (Y/n)?");
        if(!answer)
            exit(1);
        reused = false;
    }
    for (i = 0; (i < eeprom_no) && !reused; i++) {
        for (j = 0; j < uart_no; j++) {
            if (eeprom_pins[i] == uart_pins[j]) {
                reused = true;
                break;
            }
        }
    }
    if (reused) {
        printf(">>>> (Error) A UART pin is shared with an EEPROM pin.\n");
        answer = get_user_feedback("\nContinue (Y/n)?");
        if(!answer)
            exit(1);
        reused = false;
    }

    // Get the number of alternate keymaps (due to 'Fn' keys)
    get_numeric_input("\nGive the number of the keymap sets that exist for this setup (due to 'Fn' keys): ", &fn_no);
    if ((fn_no <= 0) || (fn_no > 10)) {
        fn_no = 1;
        printf(">>>> (Warning) Invalid number entered. Default will be used (=1).\n");
    }

    // Get the Matrix setup no
    get_numeric_input("\nGive the index of this setup: ", &setup);

    // Summary output
    printf("\n\n*\n* Summary for MATRIX_SETUP %d\n*\n", setup);
    printf("Matrix size (rows by columns) : %d by %d\n", row_no, column_no);
    printf("Columns are :\n");
    for (i = 0; i < column_no; i++)
        printf("\t[%d] : %02d\n", i, column_pins[i]);
    printf("Rows are :\n");
    for (i = 0; i < row_no; i++)
        printf("\t[%d] : %02d\n", i, row_pins[i]);
    if (led_no) {
        printf("LED pins are : \n");
#if 0
        for (i = 0; i < led_no; i++)
            printf("\t[%d] : %02d\n", i, led_pins[i]);
#else
        printf("\tGreen LED : %02d\n", led_pins[0]);
        printf("\tRed LED   : %02d\n", led_pins[1]);
#endif
        printf("LEDs are active %s.\n", led_active ? "high" : "low");
    }
    if (eeprom_no) {
        printf("EEPROM pins are : \n");
#if 0
        for (i = 0; i < eeprom_no; i++)
            printf("\t[%d] : %02d\n", i, eeprom_pins[i]);
#else
        printf("\tSCK : %02d\n", eeprom_pins[0]);
        printf("\tSDA : %02d\n", eeprom_pins[1]);
#endif
    }
    printf("Produced file names\n");
    printf("\tapp_kbd_matrix_setup_%d.h\n", setup);
    printf("\tapp_kbd_key_matrix_setup_%d.h\n", setup);

    answer = get_user_feedback("Correct (Y/n)?");
    if (!answer)
        exit(1);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // END OF USER INTERACTION!
    ///////////////////////////////////////////////////////////////////////////////////////////////

    // Prepare the app_kbd_matrix_setup_X.h file
    prepare_file1(setup, fn_no, row_no, row_pins, column_no, column_pins, led_no, led_pins, eeprom_no, eeprom_pins);

    // Prepare the app_kbd_key_matrix_setup_X.h file
    prepare_file2(setup, fn_no, row_no, row_pins, column_no, column_pins, led_no, led_pins, led_active, eeprom_no, eeprom_pins);

    return 0;
}




void strupper(char *s) {
    while (*s) {
        if ((*s >= 'a' ) && (*s <= 'z')) *s -= ('a'-'A');
        s++;
    }
}


void prepare_file1(int setup, int fn_no, int row_no, int *row, int col_no, int *col, int led_no, int *led, int eeprom_no, int *eeprom)
{
    int i, j, k;
    char sbuf[64];
    FILE *fp;
    bool port_has_input[4] = {false, false, false, false};

    snprintf(sbuf, sizeof(sbuf), "app_kbd_matrix_setup_%d.h", setup);

    fp = fopen(sbuf, "w");
    if (fp == NULL) {
        printf("Error in opening app_kbd_matrix_setup_X.h file for writing...\n");
        return;
    }

    fprintf(fp, "%s\n\n", disclaimer);

    snprintf(sbuf, sizeof(sbuf), "app_kbd_matrix_setup_%d_h", setup);
    strupper(sbuf);
    fprintf(fp, "#ifndef _%s_\n", sbuf);
    fprintf(fp, "#define _%s_\n\n", sbuf);

    fprintf(fp, "%s", file_1_start_A);
    fprintf(fp, "%d", setup);
    fprintf(fp, "%s", file_1_start_B);
    fprintf(fp, "%s", file_1_eeprom_pair);
    fprintf(fp, "%s", file_1_ext_sleep);

    fprintf(fp, "#define KBD_NR_INPUTS\t\t(%d)\n", col_no);
    fprintf(fp, "#define KBD_NR_OUTPUTS\t\t(%d)\n\n\n", row_no);

    for (i = 0; i < col_no; i++) {
        int port = (col[i] / 10);
        int pin = (col[i] % 10);

        if (port < 4)
            port_has_input[port] = true;

        fprintf(fp, "#define COLUMN_%d_PORT\t\t(%d)\t//port: %d, pin: %d\n", i, port, port, pin);
        fprintf(fp, "#define COLUMN_%d_PIN\t\t(%d)\n\n", i, pin);
    }
    fprintf(fp, "\n// used for cycle optimization\n");
    for (i = 0; i < 4; i++)
        fprintf(fp, "#define P%d_HAS_INPUT\t\t(%d)\n", i, (port_has_input[i] ? 1 : 0) );
    fprintf(fp, "\n");


    for (i = 0; i < row_no; i++) {
        int port = (row[i] / 10);
        int pin = (row[i] % 10);

        fprintf(fp, "#define ROW_%d_PORT\t\t\t(%d)\t//port: %d, pin: %d\n", i, port, port, pin);
        fprintf(fp, "#define ROW_%d_PIN\t\t\t(%d)\n\n", i, pin);
    }
    fprintf(fp, "\n");


    fprintf(fp, "// Masks for the initialization of the KBD controller\n");
    //MASK_P0
    fprintf(fp, "#define MASK_P0\t\t\t\t(0x4000\t| ");
    for (j = 0; j < col_no; j++) {
        char tbuf[40];

        snprintf(tbuf, sizeof(tbuf), "SET_MASK0_FROM_COLUMN(%d)", j);
        fprintf(fp, "%-40s", tbuf);

        if ((j+1) == col_no)
            fprintf(fp, ")\n");
        else if ( 0 == ((j+1) % 4))
            fprintf(fp, "\\\n\t\t\t\t\t\t\t\t\t| ");
        else
            fprintf(fp, "| ");
    }
    fprintf(fp, "uint16_t mask_p0 = MASK_P0;\n\n");
    //MASK_P12
    fprintf(fp, "#define MASK_P12\t\t\t(0x0000\t| ");
    for (j = 0; j < col_no; j++) {
        char tbuf[40];

        snprintf(tbuf, sizeof(tbuf), "SET_MASK12_FROM_COLUMN(%d)", j);
        fprintf(fp, "%-40s", tbuf);

        if ((j+1) == col_no)
            fprintf(fp, ")\n");
        else if ( 0 == ((j+1) % 4))
            fprintf(fp, "\\\n\t\t\t\t\t\t\t\t\t| ");
        else
            fprintf(fp, "| ");
    }
    fprintf(fp, "uint16_t mask_p12 = MASK_P12;\n\n");
    //MASK_P3
    fprintf(fp, "#define MASK_P3\t\t\t\t(0x0000\t| ");
    for (j = 0; j < col_no; j++) {
        char tbuf[40];

        snprintf(tbuf, sizeof(tbuf), "SET_MASK3_FROM_COLUMN(%d)", j);
        fprintf(fp, "%-40s", tbuf);

        if ((j+1) == col_no)
            fprintf(fp, ")\n");
        else if ( 0 == ((j+1) % 4))
            fprintf(fp, "\\\n\t\t\t\t\t\t\t\t\t| ");
        else
            fprintf(fp, "| ");
    }
    fprintf(fp, "uint16_t mask_p3 = MASK_P3;\n\n");


    fprintf(fp, "// Masks for the initialization of the WKUP controller\n");
    for (i = 0; i < 4; i++) {
        fprintf(fp, "#define WKUP_MASK_P%d\t\t(\t", i);

        for (j = 0; j < col_no; j++) {
            char tbuf[40];

            snprintf(tbuf, sizeof(tbuf), "SET_WKUP_MASK_FROM_COLUMN(%d, %d)", i, j);
            fprintf(fp, "%-40s", tbuf);

            if ((j+1) == col_no)
                fprintf(fp, ")\n");
            else if ( 0 == ((j+1) % 4))
                fprintf(fp, "\\\n\t\t\t\t\t\t\t  | ");
            else
                fprintf(fp, "| ");
        }
        fprintf(fp, "uint16_t wkup_mask_p%d = WKUP_MASK_P%d;\n\n", i, i);
    }

    //kbd_input_ports[]
    fprintf(fp, "%s", file_1_input_array);
    for (i = 0; i < col_no; i++) {
        int port = (col[i] / 10);
        int pin = (col[i] % 10);

        char tbuf[40];

        if (i < (col_no-1))
            snprintf(tbuf, sizeof(tbuf), "\tCOL(%d),", i);
        else
            snprintf(tbuf, sizeof(tbuf), "\tCOL(%d)", i);

        if (i == 0)
            fprintf(fp, "%-29s// column %d (P%d[%d])\n", tbuf, i, port, pin);
        else
            fprintf(fp, "%-29s// column %d\n", tbuf, i);
    }
    fprintf(fp, "%s", file_1_close_array);

    //kbd_output_mode_regs[]
    fprintf(fp, "%s", file_1_output_mode_array);
    for (i = 0; i < row_no; i++) {
        fprintf(fp, "\tSET_OUTPUT_MODE_REG(%d)", i);
        if (i < (row_no-1))
            fprintf(fp, ",\n");
        else
            fprintf(fp, "\n");
    }
    fprintf(fp, "%s", file_1_close_array);

    //kbd_output_reset_data_regs[]
    fprintf(fp, "%s", file_1_reset_data_array);
    for (i = 0; i < row_no; i++) {
        fprintf(fp, "\tSET_RESET_REG(%d)", i);
        if (i < (row_no-1))
            fprintf(fp, ",\n");
        else
            fprintf(fp, "\n");
    }
    fprintf(fp, "%s", file_1_close_array);

    //kbd_out_bitmasks
    fprintf(fp, "%s", file_1_bitmask_array);
    for (i = 0; i < row_no; i++) {
        fprintf(fp, "\tSET_BITMAP(%d)", i);
        if (i < (row_no-1))
            fprintf(fp, ",\n");
        else
            fprintf(fp, "\n");
    }
    fprintf(fp, "%s", file_1_close_array);

    //kbd_input_mode_regs[]
    fprintf(fp, "%s", file_1_input_mode_array);
    for (i = 0; i < col_no; i++) {
        fprintf(fp, "\tSET_INPUT_MODE_REG(%d)", i);
        if (i < (col_no-1))
            fprintf(fp, ",\n");
        else
            fprintf(fp, "\n");
    }
    fprintf(fp, "%s", file_1_close_array);

    //DELAYED_WAKEUP
    fprintf(fp, "%s\n", file_1_delayed_wakeup);

    //KBD_NR_SETS
    fprintf(fp, "// extra sets for 'hidden modifiers', e.g. the 'Fn' key\n");
    fprintf(fp, "#define KBD_NR_SETS\t\t(%d)\n\n", fn_no);

    //KeyMap
    fprintf(fp, "%s", file_1_keymap_start);
    fprintf(fp, "      ");
    for (i = 0; i < fn_no; i++) {
        for (j = 0; j < col_no; j++) {
            char tbuf[10];

            snprintf(tbuf, sizeof(tbuf), "%d", j);
            fprintf(fp, "%-10s", tbuf);
        }
        fprintf(fp, "\n      ");

        for (j = 0; j < col_no; j++) {
            fprintf(fp, "----------");
        }
        fprintf(fp, "\n      ");

        for (k = 0; k < row_no; k++) {
            for (j = 0; j < col_no; j++) {
                char tbuf[10];

                snprintf(tbuf, sizeof(tbuf), "#");
                fprintf(fp, "%-10s", tbuf);
            }
            fprintf(fp, "\n      ");
        }

        for (j = 0; j < col_no; j++) {
            fprintf(fp, "----------");
        }
        fprintf(fp, "*/\n");

        for (k = 0; k < row_no; k++) {
            fprintf(fp, "\t{ ");
            for (j = 0; j < col_no; j++) {
                char tbuf[10];

                if (j < (col_no-1))
                    snprintf(tbuf, sizeof(tbuf), "0x0000,");
                else
                    snprintf(tbuf, sizeof(tbuf), "0x0000");
                fprintf(fp, "%-10s", tbuf);
            }
            if (k == (row_no-1))
                fprintf(fp, "  }  // %d\n ", k);
            else
                fprintf(fp, "  }, // %d\n ", k);
        }
        if (i == (fn_no-1))
            fprintf(fp, " }\n");
        else {
            fprintf(fp, " },\n");
            fprintf(fp, "  {\n");
            fprintf(fp, "/*    Fn key 0x%02d pressed\n      ", (i+1));
        }
    }
    fprintf(fp, "%s", file_1_keymap_end);

    fprintf(fp, "\n#endif //_%s_\n", sbuf);

    fclose(fp);
}


int lookup_array(int *array, int size, int pin)
{
    int i;
    int ret = GPIO_COUNT;

    for (i = 0; i < size; i++) {
        if (array[i] == pin) {
            ret = i;
            break;
        }
    }

    return ret;
}


bool lookup_all_arrays(int row_no, int *row, int col_no, int *col, int led_no, int *led, int eeprom_no, int *eeprom, int pin)
{
    bool found;

    do {
        found = (lookup_array(col, col_no, pin) != GPIO_COUNT);
        if (found) break;

        found = (lookup_array(row, row_no, pin) != GPIO_COUNT);
        if (found) break;

        found = (lookup_array(led, led_no, pin) != GPIO_COUNT);
        if (found) break;

        found = (lookup_array(eeprom, eeprom_no, pin) != GPIO_COUNT);
    } while(0);

    return found;
}


void prepare_file2(int setup, int fn_no, int row_no, int *row, int col_no, int *col, int led_no, int *led, bool led_level, int eeprom_no, int *eeprom)
{
    int i;
    char sbuf[64], tbuf[85], name_s[15], port_s[15], pin_s[15];
    FILE *fp;
    bool found_tx = false;
    bool found_rx = false;

    snprintf(sbuf, sizeof(sbuf), "app_kbd_key_matrix_setup_%d.h", setup);

    fp = fopen(sbuf, "w");
    if (fp == NULL) {
        printf("Error in opening app_kbd_key_matrix_setup_X.h file for writing...\n");
        return;
    }

    fprintf(fp, "%s\n\n", disclaimer);

    snprintf(sbuf, sizeof(sbuf), "app_kbd_key_matrix_setup_%d_h", setup);
    strupper(sbuf);
    fprintf(fp, "#ifndef _%s_\n", sbuf);
    fprintf(fp, "#define _%s_\n\n", sbuf);

    fprintf(fp, "%s", file_2_start_A);
    fprintf(fp, "%d", setup);
    fprintf(fp, "%s", file_2_start_B);
    if (eeprom_no) {
        fprintf(fp, "%s", file_2_has_eeprom);
        fprintf(fp, "// Pin definition (irrelevant to whether EEPROM pins are used or not since the HW setup is fixed!)\n");
        fprintf(fp, "#define I2C_SDA_PORT\t\tGPIO_PORT_%d\n", (eeprom[1] / 10));
        fprintf(fp, "#define I2C_SDA_PIN\t\t\tGPIO_PIN_%d\n", (eeprom[1] % 10));
        fprintf(fp, "#define I2C_SCL_PORT\t\tGPIO_PORT_%d\n", (eeprom[0] / 10));
        fprintf(fp, "#define I2C_SCL_PIN\t\t\tGPIO_PIN_%d\n\n", (eeprom[0] % 10));
    }
    else {
        fprintf(fp, "%s", file_2_no_eeprom);
    }

    if (led_no) {
        fprintf(fp, "// Pin definition (irrelevant to whether LED pins are used or not since the HW setup is fixed!)\n");
        fprintf(fp, "#define KBD_GREEN_LED_PORT\tGPIO_PORT_%d\n", (led[0] / 10));
        fprintf(fp, "#define KBD_GREEN_LED_PIN\tGPIO_PIN_%d\n\n", (led[0] % 10));
        fprintf(fp, "#define KBD_RED_LED_PORT\tGPIO_PORT_%d\n", (led[1] / 10));
        fprintf(fp, "#define KBD_RED_LED_PIN\t\tGPIO_PIN_%d\n\n", (led[1] % 10));
        fprintf(fp, "#undef INIT_LED_PINS\n");
        fprintf(fp, "#define INIT_LED_PINS\t\t(1)\n");
        fprintf(fp, "#define LED_STATE_ON\t\t%s\t\t\t// LEDs are active %s\n", led_level ? "true" : "false", led_level ? "high" : "low");
        fprintf(fp, "#define LED_STATE_OFF\t\t%s\n\n", led_level ? "false" : "true");
    }
    else {
        fprintf(fp, "%s", file_2_no_leds);
    }

    //Pins Reservation

    //CFG_PRINTF
    if (uart_no == 1)
        found_tx = lookup_all_arrays(row_no, row, col_no, col, led_no, led, eeprom_no, eeprom, uart_pins[0]);
    else if (uart_no == 2) {
        found_tx = lookup_all_arrays(row_no, row, col_no, col, led_no, led, eeprom_no, eeprom, uart_pins[0]);
        found_rx = lookup_all_arrays(row_no, row, col_no, col, led_no, led, eeprom_no, eeprom, uart_pins[1]);
    }
    if(found_tx) {
        fprintf(fp, "#if !defined(CFG_PRINTF)\n");

        snprintf(tbuf, sizeof(tbuf), "#define DECLARE_UART_USED_PINS");
        fprintf(fp, "%-85s\t\\\n", tbuf);

        snprintf(tbuf, sizeof(tbuf), "    {");
        fprintf(fp, "%-85s\t\\\n", tbuf);

        {
            int pos = lookup_array(col, col_no, uart_pins[0]);
            int actual_pos;

            if (pos == GPIO_COUNT)
                actual_pos = lookup_array(row, row_no, uart_pins[0]);
            else
                actual_pos = pos;

            if (pos != GPIO_COUNT) {
                snprintf(name_s, sizeof(name_s), "INPUT_COL_%d", actual_pos);
                snprintf(port_s, sizeof(port_s), "GPIO_PORT_%d", (col[actual_pos] / 10));
                snprintf(pin_s, sizeof(pin_s), "GPIO_PIN_%d", (col[actual_pos] % 10));

                snprintf(tbuf, sizeof(tbuf), "        RESERVE_GPIO( %15s,%15s,%15s,%15s", name_s, port_s, pin_s, "PID_GPIO );");
            }
            else {
                snprintf(name_s, sizeof(name_s), "OUTPUT_ROW_%d", actual_pos);
                snprintf(port_s, sizeof(port_s), "GPIO_PORT_%d", (row[actual_pos] / 10));
                snprintf(pin_s, sizeof(pin_s), "GPIO_PIN_%d", (row[actual_pos] % 10));

                snprintf(tbuf, sizeof(tbuf), "        RESERVE_GPIO( %15s,%15s,%15s,%15s", name_s, port_s, pin_s, "PID_GPIO );");
            }

            fprintf(fp, "%-85s\t\\\n", tbuf);

            if (found_rx) {
                pos = lookup_array(col, col_no, uart_pins[1]);

                if (pos == GPIO_COUNT)
                    actual_pos = lookup_array(row, row_no, uart_pins[1]);
                else
                    actual_pos = pos;

                if (pos != GPIO_COUNT) {

                    snprintf(name_s, sizeof(name_s), "INPUT_COL_%d", actual_pos);
                    snprintf(port_s, sizeof(port_s), "GPIO_PORT_%d", (col[actual_pos] / 10));
                    snprintf(pin_s, sizeof(pin_s), "GPIO_PIN_%d", (col[actual_pos] % 10));

                    snprintf(tbuf, sizeof(tbuf), "        RESERVE_GPIO( %15s,%15s,%15s,%15s", name_s, port_s, pin_s, "PID_GPIO );");
                }
                else {
                    snprintf(name_s, sizeof(name_s), "OUTPUT_ROW_%d", actual_pos);
                    snprintf(port_s, sizeof(port_s), "GPIO_PORT_%d", (row[actual_pos] / 10));
                    snprintf(pin_s, sizeof(pin_s), "GPIO_PIN_%d", (row[actual_pos] % 10));

                    snprintf(tbuf, sizeof(tbuf), "        RESERVE_GPIO( %15s,%15s,%15s,%15s", name_s, port_s, pin_s, "PID_GPIO );");
                }

                fprintf(fp, "%-85s\t\\\n", tbuf);
            }

        }
        fprintf(fp, "    }\n");
        fprintf(fp, "#else\n");
        fprintf(fp, "#define DECLARE_UART_USED_PINS\n");
        fprintf(fp, "#endif\n\n");
    }
    else
        fprintf(fp, "#define DECLARE_UART_USED_PINS\n\n");

    //EEPROM pins
    if (eeprom_no) {
        snprintf(tbuf, sizeof(tbuf), "#define DECLARE_EEPROM_PINS");
        fprintf(fp, "%-85s\t\\\n", tbuf);

        snprintf(tbuf, sizeof(tbuf), "    {");
        fprintf(fp, "%-85s\t\\\n", tbuf);

        snprintf(name_s, sizeof(name_s), "EEPROM_SDA");
        snprintf(port_s, sizeof(port_s), "GPIO_PORT_%d", (eeprom[1] / 10));
        snprintf(pin_s, sizeof(pin_s), "GPIO_PIN_%d", (eeprom[1] % 10));
        snprintf(tbuf, sizeof(tbuf), "        RESERVE_GPIO( %15s,%15s,%15s,%15s", name_s, port_s, pin_s, "PID_I2C_SDA );");
        fprintf(fp, "%-85s\t\\\n", tbuf);

        snprintf(name_s, sizeof(name_s), "EEPROM_SCL");
        snprintf(port_s, sizeof(port_s), "GPIO_PORT_%d", (eeprom[0] / 10));
        snprintf(pin_s, sizeof(pin_s), "GPIO_PIN_%d", (eeprom[0] % 10));
        snprintf(tbuf, sizeof(tbuf), "        RESERVE_GPIO( %15s,%15s,%15s,%15s", name_s, port_s, pin_s, "PID_I2C_SCL );");
        fprintf(fp, "%-85s\t\\\n", tbuf);
        fprintf(fp, "    }\n\n");
    }
    else
        fprintf(fp, "#define DECLARE_EEPROM_PINS\n\n");

    //LED pins
    if (led_no) {
        snprintf(tbuf, sizeof(tbuf), "#define DECLARE_LED_PINS");
        fprintf(fp, "%-85s\t\\\n", tbuf);

        snprintf(tbuf, sizeof(tbuf), "    {");
        fprintf(fp, "%-85s\t\\\n", tbuf);

        snprintf(name_s, sizeof(name_s), "GREEN_LED");
        snprintf(port_s, sizeof(port_s), "GPIO_PORT_%d", (led[0] / 10));
        snprintf(pin_s, sizeof(pin_s), "GPIO_PIN_%d", (led[0] % 10));

        snprintf(tbuf, sizeof(tbuf), "        RESERVE_GPIO( %15s,%15s,%15s,%15s", name_s, port_s, pin_s, "PID_GPIO );");
        fprintf(fp, "%-85s\t\\\n", tbuf);

        snprintf(name_s, sizeof(name_s), "RED_LED");
        snprintf(port_s, sizeof(port_s), "GPIO_PORT_%d", (led[1] / 10));
        snprintf(pin_s, sizeof(pin_s), "GPIO_PIN_%d", (led[1] % 10));

        snprintf(tbuf, sizeof(tbuf), "        RESERVE_GPIO( %15s,%15s,%15s,%15s", name_s, port_s, pin_s, "PID_GPIO );");
        fprintf(fp, "%-85s\t\\\n", tbuf);
        fprintf(fp, "    }\n\n");
    }
    else
        fprintf(fp, "#define DECLARE_LED_PINS\n\n");

    //KEYBOARD pins
    {
        snprintf(tbuf, sizeof(tbuf), "#define DECLARE_KEYBOARD_GPIOS");
        fprintf(fp, "%-85s\t\\\n", tbuf);

        snprintf(tbuf, sizeof(tbuf), "    {");
        fprintf(fp, "%-85s\t\\\n", tbuf);

        for (i = 0; i < col_no; i++) {
            if (lookup_array(uart_pins, uart_no, col[i]) != GPIO_COUNT)
                continue;

            snprintf(name_s, sizeof(name_s), "INPUT_COL_%d", i);
            snprintf(port_s, sizeof(port_s), "GPIO_PORT_%d", (col[i] / 10));
            snprintf(pin_s, sizeof(pin_s), "GPIO_PIN_%d", (col[i] % 10));

            snprintf(tbuf, sizeof(tbuf), "        RESERVE_GPIO( %15s,%15s,%15s,%15s", name_s, port_s, pin_s, "PID_GPIO );");
            fprintf(fp, "%-85s\t\\\n", tbuf);
        }

        for (i = 0; i < row_no; i++) {
            if (lookup_array(uart_pins, uart_no, row[i]) != GPIO_COUNT)
                continue;

            snprintf(name_s, sizeof(name_s), "OUTPUT_ROW_%d", i);
            snprintf(port_s, sizeof(port_s), "GPIO_PORT_%d", (row[i] / 10));
            snprintf(pin_s, sizeof(pin_s), "GPIO_PIN_%d", (row[i] % 10));

            snprintf(tbuf, sizeof(tbuf), "        RESERVE_GPIO( %15s,%15s,%15s,%15s", name_s, port_s, pin_s, "PID_GPIO );");
            fprintf(fp, "%-85s\t\\\n", tbuf);
        }

        snprintf(tbuf, sizeof(tbuf), "        DECLARE_EEPROM_PINS");
        fprintf(fp, "%-85s\t\\\n", tbuf);
        snprintf(tbuf, sizeof(tbuf), "        DECLARE_LED_PINS");
        fprintf(fp, "%-85s\t\\\n", tbuf);
        snprintf(tbuf, sizeof(tbuf), "        DECLARE_UART_USED_PINS");
        fprintf(fp, "%-85s\t\\\n", tbuf);

        fprintf(fp, "    }\n");
    }

    fprintf(fp, "\n\n/// @} APP\n");
    fprintf(fp, "\n#endif //_%s_\n", sbuf);

    fclose(fp);
}

