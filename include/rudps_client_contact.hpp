
#ifndef RUDPS_CLIENT_CONTACT_HPP
#define RUDPS_CLIENT_CONTACT_HPP

#include <interface.hpp>
#include <rudps_contact.hpp>

class RUDPSClientContact : public IReceiverOfTypePacket
{
private:
    ISender& m_sender;
public:
    explicit RUDPSClientContact(ISender& sender) noexcept
        : m_sender {sender}
    {
               
    }

    void Contact(sockaddr_in& address) noexcept
    {
        m_sender.Send(std::make_unique<RUDPSContactRequest>(address, RUDPS_TYPE_CONTACT));
    }
        

    void Receive(std::unique_ptr<TypeReceivePacket>&& receive_packet_ptr) noexcept override
    {
        auto& raw_packet = *receive_packet_ptr->receive_packet;

        if (RUDPS_CONTACT_REPLY_SIZE != raw_packet.message.size)
        {
            return;
        }
        else
        {
            printf("Contact reply sizes match at least.\n");
        }
    }
};

#endif

