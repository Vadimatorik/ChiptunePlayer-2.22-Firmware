#include "socket_emul_layer.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>
#include <stdio.h>

int get_socket_fd (uint16_t port) {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("Error calling socket");
        return -1;
    }

    // Соединяемся по определённому порту с хостом.
    struct sockaddr_in peer;
    peer.sin_family = AF_INET;
    peer.sin_port = htons(port);
    peer.sin_addr.s_addr = inet_addr("127.0.0.1");
    int result = connect(s, (struct sockaddr *)&peer, sizeof(peer));
    if (result) {
        perror("Error calling connect");
        return -1;
    }

    return s;
}

void socket_gpio_set (int fd) {
    static uint8_t state = 0xFF;
    while (send(fd, &state, 1, MSG_CONFIRM) != 1);
}

void socket_gpio_reset (int fd) {
    static uint8_t state = 0;
    while (send(fd, &state, 1, MSG_CONFIRM) != 1);
}

void socket_spi_tx (int fd, void *d, uint32_t len) {
    while (send(fd, d, len, MSG_CONFIRM) != 1);
}