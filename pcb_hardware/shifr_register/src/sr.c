#include "mc_hardware.h"

#define SR_NUM 3

uint8_t sr_data[SR_NUM] = {0};


int sr_read_byte (uint32_t byte_num, uint8_t *return_byte) {
    if (byte_num >= SR_NUM) {
        return -1;
    }

    *return_byte = sr_data[byte_num];

    return 0;
}

int sr_write_byte (uint32_t byte_num, uint8_t val) {
    if (byte_num >= SR_NUM)
        return -1;

    sr_data[byte_num] = val;

    return 0;
}

int sr_update () {
    return spi_board_device_sr_tx(sr_data, SR_NUM);
}

int init_sr () {
    reset_pin_sr_strob();
    sr_update();
}

int sr_port_read (uint32_t port_num, uint8_t *return_byte) {
    return sr_read_byte(port_num, return_byte);
}

int sr_port_write (uint32_t port_num, uint8_t val) {
    int rv = 0;
    if ((rv = sr_write_byte(port_num, val)) != 0) {
        return rv;
    }

    return sr_update();
}

int sr_port_toggle (uint32_t port_num) {
    int rv = 0;
    uint8_t port_value = 0;
    if ((rv = sr_read_byte(port_num, &port_value)) != 0) {
        return rv;
    }

    port_value = ~port_value;

    if ((rv = sr_write_byte(port_num, port_value)) != 0) {
        return rv;
    }

    return sr_update();
}

int sr_port_reset (uint32_t port_num) {
    int rv = 0;
    if ((rv = sr_write_byte(port_num, 0)) != 0) {
        return rv;
    }

    return sr_update();
}

int sr_pin_set (uint32_t port_num, uint32_t pin_num) {
    int rv = 0;
    uint8_t port_value = 0;

    if ((rv = sr_port_read(port_num, &port_value)) != 0) {
        return rv;
    }

    port_value |= 1 << pin_num;

    return sr_port_write(port_num, port_value);
}

int sr_pin_reset (uint32_t port_num, uint32_t pin_num) {
    int rv = 0;
    uint8_t port_value = 0;

    if ((rv = sr_port_read(port_num, &port_value)) != 0) {
        return rv;
    }

    port_value &= ~((uint8_t)(1 << pin_num));

    return sr_port_write(port_num, port_value);
}


#define SR_PWR_AY_1_ON_PORT_NUM 2
#define SR_PWR_AY_2_ON_PORT_NUM 2
#define SR_AY_1_RES_PORT_NUM 2
#define SR_AY_2_RES_PORT_NUM 2
#define SR_BC1_PORT_NUM 2
#define SR_BDIR_PORT_NUM 2
#define SR_SHDN_PORT_NUM 2

#define SR_PWR_AY_1_ON_PIN_NUM 0
#define SR_PWR_AY_2_ON_PIN_NUM 1
#define SR_AY_1_RES_PIN_NUM 2
#define SR_AY_2_RES_PIN_NUM 3
#define SR_BC1_PIN_NUM 4
#define SR_BDIR_PIN_NUM 5
#define SR_SHDN_PIN_NUM 6

int sr_set_pin_pwr_ay_1_on () {
    return sr_pin_set(SR_PWR_AY_1_ON_PORT_NUM, SR_PWR_AY_1_ON_PIN_NUM);
}

int sr_set_pin_pwr_ay_2_on () {
    return sr_pin_set(SR_PWR_AY_2_ON_PORT_NUM, SR_PWR_AY_2_ON_PIN_NUM);
}

int sr_set_pin_ay_1_res () {
    return sr_pin_set(SR_AY_1_RES_PORT_NUM, SR_AY_1_RES_PIN_NUM);
}

int sr_set_pin_ay_2_res () {
    return sr_pin_set(SR_AY_2_RES_PORT_NUM, SR_AY_2_RES_PIN_NUM);
}

int sr_set_pin_bc1 () {
    return sr_pin_set(SR_BC1_PORT_NUM, SR_BC1_PIN_NUM);
}

int sr_set_pin_bdir () {
    return sr_pin_set(SR_BDIR_PORT_NUM, SR_BDIR_PIN_NUM);
}

int sr_set_pin_ad5204_shdn () {
    return sr_pin_set(SR_SHDN_PORT_NUM, SR_SHDN_PIN_NUM);
}


int sr_reset_pin_pwr_ay_1_on () {
    return sr_pin_reset(SR_PWR_AY_1_ON_PORT_NUM, SR_PWR_AY_1_ON_PIN_NUM);
}

int sr_reset_pin_pwr_ay_2_on () {
    return sr_pin_reset(SR_PWR_AY_2_ON_PORT_NUM, SR_PWR_AY_2_ON_PIN_NUM);
}

int sr_reset_pin_ay_1_res () {
    return sr_pin_reset(SR_AY_1_RES_PORT_NUM, SR_AY_1_RES_PIN_NUM);
}

int sr_reset_pin_ay_2_res () {
    return sr_pin_reset(SR_AY_2_RES_PORT_NUM, SR_AY_2_RES_PIN_NUM);
}

int sr_reset_pin_bc1 () {
    return sr_pin_reset(SR_BC1_PORT_NUM, SR_BC1_PIN_NUM);
}

int sr_reset_pin_bdir () {
    return sr_pin_reset(SR_BDIR_PORT_NUM, SR_BDIR_PIN_NUM);
}

int sr_reset_pin_ad5204_shdn () {
    return sr_pin_reset(SR_SHDN_PORT_NUM, SR_SHDN_PIN_NUM);
}