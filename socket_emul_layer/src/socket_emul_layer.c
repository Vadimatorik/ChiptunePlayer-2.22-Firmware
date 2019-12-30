#include "socket_emul_layer.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>
#include <stdio.h>

static int s_lcd = 0;

static const uint8_t CODE_PIN_CS = 0;
static const uint8_t CODE_PIN_DC = 1;
static const uint8_t CODE_PIN_RST = 2;
static const uint8_t CODE_BYTE_SPI = 3;

int init_sockets () {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("Error calling socket");
        return -1;
    }

    // Соединяемся по определённому порту с хостом.
    struct sockaddr_in peer;
    peer.sin_family = AF_INET;
    peer.sin_port = htons(55000);
    peer.sin_addr.s_addr = inet_addr("127.0.0.1");
    int result = connect(s, (struct sockaddr *)&peer, sizeof(peer));
    if (result) {
        perror("Error calling connect");
        return -1;
    }

    s_lcd = s;

    return 0;
}

void socket_gpio_lcd_cs_set  (uint8_t state) {
    uint8_t msg[2] = {0};
    msg[0] = CODE_PIN_CS;
    msg[1] = state;

    send(s_lcd, &msg, sizeof(msg), 0);
}

void socket_gpio_lcd_dc_set (uint8_t state) {
    uint8_t msg[2] = {0};
    msg[0] = CODE_PIN_DC;
    msg[1] = state;

    send(s_lcd, &msg, sizeof(msg), 0);
}

void socket_gpio_lcd_rst_set (uint8_t state) {
    uint8_t msg[2] = {0};
    msg[0] = CODE_PIN_RST;
    msg[1] = state;

    send(s_lcd, &msg, sizeof(msg), 0);
}

void socket_spi_lcd_tx (void *d, uint32_t len) {
    uint8_t *data = (uint8_t *)d;

    uint8_t msg[2] = {0};
    msg[0] = CODE_BYTE_SPI;

    for (uint32_t i = 0; i < len; i++) {
        msg[1] = *data;
        data++;

        send(s_lcd, &msg, sizeof(msg), 0);
    }
}