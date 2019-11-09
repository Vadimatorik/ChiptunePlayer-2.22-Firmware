#pragma once

#ifdef __cplusplus

#include "uart_base.h"

#ifdef HAL_UART_MODULE_ENABLED

namespace mc {

class uart_terminal : public uart_base {
public:
    uart_terminal (const uart_cfg *const cfg, uint32_t cfg_num = 1, uint32_t thread_prio = 2);

public:
    void uart_irq_handler (void);

private:
    static void thread (void *obj);

private:
    void char_parser (char c, uint32_t &i);

private:
    static const uint32_t TB_THREAD_SIZE = 200;
    StackType_t tb_thread[TB_THREAD_SIZE] = {0};
    StaticTask_t ts_thread;

private:
    QueueHandle_t q_answer = nullptr;
    static const uint32_t Q_LEN = 10;
    static const uint32_t Q_ITEM_SIZE = sizeof(char);
    uint8_t qb[Q_LEN * Q_ITEM_SIZE] = {0};
    StaticQueue_t qs;

private:
    const uint8_t BACK_SPACE_8 = 8;
    const uint8_t BACK_SPACE_127 = 127;
    int input_pos = 0;

private:
    // 255 на строку + 0-терминатор
    // + немного на \n\r.
    uint8_t buf_repack_answer[300] = {0};

};

}

#endif

#endif
