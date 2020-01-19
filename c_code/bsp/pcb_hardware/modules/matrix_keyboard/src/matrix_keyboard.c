#include "matrix_keyboard.h"
#include "freertos_obj.h"
#include "mc_hardware.h"

#if defined(SOFT)
#include "socket_emul_layer.h"
#endif

// Флаги состояния  одной клавиши.
typedef struct matrix_keyboard_status {
    // Флаг состояния нажатия до текущей проверки (окончание предыдущей).
    uint8_t press;

    // Флаг состояния проверки на дребезг:
    uint8_t bounce;

    // Флаг события "произошло длительное нажатие":
    uint8_t event_long_click;

    // Оставшееся время до окончания проверки на дребезг (в мс).
    uint32_t bounce_time;

    /*
     * Сколько прошло времени с момента нажатия клавиши
     * (идет начиная с проверки дребезга).
     */
    uint32_t button_long_click_time;
} matrix_keyboard_key_status;


static StackType_t matrix_keyboard_thread_stack[MATRIX_KEYBOARD_THREAD_STACK_SIZE] = {0};


static StaticTask_t matrix_keyboard_thread_struct = {0};

#define B_NUM 7

static const matrix_keyboard_one_button_cfg b_cfg[B_NUM] = {
    {B_UP,     50, 1000},
    {B_DOWN,   50, 1000},
    {B_LEFT,   50, 1000},
    {B_RIGHT,  50, 1000},
    {B_ENTER,  50, 1000},
    {B_RETURN, 50, 1000},
    {B_MENU,   50, 1000}
};


matrix_keyboard_key_status b_status[B_NUM] = {0};

#if defined(HARD)

static uint8_t get_b_state (uint8_t id) {
    set_pin_br_0();
    set_pin_br_1();
    set_pin_br_2();

    switch (id) {
        case 0:
            reset_pin_br_0();
            return !read_pin_bc_1();
        case 1:
            reset_pin_br_2();
            return !read_pin_bc_1();
        case 2:
            reset_pin_br_1();
            return !read_pin_bc_0();
        case 3:
            reset_pin_br_1();
            return !read_pin_bc_2();
        case 4:
            reset_pin_br_1();
            return !read_pin_bc_1();
        case 5:
            reset_pin_br_0();
            return !read_pin_bc_2();
        case 6:
            reset_pin_br_0();
            return !read_pin_bc_0();
    }

    return 0;
}

#elif defined(SOFT)

static uint8_t get_b_state (uint8_t id) {
    return socket_get_button_state(id);
}

#endif

static void process_press (uint32_t b_number) {
    matrix_keyboard_key_status *s = &b_status[b_number];
    const matrix_keyboard_one_button_cfg *p_st = &b_cfg[b_number];

    //. Если до этого момента кнопка была сброшена.
    if (!s->press) {
        s->press = -1; // Показываем, что нажатие произошло.
        s->bounce = -1; // Начинаем проверку на дребезг.
        s->bounce_time = p_st->stabilization_time_ms; // Устанавливаем время для проверки дребезга контактов.
    } else { // Если мы уже некоторое время держим эту кнопку.
        if (s->bounce) { // Если идет проверка на дребезг.
            if (s->bounce_time - MATRIX_KEYBOARD_PERIOD_MS > 0) { /*
                                                               * Если время проверки еще не закончилось
                                                               * (мы от оставшегося времени отнимаем время,
                                                               * прошедшее с предыдущей проверки).
                                                               */
                s->bounce_time -= MATRIX_KEYBOARD_PERIOD_MS;
            } else { // Если время проверки закончилось и, при этом, все это время была зажата, то успех.
                s->bounce = 0; // Проверка больше не проводится.
                matrix_keyboard_event_press(p_st->id);
            }
        } else { // Если проверка на дребезг уже прошла, при этом клавишу еще держат.

            if (p_st->long_press_detection_time == 0) { // Если мы не отслеживаем длительное нажатие - выходим.
                return;
            }

            if (s->event_long_click) {
                return; // Если мы уже отдали флаг о том, что нажатие длительное - выходим.
            }

            s->button_long_click_time += MATRIX_KEYBOARD_PERIOD_MS; // Прибавляем время, прошедшее с последней проверки.

            /*
             * Если прошло времени меньше, чем нужно для того, чтобы кнопка
             * считалась долго зажатой - выходим.
             */
            if (s->button_long_click_time < p_st->long_press_detection_time) {
                return;
            }

            s->event_long_click = -1; // Дождались, кнопка долго зажата.
            matrix_keyboard_event_long_press(p_st->id);
        }
    }
}

static void process_not_press (uint32_t b_number) {
    matrix_keyboard_key_status *s = &b_status[b_number];
    const matrix_keyboard_one_button_cfg *p_st = &b_cfg[b_number];

    if (s->press == 0) {
        return; // Если она и до этого была отпущена - выходим.
    }

    s->press = 0;

    if (s->bounce) { // Если кнопка проходила проверку на дребезг, но не прошла.
        s->bounce = 0;
        return;
    }

    s->bounce_time = 0;
    s->button_long_click_time = 0;

    // Если проверку состояния мы все-таки прошли.
    if (s->event_long_click) { // Если произошло длительное нажатие.
        s->event_long_click = 0;

        matrix_keyboard_event_long_click(p_st->id);
    }

    matrix_keyboard_event_click(p_st->id);
}

static void matrix_keyboard_thread (__attribute__((unused)) void *obj) {
    TickType_t last_wake_time = xTaskGetTickCount();

    while (1) {
        for (uint32_t i = 0; i < sizeof(b_cfg)/sizeof(b_cfg[0]); i++) {
            uint8_t b_state = get_b_state(b_cfg[i].id);

            if (b_state) {
                process_press(i);
            } else {
                process_not_press(i);
            }
        }

        vTaskDelayUntil(&last_wake_time, MATRIX_KEYBOARD_PERIOD_MS);
    }
}

int init_matrix_keyboard () {
    xTaskCreateStatic(matrix_keyboard_thread, "matrix_keyboard",
                      MATRIX_KEYBOARD_THREAD_STACK_SIZE, NULL, MATRIX_KEYBOARD_THREAD_PRIO,
                      matrix_keyboard_thread_stack, &matrix_keyboard_thread_struct);
    return 0;
}
