
#ifndef UDP_HPP
#define UDP_HPP

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <arpa/inet.h>

#include <array>
#include <vector>
#include <deque>
#include <memory>

#include <config.hpp>

struct Message
{
    uint8_t data[MTU];
    size_t size;
};

struct RecvPacket
{
    sockaddr_in address;
    Message message;
};

struct SendPacket
{
    sockaddr_in& address;
    std::vector<iovec> iovs;

    explicit SendPacket(sockaddr_in& address_, size_t iov_size) noexcept
        : address {address_}
        , iovs {iov_size}
    {

    }

    virtual ~SendPacket() {}
};

struct IPacketReceiver
{
    virtual void Receive(std::unique_ptr<RecvPacket>&& packet_ptr) = 0;
    virtual ~IPacketReceiver() {}
};

class UDP
{
private:
    const int m_socket_fd;
    mmsghdr m_recv_mmhs[MAX_RECV_UNIT];
    iovec m_recv_iovs[MAX_RECV_UNIT];
    std::array<std::unique_ptr<RecvPacket>, MAX_RECV_UNIT> m_recv_packet_ptrs;
    mmsghdr m_send_mmhs[MAX_SEND_UNIT];
    std::deque<std::unique_ptr<SendPacket>> m_send_packet_ptr_queue;
public:
    explicit UDP(const uint16_t port) noexcept
        : m_socket_fd {socket(AF_INET, SOCK_DGRAM, 0)}
        , m_recv_mmhs {}
        , m_recv_iovs {}
        , m_recv_packet_ptrs {}
        , m_send_mmhs {}
        , m_send_packet_ptr_queue {}
    {
        if (m_socket_fd == -1)
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

        int bind_result = bind(m_socket_fd, (sockaddr*)&address, sizeof(address));

        if (bind_result == -1)
        {
            perror("bind");
            exit(1);
        }

        for (size_t i = 0; i < MAX_RECV_UNIT; ++i)
        {
            m_recv_packet_ptrs[i] = std::make_unique<RecvPacket>();
            SetToMessageHeader(m_recv_mmhs[i].msg_hdr, *m_recv_packet_ptrs[i], m_recv_iovs[i]);
        }
    }

    ~UDP() noexcept
    {
        close(m_socket_fd);
    }

    void RecvUpdate(IPacketReceiver& packet_receiver) noexcept
    {
        const int recvmmsg_result = recvmmsg(m_socket_fd, m_recv_mmhs, MAX_RECV_UNIT, MSG_DONTWAIT, NULL);

        if (recvmmsg_result > 0)
        {
            for (int i = 0; i < recvmmsg_result; ++i)
            {
                m_recv_packet_ptrs[i]->message.size = m_recv_mmhs[i].msg_len;
                packet_receiver.Receive(std::move(m_recv_packet_ptrs[i]));
                m_recv_packet_ptrs[i] = std::make_unique<RecvPacket>();
                SetToMessageHeader(m_recv_mmhs[i].msg_hdr, *m_recv_packet_ptrs[i], m_recv_iovs[i]);
            }
        }
    }

    void Send(std::unique_ptr<SendPacket>&& send_packet_ptr) noexcept
    {
        printf("send!\n");
        m_send_packet_ptr_queue.push_back(std::move(send_packet_ptr));
    }

    void SendUpdate() noexcept
    {
        const size_t send_packet_queue_size = m_send_packet_ptr_queue.size();
        const size_t sendable_packet_size = 
            (send_packet_queue_size < MAX_SEND_UNIT) ? send_packet_queue_size : MAX_SEND_UNIT;
        for (size_t i = 0; i < sendable_packet_size; ++i)
        {
            SetToMessageHeader(m_send_mmhs[i].msg_hdr, *m_send_packet_ptr_queue[i]);
        }
        sendmmsg(m_socket_fd, m_send_mmhs, sendable_packet_size, MSG_DONTWAIT);
        for (size_t i = 0; i < sendable_packet_size; ++i)
        {
            m_send_packet_ptr_queue.pop_front();
        }
    }

private:
    static void SetToMessageHeader(msghdr& msg_hdr, RecvPacket& recv_packet, iovec& iov) noexcept
    {
        iov.iov_base    = recv_packet.message.data;
        iov.iov_len     = sizeof(recv_packet.message.data);

        msg_hdr.msg_name        = &recv_packet.address;
        msg_hdr.msg_namelen     = sizeof(recv_packet.address);
        msg_hdr.msg_iov         = &iov;
        msg_hdr.msg_iovlen      = 1;
        msg_hdr.msg_control     = NULL;
        msg_hdr.msg_controllen  = 0;
        msg_hdr.msg_flags       = 0;
    }

    static void SetToMessageHeader(msghdr& msg_hdr, SendPacket& send_packet) noexcept
    {
        msg_hdr.msg_name        = &send_packet.address;
        msg_hdr.msg_namelen     = sizeof(send_packet.address);
        msg_hdr.msg_iov         = send_packet.iovs.data();
        msg_hdr.msg_iovlen      = send_packet.iovs.size();
        msg_hdr.msg_control     = NULL;
        msg_hdr.msg_controllen  = 0;
        msg_hdr.msg_flags       = 0;
    }
};

class TestSendPacket : public SendPacket
{
private:
    std::unique_ptr<RecvPacket> m_recv_packet_ptr;
public:
    explicit TestSendPacket(std::unique_ptr<RecvPacket>&& recv_packet_ptr) noexcept
        : SendPacket {recv_packet_ptr->address, 1}
        , m_recv_packet_ptr {std::move(recv_packet_ptr)}
    {
        iovs[0].iov_base   = m_recv_packet_ptr->message.data;
        iovs[0].iov_len    = m_recv_packet_ptr->message.size;
    }
};

class TestUDP : public IPacketReceiver
{
private:
    UDP udp;
public:
    explicit TestUDP() noexcept
        : udp {53548}
    {
        for (;;)
        {
            udp.RecvUpdate(*this);
            udp.SendUpdate();
        }
    }

private:
    void Receive(std::unique_ptr<RecvPacket>&& recv_packet_ptr) noexcept override
    {
        printf("msg size : %lu\n", recv_packet_ptr->message.size);
        printf("msg data : %s \n", recv_packet_ptr->message.data);

        auto send_packet_ptr = std::make_unique<TestSendPacket>(std::move(recv_packet_ptr));
        udp.Send(std::move(send_packet_ptr));
    }
};

#include <iostream>

int test_udp()
{
    TestUDP udp;
    return 0;
}

#endif

