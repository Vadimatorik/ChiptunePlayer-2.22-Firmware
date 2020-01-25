#include "aym_hardware.h"

#include "aym_parser.h"

#include "freertos_headers.h"
#include "ff.h"
#include <string.h>
#include <stdlib.h>

#define ERR_OS_MALLOC_FAIL 20

#define PSG_FILE_MARKER_FORMAT 0x1A
#define PSG_FILE_MARKER_INTERRUPT 0xFF
#define PSG_FILE_MARKER_SKIP 0xFE
#define PSG_FILE_MARKER_END_TRACK 0xFD

#define MAX_REG_NUMBER 0xF

#include <stdint.h>
#include <errno.h>

enum PSG_PARSE_STATE {
    PSG_PARSE_STATE_NO = 0,
    PSG_PARSE_STATE_GOT_SKIP_FLAG = 1,
    PSG_PARSE_STATE_GOT_END_FLAG = 2,
    PSG_PARSE_STATE_GOT_REG = 3
};

typedef struct _psg_parser_state {
    uint8_t chip_num;
    uint8_t flag_handler_got;
    uint32_t file_len;
    uint8_t state; // PSG_PARSE_STATE
} psg_parser_state_t;

typedef struct __attribute__((__packed__)) _psg_handler {
    uint8_t sign[3];
    uint8_t marker;
    uint8_t version;
    uint8_t interrupt;
    uint8_t padding[10];
} psg_handler_t;

static int check_handler (psg_parser_state_t *cfg, const uint8_t *f_block) {
    if (cfg->file_len < sizeof(psg_handler_t)) {
        return ENOEXEC;
    }

    psg_handler_t *s = (psg_handler_t *)f_block;

    if (memcmp(s->sign, "PSG", sizeof(s->sign)) != 0) {
        return ENOEXEC;
    }

    if (s->marker != PSG_FILE_MARKER_FORMAT) {
        return ENOEXEC;
    }

    return 0;
}

static int parse (psg_parser_state_t *cfg, const uint8_t *f_block, uint32_t f_block_len, add_element send_msg,
                  uint32_t *ret_len_tick) {
    if (cfg->state == PSG_PARSE_STATE_GOT_END_FLAG) {
        return 0;
    }

    aym_reg_data_t msg = {0};
    msg.chip_num = cfg->chip_num;

    uint32_t pos = 0;

    int rv = 0;

    if (cfg->flag_handler_got == 0) {
        if ((rv = check_handler(cfg, f_block)) != 0) {
            return rv;
        }

        psg_handler_t *s = (psg_handler_t *)f_block;

        uint32_t seek = 0;
        if (s->version == PSG_FILE_MARKER_INTERRUPT) {
            seek = offsetof(psg_handler_t, version);
        } else {
            seek = sizeof(psg_handler_t);
        }

        f_block_len -= seek;
        pos = seek;

        cfg->flag_handler_got = -1;
    }

    while (pos < f_block_len) {
        if (cfg->state == PSG_PARSE_STATE_NO) {
            if (f_block[pos] == PSG_FILE_MARKER_INTERRUPT) {
                msg.reg = REG_PAUSE;
                if (send_msg) {
                    if ((rv = send_msg(&msg)) != 0) {
                        return rv;
                    }
                } else {
                    *ret_len_tick = (*ret_len_tick) + 1;
                }
            } else if (f_block[pos] == PSG_FILE_MARKER_SKIP) {
                msg.reg = REG_PAUSE;
                cfg->state = PSG_PARSE_STATE_GOT_SKIP_FLAG;
            } else if (f_block[pos] == PSG_FILE_MARKER_END_TRACK) {
                cfg->state = PSG_PARSE_STATE_GOT_END_FLAG;
                return 0;
            } else if (f_block[pos] < 15) { // register
                msg.reg = f_block[pos];
                cfg->state = PSG_PARSE_STATE_GOT_REG;
            }

            pos++;
            continue;
        }

        if (cfg->state == PSG_PARSE_STATE_GOT_SKIP_FLAG) {
            if (send_msg) {
                for (uint8_t i = 0; i < f_block[pos]*4; i++) {
                    if ((rv = send_msg(&msg)) != 0) {
                        return rv;
                    }
                }
            } else {
                *ret_len_tick = (*ret_len_tick) + f_block[pos]*4;
            }
        } else if (cfg->state == PSG_PARSE_STATE_GOT_REG) {
            if (msg.reg > MAX_REG_NUMBER) {
                continue;
            }
            if (send_msg) {
                msg.data = f_block[pos];
                if ((rv = send_msg(&msg)) != 0) {
                    return rv;
                }
            }
        }

        cfg->state = PSG_PARSE_STATE_NO;
        pos++;
    }

    return 0;
}

int aym_psg_get_len_tick (const char *file_path, uint32_t *ret_len_tick) {
    int rv = 0;

    FIL *f = malloc(sizeof(FIL));
    if (f == NULL) {
        return ERR_OS_MALLOC_FAIL;
    }

    rv = f_open(f, file_path, FA_READ);
    if (rv != 0) {
        free(f);
        return rv * -1;
    }

    static const uint32_t PSG_READ_BUFFER = 128;
    uint8_t *f_buf = malloc(PSG_READ_BUFFER);
    if (f_buf == NULL) {
        f_close(f);
        free(f);
        return ERR_OS_MALLOC_FAIL;
    }

    psg_parser_state_t parser_cfg = {0};

    UINT f_len = f_size(f);
    parser_cfg.file_len = f_len;

    while (f_len) {
        UINT real_read_num = 0;
        rv = f_read(f, f_buf, PSG_READ_BUFFER, &real_read_num);
        if (rv != 0) {
            f_close(f);
            free(f);
            free(f_buf);
            return rv;
        }

        if (parse(&parser_cfg, f_buf, real_read_num, NULL, ret_len_tick) != 0) {
            f_close(f);
            free(f);
            free(f_buf);
            return FR_INVALID_OBJECT;
        }

        f_len -= real_read_num;
    }

    rv = f_close(f);
    free(f);
    free(f_buf);
    return rv * -1;
}

int aym_psg_play (const char *file_path, add_element send_msg) {
    int rv = 0;

    FIL *f = malloc(sizeof(FIL));
    if (f == NULL) {
        return ERR_OS_MALLOC_FAIL;
    }

    rv = f_open(f, file_path, FA_READ);
    if (rv != 0) {
        free(f);
        return rv * -1;
    }

    static const uint32_t PSG_READ_BUFFER = 128;
    uint8_t *f_buf = malloc(PSG_READ_BUFFER);
    if (f_buf == NULL) {
        f_close(f);
        free(f);
        return ERR_OS_MALLOC_FAIL;
    }

    psg_parser_state_t parser_cfg = {0};

    UINT f_len = f_size(f);
    parser_cfg.file_len = f_len;

    while (f_len) {
        UINT real_read_num = 0;
        rv = f_read(f, f_buf, PSG_READ_BUFFER, &real_read_num);
        if (rv != 0) {
            f_close(f);
            free(f);
            free(f_buf);
            return rv * -1;
        }

        if ((rv = parse(&parser_cfg, f_buf, real_read_num, send_msg, NULL)) != 0) {
            f_close(f);
            free(f);
            free(f_buf);
            return rv;
        }

        f_len -= real_read_num;
    }

    rv = f_close(f);
    free(f);
    free(f_buf);
    return rv * -1;
}
