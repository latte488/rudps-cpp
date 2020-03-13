
#ifndef RUDPS_CLIENT_HPP
#define RUDPS_CLIENT_HPP

#include <interface.hpp>
#include <frame_timer.hpp>
#include <rudps_contact.hpp>

class RUDPSClient
{
private:
    IUDP& m_udp;
    sockaddr_in m_address;
    uint8_t m_type;
    FrameTimer m_timer;
    size_t m_try_count;
    void (RUDPSClient::*m_update)();
public:
    explicit RUDPSClient(IUDP& udp, const uint16_t port, const char* host) noexcept
        : m_udp {udp}
        , m_address
        {
            .sin_family = AF_INET,
            .sin_port   = htons(port),
            .sin_addr   = { .s_addr = inet_addr(host) },
            .sin_zero   = {}
        }
        , m_type {}
        , m_timer {}
        , m_try_count {}
        , m_update {}
    {
        
    }

private:
    void Contact() noexcept
    {
        m_type = RUDPS_TYPE_CONTACT;
        m_udp.Send(std::make_unique<RUDPSContactRequest>(m_address, m_type));
    }
};

#endif

