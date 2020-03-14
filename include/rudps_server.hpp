
#ifndef RUDPS_SERVER_HPP
#define RUDPS_SERVER_HPP

#include <unordered_map>

#include <interface.hpp>
#include <rudps_contact.hpp>

struct Nonce
{
    uint8_t data[RUDPS_CONTACT_NONCE_SIZE];
};

class RUDPSServer : public IReceiverOfPacket
{
private:
    IUDP& m_udp;
    DH2048 m_dh;
    std::unordered_map<uint16_t, std::unordered_map<uint32_t, Nonce>> m_wait_nonce_map;
    void (RUDPSServer::*m_receivers[RUDPS_TYPE_END])(std::unique_ptr<ReceivePacket>&&);
    void (RUDPSServer::*m_update)();
public:
    explicit RUDPSServer
    (
        IUDP& udp
    ) noexcept
        : m_udp {udp}
        , m_dh {}
        , m_wait_nonce_map {}
    {
        m_receivers[RUDPS_TYPE_CONTACT] = &RUDPSServer::ReceiveOfContact;
        m_receivers[RUDPS_TYPE_AUTH]    = &RUDPSServer::ReceiveOfAuth;
        m_receivers[RUDPS_TYPE_MESSAGE] = &RUDPSServer::ReceiveOfMessage;

        for (uint8_t i = 0; i < RUDPS_TYPE_END; ++i)
        {
            m_receivers[i] = &RUDPSServer::ReceiveOfEmpty;
        }
    }

    void Receive(std::unique_ptr<ReceivePacket>&& packet_ptr) noexcept override
    {
        uint8_t& type = packet_ptr->message.data[0];
        if (type >= RUDPS_TYPE_END)
        {
            return;
        }

        (this->*m_receivers[type])(std::move(packet_ptr));
    }
    
    void ReceiveOfEmpty(std::unique_ptr<ReceivePacket>&&) noexcept
    {

    }

    void ReceiveOfContact(std::unique_ptr<ReceivePacket>&& packet_ptr) noexcept
    {
        printf("0\n");

        auto& port      = packet_ptr->address.sin_port;
        auto& address   = packet_ptr->address.sin_addr.s_addr;

        if (RUDPS_CONTACT_REQUEST_SIZE != packet_ptr->message.size)
        {
            printf("1\n");
            return;
        }

        if (m_wait_nonce_map.size() > RUDPS_CONTACT_MAX)
        {
            printf("2\n");
            return;
        }

        // Return if the received packet already exists.
        auto wait_nonce_pair = m_wait_nonce_map.find(port);
        if ((wait_nonce_pair != m_wait_nonce_map.end()) 
        &&  (wait_nonce_pair->second.count(address) != 0))
        {
            printf("3\n");
            return;
        }
        
        auto& nonce = m_wait_nonce_map[port][address];
        m_udp.Send(std::make_unique<RUDPSContactReply>
        (
            std::move(packet_ptr),
            nonce.data,
            m_dh.public_key
        ));
    }

    void ReceiveOfAuth(std::unique_ptr<ReceivePacket>&& packet_ptr) noexcept
    {
        
    }

    void ReceiveOfMessage(std::unique_ptr<ReceivePacket>&& packet_ptr) noexcept
    {

    }
};

#define TEST
#ifdef TEST

#include <udp.hpp>

int rudps_server_contact_test()
{
    UDP udp;
    udp.Bind(53548);
    RUDPSServer{udp};
    return 0;
}

#endif

#endif

