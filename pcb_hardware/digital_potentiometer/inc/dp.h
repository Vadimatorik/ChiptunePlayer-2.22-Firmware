#pragma once

int init_dp ();

int dp_set_a1 (uint8_t val);
int dp_set_a2 (uint8_t val);
int dp_set_b1 (uint8_t val);
int dp_set_b2 (uint8_t val);
int dp_set_c1 (uint8_t val);
int dp_set_c2 (uint8_t val);
int dp_set_l (uint8_t val);
int dp_set_r (uint8_t val);

int dp_set_shdn ();
int dp_reset_shdn ();

int dp_set_value (uint8_t chip_num, uint8_t reg, uint8_t value);
int dp_read_value (uint8_t chip_num, uint8_t reg, uint8_t *data);