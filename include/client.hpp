
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <interface.hpp>

class Client : public INetworkIO
{
private:
    IUDP& m_udp;
public:
    explicit Client(IUDP& udp) noexcept
        : m_udp {udp}
    {
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

#include <frame_timer.hpp>
#include <string.h>

class TestEchoClient : public IReceiverOfPacket
{
private:
    Client m_client;
    Frame m_frame;
    FrameTimer m_timer;

public:
    explicit TestEchoClient(IUDP& udp) noexcept
        : m_client {udp}
        , m_frame {}
        , m_timer {}
    {
        m_timer.Setup(1e9);

        static sockaddr_in address
        {
            .sin_family = AF_INET,
            .sin_port   = htons(53548),
            .sin_addr   = { .s_addr = inet_addr("127.0.0.1") },
            .sin_zero   = {}
        };
        
        static char message[MTU] = {};
        

        for (;;)
        {
            m_frame.Update();
            m_timer.Update(m_frame);

            m_client.UpdateOfReceivePacket(*this);
            
            if (m_timer.IsExpired())
            {
                m_timer.Reset();
                printf("message : ");
                if (scanf("%s", message) == 0)
                {
                    exit(0);
                }
                auto send_packet_ptr = std::make_unique<SendPacket>(address, 1);
                (*send_packet_ptr->iovs)[0].iov_base = message;
                (*send_packet_ptr->iovs)[0].iov_len  = strlen(message);
                m_client.Send(std::move(send_packet_ptr));
            }

            m_client.UpdateOfSendPacket();
        }
    }
    
    void Receive(std::unique_ptr<ReceivePacket>&& recv_packet_ptr) noexcept override
    {   
        printf("msg size : %lu\n", recv_packet_ptr->message.size);
        printf("msg data : %s \n", recv_packet_ptr->message.data);
        printf("\n");
    }
};

#include <udp.hpp>

int client_test()
{
    UDP udp;
    TestEchoClient{udp};
    return 0;
}

#endif

#endif

