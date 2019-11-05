#pragma once

void set (void);

void reset (void);

void toggle (void);

void set (bool state);

void set (int state);

void set (uint8_t state);

bool read (void);

void write (uint8_t data);

void reset (void);

void toggle (void);

uint8_t read (void);

void sr_set_pin_ad5204_shdn ();
void sr_reset_pin_ad5204_shdn ();