
#ifndef TYPE_SERVER_HPP
#define TYPE_SERVER_HPP

#include <server.hpp>
#include <type_divider.hpp>

class TypeServer
{
private:
    Server m_server;
    TypeDivider m_type_divider;
public:
    explicit TypeServer(IUDP& udp, const uint16_t port) noexcept
        : m_server {udp, port}
        , m_type_divider {}
    {

    }

    void SetReceiver(const uint8_t type, std::unique_ptr<ITypeReceiver>&& receiver_ptr) noexcept
    {
        m_type_divider.SetReceiver(type, std::move(receiver_ptr));
    }

    void RecvUpdate() noexcept
    {
        m_server.RecvUpdate(m_type_divider);
    }

    void SendUpate() noexcept
    {
        m_server.SendUpdate();
    }

    void Send(std::unique_ptr<SendPacket>&& send_packet_ptr) noexcept
    {
        
    }
};

#endif

