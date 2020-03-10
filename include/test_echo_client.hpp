
#ifndef TEST_ECHO_CLIENT_HPP
#define TEST_ECHO_CLIENT_HPP

#define TEST
#ifdef TEST

#include <string.h>

#include <interface.hpp>
#include <udp.hpp>
#include <frame_timer.hpp>

class TestEchoClient : public IReceiverOfPacket
{
private:
    IUDP& m_client;
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


int test_echo_client_test()
{
    UDP udp;
    TestEchoClient{udp};
    return 0;
}

#endif

#endif

