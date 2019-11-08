#include "mc_hardware.h"
#include "sr.h"

#include <stdint.h>
#include <errno.h>

#define AD5204_CHIP_COUNT 2
#define AD5204_BUF_SIZE(COUNT) ((COUNT * 11) /8) + ((((COUNT * 11) % 8) != 0 )?1:0) + 1

static uint8_t tx_buf[AD5204_BUF_SIZE(AD5204_CHIP_COUNT)] = {0};
static uint8_t reg_data[AD5204_CHIP_COUNT][4] = {0};

void set_shdn () {
    sr_set_pin_ad5204_shdn();
}

void reset_shdn () {
    sr_reset_pin_ad5204_shdn();
}

int set_value (uint8_t chip_num, uint8_t reg, uint8_t value) {
    if ((chip_num >= AD5204_CHIP_COUNT) | (reg >= 4)) {
        return EINVAL;
    }

    reg_data[chip_num][reg] = value;

    int sm_byte = AD5204_BUF_SIZE(AD5204_CHIP_COUNT) - 1;
    int sm_bit = 0;

    for (uint32_t ch_i = 0; ch_i < AD5204_CHIP_COUNT; ch_i++) {
        tx_buf[sm_byte] |= reg_data[ch_i][reg] << sm_bit;
        tx_buf[sm_byte - 1] |= reg_data[ch_i][reg] >> (8 - sm_bit);

        sm_byte--;

        tx_buf[sm_byte] |= reg << sm_bit;
        tx_buf[sm_byte - 1] |= reg >> (8 - sm_bit);

        sm_bit += 3;
        if (sm_bit > 7) {
            sm_bit -= 8;
            sm_byte--;
        }
    }

    return spi_board_device_ad5204_tx(&tx_buf[1], AD5204_BUF_SIZE(AD5204_CHIP_COUNT) - 1);
}

int read_value (uint8_t chip_num, uint8_t reg, uint8_t *data) {
    if ((chip_num >= AD5204_CHIP_COUNT) | (reg >= 4)) {
        return EINVAL;
    }

    *data = reg_data[chip_num][reg];
    return 0;
}