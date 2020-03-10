
#ifndef TYPE_PACKET_HPP
#define TYPE_PACKET_HPP

#include <packet.hpp>

struct TypeRecvPacket
{
    std::unique_ptr<::RecvPacket> recv_packet;
    uint8_t& type;
    uint8_t* const data;
    const size_t size;

    explicit TypeRecvPacket(std::unique_ptr<RecvPacket>&& recv_packet_) noexcept
        : recv_packet {std::move(recv_packet_)}
        , type {recv_packet->message.data[0]}
        , data {&recv_packet->message.data[1]}
        , size {recv_packet->message.size - 1}
    {

    }
};

#endif

