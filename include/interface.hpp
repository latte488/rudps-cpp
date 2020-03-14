
#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <stdint.h>

#include <packet.hpp>

struct IReceiverOfPacket
{
    virtual void Receive(std::unique_ptr<ReceivePacket>&&) = 0;
    virtual ~IReceiverOfPacket() {}
};

struct ISender
{
    virtual void Send(std::unique_ptr<SendPacket>&&) = 0;
    virtual ~ISender() {}
};
    
struct IUDP : public ISender
{
    virtual void UpdateOfReceivePacket(IReceiverOfPacket&) = 0;
    virtual void UpdateOfSendPacket() = 0;
};

#endif

