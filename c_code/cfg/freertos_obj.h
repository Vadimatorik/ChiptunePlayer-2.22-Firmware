#pragma once

#define MATRIX_KEYBOARD_THREAD_PRIO 2
#define LUA_INTERACTIVE_THREAD_PRIO 2
#define LUA_MAIN_THREAD_PRIO 3

#define MATRIX_KEYBOARD_THREAD_STACK_SIZE 400
#define LUA_INTERACTIVE_THREAD_STACK_SIZE 1000
#define LUA_MAIN_THREAD_STACK_SIZE 1000

#define MATRIX_KEYBOARD_PERIOD_MS 10

#define LUA_OS_CMD_QUEUE_SIZE 5