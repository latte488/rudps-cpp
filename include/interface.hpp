
#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <memory>

#include <packet.hpp>


struct IBinder
{
    virtual void Bind(const uint16_t port) = 0;
    virtual ~IBinder() {}
};

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

struct IUDP 
    : public IBinder
    , public IPacketUpdater
    , public IPacketSender
{

};

struct IClient 
    : public IPacketUpdater
    , public IPacketSender
{

};

struct IServer 
    : public IPacketUpdater
    , public IPacketSender
{

};

#endif

