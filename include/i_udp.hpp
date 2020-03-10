
#ifndef I_UDP_HPP
#define I_UDP_HPP

#include <i_binder.hpp>
#include <i_udp_sender.hpp>

struct IUDP
    : public IBinder
    , public IUDPSender
{

};

#endif

