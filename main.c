#include "freertos_headers.h"
#include "mc_hardware.h"
#include "l.h"
#include "dp.h"

#include <errno.h>

#define TASK_LUA_STACK_SIZE 1000

static StaticTask_t task_lua_buf;
static StackType_t task_lua_stack[TASK_LUA_STACK_SIZE];

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

    if (xTaskCreateStatic(task_lua_interactive, "lua", TASK_LUA_STACK_SIZE, NULL, 3, task_lua_stack, &task_lua_buf) == NULL) {
        return ENOMEM;
    }

    vTaskStartScheduler();
}
