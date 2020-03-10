
#ifndef TYPE_EMPTY_RECEIVER_HPP
#define TYPE_EMPTY_RECEIVER_HPP

#include <i_type_receiver.hpp>

class TypeEmptyReceiver : public ITypeReceiver
{
    void Receive(std::unique_ptr<TypeRecvPacket>&&) noexcept
    {

    }
};


#endif

