
#ifndef CLIENT_FOR_UDP_HPP
#define CLIENT_FOR_UDP_HPP

#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>

#include <iostream>

#include <config.hpp>

const uint16_t  port        = 53548;
const char      host[]      = "127.0.0.1";
const char      send_data[] = "Hello world!";

int test_client_for_udp()
{
    const int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (socket_fd == -1)
    {
        perror("socket");
        exit(1);
    }

    sockaddr_in address
    {
        .sin_family = AF_INET,
        .sin_port   = htons(port),
        .sin_addr   = { .s_addr = inet_addr(host) },
        .sin_zero   = {}
    };

    const int connect_result = connect(socket_fd, (sockaddr*)&address, sizeof(address));

    if (connect_result == -1)
    {
        perror("connect");
        exit(1);
    }

    const int send_size = send(socket_fd, send_data, sizeof(send_data), 0);

    if (send_size == -1)
    {
        perror("send");
        exit(1);
    }

    static uint8_t recv_data[MTU] = {};

    const int recv_size = recv(socket_fd, recv_data, sizeof(recv_data), 0);

    if (recv_size == -1)
    {
        perror("recv");
        exit(1);
    }

    printf("recv size : %d\n", recv_size);
    printf("recv data : %s\n", recv_data);

    return 0;
}

#endif

