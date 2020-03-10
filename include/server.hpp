
#ifndef SERVER_HPP
#define SERVER_HPP

#include <i_udp.hpp>

class Server : public IUDPSender
{
private:
    IUDP& m_udp;
public:
    explicit Server(IUDP& udp, const uint16_t port) noexcept
        : m_udp {udp}
    {
        m_udp.Bind(port);
    }

    void RecvUpdate(IReceiver& receiver) noexcept override
    {
        m_udp.RecvUpdate(receiver);
    }

    void SendUpdate() noexcept override
    {
        m_udp.SendUpdate();
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
    std::unique_ptr<RecvPacket> m_recv_packet_ptr;
public:
    explicit TestEchoReply(std::unique_ptr<RecvPacket>&& recv_packet_ptr) noexcept
        : SendPacket {recv_packet_ptr->address, 1}
        , m_recv_packet_ptr {std::move(recv_packet_ptr)}
    {
        (*iovs)[0].iov_base   = m_recv_packet_ptr->message.data;
        (*iovs)[0].iov_len    = m_recv_packet_ptr->message.size;
    }
};


class TestEchoServer : public IReceiver
{
private:
    Server m_server;
public:
    explicit TestEchoServer(IUDP& udp) noexcept
        : m_server {udp, 53548}
    {
        for (;;)
        {
            m_server.RecvUpdate(*this);
            m_server.SendUpdate();
        }
    }

    void Receive(std::unique_ptr<RecvPacket>&& recv_packet_ptr) noexcept override
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

