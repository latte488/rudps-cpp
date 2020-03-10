
#ifndef SERVER_HPP
#define SERVER_HPP

#include <interface.hpp>

class Server : public INetworkIO
{
private:
    IUDP& m_udp;
public:
    explicit Server(IUDP& udp, const uint16_t port) noexcept
        : m_udp {udp}
    {
        m_udp.Bind(port);
    }

    void UpdateOfReceivePacket(IReceiverOfPacket& receiver) noexcept override
    {
        m_udp.UpdateOfReceivePacket(receiver);
    }

    void UpdateOfSendPacket() noexcept override
    {
        m_udp.UpdateOfSendPacket();
    }

    void Send(std::unique_ptr<SendPacket>&& send_packet_ptr) noexcept override
    {
        m_udp.Send(std::move(send_packet_ptr));
    }
};

#define TEST
#ifdef TEST

class TestEchoReply : public SendPacket
{
private:
    std::unique_ptr<ReceivePacket> m_recv_packet_ptr;
public:
    explicit TestEchoReply(std::unique_ptr<ReceivePacket>&& recv_packet_ptr) noexcept
        : SendPacket {recv_packet_ptr->address, 1}
        , m_recv_packet_ptr {std::move(recv_packet_ptr)}
    {
        (*iovs)[0].iov_base   = m_recv_packet_ptr->message.data;
        (*iovs)[0].iov_len    = m_recv_packet_ptr->message.size;
    }
};


class TestEchoServer : public IReceiverOfPacket
{
private:
    Server m_server;
public:
    explicit TestEchoServer(IUDP& udp) noexcept
        : m_server {udp, 53548}
    {
        for (;;)
        {
            m_server.UpdateOfReceivePacket(*this);
            m_server.UpdateOfSendPacket();
        }
    }

    void Receive(std::unique_ptr<ReceivePacket>&& recv_packet_ptr) noexcept override
    {
        printf("msg size : %lu\n", recv_packet_ptr->message.size);
        printf("msg data : %s \n", recv_packet_ptr->message.data);

        auto send_packet_ptr = std::make_unique<TestEchoReply>(std::move(recv_packet_ptr));
        m_server.Send(std::move(send_packet_ptr));

        printf("\n");
    }
};

#include <udp.hpp>

int server_test()
{
    UDP udp;
    TestEchoServer{udp};
    return 0;
}

#endif

#endif

