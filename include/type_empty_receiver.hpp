
#ifndef TYPE_EMPTY_RECEIVER_HPP
#define TYPE_EMPTY_RECEIVER_HPP

#include <interface.hpp>

class TypeEmptyReceiver : public IReceiverOfTypePacket
{
    void Receive(std::unique_ptr<TypeReceivePacket>&&) noexcept
    {

    }
};


#endif

