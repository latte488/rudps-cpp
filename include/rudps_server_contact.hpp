
#ifndef RUDPS_SERVER_CONTACT_HPP
#define RUDPS_SERVER_CONTACT_HPP

#include <unordered_map>

#include <interface.hpp>
#include <rudps_contact.hpp>

struct Nonce
{
    uint8_t data[RUDPS_CONTACT_NONCE_SIZE];
};

class RUDPSServerContact : public IReceiverOfTypePacket
{
private:
    ISender& m_sender;
    std::unordered_map<uint16_t, std::unordered_map<uint32_t, Nonce>> m_wait_nonce_map;
    uint8_t* const m_public_key;
public:
    explicit RUDPSServerContact
    (
        ISender& sender, 
        std::unordered_map<uint16_t, std::unordered_map<uint32_t, Nonce>>& wait_nonce_map,
        uint8_t* const public_key
    ) noexcept
        : m_sender {sender}
        , m_wait_nonce_map {wait_nonce_map}
        , m_public_key {public_key}
    {
        
    }

    void Receive(std::unique_ptr<TypeReceivePacket>&& packet_ptr) noexcept override
    {
        auto& port      = packet_ptr->address.sin_port;
        auto& address   = packet_ptr->address.sin_addr.s_addr;
        auto& raw_packet= packet_ptr->receive_packet;

        if (RUDPS_CONTACT_REQUEST_SIZE != raw_packet->message.size)
        {
            return;
        }

        if (m_wait_nonce_map.size() > RUDPS_CONTACT_MAX)
        {
            return;
        }

        // Return if the received packet already exists.
        auto wait_nonce_pair = m_wait_nonce_map.find(port);
        if ((wait_nonce_pair != m_wait_nonce_map.end()) 
        &&  (wait_nonce_pair->second.count(address) != 0))
        {
            return;
        }
        
        auto& nonce = m_wait_nonce_map[port][address];
        m_sender.Send(std::make_unique<RUDPSContactReply>
        (
            std::move(packet_ptr),
            packet_ptr->type,
            nonce.data,
            m_public_key
        ));
    }
};

#endif

