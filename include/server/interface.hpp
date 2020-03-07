
#ifndef SERVER_INTERFACE_HPP
#define SERVER_INTERFACE_HPP

#include <memory>

#include <server/packet.hpp>

namespace server
{

struct IPacketReceiver
{
    virtual void Receive(std::unique_ptr<RecvPacket>&&) = 0;
    virtual ~IPacketReceiver() {}
};

struct IPacketUpdater
{
    virtual void RecvUpdate(IPacketReceiver&) = 0;
    virtual void SendUpdate() = 0;
    virtual ~IPacketUpdater() {}
};

struct IPacketSender
{
    virtual void Send(std::unique_ptr<SendPacket>&&) = 0;
    virtual ~IPacketSender() {}
};

struct IPacketUpdaterAndIPacketSender : public IPacketUpdater, public IPacketSender
{

};

}

#endif

