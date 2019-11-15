#include "aym_hardware.h"

#include "freertos_headers.h"

#define HANDLER_FULL_BYTE_SIZE 16
#define HANDLER_SHORT_BYTE_SIZE 3
#define HANDLER_MARKER_POINT_BYTE 3

#define PSG_FILE_MARKER_FORMAT 0x1A
#define PSG_FILE_MARKER_INTERRUPT 0xFF
#define PSG_FILE_MARKER_FREE_INTERRUPT 0xFE
#define PSG_FILE_MARKER_END_TRACK 0xFD

#define MAX_REG_NUMBER 0xF

#include <stdint.h>
#include <string.h>
#include <errno.h>

static uint8_t flag_handler_got = 0;
static uint8_t reg_got = 0;

void aym_psg_reset () {
    flag_handler_got = 0;
    reg_got = 0;
}

int aym_psg_play (uint8_t chip_num, const uint8_t *data, uint32_t len) {
    aym_reg_data_t msg;
    msg.chip_num = chip_num;

    uint32_t pos = 0;

    if (flag_handler_got == 0) {
        if (len < HANDLER_FULL_BYTE_SIZE) {
            return ENOEXEC;
        }

        if (data[HANDLER_MARKER_POINT_BYTE] == PSG_FILE_MARKER_FORMAT) {
            len -= HANDLER_FULL_BYTE_SIZE;
            pos = HANDLER_FULL_BYTE_SIZE;
        } else {
            len -= HANDLER_SHORT_BYTE_SIZE;
            pos = HANDLER_SHORT_BYTE_SIZE;
        }
    }



    while (pos < len) {
        if (reg_got == 1) {
            reg_got = 0;
            if (msg.reg <= MAX_REG_NUMBER) {
                msg.data = data[pos];
                while (add_aym_element(&msg) != 0) {
                    vTaskDelay(20);
                }
            };

            pos++;
            continue;
        }

        if (data[pos] == PSG_FILE_MARKER_INTERRUPT) {
            msg.reg = REG_PAUSE;
            while (add_aym_element(&msg) != 0) {
                vTaskDelay(20);
            }
            pos++;
            continue;
        };

        if (data[pos] == PSG_FILE_MARKER_FREE_INTERRUPT) {
            msg.reg = REG_PAUSE;
            pos++;
            for (uint8_t i = 0; i < data[pos]; i++) {
                while (add_aym_element(&msg) != 0) {
                    vTaskDelay(20);
                }
            }
            pos++;

            continue;
        }

        if (data[pos] == PSG_FILE_MARKER_END_TRACK) {
            break;
        }

        msg.reg = data[pos];
        reg_got = 1;
        pos++;
    }

}