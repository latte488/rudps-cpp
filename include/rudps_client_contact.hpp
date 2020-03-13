
#ifndef RUDPS_CLIENT_CONTACT_HPP
#define RUDPS_CLIENT_CONTACT_HPP



#include <interface.hpp>
#include <frame_timer.hpp>
#include <rudps_contact.hpp>

class RUDPSClientContact : public IReceiverOfTypePacket
{
private:
    sockaddr_in& m_address;
    ISender& m_sender;
    uint8_t m_type;
    FrameTimer m_timer;
    size_t m_try_count;
    void (RUDPSClientContact::*m_update)();
public:
    explicit RUDPSClientContact(sockaddr_in& address, ISender& sender, uint8_t& type) noexcept
        : m_address {address}
        , m_sender {sender}
        , m_type {type}
        , m_timer {}
        , m_try_count {}
        , m_update {&RUDPSClientContact::UpdateOfTypePacket}
    {
        m_timer.Setup(RUDPS_CONTACT_SESSION_TIMEOUT);
        Contact();
    }

    void UpdateOfTypePacket() noexcept override
    {
        (this->*m_update)();
    }

    void UpdateOfContact() noexcept
    {
        if (m_timer.IsExpired())
        {
            m_timer.Reset();
            m_try_count++;
            if (m_try_count >= RUDPS_CONTACT_TRY_MAX)
            {
                m_update = &RUDPSClientContact::UpdateOfEmpty;
            }
            Contact();
        }
    }

    void UpdateOfEmpty() noexcept
    {

    }

    void Receive(std::unique_ptr<TypeReceivePacket>&& receive_packet_ptr) noexcept override
    {
        auto& raw_packet = *receive_packet_ptr->receive_packet;

        if (RUDPS_CONTACT_REPLY_SIZE != raw_packet.message.size)
        {
            return;
        }

        m_update = &RUDPSClientContact::UpdateOfEmpty;


    }

private:
    void Contact() noexcept
    {
        m_type = RUDPS_TYPE_CONTACT;
        m_sender.Send(std::make_unique<RUDPSContactRequest>(m_address, m_type));
    }
};


#define TEST
#ifdef TEST

#include <type_udp.hpp>
#include <udp.hpp>

class TestClient
{
private:
    TypeUDP m_tudp;
    uint8_t m_type;
    sockaddr_in m_address;
public:
    explicit TestClient(IUDP& udp) noexcept
        : m_tudp {udp}
        , m_type {}
        , m_address
        { 
            .sin_family = AF_INET,
            .sin_port   = htons(53548),
            .sin_addr   = { .s_addr = inet_addr("127.0.0.1") },
            .sin_zero   = {}
        }

    {
        auto contact = std::make_unique<RUDPSClientContact>(m_address, m_tudp, m_type);
        m_tudp.SetReceiver(RUDPS_TYPE_CONTACT, std::move(contact));
        
        for (;;)
        {
            m_tudp.UpdateOfReceivePacket();
            m_tudp.UpdateOfSendPacket();
        }
    }
};

int rudps_client_contact_test()
{
    UDP udp;
    TestClient{udp};
    return 0;
}

#endif

#endif

