
#ifndef I_RECEIVER_HPP
#define I_RECEIVER_HPP

#include <packet.hpp>

struct IReceiver
{
    virtual void Receive(std::unique_ptr<RecvPacket>&&) = 0;
    virtual ~IReceiver() {}
};

#endif

