
#ifndef I_RECV_UPDATER_HPP
#define I_RECV_UPDATER_HPP

#include <i_receiver.hpp>

struct IRecvUpdater
{
    virtual void RecvUpdate(IReceiver&) = 0;
};

#endif

