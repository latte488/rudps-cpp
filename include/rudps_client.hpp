
#ifndef RUDPS_CLIENT_HPP
#define RUDPS_CLIENT_HPP

#include <interface.hpp>
#include <frame_timer.hpp>
#include <rudps_contact.hpp>

class RUDPSClient : public IReceiverOfPacket
{
private:
    IUDP& m_udp;
    sockaddr_in m_address;
    uint8_t m_type;
    FrameTimer m_timer;
    size_t m_try_count;
    void (RUDPSClient::*m_receive)(std::unique_ptr<ReceivePacket>&&);
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

    void Receive(std::unique_ptr<ReceivePacket>&& recv_packet_ptr) noexcept override
    {
        (this->*m_receive)(std::move(recv_packet_ptr));
    }

    void Update() noexcept
    {
        (this->*m_update)();
    }

private:
    void UpdateOfEmpty() noexcept
    {
        
    }

    void Contact() noexcept
    {
        m_type = RUDPS_TYPE_CONTACT;
        m_udp.Send(std::make_unique<RUDPSContactRequest>(m_address, m_type));
    }

    void ReceiveOfContact(std::unique_ptr<ReceivePacket>&& recv_packet_ptr) noexcept
    {
        if (RUDPS_CONTACT_REPLY_SIZE != recv_packet_ptr->message.size)
        {
            return;
        }

        printf("ok\n");
    }
    
    void UpdateOfContact() noexcept
    {
        if (m_timer.IsExpired())
        {
            m_timer.Reset();
            if (++m_try_count >= RUDPS_CONTACT_TRY_MAX)
            {
                m_update = &RUDPSClient::UpdateOfEmpty;
            }
            Contact();
        }
    }
};

#endif

