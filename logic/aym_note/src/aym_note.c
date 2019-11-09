#include <stdint.h>
#include "aym_hardware.h"

static const uint16_t array_divider_chip[108] = {
    0xfbe, 0xedc, 0xe06, 0xd3d, 0xc7f, 0xbcb, 0xb22, 0xa82,
    0x9eb, 0x95c, 0x8d6, 0x857, 0x7df, 0x76e, 0x703, 0x69e,
    0x63f, 0x5e5, 0x591, 0x541, 0x4f5, 0x4ae, 0x46b, 0x42b,
    0x3ef, 0x3b7, 0x381, 0x34f, 0x31f, 0x2f2, 0x2c8, 0x2a0,
    0x27a, 0x257, 0x235, 0x215, 0x1f7, 0x1db, 0x1c0, 0x1a7,
    0x18f, 0x179, 0x164, 0x150, 0x13d, 0x12b, 0x11a, 0x10a,
    0xfb, 0xed, 0xe0, 0xd3, 0xc7, 0xbc, 0xb2, 0xa8,
    0x9e, 0x95, 0x8d, 0x85, 0x7d, 0x76, 0x70, 0x69,
    0x63, 0x5e, 0x59, 0x54, 0x4f, 0x4a, 0x46, 0x42,
    0x3e, 0x3b, 0x38, 0x34, 0x31, 0x2f, 0x2c, 0x2a,
    0x27, 0x25, 0x23, 0x21, 0x1f, 0x1d, 0x1c, 0x1a,
    0x18, 0x17, 0x16, 0x15, 0x13, 0x12, 0x11, 0x10,
    0xf, 0xe, 0xe, 0xd, 0xc, 0xb, 0xb, 0xa,
    0x9, 0x9, 0x8, 0x8
};
/*
int aym_note_set_channel (uint8_t chip_num, uint8_t ch, uint8_t state) {
    aym_reg_data_t msg;
    msg.chip_num = chip_num;
    msg.reg = 0x07;

    if (state) {                                                                // Если включить канал.
        this->cfg->r7_reg[number_ay] &= ~(1 << channel);
    } else {
        this->cfg->r7_reg[number_ay] |= 1 << channel;                                // Если отключить.
    }
    ay_queue_struct buf;
    buf.data = this->cfg->r7_reg[number_ay];
    buf.reg = 7;
    buf.number_chip = number_ay;
    this->queue_add_element(&buf);    // Выбираем R7.
}*/

int aym_note_reset (uint8_t chip_num) {
    aym_reg_data_t msg;
    msg.chip_num = chip_num;
    msg.reg = 0x07;
    msg.data = 0b111000;
    return add_aym_element(&msg);
}

int aym_note_write_note_to_channel (uint8_t chip_num, uint8_t ch, uint8_t note) {
    uint8_t older_byte = (uint8_t)(array_divider_chip[note] >> 8);
    uint8_t junior_byte = (uint8_t)array_divider_chip[note];

    aym_reg_data_t msg;
    msg.chip_num = chip_num;

    int rv = 0;

    switch (ch) {      // Записываем ноту в выбранный канал.
        case 0:
            msg.reg = 0;
            msg.data = junior_byte;
            if ((rv = add_aym_element(&msg)) != 0) {
                return rv;
            }

            msg.reg = 1;
            msg.data = older_byte;
            return add_aym_element(&msg);

        case 1:
            msg.reg = 2;
            msg.data = junior_byte;
            if ((rv = add_aym_element(&msg)) != 0) {
                return rv;
            }

            msg.reg = 3;
            msg.data = older_byte;
            return add_aym_element(&msg);

        case 2:
            msg.reg = 4;
            msg.data = junior_byte;
            if ((rv = add_aym_element(&msg)) != 0) {
                return rv;
            }

            msg.reg = 5;
            msg.data = older_byte;
            return add_aym_element(&msg);
    }

    return 0;
}

int aym_note_set_volume_channel (uint8_t chip_num, uint8_t ch, uint8_t volume) {
    aym_reg_data_t msg;
    msg.chip_num = chip_num;
    msg.data = volume;

    switch (ch) {
        case 0:
            msg.reg = 8;
            break;

        case 1:
            msg.reg = 9;
            break;

        case 2:
            msg.reg = 10;
            break;
    }

    return add_aym_element(&msg);
}