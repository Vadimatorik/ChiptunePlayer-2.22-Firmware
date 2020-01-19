#include "socket_emul_layer.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include "freertos_headers.h"

static int s_lcd = 0;
static int s_keyboard = 0;
static int s_ay[2] = {0};

static const uint8_t CODE_PIN_CS = 0;
static const uint8_t CODE_PIN_DC = 1;
static const uint8_t CODE_PIN_RST = 2;
static const uint8_t CODE_BYTE_SPI = 3;

static int init_socket (int *retrun_fd, uint16_t port, const char *name_port) {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        printf("Error calling socket %s", name_port);
        return -1;
    }

    // Соединяемся по определённому порту с хостом.
    struct sockaddr_in peer;
    peer.sin_family = AF_INET;
    peer.sin_port = htons(port);
    peer.sin_addr.s_addr = inet_addr("127.0.0.1");
    int result = connect(s, (struct sockaddr *)&peer, sizeof(peer));
    if (result) {
        printf("Error calling connect port %s", name_port);
        return -1;
    }

    *retrun_fd = s;

    return 0;
}

int init_sockets () {
    if (init_socket(&s_lcd, 55000, "lcd") != 0) {
        return -1;
    }

    if (init_socket(&s_keyboard, 56000, "keyboard") != 0) {
        return -1;
    }

    return 0;
}

typedef struct _lcd_msg {
    uint8_t module;
    uint8_t data;
} __attribute__((packed)) lcd_msg_t;

static void send_lcd_msg (lcd_msg_t *msg) {
    if (write(s_lcd, msg, sizeof(lcd_msg_t)) != sizeof(lcd_msg_t)) {
        exit(EIO);
    }
}

void socket_gpio_lcd_cs_set (uint8_t state) {
    lcd_msg_t msg = {0};
    msg.module = CODE_PIN_CS;
    msg.data = state;
    send_lcd_msg(&msg);
}

void socket_gpio_lcd_dc_set (uint8_t state) {
    lcd_msg_t msg = {0};
    msg.module = CODE_PIN_DC;
    msg.data = state;
    send_lcd_msg(&msg);
}

void socket_gpio_lcd_rst_set (uint8_t state) {
    lcd_msg_t msg = {0};
    msg.module = CODE_PIN_RST;
    msg.data = state;
    send_lcd_msg(&msg);
}

void socket_spi_lcd_tx (void *d, uint32_t len) {
    uint8_t *data = (uint8_t *)d;

    lcd_msg_t msg = {0};
    msg.module = CODE_BYTE_SPI;

    for (uint32_t i = 0; i < len; i++) {
        msg.data = *data;
        data++;

        send_lcd_msg(&msg);
    }
}

uint8_t socket_get_button_state (uint8_t i) {
    if (write(s_keyboard, &i, sizeof(i)) != sizeof(i)) {
        exit(EIO);
    }

    uint8_t state = 0;
    while (read(s_keyboard, &state, sizeof(state)) != 1);
    return state;
}

typedef struct _socket_ay_msg {
    uint32_t time;
    uint8_t type;
    uint8_t data;
} __attribute__((packed)) socket_ay_msg_t;

static void send_ay_msg (int ay_fd, socket_ay_msg_t *msg) {
    if (write(s_lcd, msg, sizeof(socket_ay_msg_t)) != sizeof(socket_ay_msg_t)) {
        exit(EIO);
    }
}

static const uint8_t CODE_AY_REG= 0;
static const uint8_t CODE_AY_DATA = 1;

void socket_ay_reg_set (uint8_t ay_num, uint8_t reg) {
    if (ay_num >= 2) {
        exit(EINVAL);
    }

    socket_ay_msg_t msg = {0};
    msg.time = xTaskGetTickCount();
    msg.type = CODE_AY_REG;
    msg.data = reg;

    send_ay_msg(s_ay[ay_num], &msg);
}

void socket_ay_data_set (uint8_t ay_num, uint8_t data) {
    if (ay_num >= 2) {
        exit(EINVAL);
    }

    socket_ay_msg_t msg = {0};
    msg.time = xTaskGetTickCount();
    msg.type = CODE_AY_DATA;
    msg.data = data;

    send_ay_msg(s_ay[ay_num], &msg);
}