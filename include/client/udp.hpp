
#ifndef CLIENT_UDP_HPP
#define CLIENT_UDP_HPP

#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <unistd.h>

#include <memory>

#include <config.hpp>
#include <client/interface.hpp>

namespace client
{

class UDP : public IReceiverAndISender
{
private:
    const int m_socket_fd;
    iovec m_recv_iov;
    msghdr m_msghdr;
public:
    explicit UDP(const uint16_t port, const char* host) noexcept
        : m_socket_fd {socket(AF_INET, SOCK_DGRAM, 0)}
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
            .sin_addr   = { .s_addr = inet_addr(host) },
            .sin_zero   = {}
        };

        int connect_result = connect(m_socket_fd, (sockaddr*)&address, sizeof(address));

        if (connect_result == -1)
        {
            perror("connect");
            exit(1);
        }
    }

    std::unique_ptr<Message> Recv() noexcept override
    {
        auto message_ptr = std::make_unique<Message>();

        SetToMessageHeader(m_msghdr, *message_ptr, m_recv_iov);
        
        const int recvmsg_result = recvmsg(m_socket_fd, &m_msghdr, 0);
    
        if (recvmsg_result == -1)
        {
            perror("recvmg");
            exit(1);
        }
        
        message_ptr->size = recvmsg_result;

        return std::move(message_ptr);
    }

    void Send(std::unique_ptr<SendPacket>&& send_packet) noexcept override
    {
        SetToMessageHeader(m_msghdr, *send_packet);
        
        const int sendmsg_result = sendmsg(m_socket_fd, &m_msghdr, 0);
    
        if (sendmsg_result == -1)
        {
            perror("sendmsg");
            exit(1);
        }
    }

private:
    static void SetToMessageHeader(msghdr& msg_hdr, Message& message, iovec& iov) noexcept
    {
        iov.iov_base    = message.data;
        iov.iov_len     = sizeof(message.data);

        msg_hdr.msg_name        = NULL;
        msg_hdr.msg_namelen     = 0;
        msg_hdr.msg_iov         = &iov;
        msg_hdr.msg_iovlen      = 1;
        msg_hdr.msg_control     = NULL;
        msg_hdr.msg_controllen  = 0;
        msg_hdr.msg_flags       = 0;
    }

    static void SetToMessageHeader(msghdr& msg_hdr, SendPacket& send_packet) noexcept
    {
        msg_hdr.msg_name        = NULL;
        msg_hdr.msg_namelen     = 0;
        msg_hdr.msg_iov         = send_packet.iovs.data();
        msg_hdr.msg_iovlen      = send_packet.iovs.size();
        msg_hdr.msg_control     = NULL;
        msg_hdr.msg_controllen  = 0;
        msg_hdr.msg_flags       = 0;
    }
};

#define TEST
#ifdef TEST

class TestReceiverAndSender
{
private:
    IReceiver& m_receiver;
    ISender& m_sender;
public:
    explicit TestReceiverAndSender(IReceiverAndISender& receiver_and_sender) noexcept
        : m_receiver {receiver_and_sender}
        , m_sender {receiver_and_sender}
    {
        static char message[] = "Hello world!";
        auto send_packet_ptr = std::make_unique<SendPacket>(1);
        send_packet_ptr->iovs[0].iov_base = message;
        send_packet_ptr->iovs[0].iov_len = sizeof(message);
        m_sender.Send(std::move(send_packet_ptr));
        auto recv_message_ptr = m_receiver.Recv();
        printf("recv size %lu\n", recv_message_ptr->size);
        printf("recv data %s \n", recv_message_ptr->data);
    }
};


int udp_test()
{
    UDP udp(53548, "127.0.0.1");
    TestReceiverAndSender{udp};
    return 0;
}

#endif

}

#endif

