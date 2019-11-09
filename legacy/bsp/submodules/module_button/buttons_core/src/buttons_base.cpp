#include "buttons_base.h"
#include <string.h>

#ifdef MODULE_BUTTONS_BASE

namespace button {

base::base (const base_cfg *const cfg) :
    cfg(cfg) {
    xTaskCreateStatic(this->task,
                               "ButtonBase",
                               TASK_STACK_SIZE,
                               this,
                               this->cfg->task_prio,
                               this->task_stack,
                               &this->task_struct);
    
    this->s = new status[this->cfg->cfg_array_size];
    memset(this->s, 0, sizeof(button::status) * this->cfg->cfg_array_size);
}

void base::send_message_event_press (one_button_cfg *p_st) {
    if (p_st->press.s != nullptr) {
        xSemaphoreGive(*p_st->press.s);
    }
    
    if (p_st->press.q != nullptr) {
        xQueueSend(*p_st->press.q, &p_st->press.v, 0);
    }
}

void base::send_message_event_long_press (one_button_cfg *p_st) {
    if (p_st->long_press.s != nullptr) {
        xSemaphoreGive(*p_st->long_press.s);
    }
    
    if (p_st->long_press.q != nullptr) {
        xQueueSend(*p_st->long_press.q, &p_st->long_press.v, 0);
    }
}

void base::send_message_event_long_click (one_button_cfg *p_st) {
    if (p_st->long_click.s != nullptr) {
        xSemaphoreGive(*p_st->long_click.s);
    }
    
    if (p_st->long_click.q != nullptr) {
        xQueueSend(*p_st->long_click.q, &p_st->long_click.v, 0);
    }
}

void base::send_message_event_click (one_button_cfg *p_st) {
    if (p_st->click.s != nullptr) {
        xSemaphoreGive(*p_st->click.s);
    }
    
    if (p_st->click.q != nullptr) {
        xQueueSend(*p_st->click.q, &p_st->click.v, 0);
    }
}

void base::process_press (uint32_t b_number) {
    // Сократим запись заранее полученным указателем.
    status *s = &this->s[b_number];
    one_button_cfg *p_st = &this->cfg->cfg_array[b_number];
    
    //. Если до этого момента кнопка была сброшена.
    if (s->press == false) {
        s->press = true; // Показываем, что нажатие произошло.
        s->bounce = true; // Начинаем проверку на дребезг.
        s->bounce_time = p_st->stabilization_time_ms; // Устанавливаем время для проверки дребезга контактов.
    } else { // Если мы уже некоторое время держим эту кнопку.
        if (s->bounce == true) { // Если идет проверка на дребезг.
            if (s->bounce_time - this->cfg->task_delay_ms > 0) { /*
                                                               * Если время проверки еще не закончилось
                                                               * (мы от оставшегося времени отнимаем время,
                                                               * прошедшее с предыдущей проверки).
                                                               */
                s->bounce_time -= this->cfg->task_delay_ms;
            } else { // Если время проверки закончилось и, при этом, все это время была зажата, то успех.
                s->bounce = false; // Проверка больше не проводится.
                this->send_message_event_press(p_st);
            }
        } else { /*
                  * Если проверка на дребезг уже прошла, при этом клавишу еще держат.
                  */
            if (p_st->long_press_detection_time_sec == 0) { // Если мы не отслеживаем длительное нажатие - выходим.
                return;
            }
            
            if (s->event_long_click == true) // Если мы уже отдали флаг о том, что нажатие длительное - выходим.
                return;
            
            s->button_long_click_time += this->cfg->task_delay_ms; // Прибавляем время, прошедшее с последней проверки.
            
            /*
             * Если прошло времени меньше, чем нужно для того, чтобы кнопка
             * считалась долго зажатой - выходим.
             */
            if (s->button_long_click_time < p_st->long_press_detection_time_sec * 1000) {
                return;
            }
            
            s->event_long_click = true; // Дождались, кнопка долго зажата.
            this->send_message_event_long_press(p_st);
        }
    }
}

void base::process_not_press (uint32_t b_number) {
    status *s = &this->s[b_number];
    one_button_cfg *p_st = &this->cfg->cfg_array[b_number];
    
    if (s->press == false) {
        return; // Если она и до этого была отпущена - выходим.
    }
    
    s->press = false;
    
    if (s->bounce == true) { // Если кнопка проходила проверку на дребезг, но не прошла.
        s->bounce = false;
        return;
    }
    
    s->bounce_time = 0;
    s->button_long_click_time = 0;
    
    // Если проверку состояния мы все-таки прошли.
    if (s->event_long_click == true) { // Если произошло длительное нажатие.
        s->event_long_click = false;
        
        this->send_message_event_long_click(p_st);
    }
    
    this->send_message_event_click(p_st);
}

void base::task (void *obj) {
    button::base *o = reinterpret_cast<button::base *>(obj);
    
    while (true) {
        for (uint32_t i = 0; i < o->cfg->cfg_array_size; i++) {
            bool buttonState;
            buttonState = o->cfg->get_button_state(o->cfg->cfg_array[i].id);
            
            if (buttonState == true) {
                o->process_press(i);
            } else {
                o->process_not_press(i);
            }
        }
        
        vTaskDelay(o->cfg->task_delay_ms);
    }
}
    
}

#endif