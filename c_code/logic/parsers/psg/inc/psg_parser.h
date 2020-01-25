#pragma once

int aym_psg_get_len_tick (const char *file_path, uint32_t *ret_len_tick);

/*!
 * @return >0 - код ошибки из функции send_msg.
 *         <0 - код ошибки от FATFS.
 *         0 - успешно завершено.
 */
int aym_psg_play (const char *file_path, add_element send_msg);