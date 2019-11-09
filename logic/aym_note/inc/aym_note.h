#pragma once

int aym_note_reset (uint8_t chip_num);
int aym_note_set_channel (uint8_t chip_num, uint8_t ch, uint8_t state);
int aym_note_write_note_to_channel (uint8_t chip_num, uint8_t ch, uint8_t note);
int aym_note_set_volume_channel (uint8_t chip_num, uint8_t ch, uint8_t volume);