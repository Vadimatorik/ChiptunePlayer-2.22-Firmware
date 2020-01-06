#include "aym_hardware.h"
#include "freertos_headers.h"
#include "sr.h"
#include "freertos_obj.h"

#include <errno.h>

#define AY_NUM 2

#define AY_1_PORT_INDEX 2
#define AY_2_PORT_INDEX 1

typedef enum {
    QUEUE_STATE_EMPTY = 0,
    QUEUE_STATE_NOT_EMPTY = 0xFF
} QUEUE_STATE;

static const uint8_t aym_port_index_array[AY_NUM] = {AY_1_PORT_INDEX, AY_2_PORT_INDEX};

__attribute__ ((section (".bss_ccm")))
static StackType_t tb[AYM_HARDWARE_THREAD_STACK_SIZE] = {0};

__attribute__ ((section (".bss_ccm")))
static StaticTask_t ts = {0};

#define AYM_REG_R7_DEFAULT_VALUE 0b111111

typedef struct __attribute__((packed)) _chip_reg_data {
    uint8_t reg[16];
} chip_reg_data;

__attribute__ ((section (".bss_ccm")))
chip_reg_data aym_data[AY_NUM] = {0};

__attribute__ ((section (".bss_ccm")))
static SemaphoreHandle_t tim_irq_request = NULL;

__attribute__ ((section (".bss_ccm")))
static StaticSemaphore_t tim_irq_request_buf = {0};

__attribute__ ((section (".bss_ccm")))
TimerHandle_t irq_timer = NULL;

typedef struct _aym_out_data {
    uint8_t reg;
    uint8_t data;
} aym_out_data;


__attribute__ ((section (".bss_ccm")))
static QueueHandle_t aym_reg_data_queue[AY_NUM] = {0};

__attribute__ ((section (".bss_ccm")))
static StaticQueue_t aym_reg_data_str[AY_NUM] = {0};

__attribute__ ((section (".bss_ccm")))
static uint8_t aym_reg_data_queue_buf[AY_NUM][YM_REG_DATA_QUEUE_LEN*sizeof(aym_reg_data_t)] = {0};

__attribute__ ((section (".bss_ccm")))
static uint32_t tic_ff = 0;

static int set_reg () {
    int rv = 0;
    if ((rv = sr_set_pin_bc1()) != 0) {
        return rv;
    }

    if ((rv = sr_set_pin_bdir()) != 0) {
        return rv;
    }

    if ((rv = sr_reset_pin_bdir()) != 0) {
        return rv;
    }

    if ((rv = sr_reset_pin_bc1()) != 0) {
        return rv;
    }

    return 0;
}

static int set_data () {
    int rv = 0;

    if ((rv = sr_set_pin_bdir()) != 0) {
        return rv;
    }

    if ((rv = sr_reset_pin_bdir()) != 0) {
        return rv;
    }

    return 0;
}

void queue_clear () {
    for (int chip_index = 0; chip_index < AY_NUM; chip_index++) {
        xQueueReset(aym_reg_data_queue[chip_index]);
    }
}

static QUEUE_STATE check_queue_empty () {
    for (int chip_index = 0; chip_index < AY_NUM; chip_index++) {
        if (uxQueueMessagesWaiting(aym_reg_data_queue[chip_index]) != 0) {
            return QUEUE_STATE_NOT_EMPTY;
        }
    }

    return QUEUE_STATE_EMPTY;
}

static void clean_buffer_ay_reg () {
    for (uint32_t chip_index = 0; chip_index < AY_NUM; chip_index++) {
        for (uint32_t reg_num = 0; reg_num < 7; reg_num++) {
            aym_data[chip_index].reg[reg_num] = 0;
        }

        aym_data[chip_index].reg[7] = AYM_REG_R7_DEFAULT_VALUE;

        for (uint32_t reg_num = 8; reg_num < 16; reg_num++) {
            aym_data[chip_index].reg[reg_num] = 0;
        }
    }
}

