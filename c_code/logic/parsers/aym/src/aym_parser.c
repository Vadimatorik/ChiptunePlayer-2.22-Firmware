#include "aym_parser.h"

#include "freertos_headers.h"
#include "freertos_obj.h"

#include "psg_parser.h"

#include "dp.h"
#include "mc_hardware.h"
#include "sr.h"
#include "ltc6903.h"

#include <errno.h>
#include <string.h>
#include <stdlib.h>

enum AYM_FORMAT {
    AYM_FORMAT_NO = 0,
    AYM_FORMAT_PSG = 1
};

static uint32_t get_format (const char *file_path) {
    return AYM_FORMAT_PSG; // TODO: добавить поддержку других форматов.
}

int aym_get_len_tick (const char *file_path, uint32_t *ret_len_tick) {
    uint32_t format = get_format(file_path);
    if (format == AYM_FORMAT_NO) {
        return EINVAL;
    }

    switch (format) {
        case AYM_FORMAT_PSG:
            return aym_psg_get_len_tick(file_path, ret_len_tick);

        default:
            return EINVAL;
    }
}

int aym_get_len_sec (const char *file_path, uint32_t *ret_len_sec) {
    uint32_t ret_len_tick = 0;

    int rv = aym_get_len_tick(file_path, &ret_len_tick);

    if (rv == 0) {
        *ret_len_sec = ret_len_tick / 50;
    }

    return rv;
}

enum AYM_CMD {
    AYM_CMD_PLAY = 0,
    AYM_CMD_PAUSE = 1,
    AYM_CMD_STOP = 2,
};

typedef struct _aym_cmd {
    uint8_t cmd;
    union {
        char* path_to_file;
    } arg;
} aym_cmd;

static QueueHandle_t aym_cmd_queue = {0};
static StaticQueue_t aym_cmd_str = {0};
static uint8_t aym_cmd_queue_buf[AYM_CMD_QUEUE_LEN*sizeof(aym_cmd)] = {0};

int aym_play (const char *file_path) {
    char *f_path = malloc(strlen(file_path) + 1);
    if (f_path == NULL) {
        return ENOMEM;
    }

    strcpy(f_path, file_path);

    aym_cmd q_msg = {0};
    q_msg.cmd = AYM_CMD_PLAY;
    q_msg.arg.path_to_file = f_path;

    xQueueSend(aym_cmd_queue, &q_msg, portMAX_DELAY);

    return 0;
}

int aym_pause () {
    aym_cmd q_msg = {0};
    q_msg.cmd = AYM_CMD_PAUSE;
    xQueueSend(aym_cmd_queue, &q_msg, portMAX_DELAY);
    return 0;
}

int aym_stop () {
    aym_cmd q_msg = {0};
    q_msg.cmd = AYM_CMD_STOP;
    xQueueSend(aym_cmd_queue, &q_msg, portMAX_DELAY);
    return 0;
}

static int add_element_to_low_aym_queue (aym_reg_data_t *msg) {
    int rv = 0;
    aym_cmd q_msg = {0};

    while (1) {
        if (xQueueReceive(aym_cmd_queue, &q_msg, 0) == pdTRUE) { // Приказ во время воспроизведения трека.
            if (q_msg.cmd == AYM_CMD_PAUSE) { // После паузы 2 пути. Выход или продолжить.
                set_pause();
                xQueueReceive(aym_cmd_queue, &q_msg, portMAX_DELAY);

                switch (q_msg.cmd) {
                    case AYM_CMD_PAUSE:
                        reset_pause();
                        break;

                    case AYM_CMD_STOP:
                        queue_clear();
                        clear_aym_hardware();
                        return AYM_CMD_STOP;

                    default:
                        queue_clear();
                        clear_aym_hardware();
                        return AYM_CMD_STOP;
                }
            } else if (q_msg.cmd == AYM_CMD_STOP) {
                queue_clear();
                clear_aym_hardware();
                return AYM_CMD_STOP;
            }
        }

        rv = add_aym_element(msg);
        if (rv == 0) {
            return 0;
        } else {
            vTaskDelay(10);
        }
    }
}

static void aym_thread (__attribute__((unused)) void *obj) {
    aym_cmd q_msg = {0};
    int rv = 0;

    while (1) {
        xQueueReceive(aym_cmd_queue, &q_msg, portMAX_DELAY);

        switch (q_msg.cmd) {
            case AYM_CMD_PLAY:
                sr_set_pin_ay_1_res();
                sr_reset_pin_pwr_ay_1_on();
                set_pin_pwr_5_v();
                sr_set_pin_pwr_ay_1_on();
                sr_reset_pin_ay_1_res();
                clear_aym_hardware();
                dp_reset_shdn();

                ltc6903_start();
                ltc6903_set_requency(1.77e6, LTC6903_OUTPUT_MODE_CLK_ON_INV_OFF);

                /*
                dp_set_a1 (256/2);
                dp_set_a2 (256/2);
                dp_set_b1 (256/2);
                dp_set_b2 (256/2);
                dp_set_c1 (256/2);
                dp_set_c2 (256/2);
                dp_set_l (256/2);
                dp_set_r (256/2);*/

                rv = aym_psg_play(q_msg.arg.path_to_file, add_element_to_low_aym_queue);
                if (rv != 0) {
                    // TODO: потом пробросить отправку наверх сообщения об ошибке при работе.
                }
                free(q_msg.arg.path_to_file);
                break;

            default:
                break;
        }
    }
}

static StackType_t aym_thread_stack[AYM_THREAD_STACK_SIZE] = {0};
static StaticTask_t aym_thread_struct = {0};

int init_aym () {
    aym_cmd_queue = xQueueCreateStatic(AYM_CMD_QUEUE_LEN, sizeof(aym_cmd), aym_cmd_queue_buf, &aym_cmd_str);

    xTaskCreateStatic(aym_thread, "aym",
                      AYM_THREAD_STACK_SIZE, NULL, AYM_THREAD_PRIO,
                      aym_thread_stack, &aym_thread_struct);

    return 0;
}

