
#ifndef UDP_HPP
#define UDP_HPP

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <arpa/inet.h>

#include <array>
#include <vector>
#include <memory>

#include <config.hpp>

struct Message
{
    uint8_t data[MTU];
    size_t size;
};

struct Packet
{
    sockaddr_in address;
    Message message;
};

struct IPacketReceiver
{
    virtual void Update(std::unique_ptr<Packet>&& packet_ptr) = 0;
};

struct UDP
{
    const int socket_fd;
    mmsghdr recv_mmhs[MAX_RECV_UNIT];
    iovec recv_iovs[MAX_RECV_UNIT];
    std::array<std::unique_ptr<Packet>, MAX_RECV_UNIT> packet_ptrs;   

    explicit UDP(uint16_t port) noexcept
        : socket_fd {socket(AF_INET, SOCK_DGRAM, 0)}
        , recv_mmhs {}
        , recv_iovs {}
    {
        if (socket_fd == -1)
        {
            perror("socket");
            exit(1);
        }

        sockaddr_in address
        {
            .sin_family = AF_INET,
            .sin_port   = htons(port),
            .sin_addr   = { .s_addr = INADDR_ANY },
            .sin_zero   = {}
        };

        int bind_result = bind(socket_fd, (sockaddr*)&address, sizeof(address));

        if (bind_result == -1)
        {
            perror("bind");
            exit(1);
        }

        for (size_t i = 0; i < MAX_RECV_UNIT; ++i)
        {
            packet_ptrs[i] = std::unique_ptr<Packet>();
            SetToMessageHeader(recv_mmhs[i].msg_hdr, *packet_ptrs[i], recv_iovs[i]);
        }
    }

    ~UDP() noexcept
    {
        close(socket_fd);
    }

    void Update(IPacketReceiver& packet_receiver) noexcept
    {
        for (size_t i = 0; i < MAX_RECV_UNIT; ++i)
        {
            msghdr& msg_hdr = recv_mmhs[i].msg_hdr;
            Packet& packet  = *packet_ptrs[i];

            msg_hdr.msg_name        = &packet.address;
            msg_hdr.msg_namelen     = sizeof(packet.address);
            msg_hdr.msg_iov         = &recv_iovs[i];
            msg_hdr.msg_iovlen      = 1;
            msg_hdr.msg_control     = NULL;
            msg_hdr.msg_controllen  = 0;
            msg_hdr.msg_flags       = 0;
        }

        const int recvmmsg_result = recvmmsg(socket_fd, recv_mmhs, MAX_RECV_UNIT, MSG_DONTWAIT, NULL);

        if (recvmmsg_result > 0)
        {
            for (int i = 0; i < recvmmsg_result; ++i)
            {
                packet_ptrs[i]->message.size = recv_mmhs[i].msg_len;
                packet_receiver.Update(std::move(packet_ptrs[i]));
                packet_ptrs[i] = std::make_unique<Packet>();
                SetToMessageHeader(recv_mmhs[i].msg_hdr, *packet_ptrs[i], recv_iovs[i]);
            }
        }
    }

private:

    
    static void SetToMessageHeader(msghdr& msg_hdr, Packet& packet, iovec& iov) noexcept
    {
        msg_hdr.msg_name        = &packet.address;
        msg_hdr.msg_namelen     = sizeof(packet.address);
        msg_hdr.msg_iov         = &iov;
        msg_hdr.msg_iovlen      = 1;
        msg_hdr.msg_control     = NULL;
        msg_hdr.msg_controllen  = 0;
        msg_hdr.msg_flags       = 0;
    }
};

#endif

