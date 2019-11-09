#include "freertos_headers.h"
#include "mc_hardware.h"
#include "l.h"
#include "dp.h"
#include "sr.h"
#include "aym_hardware.h"
#include <errno.h>
#include "ltc6903.h"
#include "aym_psg_parser.h"

#define TASK_LUA_STACK_SIZE 1000

static StaticTask_t task_lua_buf;
static StackType_t task_lua_stack[TASK_LUA_STACK_SIZE];

#define TASK_UP_DOWN_BUTTON 400

static StaticTask_t task_up_down_button_buf;
static StackType_t task_up_down_button_stack[TASK_UP_DOWN_BUTTON];

#define AY_DIP_28_PIN_INDEX 0
#define AY_DIP_40_PIN_INDEX 1

extern const uint8_t psg_track_test[];
extern uint32_t psg_track_test_len;

static void task_up_down_button (void *p) {
    p = p;

    sr_set_pin_ay_2_res();
    sr_reset_pin_pwr_ay_2_on();
    set_pin_pwr_5_v();
    sr_set_pin_pwr_ay_2_on();
    sr_reset_pin_ay_2_res();
    clear_aym_hardware();

    reset_shdn();

    ltc6903_start();
    ltc6903_set_requency(1.77e6, LTC6903_OUTPUT_MODE_CLK_ON_INV_OFF);

    start_tim_int_ay();

    aym_psg_reset();
    aym_psg_play(AY_DIP_40_PIN_INDEX, psg_track_test, psg_track_test_len);

    while (1) {
        vTaskDelay(1000);
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

    if ((rv = set_tim_lcd_pwm_duty(0.1)) != 0) {
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
