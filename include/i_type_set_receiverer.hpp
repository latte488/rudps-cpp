
#ifndef I_TYPE_SET_RECEIVERER_HPP
#define I_TYPE_SET_RECEIVERER_HPP

#include <i_type_receiver.hpp>

struct ITypeSetReceiverer
{
    virtual void SetReceiver(const uint8_t type, std::unique_ptr<ITypeReceiver>&& receiver_ptr) = 0;
    virtual ~ITypeSetReceiverer() {}
};

#endif

