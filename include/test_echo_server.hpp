
#ifndef TEST_ECHO_SERVER_HPP
#define TEST_ECHO_SERVER_HPP

#define TEST
#ifdef TEST

#include <interface.hpp>
#include <udp.hpp>

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
    IUDP& m_server;
public:
    explicit TestEchoServer(IUDP& udp) noexcept
        : m_server {udp}
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

int test_echo_server_test()
{
    UDP udp;
    udp.Bind(53548);
    TestEchoServer{udp};
    return 0;
}

#endif

#endif

