
#ifndef RUDPS_CLIENT__HPP
#define RUDPS_CLIENT_HPP

#define TYPE_RUDPS_CONTACT  0
#define TYPE_RUDPS_AUTH     1
#define TYPE_RUDPS_MESSAGE  2

#include <client.hpp>

class RUDPSClient : public IPacketReceiver
{
private:
    Client m_client;
    Frame m_frame;
    FrameTimer m_timer;
    void (RUDPSClient::*m_update)();
public:
    explicit RUDPSClient(IUDP& udp) noexcept
        : m_client {udp}
        , m_frame {}
        , m_timer {}
    {
        m_timer.Setup(RUDPS_CONTACT_SESSION_TIMEOUT);

        static sockaddr_in address
        {   
            .sin_family = AF_INET,
            .sin_port   = htons(53548),
            .sin_addr   = { .s_addr = inet_addr("127.0.0.1") },
            .sin_zero   = {}
        };

        for (;;)
        {
            m_frame.Update();
            m_timer.Update(m_frame);

            m_client.RecvUpdate(*this);
            

        }

    }

    void Receive(std::unique_ptr<RecvPacket>&& recv_packet) noexcept
    {
        
    }


};

#endif

