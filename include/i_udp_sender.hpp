
#ifndef I_UDP_SENDER_HPP
#define I_UDP_SENDER_HPP

#include <i_recv_updater.hpp>
#include <i_send_updater.hpp>
#include <i_sender.hpp>

struct IUDPSender 
    : public IRecvUpdater
    , public ISendUpdater
    , public ISender
{
    
};

#endif

