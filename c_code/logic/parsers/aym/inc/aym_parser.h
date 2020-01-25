#pragma once

#include "aym_hardware.h"

typedef int (*add_element) (aym_reg_data_t *msg);

int init_aym ();

int aym_get_len_tick (const char *file_path, uint32_t *ret_len_tick);
int aym_get_len_sec (const char *file_path, uint32_t *ret_len_sec);

int aym_play (const char *file_path);
int aym_pause ();
int aym_stop ();