
#ifndef CLIENT_INTERFACE_HPP
#define CLIENT_INTERFACE_HPP

#include <memory>

#include <message.hpp>
#include <client/send_packet.hpp>

namespace client
{

struct IReceiver
{
    virtual std::unique_ptr<Message> Recv() = 0;
    virtual ~IReceiver() {}
};

struct ISender
{
    virtual void Send(std::unique_ptr<SendPacket>&& send_packet) = 0;
    virtual ~ISender() {}
};

struct IReceiverAndISender : public IReceiver, public ISender
{

};

}

#endif