static int update_all_reg () {
    int rv = 0;

    for (uint32_t reg_index = 0; reg_index < 16; reg_index++) {
        for (uint32_t ay_index = 0; ay_index < AY_NUM; ay_index++) {
            if ((rv = sr_port_write(aym_port_index_array[ay_index], reg_index)) != 0) {
                return rv;
            }
        }

        if ((rv = set_reg()) != 0) {
            return rv;
        }

        for (uint32_t ay_index = 0; ay_index < AY_NUM; ay_index++) {
            if ((rv = sr_port_write(aym_port_index_array[ay_index], aym_data[ay_index].reg[reg_index])) != 0) {
                return rv;
            }
        }

        if ((rv = set_data()) != 0) {
            return rv;
        }
    }
}

int clear_aym_hardware () {
    clean_buffer_ay_reg();
    return update_all_reg();
}

int flags_is_set (uint8_t *flags) {
    for (int i = 0; i < AY_NUM; i++) {
        if (flags[i] == 0) {
            return -1;
        }
    }

    return 0;
}

static void task_aym (void *p) {
    p = p;

    aym_out_data q_data_buf[AY_NUM] = {0};
    xTimerStart(irq_timer, 0);

    while (1) {
        xSemaphoreTake (tim_irq_request, portMAX_DELAY);
        if (check_queue_empty() == QUEUE_STATE_EMPTY) {
            continue;
        }

        uint8_t q_not_use[AY_NUM] = {0};

        while (flags_is_set(q_not_use) != 0) {
            for (int chip_index = 0; chip_index < AY_NUM; chip_index++) {
                uint32_t q_item_num = uxQueueMessagesWaiting(aym_reg_data_queue[chip_index]);

                if (q_item_num != 0) {
                    xQueueReceive(aym_reg_data_queue[chip_index], &q_data_buf[chip_index], 0);
                    if (q_data_buf[chip_index].reg == REG_PAUSE) {
                        q_not_use[chip_index] = 0xFF;
                        q_data_buf[chip_index].reg = 18;
                    } else {
                        aym_data[chip_index].reg[q_data_buf[chip_index].reg] = q_data_buf[chip_index].data;
                    }
                } else {
                    q_not_use[chip_index] = 0xFF;
                    q_data_buf[chip_index].reg = 18;
                }
            }

            for (int chip_index = 0; chip_index < AY_NUM; chip_index++) {
                sr_write_byte(aym_port_index_array[chip_index], q_data_buf[chip_index].reg);
            }

            sr_update();

            set_reg();

            for (int chip_index = 0; chip_index < AY_NUM; chip_index++) {
                sr_write_byte(aym_port_index_array[chip_index], q_data_buf[chip_index].data);
            }

            sr_update();

            set_data();
        }

        tic_ff++;
        if (tic_ff != 50) {
            continue;
        }

        tic_ff = 0;
    }
}


static void tim_irq_handler (TimerHandle_t tim) {
    xSemaphoreGive(tim_irq_request);
}

int init_aym_hardware () {
    tim_irq_request = xSemaphoreCreateBinaryStatic(&tim_irq_request_buf);

    irq_timer = xTimerCreate("aym_timer", 20, pdTRUE, NULL, tim_irq_handler);

    xTaskCreateStatic(task_aym, "aym_hardware", AYM_HARDWARE_THREAD_STACK_SIZE, NULL, AYM_HARDWARE_THREAD_PRIO,
                      tb, &ts);
    for (int i = 0; i < AY_NUM; i++) {
        aym_reg_data_queue[i] = xQueueCreateStatic(YM_REG_DATA_QUEUE_LEN, sizeof(aym_reg_data_t),
                                                   &aym_reg_data_queue_buf[i][0], &aym_reg_data_str[i]);
    }


    return 0;
}

int add_aym_element (aym_reg_data_t *item) {
    aym_out_data buf;
    buf.reg = item->reg;
    buf.data = item->data;

    if (xQueueSend(aym_reg_data_queue[item->chip_num], &buf, 0) == pdTRUE) {
        return 0;
    } else {
        return ENOMEM;
    }
}


