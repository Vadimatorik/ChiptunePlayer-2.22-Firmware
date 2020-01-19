#pragma once

void aym_psg_reset ();

int aym_psg_get_len_tick (const char *file_path, uint32_t *ret_len_tick);
int aym_psg_get_len_sec (const char *file_path, uint32_t *ret_len_sec);

int aym_psg_play (const char *file_path, uint8_t chip_number);
int aym_psg_set_pause (uint8_t chip_number);
int aym_psg_reset_pause (uint8_t chip_number);
int aym_psg_stop (uint8_t chip_number);