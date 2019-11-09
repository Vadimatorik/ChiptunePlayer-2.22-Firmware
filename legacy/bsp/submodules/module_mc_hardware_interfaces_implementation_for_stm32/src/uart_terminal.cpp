#include "uart_terminal.h"

#ifdef HAL_UART_MODULE_ENABLED

namespace mc {

uart_terminal::uart_terminal (const uart_cfg *const cfg, uint32_t cfg_num, uint32_t thread_prio) :
    uart_base(cfg, cfg_num) {
    xTaskCreateStatic(uart_terminal::thread,
                               "uart_terminal",
                               TB_THREAD_SIZE,
                               this,
                               thread_prio,
                               this->tb_thread,
                               &this->ts_thread);

    this->q_answer = xQueueCreateStatic(Q_LEN, Q_ITEM_SIZE, this->qb, &this->qs);
}

void uart_terminal::char_parser (char c, uint32_t &i) {
    if (c != '\r') {
        if ((c == this->BACK_SPACE_8) || (c == this->BACK_SPACE_127)) {
            if (this->input_pos > 0) {
                this->input_pos--;
                this->buf_repack_answer[i++] = c;
                this->cfg->byte_handler(c);
                return;
            }
        } else {
            this->buf_repack_answer[i++] = c;
            this->input_pos++;
            this->cfg->byte_handler(c);
        }
    } else {
        this->buf_repack_answer[i++] = '\n';
        this->buf_repack_answer[i++] = '\r';
        this->tx(this->buf_repack_answer, i, 100);
        i = 0;
        this->input_pos = 0;
        this->cfg->byte_handler(c);
    }
}

void uart_terminal::thread (void *obj) {
    auto *o = reinterpret_cast<uart_terminal*>(obj);


    while(true) {
        uint32_t i = 0;
        uint8_t  b_char = 0;
        xQueueReceive(o->q_answer, &b_char, portMAX_DELAY);
        o->char_parser(b_char, i);

        for (;i < sizeof(buf_repack_answer); i++) {
            if (xQueueReceive(o->q_answer, &b_char, 10) == pdFALSE) {
                break;
            }

            o->char_parser(b_char, i);
        }

        o->tx(o->buf_repack_answer, i, 100);
    }
}

void uart_terminal::uart_irq_handler (void) {
    static BaseType_t hp = pdFALSE;
    volatile uint8_t sr = this->u.Instance->SR;
    uint8_t data = this->u.Instance->DR;

    if (this->cfg->byte_handler) {
        if ((sr &UART_FLAG_RXNE) == UART_FLAG_RXNE) {
            xQueueSendFromISR(this->q_answer, &data, &hp);
            if (hp == pdTRUE) {
                taskYIELD();
            }
        }
    }

    HAL_UART_IRQHandler(&this->u);
}

}

#endif
