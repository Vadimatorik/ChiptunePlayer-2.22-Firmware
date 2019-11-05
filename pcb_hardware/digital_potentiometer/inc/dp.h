#pragma once

void set_shdn ();
void reset_shdn ();

int set_value (uint8_t chip_num, uint8_t reg, uint8_t value);
int read_value (uint8_t chip_num, uint8_t reg, uint8_t *data);