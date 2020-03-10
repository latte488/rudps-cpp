
#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <stdint.h>

#include <packet.hpp>
#include <type_packet.hpp>

struct IReceiverOfPacket
{
    virtual void Receive(std::unique_ptr<ReceivePacket>&&) = 0;
    virtual ~IReceiverOfPacket() {}
};

struct IReceiverOfTypePacket
{
    virtual void Receive(std::unique_ptr<TypeReceivePacket>&&) = 0;
    virtual ~IReceiverOfTypePacket() {}
};

struct INetworkIO
{
    virtual void UpdateOfReceivePacket(IReceiverOfPacket&) = 0;
    virtual void UpdateOfSendPacket() = 0;
    virtual void Send(std::unique_ptr<SendPacket>&&) = 0;
};

struct IUDP : public INetworkIO
{
    virtual void Bind(const uint16_t port) = 0;
};

#endif

