#include "mc_hardware.h"
#include "sr.h"

#include <stdint.h>
#include <errno.h>

#define AD5204_CHIP_COUNT 2
#define AD5204_BUF_SIZE(COUNT) ((COUNT * 11) /8) + ((((COUNT * 11) % 8) != 0 )?1:0) + 1

static uint8_t tx_buf[AD5204_BUF_SIZE(AD5204_CHIP_COUNT)] = {0};
static uint8_t reg_data[AD5204_CHIP_COUNT][4] = {0};

int dp_set_shdn () {
    return sr_reset_pin_ad5204_shdn();
}

int dp_reset_shdn () {
    return sr_set_pin_ad5204_shdn();
}

int init_dp () {
    set_pin_ad5204_cs();
    return 0;
}

int dp_set_value (uint8_t chip_num, uint8_t reg, uint8_t value) {
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

int dp_read_value (uint8_t chip_num, uint8_t reg, uint8_t *data) {
    if ((chip_num >= AD5204_CHIP_COUNT) | (reg >= 4)) {
        return EINVAL;
    }

    *data = reg_data[chip_num][reg];
    return 0;
}

#define DP_B1_CHIP_NUM 0
#define DP_C1_CHIP_NUM 0
#define DP_L_CHIP_NUM 0
#define DP_R_CHIP_NUM 0

#define DP_A1_CHIP_NUM 1
#define DP_A2_CHIP_NUM 1
#define DP_C2_CHIP_NUM 1
#define DP_B2_CHIP_NUM 1

#define DP_B1_CH_NUM 0
#define DP_C1_CH_NUM 1
#define DP_L_CH_NUM 2
#define DP_R_CH_NUM 3

#define DP_A1_CH_NUM 0
#define DP_A2_CH_NUM 1
#define DP_C2_CH_NUM 2
#define DP_B2_CH_NUM 3

int dp_set_a1 (uint8_t val) {
    return dp_set_value(DP_A1_CHIP_NUM, DP_A1_CH_NUM, val);
}

int dp_set_a2 (uint8_t val) {
    return dp_set_value(DP_A2_CHIP_NUM, DP_A2_CH_NUM, val);
}

int dp_set_b1 (uint8_t val) {
    return dp_set_value(DP_B1_CHIP_NUM, DP_B1_CH_NUM, val);
}

int dp_set_b2 (uint8_t val) {
    return dp_set_value(DP_B2_CHIP_NUM, DP_B2_CH_NUM, val);
}

int dp_set_c1 (uint8_t val) {
    return dp_set_value(DP_C1_CHIP_NUM, DP_C1_CH_NUM, val);
}

int dp_set_c2 (uint8_t val) {
    return dp_set_value(DP_C2_CHIP_NUM, DP_C2_CH_NUM, val);
}

int dp_set_l (uint8_t val) {
    return dp_set_value(DP_L_CHIP_NUM, DP_L_CH_NUM, val);
}

int dp_set_r (uint8_t val) {
    return dp_set_value(DP_R_CHIP_NUM, DP_R_CH_NUM, val);
}
