#include "socket_emul_layer.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <stdio.h>

static int s_lcd = 0;
static int s_keyboard = 0;

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

void socket_gpio_lcd_cs_set (uint8_t state) {
    uint8_t msg[2] = {0};
    msg[0] = CODE_PIN_CS;
    msg[1] = state;

    write(s_lcd, &msg, sizeof(msg));
}

void socket_gpio_lcd_dc_set (uint8_t state) {
    uint8_t msg[2] = {0};
    msg[0] = CODE_PIN_DC;
    msg[1] = state;

    write(s_lcd, &msg, sizeof(msg));
}

void socket_gpio_lcd_rst_set (uint8_t state) {
    uint8_t msg[2] = {0};
    msg[0] = CODE_PIN_RST;
    msg[1] = state;

    write(s_lcd, &msg, sizeof(msg));
}

void socket_spi_lcd_tx (void *d, uint32_t len) {
    uint8_t *data = (uint8_t *)d;

    uint8_t msg[2] = {0};
    msg[0] = CODE_BYTE_SPI;

    for (uint32_t i = 0; i < len; i++) {
        msg[1] = *data;
        data++;

        write(s_lcd, &msg, sizeof(msg));
    }
}

uint8_t socket_get_button_state (uint8_t i) {
    write(s_keyboard, &i, sizeof(i));
    uint8_t state = 0;
    while (read(s_keyboard, &state, sizeof(state)) != 1);
    return state;
}
