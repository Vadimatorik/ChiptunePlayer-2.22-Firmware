#include "freertos_headers.h"
#include "mc_hardware.h"
#include "l.h"
#include "dp.h"
#include "sr.h"
#include "aym_hardware.h"
#include <errno.h>
#include "ltc6903.h"
#include "aym_psg_parser.h"
#include "u8g2.h"

#define TASK_LUA_STACK_SIZE 1000

static StaticTask_t task_lua_buf;
static StackType_t task_lua_stack[TASK_LUA_STACK_SIZE];

#define TASK_UP_DOWN_BUTTON 2000

static StaticTask_t task_up_down_button_buf;
static StackType_t task_up_down_button_stack[TASK_UP_DOWN_BUTTON];

#define AY_DIP_28_PIN_INDEX 0
#define AY_DIP_40_PIN_INDEX 1

extern const uint8_t psg_track_test[];
extern uint32_t psg_track_test_len;

u8g2_t gui;

uint8_t u8x8_byte_send (U8X8_UNUSED u8x8_t *u8x8, uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr) {
    switch (msg) {
        case U8X8_MSG_BYTE_INIT:
            break;

        case U8X8_MSG_BYTE_START_TRANSFER:
        case U8X8_MSG_BYTE_END_TRANSFER:
            break;

        case U8X8_MSG_BYTE_SEND:
            spi_lcd_tx(arg_ptr, arg_int);
            return arg_int;

        case U8X8_MSG_BYTE_SET_DC:
            u8x8_gpio_SetDC(u8x8, arg_int);
            break;

        default:
            while (1);
    }
    return 1;
}

uint8_t u8x8_io (U8X8_UNUSED u8x8_t *u8x8, uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr) {
    switch (msg) {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
            break;

        case U8X8_MSG_GPIO_DC:
            if (arg_int) {
                set_pin_lcd_dc();
            } else {
                reset_pin_lcd_dc();
            }
            break;

        case U8X8_MSG_GPIO_RESET:
            if (arg_int) {
                set_pin_lcd_rst();
            } else {
                reset_pin_lcd_rst();
            }
            break;

        case U8X8_MSG_DELAY_MILLI:
            vTaskDelay(arg_int);
            break;

        default:
            while (1);
    }
    return 1;
}

#include "ff.h"

FRESULT fr = 0;
DIR d = {0};
FILINFO f_info = {0};
FIL fa = {0};

static FATFS f = {0};

uint8_t read_psg_test[1024*5];

static void task_up_down_button (void *p) {
    p = p;
/*
    u8g2_Setup_st7565_ea_dogm128_f(&gui, U8G2_R0, u8x8_byte_send, u8x8_io);
    u8g2_InitDisplay(&gui);
    u8g2_ClearBuffer(&gui);
    u8g2_SendBuffer(&gui);
    u8g2_SetContrast(&gui, 4);
    u8g2_SetPowerSave(&gui, 0);

    u8g2_SetFont(&gui, u8g2_font_ncenB08_tr);
    u8g2_DrawStr(&gui, 0, 10, "Hello World!");
    u8g2_SendBuffer(&gui);



*/


    fr = f_mount(&f, "0", 1);

    if (fr != FR_OK) {
        while (1);
    };

    fr = f_findfirst(&d, &f_info, ".", "*.psg");

    if (fr != FR_OK) {
        while (1);
    };

    sr_set_pin_ay_1_res();
    sr_reset_pin_pwr_ay_1_on();
    set_pin_pwr_5_v();
    sr_set_pin_pwr_ay_1_on();
    sr_reset_pin_ay_1_res();
    clear_aym_hardware();

    reset_shdn();

    ltc6903_start();
    ltc6903_set_requency(1.77e6, LTC6903_OUTPUT_MODE_CLK_ON_INV_OFF);

    start_tim_int_ay();

    aym_psg_reset();


    while  (1) {
        fr = f_open(&fa, f_info.fname, FA_READ);

        if (fr != FR_OK) {
            while (1);
        };

        if (f_info.fname[0] == 0) {
            while (1);
        }


        while (1) {
            UINT rlen = 0;
            fr = f_read(&fa, read_psg_test, sizeof(read_psg_test), &rlen);

            if (fr != FR_OK) {
                while (1);
            };

            aym_psg_play(AY_DIP_28_PIN_INDEX, read_psg_test, rlen);

            if (rlen < sizeof(read_psg_test)) {
                break;
            }
        }

        fr = f_close(&fa);

        if (fr != FR_OK) {
            while (1);
        };

        fr = f_findnext(&d, &f_info);

        if (fr != FR_OK) {
            while (1);
        };

        if (f_info.fname[0] == 0) {
            while (1);
        }

    }
}

int main () {
    int rv = 0;

    if ((rv = init_core()) != 0) {
        return rv;
    }

    if ((rv = init_gpio()) != 0) {
        return rv;
    }

    if ((rv = init_rcc()) != 0) {
        return rv;
    }

    if ((rv = init_uart()) != 0) {
        return rv;
    }

    if ((rv = init_spi_board()) != 0) {
        return rv;
    }

    if ((rv = init_spi_lcd()) != 0) {
        return rv;
    }

    if ((rv = init_sr()) != 0) {
        return rv;
    }

    if ((rv = init_tim_int_ay()) != 0) {
        return rv;
    }

    if ((rv = init_tim_lcd_pwm()) != 0) {
        return rv;
    }

    if ((rv = set_tim_lcd_pwm_duty(0.5)) != 0) {
        return rv;
    }

    if ((rv = start_tim_lcd_pwm()) != 0) {
        return rv;
    }

    if ((rv = init_aym_hardware()) != 0) {
        return rv;
    }

    if ((rv = ltc6903_init()) != 0) {
        return rv;
    }

    if (xTaskCreateStatic(task_lua_interactive, "lua", TASK_LUA_STACK_SIZE, NULL, 3, task_lua_stack, &task_lua_buf) ==
        NULL) {
        return ENOMEM;
    }

    if (xTaskCreateStatic(task_up_down_button, "up_down_button", TASK_UP_DOWN_BUTTON, NULL, 2,
                          task_up_down_button_stack, &task_up_down_button_buf) == NULL) {
        return ENOMEM;
    }

    vTaskStartScheduler();
}
