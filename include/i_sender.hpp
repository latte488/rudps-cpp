
#ifndef I_SENDER_HPP
#define I_SENDER_HPP

#include <packet.hpp>

struct ISender
{
    virtual void Send(std::unique_ptr<SendPacket>&&) = 0;
    virtual ~ISender() {}
};

#endif

