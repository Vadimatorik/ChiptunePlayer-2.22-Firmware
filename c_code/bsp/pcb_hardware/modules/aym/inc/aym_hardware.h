#pragma once

#include <stdint.h>

#define REG_PAUSE 0xFF

typedef struct _aym_reg_data {
    uint8_t chip_num;
    uint8_t reg;
    uint8_t data;
} aym_reg_data_t;

int init_aym_hardware ();

int aym_hard_add_aym_element (aym_reg_data_t *item);
int aym_hard_set_pause ();
int aym_hard_reset_pause ();
int aym_hard_reset ();