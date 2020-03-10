
#ifndef I_TYPE_RECV_UPDATER_HPP
#define I_TYPE_RECV_UPDATER_HPP

struct ITypeRecvUpdater
{
    virtual void RecvUpdate() = 0;
    virtual ~ITypeRecvUpdater() {}
};

#endif

