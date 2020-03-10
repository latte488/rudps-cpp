
#ifndef I_SEND_UPDATER_HPP
#define I_SEND_UPDATER_HPP

struct ISendUpdater
{
    virtual void SendUpdate() = 0;
    virtual ~ISendUpdater() {}
};

#endif

