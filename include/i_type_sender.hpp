
#ifndef I_TYPE_SENDER_HPP
#define I_TYPE_SENDER_HPP

#include <i_type_recv_updater.hpp>
#include <i_send_updater.hpp>
#include <i_sender.hpp>

class ITypeSender
    : public ITypeRecvUpdater
    , public ISendUpdater
    , public ISender
{

};


#endif

