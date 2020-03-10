
#ifndef I_TYPE_RECEIVER_HPP
#define I_TYPE_RECEIVER_HPP

#include <type_packet.hpp>

struct ITypeReceiver
{
    virtual void Receive(std::unique_ptr<TypeRecvPacket>&&) = 0;
    virtual ~ITypeReceiver() {}
};

#endif

