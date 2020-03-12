
#ifndef TYPE_PACKET_HPP
#define TYPE_PACKET_HPP

#include <packet.hpp>

struct TypeReceivePacket
{
    std::unique_ptr<ReceivePacket> receive_packet;
    sockaddr_in& address;
    uint8_t& type;
    uint8_t* const data;
    const size_t size;

    explicit TypeReceivePacket(std::unique_ptr<ReceivePacket>&& recv_packet_) noexcept
        : receive_packet {std::move(recv_packet_)}
        , address {receive_packet->address}
        , type {receive_packet->message.data[0]}
        , data {&receive_packet->message.data[1]}
        , size {receive_packet->message.size - 1}
    {

    }
};

#endif

