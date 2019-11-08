#pragma once

int init_sr ();

int sr_set_pin_pwr_ay_1_on ();
int sr_set_pin_pwr_ay_2_on ();
int sr_set_pin_ay_1_res ();
int sr_set_pin_ay_2_res ();
int sr_set_pin_bc1 ();
int sr_set_pin_bdir ();
int sr_set_pin_ad5204_shdn ();

int sr_reset_pin_pwr_ay_1_on ();
int sr_reset_pin_pwr_ay_2_on ();
int sr_reset_pin_ay_1_res ();
int sr_reset_pin_ay_2_res ();
int sr_reset_pin_bc1 ();
int sr_reset_pin_bdir ();
int sr_reset_pin_ad5204_shdn ();

int sr_pin_set (uint32_t port_num, uint32_t pin_num);
int sr_pin_reset (uint32_t port_num, uint32_t pin_num);

int sr_port_read (uint32_t port_num, uint8_t *return_byte);
int sr_port_write (uint32_t port_num, uint8_t val);
int sr_port_toggle (uint32_t port_num);
int sr_port_reset (uint32_t port_num);

int sr_read_byte (uint32_t byte_num, uint8_t *return_byte);
int sr_write_byte (uint32_t byte_num, uint8_t val);
int sr_update ();