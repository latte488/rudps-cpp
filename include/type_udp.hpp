
#ifndef TYPE_UDP_HPP
#define TYPE_UDP_HPP

#include <type_divider.hpp>

class TypeUDP : public ISender
{
private:
    IUDP& m_udp;
    TypeDivider m_type_divider;
public:
    explicit TypeUDP(IUDP& udp) noexcept
        : m_udp {udp}
        , m_type_divider {}
    {

    }

    void SetReceiver(const uint8_t type, std::unique_ptr<IReceiverOfTypePacket>&& receiver_ptr) noexcept
    {
        m_type_divider.SetReceiver(type, std::move(receiver_ptr));
    }

    void UpdateOfReceivePacket() noexcept
    {
        m_udp.UpdateOfReceivePacket(m_type_divider);
    }

    void UpdateOfSendPacket() noexcept
    {
        m_udp.UpdateOfSendPacket();
    }

    void Send(std::unique_ptr<SendPacket>&& send_packet_ptr) noexcept override
    {
        m_udp.Send(std::move(send_packet_ptr));
    }
};

#endif

