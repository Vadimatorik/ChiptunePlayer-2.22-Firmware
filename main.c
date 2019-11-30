#include "freertos_headers.h"
#include "mc_hardware.h"

#include "dp.h"
#include "sr.h"


#include <errno.h>
#include "ltc6903.h"
#include "aym_psg_parser.h"
#include "l.h"

#define TASK_LUA_STACK_SIZE 1000

static StaticTask_t task_lua_buf;
static StackType_t task_lua_stack[TASK_LUA_STACK_SIZE];

#define TASK_UP_DOWN_BUTTON 2000

static StaticTask_t task_up_down_button_buf;
static StackType_t task_up_down_button_stack[TASK_UP_DOWN_BUTTON];

#define AY_DIP_28_PIN_INDEX 0
#define AY_DIP_40_PIN_INDEX 1

//

/*
#include "ff.h"

FRESULT fr = 0;
DIR d = {0};
FILINFO f_info = {0};
FIL fa = {0};

static FATFS f = {0};

uint8_t read_psg_test[1024*1];
*/
static void task_up_down_button (void *p) {
    p = p;
/*


    u8g2_SetFont(&gui, u8g2_font_ncenB08_tr);
    u8g2_DrawStr(&gui, 0, 10, "Hello World!");
    u8g2_SendBuffer(&gui);
*/

int rv = 0;
/*
    if ((rv = init_dp()) != 0) {
        return rv;
    }

    if ((rv = init_sr()) != 0) {
        while(1);
    }



    if ((rv = init_aym_hardware()) != 0) {
        while(1);
    }

    if ((rv = ltc6903_init()) != 0) {
        while(1);
    }


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
    dp_reset_shdn();

    ltc6903_start();
    ltc6903_set_requency(1.77e6, LTC6903_OUTPUT_MODE_CLK_ON_INV_OFF);

    start_tim_int_ay();

    aym_psg_reset();
*/

    while (1) {
      /*  fr = f_open(&fa, f_info.fname, FA_READ);

        if (fr != FR_OK) {
            while (1);
        };

        if (f_info.fname[0] == 0) {
            while (1);
        }


        while (1) {
            if ((rv = dp_set_a1(255)) != 0) { // 0 - отключить звук.
                while(1);
            }

            if ((rv = dp_set_b1(255)) != 0) {
                while(1);
            }

            if ((rv = dp_set_c1(255)) != 0) {
                while(1);
            }

            if ((rv = dp_set_a2(0)) != 0) {
                while(1);
            }

            if ((rv = dp_set_b2(0)) != 0) {
                while(1);
            }

            if ((rv = dp_set_c2(0)) != 0) {
                while(1);
            }

            if ((rv = dp_set_l(255)) != 0) {
                while(1);
            }

            if ((rv = dp_set_r(255)) != 0) {
                while(1);
            }

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
        aym_psg_reset();


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
        }*/

    }
}

int main () {
    int rv = 0;
    if ((rv = init_mc_hardware()) != 0) {
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
