
#ifndef CLIENT_SEND_PACKET_HPP
#define CLIENT_SEND_PACKET_HPP

#include <arpa/inet.h>

#include <vector>

namespace client
{

struct SendPacket
{
    std::vector<iovec> iovs;

    explicit SendPacket(size_t iov_size) noexcept
        : iovs {iov_size}
    {

    }
};

}

#endif

