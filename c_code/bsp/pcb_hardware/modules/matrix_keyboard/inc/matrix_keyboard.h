#pragma once

#include "matrix_keyboard_struct.h"

enum KEY_NUM {
    B_UP = 0,
    B_DOWN = 1,
    B_LEFT = 2,
    B_RIGHT = 3,
    B_ENTER = 4,
    B_RETURN = 5,
    B_MENU = 6
};

void matrix_keyboard_event_press (uint32_t key);
void matrix_keyboard_event_click (uint32_t key);
void matrix_keyboard_event_long_press (uint32_t key);
void matrix_keyboard_event_long_click (uint32_t key);

int init_matrix_keyboard ();