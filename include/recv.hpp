#include <stdio.h>

#include <arpa/inet.h>

int test_recv()
{
    uint16_t port = 53548;

    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in address
    {
        .sin_family = AF_INET,
        .sin_port   = htons(port),
        .sin_addr   = { .s_addr = INADDR_ANY },
        .sin_zero   = {}
    };

    bind(socket_fd, (sockaddr*)&address, sizeof(address));

    uint8_t d[512];

    recvfrom(socket_fd, d, sizeof(d), 0, 0, 0);

    printf("%s\n", d);

    return 0;
}
