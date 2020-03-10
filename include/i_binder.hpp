
#ifndef I_BINDER_HPP
#define I_BINDER_HPP

#include <stdint.h>

struct IBinder
{
    virtual void Bind(const uint16_t port) = 0;
    virtual ~IBinder() {}
};

#endif

