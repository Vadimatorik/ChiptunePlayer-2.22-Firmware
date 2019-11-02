#include "freertos_headers.h"
#include "hardware.h"
#include "lua.h"

#include <errno.h>
#include <stdio.h>

#define TASK_LUA_STACK_SIZE 1000
StaticTask_t task_lua_buf;
StackType_t task_lua_stack[TASK_LUA_STACK_SIZE];

void task_lua (void *p) {
    while (1) {
        printf("%s\r\n", "Hello world!");
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

    if (xTaskCreateStatic(task_lua, "lua", TASK_LUA_STACK_SIZE, NULL, 3, task_lua_stack, &task_lua_buf) == NULL) {
        return ENOMEM;
    }

    vTaskStartScheduler();
}
