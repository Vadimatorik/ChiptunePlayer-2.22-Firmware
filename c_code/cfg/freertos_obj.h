#pragma once

#define MATRIX_KEYBOARD_THREAD_PRIO         2
#define LUA_INTERACTIVE_THREAD_PRIO         2
#define LUA_MAIN_THREAD_PRIO                3
#define UART_THREAD_PRIO                    2
#define HARD_THREAD_PRIO            4

#define HARD_THREAD_STACK_SIZE      400
#define MATRIX_KEYBOARD_THREAD_STACK_SIZE   400
#define LUA_MAIN_THREAD_STACK_SIZE          5000
#define UART_THREAD_STACK_SIZE              200

#define MATRIX_KEYBOARD_PERIOD_MS           10

#define LUA_OS_CMD_QUEUE_LEN                5
#define UART_TX_QUEUE_LEN                   10
#define YM_REG_DATA_QUEUE_LEN               32




