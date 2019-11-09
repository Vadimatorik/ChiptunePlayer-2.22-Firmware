#include "freertos_headers.h"
#include "mc_hardware.h"
#include "l.h"
#include "dp.h"
#include "sr.h"
#include "aym_hardware.h"
#include <errno.h>
#include "ltc6903.h"
#include "aym_note.h"

#define TASK_LUA_STACK_SIZE 1000

static StaticTask_t task_lua_buf;
static StackType_t task_lua_stack[TASK_LUA_STACK_SIZE];

#define TASK_UP_DOWN_BUTTON 400

static StaticTask_t task_up_down_button_buf;
static StackType_t task_up_down_button_stack[TASK_UP_DOWN_BUTTON];

#define AY_DIP_28_PIN_INDEX 1
#define AY_DIP_40_PIN_INDEX 0

static void task_up_down_button (void *p) {
    p = p;

    while(aym_note_reset(AY_DIP_40_PIN_INDEX) != 0) {
        vTaskDelay(100);
    }

    while (1) {
        static uint8_t i = 0;
        aym_note_write_note_to_channel(AY_DIP_40_PIN_INDEX, 1, i);
        i++;
        vTaskDelay(100);
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
